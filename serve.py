from http.server import HTTPServer, SimpleHTTPRequestHandler
import ssl

# Command:
# openssl req -x509 -newkey rsa:4096 -sha256 -days 3650 -nodes -keyout cse113.key -out cse113.crt -subj "/CN=localhost"

# Handler for cross origin isolation so SharedArrayBuffers are allowed
class CORSRequestHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_header('Cross-Origin-Opener-Policy', 'same-origin')
        self.send_header('Cross-Origin-Embedder-Policy', 'require-corp')
        super().end_headers()

# Create SSL context                                                                     
context = ssl.SSLContext(ssl.PROTOCOL_TLS_SERVER)
context.load_cert_chain(certfile='./cse113.crt', keyfile='./cse113.key')

# Set up HTTPS server                                                                    
server_address = ('localhost', 8000)
httpd = HTTPServer(server_address, CORSRequestHandler)
httpd.socket = context.wrap_socket(httpd.socket, server_side=True)

print("Server running at https://localhost:8000/")
httpd.serve_forever()

