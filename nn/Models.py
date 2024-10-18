import torch
import torch.nn as nn
import torch.nn.functional as F
from Constants import NUM_POINTS, NUM_OUTCOMES, D_TYPE

def is_start(x):
    values = [13, 26, 29, 34, 50, 53, 91, 94, 103, 112, 117, 132, 138, 141, 155, 174, 197, 198]
    return x in values

class ResBlock(nn.Module):
    def __init__(self, size):
        super().__init__()
        self.linear1 = nn.Linear(size, size)
        self.linear2 = nn.Linear(size, size)
        self.linear3 = nn.Linear(size, size)
            
    def forward(self, x):
        origX = x
        x = self.linear1(x)
        x = F.relu(x)
        x = self.linear2(x)
        x = F.relu(x)
        x = self.linear3(x)
        x = F.relu(x)
        x = x + origX
        return x

class Model_MapPredict(nn.Module):
    def __init__(self, input_size, num_layers, hidden_size):
        super().__init__()
        self.h_0 = torch.tensor([10 if is_start(x) else -10 for x in range(NUM_POINTS)], dtype=D_TYPE)
        self.W_ih = nn.Linear(NUM_POINTS, NUM_POINTS) # input size, hidden size
        self.W_hh = nn.Linear(NUM_POINTS, NUM_POINTS) # hidden size, hidden size
        
        self.linear1 = nn.Linear(input_size, NUM_POINTS // 4)
        self.resblock1 = ResBlock(NUM_POINTS // 4)
        self.linear2 = nn.Linear(NUM_POINTS // 4, NUM_POINTS // 2)
        self.resblock2 = ResBlock(NUM_POINTS // 2)
        self.linear3 = nn.Linear(NUM_POINTS // 2, NUM_POINTS)
        self.rnn = nn.RNN(input_size=NUM_POINTS, hidden_size=NUM_POINTS, num_layers=1, batch_first=True)
        # self.linear1 = nn.Linear(hidden_size, hidden_size // 2)
        # self.linear2 = nn.Linear(hidden_size // 2, NUM_POINTS)
        
    def forward(self, x, lengths):
        x = F.relu(self.linear1(x))
        x = self.resblock1(x)
        x = F.relu(self.linear2(x))
        x = self.resblock2(x)
        x = F.relu(self.linear3(x))
        
        # RNN without a tanh, since we want to directly use output
        x = x.transpose(0, 1)
        seq_len, batch_size, _ = x.size()
        h_0 = self.h_0.to(x.device).unsqueeze(0).expand(x.size(0), 1, NUM_POINTS)
        h_t_minus_1 = h_0
        h_t = h_0
        output = []
        for t in range(seq_len):
            h_t = self.W_ih(x[t]) + self.W_hh(h_t_minus_1)
            output.append(h_t[-1])
            h_t_minus_1 = h_t
        output = torch.stack(output)
        output = output.transpose(0, 1)
        # lengths = lengths.to(torch.device("cpu")).long()
        # 
        # packedInput = nn.utils.rnn.pack_padded_sequence(x, lengths, batch_first=True, enforce_sorted=False)
        # packedOutput, h_n = self.rnn(packedInput, h_0)
        
        # output, _ = nn.utils.rnn.pad_packed_sequence(packedOutput, batch_first=True)
            
        output = x

        return output
    
def L1_Classification_Loss(pred, actual, lengths):
    pred = torch.nn.functional.softmax(pred, dim=2)
    # Reshape into format required by CrossEntropyLoss
    actual = actual[:,:pred.size(1)]
    
    batch_size = actual.size(0)
    time_steps = actual.size(1)
    
    num_classes = pred.size(2)
    
    actual = actual.reshape((batch_size * time_steps, num_classes))
    
    pred = pred.reshape((batch_size * time_steps, num_classes))
    l = nn.L1Loss(reduction='none')
    loss = l(pred, actual)
    
    # Reshape back into format to apply mask for actual valid predictions
    loss = loss.reshape((batch_size, time_steps, num_classes))
    loss = loss.sum(dim=2)
    
    # Mask based off lenghts of actual predictions
    batch_size, max_steps = loss.size()
    mask = torch.arange(max_steps, device=lengths.device).unsqueeze(0) < lengths.unsqueeze(1)
    masked_loss = loss * mask
    
    # Calculate average loss of each entry (although not sure if this is actually good)
    #print(masked_loss)
    loss_sums = masked_loss.sum(dim=1)
    #print(loss_sums.shape)
    lengths = lengths.float()
    loss_mean = loss_sums / lengths.unsqueeze(1)
    
    return loss_mean.mean()
    
def RNN_Classification_Loss(pred, actual, lengths):
    # Reshape into format required by CrossEntropyLoss
    actual = actual[:,:pred.size(1)]
    
    batch_size = actual.size(0)
    time_steps = actual.size(1)
    
    num_classes = pred.size(2)
    
    actual = actual.reshape((batch_size * time_steps,))
    
    pred = pred.reshape((batch_size * time_steps, num_classes))
    
    # Calculate Loss for each time step (regardless of whether it was actually used)
    l = nn.CrossEntropyLoss(reduction='none')
    loss = l(pred, actual)
    
    # Reshape back into format to apply mask for actual valid predictions
    loss = loss.reshape((batch_size, time_steps))
    
    # Mask based off lenghts of actual predictions
    batch_size, max_steps = loss.size()
    mask = torch.arange(max_steps, device=lengths.device).unsqueeze(0) < lengths.unsqueeze(1)
    masked_loss = loss * mask
    
    # Calculate average loss of each entry (although not sure if this is actually good)
    #print(masked_loss)
    loss_sums = masked_loss.sum(dim=1)
    #print(loss_sums.shape)
    lengths = lengths.float()
    loss_mean = loss_sums / lengths.unsqueeze(1)
    
    return loss_mean.mean()