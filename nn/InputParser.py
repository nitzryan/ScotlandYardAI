import torch

MAP_PRED_LENGTH = 9

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

def ParseGamePredictionFile(filename):
    all_mappred_inputs = []
    all_score_inputs = []
    all_results = []
    game_mappred_inputs = []
    game_score_inputs = []
    turn_mappred_inputs = []
    turn_score_inputs = []
    with open(filename, "rb") as file:
        while True:
            byte = file.read(1)
            byte = int.from_bytes(byte, byteorder='big')
            
            if byte == 255:
                break
            elif byte == 254:
                all_mappred_inputs.append(torch.stack(game_mappred_inputs))
                all_score_inputs.append(torch.stack(game_score_inputs))
                game_mappred_inputs = []
                game_score_inputs = []
            elif byte == 253:
                all_results.append(0)
            elif byte == 252:
                all_results.append(1)
            elif byte == 251:
                game_mappred_inputs.append(torch.tensor(turn_mappred_inputs, dtype=torch.float32))
                game_score_inputs.append(torch.tensor(turn_score_inputs, dtype=torch.float32))
                turn_mappred_inputs = []
                turn_score_inputs = []
            elif len(turn_mappred_inputs) < MAP_PRED_LENGTH:
                turn_mappred_inputs.append(byte)
            else:
                turn_score_inputs.append(byte)
    
    return all_mappred_inputs, all_score_inputs, all_results

def ParseScoreRequestFile(filename):
    all_inputs = []
    game_inputs = [[], []]
    this_move_input = []
    this_map_inputs = []
    building_map = True
    with open(filename, "rb") as file:
        while True:
            byte = file.read(1)
            byte = int.from_bytes(byte, byteorder='big')
            
            if byte == 255:
                break
            elif byte == 254:
                all_inputs.append(game_inputs)
                game_inputs = [[],[]]
                building_map = True
            elif byte == 253:
                game_inputs[1].append(torch.tensor(this_move_input, dtype=torch.float32))
                this_move_input = []
            elif byte == 252:
                game_inputs[0].append(torch.tensor(this_map_inputs), dtype=torch.float32)
                this_map_inputs = []
            elif byte == 251:
                building_map = False
            elif building_map:
                this_map_inputs.append(byte)
            else:
                this_move_input.append(byte)
                
    return all_inputs