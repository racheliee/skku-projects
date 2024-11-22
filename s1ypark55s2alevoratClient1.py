import socket
import argparse
import sys

def main():
    # Parse input arguments
    parser = argparse.ArgumentParser(description="Client for REGISTER and BRIDGE")
    parser.add_argument("--id", required=True, help="Client ID")
    parser.add_argument("--port", required=True, type=int, help="Client port number")
    parser.add_argument("--server", required=True, help="Server address in the format 'IP:PORT'")
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
            print(f"Error during REGISTER: {e}")

    # Function to send a BRIDGE message
    def send_bridge():
        try:
            with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as client_socket:
                client_socket.connect((server_ip, server_port))
                message = f"BRIDGE\nclientID: {client_id}\n\n"
                message = (
                    f"BRIDGE\r\n"
                    f"clientID: {client_id}\r\n"
                    f"\r\n"
                )
                client_socket.sendall(message.encode())
                response = client_socket.recv(1024).decode()
        except Exception as e:
            print(f"Error during BRIDGE: {e}")

    # Main loop
    try:
        while True:
            command = input("Enter a command (/id, /register, /bridge): ").strip().lower()
            if command == "/id":
                print(f"Your Client ID is: {client_id}")
            elif command == "/register":
                send_register()
            elif command == "/bridge":
                send_bridge()
            else:
                print("Error: Invalid command. Use /id, /register, or /bridge.")
    except KeyboardInterrupt:
        print("\nTerminating program.")
        sys.exit(0)

if __name__ == "__main__":
    main()
