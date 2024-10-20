import socket
import sys
import struct
import traceback

from InputParser import *
from Model_Training import TrainMap, TrainScore
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
        data = connection.recv(4096)
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
            
            elif request == "train_score":
                x_map, x_score, results = ParseGamePredictionFile(file_location + filebase + "_score.txt")
                loss = TrainScore(x_map, x_score, results, filebase)
                print(f"Loss: {loss}")
                connection.send(f"{loss}".encode("utf-8"))
            
            elif request == "get_map":
                inputs = ParseSingleLocationPrediction()
                probs = GenerateProbMap(inputs, filebase)
                probsStr = ""
                for prob in probs:
                    probsStr += f"{prob * 100:.1f},"
                probsStr += "!"
                connection.send(probsStr.encode("utf-8"))
                
            elif request == "get_scores":
                inputs = ParseScoreRequestFile(file_location + filebase + "_scorerequest.txt")
                scores = GenerateScores(inputs, filebase)
                return_val = WriteScores(scores, filebase)
                connection.sent(f"{return_val}".encode("utf-8"))
            #connection.send(b"Hello, Client")
        except Exception as e:
            print(e)
            print(traceback.print_exc())
            connection.send(b"Error with request")
    connection.close()
        