import socket
import argparse
import sys
import select

'''
todo: 
- [ ] implement socket handling
- [ ] verify read-write sequence
- [ ] verify that /quit works for exiting chat
- [ ] verify that /quit works for exiting program
- [ ] use len header for chat
'''

def main():
    # Parse input arguments
    parser = argparse.ArgumentParser(
        description="Client for REGISTER and BRIDGE")
    parser.add_argument("--id", required=True, help="Client ID")
    parser.add_argument("--port", required=True, type=int,
                        help="Client port number")
    parser.add_argument("--server", required=True,
                        help="Server address in the format 'IP:PORT'")
    args = parser.parse_args()

    client_id = args.id
    client_ip = "127.0.0.1"
    client_port = args.port
    try:
        server_ip, server_port = args.server.split(":")
        server_port = int(server_port)
    except ValueError:
        print("Error: Invalid server address format. Use IP:PORT.")
        sys.exit(1)

    print(f"{client_id} is running on {client_ip}:{client_port}")
    
    # Peer information storage
    peer_info = {}

    # Function to send a REGISTER message
    def send_register():
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
                client_socket.connect((server_ip, server_port))
                message = (
                    f"REGISTER\r\n"
                    f"clientID: {client_id}\r\n"
                    f"IP: {client_ip}\r\n"
                    f"Port: {client_port}\r\n"
                    f"\r\n"
                )
                client_socket.send(message.encode())
                response = client_socket.recv(1024).decode()
                print("Server Response:", response)
        except Exception as e:
            print(f"Error during REGISTER: {e}")

    # Function to send a BRIDGE message
    def send_bridge():
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
                client_socket.connect((server_ip, server_port))
                message = (
                    f"BRIDGE\r\n"
                    f"clientID: {client_id}\r\n"
                    f"\r\n"
                )
                client_socket.sendall(message.encode())
                response = client_socket.recv(1024).decode()
                return response  # Returns peer info for CHAT
        except Exception as e:
            print(f"Error during BRIDGE: {e}")
            return None

    # Function to handle waiting mode when no peer exists
    def wait_for_peer():
        print("No peers available. Waiting for a peer to connect...")
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as listener_socket:
            listener_socket.bind((client_ip, client_port))
            listener_socket.listen(5)
            listener_socket.setblocking(False)
            sockets = [listener_socket]
            try:
                while True:
                    readable, _, _ = select.select(sockets, [], [])
                    for s in readable:
                        if s == listener_socket:
                            conn, addr = listener_socket.accept()
                            print(f"Peer connected from {addr}")
                            chat_with_peer_connection(conn)
                            return
            except KeyboardInterrupt:
                listener_socket.close()
                sys.exit(0)
                
    # Function to handle chatting with a peer
    def chat_with_peer_connection(peer_socket):
        """
        Handles a turn-based chat session with a peer.
        Only one message can be sent at a time, and the client must wait for a response before sending another message.
        """
        print("Chat session started. Type '/quit' to end.")
        peer_socket.setblocking(False)
        
        turn = True  # True means it's this client's turn to send a message
        sockets = [sys.stdin, peer_socket]

        try:
            while True:
                # Monitor sockets
                readable, _, _ = select.select(sockets, [], [])
                for s in readable:
                    if turn and s == sys.stdin:  # User input, only allowed during user's turn
                        message = sys.stdin.readline().strip()
                        if message.lower() == "/quit":
                            # Send QUIT message to the peer
                            quit_message = "CHAT\r\nType: QUIT\r\n\r\n"
                            peer_socket.sendall(quit_message.encode())
                            print("Chat session ended.")
                            return

                        # Format message with headers
                        chat_message = (
                            f"CHAT\r\n"
                            f"Type: MESSAGE\r\n"
                            f"Length: {len(message)}\r\n"
                            f"\r\n"
                            f"{message}\r\n"
                        )
                        peer_socket.sendall(chat_message.encode())
                        turn = False  # Switch to read mode
                        print("Waiting for peer response...")
                    elif not turn and s == peer_socket:  # Incoming peer message
                        try:
                            # Read and parse the incoming message
                            data = peer_socket.recv(1024).decode()
                            if not data.strip():  # Connection closed
                                print("Peer ended the chat session.")
                                return

                            # Process message headers and content
                            headers, body = data.split("\r\n\r\n", 1)
                            header_lines = headers.split("\r\n")
                            message_type = [line.split(": ")[1] for line in header_lines if line.startswith("Type")][0]

                            if message_type == "QUIT":
                                print("Peer ended the chat session.")
                                return
                            elif message_type == "MESSAGE":
                                print(f"Peer: {body.strip()}")
                                turn = True  # Switch to write mode
                        except Exception:
                            print("Connection to peer lost.")
                            return
        except KeyboardInterrupt:
            peer_socket.close()
            sys.exit(0)


    # Main loop
    try:
        while True:
            command = input(
                "Enter a command (/id, /register, /bridge, /chat, /quit): ").strip().lower()
            if command == "/id":
                print(f"{client_id}")
            elif command == "/register":
                send_register()
            elif command == "/bridge":
                response = send_bridge()
                if response:
                    # Parse the BRIDGE response
                    lines = response.split("\r\n")
                    peer_id = [line.split(": ")[1] for line in lines if line.startswith("clientID")][0]
                    if not peer_id:
                        wait_for_peer()  # Enter waiting mode if no peer is available
                    else:
                        peer_info["IP"] = [line.split(": ")[1] for line in lines if line.startswith("IP")][0]
                        peer_info["Port"] = int([line.split(": ")[1] for line in lines if line.startswith("Port")][0])
                        print(f"Peer info received: {peer_info}")
                else:
                    print("Error: Failed to retrieve peer info.")
            elif command == "/chat":
                if peer_info:
                    try:
                        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as chat_socket:
                            chat_socket.connect((peer_info["IP"], peer_info["Port"]))
                            chat_with_peer_connection(chat_socket)
                    except ConnectionRefusedError:
                        print("Error: Unable to connect to peer. The peer might not be available.")
                else:
                    print("Error: No peer info available. Use /bridge first.")
            elif command == "/quit":
                print("Exiting program.")
                sys.exit(0)
            else:
                print("Error: Invalid command. Use /id, /register, /bridge, /chat, or /quit.")
    except KeyboardInterrupt:
        print("\nTerminating program.")
        sys.exit(0)

if __name__ == "__main__":
    main()
