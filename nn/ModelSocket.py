import socket
import sys
import struct
import traceback

from InputParser import ParseLocationPredictionFile
from Model_Training import TrainMap

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
            
            if request == "train":
                inputs, outputs = ParseLocationPredictionFile("nn/models/" + filebase + ".txt")
                loss = TrainMap(inputs, outputs, filebase + "_map")
                connection.send(struct.pack('f', loss))
            
            #connection.send(b"Hello, Client")
        except Exception as e:
            print(e)
            print(traceback.print_exc())
            connection.send(b"Error with request")
    connection.close()
        