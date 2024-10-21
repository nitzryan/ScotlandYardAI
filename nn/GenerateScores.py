import torch
import copy

from Models import Model_MapPredict, Model_WinnerPredict

MAP_INPUT_SIZE = 4
SCORE_INPUT_SIZE = 30

def GenerateScores(inputs, filebase):
    all_scores = []
    
    # Load Models
    use_cuda = torch.cuda.is_available()
    if (use_cuda):
        device = torch.device("cuda")
    else:
        device = torch.device("cpu")
    
    model_map = Model_MapPredict(MAP_INPUT_SIZE)
    model_map.load_state_dict(torch.load("nn/models/" + filebase + "_map.pt", map_location=device))
    model_map.eval()
    model_map = model_map.to(device)
    
    model_score = Model_WinnerPredict(SCORE_INPUT_SIZE)
    model_score.load_state_dict(torch.load("nn/models/" + filebase + "_score.pt", map_location=device))
    model_score.eval()
    model_score = model_score.to(device)
    
    for game_input in inputs:
        # Get Probability Map for current state of each game
        prob_map_hist = torch.stack(game_input[0])
        length = torch.tensor(prob_map_hist.size(0))
        with torch.nograd():
            prob_map = model_map(prob_map_hist.to(device), length)[-1,:]
        
        # Generate Score from probability map and current game state
        gamestate = game_input[1].to(device)
        score_input = torch.cat((prob_map, gamestate), dim=0)
        score_input_detectives = copy.deepcopy(score_input)
        score_input_detectives[-1] = 3
    
    return all_scores