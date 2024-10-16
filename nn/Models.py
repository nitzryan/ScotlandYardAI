import torch
import torch.nn as nn
import torch.nn.functional as F
from Dataset import NUM_POINTS, NUM_OUTCOMES

class Model_MapPredict(nn.Module):
    def __init__(self, gamestate_size, num_layers, hidden_size):
        super().__init__()
        
        self.rnn = nn.RNN(input_size=gamestate_size, hidden_size=hidden_size, num_layers=num_layers, batch_first=False)
        self.linear = nn.Linear(hidden_size, NUM_POINTS)
        
    def forward(self, x, lengths):
        if self.training:
            x += self.mutators[:x.size(0), :x.size(1), :]
        # Apply lengths since they aren't the same for all entries
        lengths = lengths.to(torch.device("cpu")).long()
        packedInput = nn.utils.rnn.pack_padded_sequence(x, lengths, batch_first=True, enforce_sorted=False)
        
        # Generate Player State
        packedOutput, h_n = self.rnn(packedInput)
        output, _ = nn.utils.rnn.pad_packed_sequence(packedOutput, batch_first=True)
            
        # Generate War predictions
        output = self.linear(output)

        return output