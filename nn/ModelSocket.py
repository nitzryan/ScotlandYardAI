import socket
import sys

if __name__ == "__main__":
    port = int(sys.argv[1])
    serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serverSocket.bind(('localhost', port))
    serverSocket.listen(1)
    
    connection, address = serverSocket.accept()
    while True:
        data = connection.recv(256)
        if not data:
            break
        
        print(data.decode("utf-8"))
        connection.send(b"Hello, Client")
    connection.close()
        