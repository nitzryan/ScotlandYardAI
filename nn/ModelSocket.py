import socket
import sys
import struct
import traceback

from InputParser import *
from Model_Training import TrainMap
from GenerateProbMap import GenerateProbMap

file_location = "nn/models/"

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
            
            if request == "train_map":
                inputs, outputs = ParseLocationPredictionFile(file_location + filebase + ".txt")
                loss = TrainMap(inputs, outputs, filebase + "_map")
                print(f"Loss: {loss}")
                connection.send(f"{loss}".encode("utf-8"))
            
            elif request == "get_map":
                inputs = ParseSingleLocationPrediction()
                probs = GenerateProbMap(inputs, filebase)
                for prob in probs:
                    connection.send(f"{prob * 100:.1f}".encode("utf-8"))
                connection.send("!".encode("utf-8"))
            #connection.send(b"Hello, Client")
        except Exception as e:
            print(e)
            print(traceback.print_exc())
            connection.send(b"Error with request")
    connection.close()
        