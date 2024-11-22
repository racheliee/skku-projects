import socket
import argparse
import sys
import select

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
            while True:
                readable, _, _ = select.select(sockets, [], [])
                for s in readable:
                    if s == listener_socket:
                        conn, addr = listener_socket.accept()
                        print(f"Peer connected from {addr}")
                        chat_with_peer_connection(conn)
                        return

    # Function to handle chatting with a peer
    def chat_with_peer_connection(peer_socket):
        print("Chat session started. Type '/quit' to end.")
        peer_socket.setblocking(False)
        sockets = [sys.stdin, peer_socket]

        while True:
            readable, _, _ = select.select(sockets, [], [])
            for s in readable:
                if s == sys.stdin:  # User input
                    message = sys.stdin.readline().strip()
                    if message.lower() == "/quit":
                        peer_socket.sendall(b"QUIT\r\n")
                        print("Chat session ended.")
                        return
                    peer_socket.sendall((message + "\r\n").encode())
                elif s == peer_socket:  # Incoming peer message
                    try:
                        data = peer_socket.recv(1024).decode().strip()
                        if not data or data == "QUIT":
                            print("Peer ended the chat session.")
                            return
                        print(f"Peer: {data}")
                    except Exception:
                        print("Connection to peer lost.")
                        return

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
