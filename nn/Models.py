import torch
import torch.nn as nn
import torch.nn.functional as F
import time
from Constants import NUM_POINTS, NUM_OUTCOMES

class Model_MapPredict(nn.Module):
    def __init__(self, input_size, num_layers, hidden_size):
        super().__init__()
        
        self.rnn = nn.RNN(input_size=input_size, hidden_size=hidden_size, num_layers=num_layers, batch_first=True)
        self.linear = nn.Linear(hidden_size, NUM_POINTS)
        
    def forward(self, x, lengths):
        # Apply lengths since they aren't the same for all entries
        
        lengths = lengths.to(torch.device("cpu")).long()
        packedInput = nn.utils.rnn.pack_padded_sequence(x, lengths, batch_first=True, enforce_sorted=False)
        
        # Generate Player State
        packedOutput, h_n = self.rnn(packedInput)
        
        output, _ = nn.utils.rnn.pad_packed_sequence(packedOutput, batch_first=True)
            
        # Generate War predictions
        output = self.linear(output)

        return output
    
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