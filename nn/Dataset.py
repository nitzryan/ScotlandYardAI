import torch
import numpy as np
from Constants import D_TYPE, NUM_POINTS
import copy

class MapDataset(torch.utils.data.Dataset):
    def __init__(self, data, lengths, labels):
        self.data = data
        self.lengths = lengths
        self.fugitive_tile = labels
        
    def __len__(self):
        return self.data.size(dim=0)
    
    def __getitem__(self, idx):
        return self.data[idx,:], self.lengths[idx], self.fugitive_tile[idx,:]
    
class ScoreDataset(torch.utils.data.Dataset):
    def __init__(self, data, labels):
        self.fugitive_data = data
        self.labels = labels
        self.detective_data = copy.deepcopy(data)
        self.detective_data[:,-1] = 3
        
    def __len__(self):
        return self.labels.size(dim=0)
    
    def __getitem__(self, idx):
        return torch.stack((self.fugitive_data[idx,:], self.detective_data[idx,:])), torch.stack((self.labels[idx], self.labels[idx]))