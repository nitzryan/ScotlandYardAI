import socket
import sys

from InputParser import ParseLocationPredictionFile

if __name__ == "__main__":
    print("Hello, World")
    port = int(sys.argv[1])
    serverSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    serverSocket.bind(('localhost', port))
    serverSocket.listen(1)
    
    connection, address = serverSocket.accept()
    while True:
        data = connection.recv(256)
        if not data:
            break
        
        try:
            request, filebase = data.decode("utf-8").split(",")
            print(f"Request {request} for file {filebase}")
            ParseLocationPredictionFile("nn/models/" + filebase + ".txt")
            connection.send(b"Hello, Client")
        except:
            connection.send(b"Error with request")
    connection.close()
        