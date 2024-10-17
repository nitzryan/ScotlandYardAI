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
        
    def __len__(self):
        return self.data.size(dim=0)
    
    def should_augment_data(self, should_augment):
        self.should_augment = should_augment
    
    def __getitem__(self, idx):
        return self.data[idx,:], self.lengths[idx], self.fugitive_tile[idx,:]
    
