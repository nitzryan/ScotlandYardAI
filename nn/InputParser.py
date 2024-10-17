import torch

def ParseLocationPredictionFile(filename):
    this_input = []
    this_output = 0
    
    game_inputs = []
    game_outputs= []
    
    all_inputs = []
    all_outputs = []
    
    inputs_done = False
    with open(filename, "rb") as file:
        while True:
            byte = file.read(1)
            byte = int.from_bytes(byte, byteorder='big')
            
            if byte == 255:
                break
            elif byte == 254:
                all_inputs.append(torch.stack(game_inputs))
                all_outputs.append(torch.tensor(game_outputs))
                game_inputs = []
                game_outputs = []
            elif byte == 253:
                game_inputs.append(torch.tensor(this_input, dtype=torch.float32))
                game_outputs.append(this_output)
                this_input = []
            elif byte == 252:
                inputs_done = True
            elif inputs_done:
                this_output = byte
                inputs_done = False
            else:
                this_input.append(byte)
    
    return all_inputs, all_outputs

def ParseSingleLocationPrediction():
    game_inputs = []
    this_inputs = []
    with open("nn/models/user_map_pred.txt", "rb") as file:
        while True:
            byte = file.read(1)
            byte = int.from_bytes(byte, byteorder='big')
            
            if byte == 255:
                break
            elif byte == 254:
                game_inputs.append(torch.tensor(this_inputs, dtype=torch.float32))
                this_inputs = []
            else:
                this_inputs.append(byte)
                
    return game_inputs