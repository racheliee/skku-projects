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
        sys.exit(0)

    sys.stdout.write(f"{client_id} is running on {client_ip}:{client_port}\n")
    sys.stdout.flush()
    
    
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
        except Exception as e:
            sys.exit(0)

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
            return None
        except KeyboardInterrupt:
            sys.exit(0)

    # Function to handle waiting mode when no peer exists
    def wait_for_peer():
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
                            chat_with_peer_connection(conn)
                            return
            except KeyboardInterrupt:
                listener_socket.close()
                sys.exit(0)
                
    # Function to handle chatting with a peer
    def chat_with_peer_connection(peer_socket, initiator=False):
        try:
            peer_socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
            
            if initiator:
                # Send introductory message to the peer
                intro_message = (
                    f"CHAT\r\n"
                    f"Type: MESSAGE\r\n"
                    f"\r\n"
                    f"Incoming chat request from {client_id} {client_ip}:{client_port}\r\n"
                )
                peer_socket.sendall(intro_message.encode())
            
            peer_socket.setblocking(False)
            
            turn = initiator  # True means it's this client's turn to send a message
            sockets = [sys.stdin, peer_socket]

            while True:
                # Monitor sockets
                readable, _, _ = select.select(sockets, [], [])
                for s in readable:
                    if s == sys.stdin:
                        message = sys.stdin.readline().strip()
                        if message.lower() == "/quit":
                            # Send QUIT message to the peer
                            # sys.stdout.write("Chat session ended\n")
                            # sys.stdout.flush()
                            # sys.stdout.write("Exiting Program\n")
                            # sys.stdout.flush()
                            # quit_message = f"CHAT\r\nType: QUIT\r\n\r\n{client_id} has ended the chat session.\nExiting Program\n"
                            quit_message = f"CHAT\r\nType: QUIT\r\n\r\n"
                            peer_socket.sendall(quit_message.encode())
                            sys.exit(0)
                        
                    if s == peer_socket:  # Incoming peer message
                        try:
                            # Read and parse the incoming message
                            data = peer_socket.recv(1024).decode()
                            if not data.strip():  # Connection closed
                                return

                            # Process message headers and content
                            headers, body = data.split("\r\n\r\n", 1)
                            header_lines = headers.split("\r\n")
                            message_type = [line.split(": ")[1] for line in header_lines if line.startswith("Type")][0]

                            if message_type == "QUIT":
                                # sys.stdout.write(body.strip() + "\n")
                                # sys.stdout.flush()
                                sys.exit(0)
                            elif message_type == "MESSAGE":
                                sys.stdout.write(body.strip() + "\n")
                                sys.stdout.flush()
                                turn = True  # Switch to write mode
                        except Exception as e:
                            return
                    elif turn and s == sys.stdin:  # User input, only allowed during user's turn
                        # Format message with headers
                        chat_message = (
                            f"CHAT\r\n"
                            f"Type: MESSAGE\r\n"
                            f"\r\n"
                            f"{client_id}> {message}\r\n"
                        )
                        peer_socket.sendall(chat_message.encode())
                        turn = False  # Switch to read mode
        
        except KeyboardInterrupt:
            peer_socket.close()
            sys.exit(0)
        finally:
            peer_socket.close()


    # Main loop
    try:
        while True:
            command = input().strip()
            
            if command == "/id":
                sys.stdout.write(f"{client_id}\n")
                sys.stdout.flush()
            elif command == "/register":
                send_register()
            elif command == "/bridge":
                response = send_bridge()
                if response:
                    # Parse the BRIDGE response
                    lines = response.split("\r\n")
                    peer_id = [line.split(": ")[1] for line in lines if line.startswith("clientID")][0]
                    if not peer_id:
                        # sys.stdout.write(f"{client_id} IN WAIT MODE\n")
                        # sys.stdout.flush()
                        wait_for_peer()  # Enter waiting mode if no peer is available
                    else:
                        peer_info["IP"] = [line.split(": ")[1] for line in lines if line.startswith("IP")][0]
                        peer_info["Port"] = int([line.split(": ")[1] for line in lines if line.startswith("Port")][0])
                else:
                    continue
            elif command == "/chat":
                if peer_info:
                    try:
                        # Create a socket to connect to the peer
                        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as chat_socket:
                            chat_socket.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1) # flush data immediately

                            peer_ip = peer_info["IP"]
                            peer_port = peer_info["Port"]

                            # Connect to the peer
                            chat_socket.connect((peer_ip, peer_port))
                            
                            # sys.stdout.write("IN CHAT MODE\n")
                            # sys.stdout.flush()

                            # Initiate the chat session with the introductory message
                            chat_with_peer_connection(chat_socket, initiator=True)
                    except ConnectionRefusedError:
                        sys.exit(0)
                else:
                    continue
            elif command == "/quit":
                sys.exit(0)
            else:
                continue
    except KeyboardInterrupt:
        sys.exit(0)
    except Exception as e:
        sys.exit(0)

if __name__ == "__main__":
    main()


''' sys.stdin.readline() version --> works on mininet but the ./grader cannot parse the output
    # Main loop
    try:
        while True:

            for command in sys.stdin:
             #    command = input().strip()
                command = command.strip()

                if command == "/id":
                    sys.stdout.write(f"{client_id}\n")
                    sys.stdout.flush()
                elif command == "/register":
                    send_register()
                elif command == "/bridge":
                    response = send_bridge()
                    if response:
                        # Parse the BRIDGE response
                        lines = response.split("\r\n")
                        peer_id = [line.split(
                            ": ")[1] for line in lines if line.startswith("clientID")][0]
                        if not peer_id:
                            wait_for_peer()  # Enter waiting mode if no peer is available
                        else:
                            peer_info["IP"] = [line.split(
                                ": ")[1] for line in lines if line.startswith("IP")][0]
                            peer_info["Port"] = int(
                                [line.split(": ")[1] for line in lines if line.startswith("Port")][0])
                    else:
                        continue
                elif command == "/chat":
                    if peer_info:
                        try:
                            # Create a socket to connect to the peer
                            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as chat_socket:
                                # flush data immediately
                                chat_socket.setsockopt(
                                    socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)

                                peer_ip = peer_info["IP"]
                                peer_port = peer_info["Port"]

                                # Connect to the peer
                                chat_socket.connect((peer_ip, peer_port))

                                # Initiate the chat session with the introductory message
                                chat_with_peer_connection(
                                    chat_socket, initiator=True)
                        except ConnectionRefusedError:
                            sys.exit(0)
                    else:
                        continue
                elif command == "/quit":
                    sys.exit(0)
                else:
                    continue
    except KeyboardInterrupt:
        sys.exit(0)
    except Exception as e:
        sys.exit(0)


if __name__ == "__main__":
    main()
'''