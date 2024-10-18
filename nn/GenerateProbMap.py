import torch
from Model_Training import MAP_HIDDEN_SIZE, MAP_LAYERS
from Models import Model_MapPredict

def GenerateProbMap(inputs, filebase):
    input_stacked = torch.stack(inputs)
    
    with open("nn/models/" + filebase + "_map_meanstds.txt", "r") as file:
        mean_line = file.readline().strip()
        means_list = [float(x) for x in mean_line.split()]
        std_line = file.readline().strip()
        stds_list = [float(x) for x in std_line.split()]
        
    means = torch.tensor(means_list)
    stds = torch.tensor(stds_list)
    input_normalized = [(el - means) / stds for el in input_stacked]
    input_tensor = torch.stack(input_normalized)
    input_tensor = input_tensor.unsqueeze(0)
    
    lengths = torch.tensor(input_tensor.size(1))
    lengths = lengths.unsqueeze(0)
    
    device = torch.device("cpu")
    network = Model_MapPredict(input_tensor.size(2), MAP_LAYERS, MAP_HIDDEN_SIZE)
    network.load_state_dict(torch.load("nn/models/" + filebase + "_map.pt", map_location=device))
    network.eval()
    network = network.to(device)
    
    probs = network(input_tensor, lengths)
    probs = probs.squeeze(0)
    probs = torch.nn.functional.softmax(probs, dim=1)
    probs = probs[-1,:]
    return probs[1:]