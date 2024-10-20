import torch
from sklearn.model_selection import train_test_split # type: ignore
from torch.optim import lr_scheduler

from Dataset import MapDataset, ScoreDataset
from Models import Model_MapPredict, Model_WinnerPredict, RNN_Classification_Loss
from Model_TrainingLoop_Lengths import trainAndGraph_Lengths
from Model_TrainingLoop import trainAndGraph

MAP_HIDDEN_SIZE = 600
MAP_LAYERS = 4

def TrainMap(map_inputs, map_outputs, model_name):
    torch.set_printoptions(threshold=float('inf'))
    
    map_inputs_onelist = torch.cat(map_inputs)
    means = torch.mean(map_inputs_onelist, dim=0)
    stds = torch.std(map_inputs_onelist, dim=0)
    stds = torch.where(stds == 0, torch.tensor(1), stds)
    with open("nn/models/" + model_name + "_meanstds.txt", "w") as file:
        for i in range(means.size(0)):
            file.write(f"{means[i]} ")
        file.write("\n")
        for i in range(stds.size(0)):
            file.write(f"{stds[i]} ")
    
    map_inputs = [(el - means) / stds for el in map_inputs]
    
    x_train, x_test, y_train, y_test = train_test_split(map_inputs, map_outputs, test_size=0.25, random_state=4980)
    train_lengths = torch.tensor([len(seq) for seq in x_train])
    test_lengths = torch.tensor([len(seq) for seq in x_test])
    x_train_padded = torch.nn.utils.rnn.pad_sequence(x_train, batch_first=True)
    x_test_padded = torch.nn.utils.rnn.pad_sequence(x_test, batch_first=True)
    y_train_padded = torch.nn.utils.rnn.pad_sequence(y_train, batch_first=True)
    y_test_padded = torch.nn.utils.rnn.pad_sequence(y_test, batch_first=True)
    
    train_dataset = MapDataset(x_train_padded, train_lengths, y_train_padded)
    test_dataset = MapDataset(x_test_padded, test_lengths, y_test_padded)
    
    
    batch_size = 800
    train_generator = torch.utils.data.DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
    test_generator = torch.utils.data.DataLoader(test_dataset, batch_size=batch_size, shuffle=False)
    
    use_cuda = torch.cuda.is_available()
    if (use_cuda):
        device = torch.device("cuda")
    else:
        device = torch.device("cpu")
    
    input_size = x_train_padded.size(2)
    network = Model_MapPredict(input_size)
    network = network.to(device)
    
    optimizer = torch.optim.Adam(network.parameters(), lr=0.0004)
    scheduler = lr_scheduler.ReduceLROnPlateau(optimizer, factor=0.8, patience=20, cooldown=5, verbose=False)
    loss_function = RNN_Classification_Loss
    #loss_function = L1_Classification_Loss
    
    num_epochs = 4000
    loss = trainAndGraph_Lengths(network, 
                         train_generator, 
                         test_generator, 
                         loss_function, 
                         optimizer,
                         scheduler, 
                         num_epochs, 
                         device,
                         logging_interval=10, 
                         early_stopping_cutoff=100, 
                         should_output=True,
                         model_name="nn/models/" + model_name + ".pt")
    return loss

def TrainScore(x_map, x_score, results, filebase):
    torch.set_printoptions(threshold=float('inf'))
    
    # Generate predictions on fugitive location from Map.pt
    with open("nn/models/" + filebase + "_map_meanstds.txt", "r") as file:
        mean_line = file.readline().strip()
        means_list = [float(x) for x in mean_line.split()]
        std_line = file.readline().strip()
        stds_list = [float(x) for x in std_line.split()]
        
    means = torch.tensor(means_list)
    stds = torch.tensor(stds_list)
    for i in range(len(x_map)):
        x_map[i] = (x_map[i] - means) / stds
    
    map_lengths = torch.tensor([len(seq) for seq in x_map])
    map_padded = torch.nn.utils.rnn.pad_sequence(x_map, batch_first=True)
    
    use_cuda = torch.cuda.is_available()
    if (use_cuda):
        device = torch.device("cuda")
    else:
        device = torch.device("cpu")
    input_size = map_padded.size(2)
    network = Model_MapPredict(input_size)
    network.load_state_dict(torch.load("nn/models/" + filebase + "_map.pt", map_location=device))
    network.eval()
    network = network.to(device)
    with torch.no_grad():
        map_probs = network(map_padded.to(device), map_lengths).to(torch.device("cpu"))
    
    map_probs_list = [map_probs[i, :map_lengths[i]] for i in range(map_lengths.size(0))]
    
    # Normalize X_Score
    x_score_onelist = torch.cat(x_score)
    means = torch.mean(x_score_onelist, dim=0)
    stds = torch.std(x_score_onelist, dim=0)
    stds = torch.where(stds == 0, torch.tensor(1), stds)
    with open("nn/models/" + filebase + "_score_meanstds.txt", "w") as file:
        for i in range(means.size(0)):
            file.write(f"{means[i]} ")
        file.write("\n")
        for i in range(stds.size(0)):
            file.write(f"{stds[i]} ")
    
    x_score_norm = [(el - means) / stds for el in x_score]
    
    total_xs = []
    all_ys = []
    for i in range(len(map_probs_list)):
        total_xs.append(torch.cat((map_probs_list[i], x_score_norm[i]), dim=1))
        for _ in range(map_probs_list[i].size(0)):
            all_ys.append(torch.tensor(results[i]))
    
    all_xs = torch.cat(total_xs, dim=0)
    all_ys = torch.stack(all_ys)
    
    # Train on data
    x_train, x_test, y_train, y_test = train_test_split(all_xs, all_ys, test_size=0.25, random_state=4980)
    train_dataset = ScoreDataset(x_train, y_train)
    test_dataset = ScoreDataset(x_test, y_test)
    
    batch_size = 800
    train_generator = torch.utils.data.DataLoader(train_dataset, batch_size=batch_size, shuffle=True)
    test_generator = torch.utils.data.DataLoader(test_dataset, batch_size=batch_size, shuffle=False)
    
    network = Model_WinnerPredict(x_train.size(1))
    network = network.to(device)
    optimizer = torch.optim.Adam(network.parameters(), lr=0.0004)
    scheduler = lr_scheduler.ReduceLROnPlateau(optimizer, factor=0.8, patience=20, cooldown=5, verbose=False)
    loss_function = torch.nn.CrossEntropyLoss(reduction='mean')
    
    num_epochs = 4000
    loss = trainAndGraph(network, 
                         train_generator, 
                         test_generator, 
                         loss_function, 
                         optimizer,
                         scheduler, 
                         num_epochs, 
                         device,
                         logging_interval=10, 
                         early_stopping_cutoff=100, 
                         should_output=True,
                         model_name="nn/models/" + filebase + "_score.pt")
    return loss