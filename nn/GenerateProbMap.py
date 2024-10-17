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
    print(input_tensor.shape)
    input_tensor = input_tensor.unsqueeze(0)
    print(input_tensor.shape)
    
    lengths = torch.tensor(input_tensor.size(1))
    lengths = lengths.unsqueeze(0)
    print(lengths.shape)
    
    device = torch.device("cpu")
    network = Model_MapPredict(input_tensor.size(2), MAP_LAYERS, MAP_HIDDEN_SIZE)
    network.load_state_dict(torch.load("nn/models/" + filebase + "_map.pt", map_location=device))
    network.eval()
    network = network.to(device)
    
    probs = network(input_tensor, lengths)
    print(probs.shape)
    probs = probs.squeeze(0)
    print(probs.shape)
    probs = torch.nn.functional.softmax(probs, dim=1)
    print(probs.shape)
    probs = probs[-1,:]
    print(probs.shape)
    return probs