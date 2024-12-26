import socket
import argparse
import sys
import signal
import select

# Data structure to store registered clients
clients = {}
server_socket = None

def handle_sigint(signum, frame):
    global server_socket
    if server_socket:
        server_socket.close()
    sys.exit(0)

def main():
    # Parse input arguments
    parser = argparse.ArgumentParser(description="Server for Chat Application")
    parser.add_argument("--port", required=True, type=int, help="Port number to listen on")
    args = parser.parse_args()

    server_ip = "127.0.0.1"
    server_port = args.port
    
    signal.signal(signal.SIGINT, handle_sigint)

    # Create and bind the server socket
    try:
        server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server_socket.bind((server_ip, server_port))
        server_socket.listen(5)
        sys.stdout.write(f"Server is listening on {server_ip}:{server_port}\n")
        sys.stdout.flush()
    except Exception as e:
        sys.exit(0)

    sockets = [server_socket, sys.stdin]

    while True:
        try:
            # Monitor server socket and stdin for readiness
            readable, _, _ = select.select(sockets, [], [])
            for s in readable:
                if s == server_socket:
                    # Accept incoming client connections
                    client_socket, client_address = server_socket.accept()
                    handle_client(client_socket)
                elif s == sys.stdin:
                    # Handle stdin commands
                    command = sys.stdin.readline().strip()
                    if command == "/info":
                        handle_info()
                    else:
                        continue
        except KeyboardInterrupt:
            handle_sigint(None, None)
        except Exception as e:
            print(f"Error in main loop: {e}")

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
            return
    except Exception as e:
        return
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
            return

        # Register the client
        clients[client_id] = {"IP": client_ip, "Port": client_port}
        sys.stdout.write(f"REGISTER: {client_id} from {client_ip}:{client_port}\n")
        sys.stdout.flush()

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
        return

def handle_bridge(client_socket, lines):
    try:
        # Extract the requesting client ID
        client_id = None
        for line in lines[1:]:
            if line.startswith("clientID:"):
                client_id = line.split(": ")[1]

        if not client_id:
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
            sys.stdout.write(f"BRIDGE: {client_id} {clients[client_id]['IP']}:{clients[client_id]['Port']}\n")
            sys.stdout.flush()
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
            sys.stdout.write(
                f"BRIDGE: {client_id} {clients[client_id]['IP']}:{clients[client_id]['Port']} "
                f"{peer_id} {peer_info['IP']}:{peer_info['Port']}\n"
            )
            sys.stdout.flush()
        client_socket.sendall(response.encode())
    except Exception as e:
        return
        
        
def handle_info():
    if not clients:
        return
    
    for client_id, client_info in clients.items():
        sys.stdout.write(f"{client_id} {client_info['IP']}:{client_info['Port']}\n")
    sys.stdout.flush()

if __name__ == "__main__":
    main()
