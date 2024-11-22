import socket
import argparse
import sys

# Data structure to store registered clients
clients = {}

def main():
    # Parse input arguments
    parser = argparse.ArgumentParser(description="Server for Chat Application")
    parser.add_argument("--port", required=True, type=int, help="Port number to listen on")
    args = parser.parse_args()

    server_ip = "127.0.0.1"
    server_port = args.port

    # Create and bind the server socket
    try:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.bind((server_ip, server_port))
        server_socket.listen(5)
        print(f"Server listening on {server_ip}:{server_port}")
    except Exception as e:
        print(f"Error setting up the server: {e}")
        sys.exit(1)

    while True:
        try:
            # Accept incoming client connections
            client_socket, client_address = server_socket.accept()
            print(f"Connection established with {client_address}")
            handle_client(client_socket)
        except KeyboardInterrupt:
            print("\nShutting down the server.")
            server_socket.close()
            sys.exit(0)
        except Exception as e:
            print(f"Error handling client: {e}")

def handle_client(client_socket):
    try:
        # Receive the client message
        data = client_socket.recv(1024).decode().strip()
        if not data:
            return

        # Parse the request
        lines = data.split("\r\n")
        request_type = lines[0]

        if request_type == "REGISTER":
            handle_register(client_socket, lines)
        elif request_type == "BRIDGE":
            handle_bridge(client_socket, lines)
        else:
            send_error(client_socket, "Invalid Request")
    except Exception as e:
        print(f"Error processing client request: {e}")
    finally:
        client_socket.close()

def handle_register(client_socket, lines):
    try:
        # Extract client information from headers
        client_id = None
        client_ip = None
        client_port = None

        for line in lines[1:]:
            if line.startswith("clientID:"):
                client_id = line.split(": ")[1]
            elif line.startswith("IP:"):
                client_ip = line.split(": ")[1]
            elif line.startswith("Port:"):
                client_port = line.split(": ")[1]

        if not (client_id and client_ip and client_port):
            send_error(client_socket, "Missing required headers for REGISTER")
            return

        # Register the client
        clients[client_id] = {"IP": client_ip, "Port": client_port}
        print(f"REGISTER: {client_id} from {client_ip}:{client_port}")

        # Send REGACK response
        response = (
            f"REGACK\r\n"
            f"clientID: {client_id}\r\n"
            f"IP: {client_ip}\r\n"
            f"Port: {client_port}\r\n"
            f"Status: registered\r\n"
            f"\r\n"
        )
        client_socket.sendall(response.encode())
    except Exception as e:
        print(f"Error handling REGISTER: {e}")

def handle_bridge(client_socket, lines):
    try:
        # Extract the requesting client ID
        client_id = None
        for line in lines[1:]:
            if line.startswith("clientID:"):
                client_id = line.split(": ")[1]

        if not client_id:
            send_error(client_socket, "Missing clientID in BRIDGE request")
            return

        # Find another client to bridge
        peer_id = None
        for registered_client in clients:
            if registered_client != client_id:
                peer_id = registered_client
                break

        if not peer_id:
            # No other clients available
            response = (
                f"BRIDGEACK\r\n"
                f"clientID: \r\n"
                f"IP: \r\n"
                f"Port: \r\n"
                f"\r\n"
            )
            print(f"BRIDGE: {client_id} {clients[client_id]['IP']}:{clients[client_id]['Port']} -> No peer available")
        else:
            # Return the peer information
            peer_info = clients[peer_id]
            response = (
                f"BRIDGEACK\r\n"
                f"clientID: {peer_id}\r\n"
                f"IP: {peer_info['IP']}\r\n"
                f"Port: {peer_info['Port']}\r\n"
                f"\r\n"
            )
            print(
                f"BRIDGE: {client_id} {clients[client_id]['IP']}:{clients[client_id]['Port']} "
                f"{peer_id} {peer_info['IP']}:{peer_info['Port']}"
            )
        client_socket.sendall(response.encode())
    except Exception as e:
        print(f"Error handling BRIDGE: {e}")

def send_error(client_socket, message):
    response = f"ERROR\r\nMessage: {message}\r\n\r\n"
    client_socket.sendall(response.encode())

if __name__ == "__main__":
    main()
