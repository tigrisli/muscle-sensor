import socket

def create_tcp_server(host, port):
    # Create a socket object using IPv4 and TCP protocol
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    # Bind the socket to the specified host and port
    server_socket.bind((host, port))
    
    # Enable the server to accept connections, with a specified backlog
    server_socket.listen(5)
    print(f"Server is listening on {host}:{port}")
    
    try:
        while True:
            # Accept a connection from a client
            client_socket, addr = server_socket.accept()
            print(f"Connected by {addr}")
            
            # Receive data from the client in a loop
            while True:
                data = client_socket.recv(1024)  # Buffer size is 1024 bytes
                if not data:
                    break  # No more data from client, exit loop
                print(f"Received: {data.decode()} from {addr}")
            
            # Close the client socket
            client_socket.close()
            print(f"Connection closed with {addr}")
    except KeyboardInterrupt:
        print("Server is shutting down.")
    finally:
        # Always close the server socket on exit
        server_socket.close()

if __name__ == "__main__":
    HOST = '0.0.0.0'  # Use '0.0.0.0' to listen on all available network interfaces
    PORT = 5000        # Port to listen on (use ports > 1023)
    create_tcp_server(HOST, PORT)
