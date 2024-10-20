import torch
import numpy as np
from Constants import D_TYPE, NUM_POINTS
import warnings

class MapDataset(torch.utils.data.Dataset):
    def __init__(self, data, lengths, labels):
        self.data = data
        self.lengths = lengths
        with warnings.catch_warnings(): # Get warning for data copy, which is okay since this is only run once
            warnings.filterwarnings("ignore", category=UserWarning, message='.*non-contiguous.*')
            self.fugitive_tile = labels
            #self.fugitive_tile = torch.nn.functional.one_hot(labels, NUM_POINTS).to(D_TYPE)
        
    def __len__(self):
        return self.data.size(dim=0)
    
    def __getitem__(self, idx):
        return self.data[idx,:], self.lengths[idx], self.fugitive_tile[idx,:]
    
class ScoreDataset(torch.utils.data.Dataset):
    def __init__(self, data, labels):
        self.data = data
        self.labels = labels
        
    def __len__(self):
        return self.data.size(dim=0)
    
    def __getitem__(self, idx):
        return self.data[idx,:], self.labels[idx]