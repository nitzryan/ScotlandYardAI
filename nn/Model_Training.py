import torch
from sklearn.model_selection import train_test_split # type: ignore
from torch.optim import lr_scheduler

from Dataset import MapDataset
from Models import Model_MapPredict, RNN_Classification_Loss
from Model_TrainingLoop import trainAndGraph

def TrainMap(map_inputs, map_outputs, model_name):
    torch.set_printoptions(threshold=float('inf'))
    
    map_inputs_onelist = torch.cat(map_inputs)
    means = torch.mean(map_inputs_onelist, dim=0)
    stds = torch.std(map_inputs_onelist, dim=0)
    stds = torch.where(stds == 0, torch.tensor(1), stds)
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
    num_layers = 3
    hidden_size = 300
    network = Model_MapPredict(input_size, num_layers, hidden_size)
    network = network.to(device)
    
    optimizer = torch.optim.Adam(network.parameters(), lr=0.002)
    scheduler = lr_scheduler.ReduceLROnPlateau(optimizer, factor=0.5, patience=20, cooldown=5, verbose=False)
    loss_function = RNN_Classification_Loss
    
    num_epochs = 400
    loss = trainAndGraph(network, 
                         train_generator, 
                         test_generator, 
                         loss_function, 
                         optimizer,
                         scheduler, 
                         num_epochs, 
                         device,
                         logging_interval=1, 
                         early_stopping_cutoff=40, 
                         should_output=True,
                         model_name="nn/models/" + model_name + ".pt")
    return loss