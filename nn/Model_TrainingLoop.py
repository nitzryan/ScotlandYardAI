import matplotlib.pyplot as plt
import torch

def train(network,  data_generator, loss_function, optimizer, device, logging = 200, should_output=True):
  network.train() #updates any network layers that behave differently in training and execution
  avg_loss = 0
  num_batches = 0
  for batch, (data, length, target) in enumerate(data_generator):
    data, length, target = data.to(device), length.to(device), target.to(device)
    optimizer.zero_grad()
    output = network(data, length)
    loss = loss_function(output, target, length)
    loss.backward()
    optimizer.step()
    avg_loss += loss.item()
    num_batches += 1
    if should_output and ((batch+1)%logging == 0): print('Batch [%d/%d], Train Loss: %.4f' %(batch+1, len(data_generator.dataset)/len(output), avg_loss/num_batches))
  
  avg_loss /= num_batches
  
  return avg_loss

def test(network, test_loader, loss_function, device):
  network.eval() #updates any network layers that behave differently in training and execution
  avg_loss = 0
  num_batches = 0
  with torch.no_grad():
    for data, length, target in test_loader:
      data, length, target = data.to(device), length.to(device), target.to(device)
      output = network(data, length)
      loss = loss_function(output, target, length)
      avg_loss += loss.item()
      num_batches += 1
  
  avg_loss /= num_batches
  
  #print('\nTest set: Avg. loss: {:.4f})\n'.format(test_loss))
  return avg_loss

def count_parameters(model):
    return sum(p.numel() for p in model.parameters() if p.requires_grad)

def logResults(epoch, num_epochs, train_loss, test_loss, print_interval=1000, should_output=True):
  if should_output and (epoch%print_interval == 0):  
    print('Epoch [%d/%d], Train Loss: %.4f, Test Loss: %.4f' %(epoch+1, num_epochs, train_loss, test_loss))

def graphLoss(epoch_counter, train_loss_hist, test_loss_hist, loss_name="Loss", start = 0, graph_y_range=None, title=""):
  fig = plt.figure()
  plt.plot(epoch_counter[start:], train_loss_hist[start:], color='blue')
  plt.plot(epoch_counter[start:], test_loss_hist[start:], color='red')
  plt.title(title)
  if graph_y_range is not None:
    plt.ylim(graph_y_range)
  plt.legend(['Train Loss', 'Test Loss'], loc='upper right')
  plt.xlabel('#Epochs')
  plt.ylabel(loss_name)

def trainAndGraph(network, training_generator, testing_generator, loss_function, optimizer, scheduler, num_epochs, device, logging_interval=1, early_stopping_cutoff=20, should_output=True, graph_y_range=None, model_name="no_name.pt"):
  #Arrays to store training history
  test_loss_history = []
  epoch_counter = []
  train_loss_history = []
  last_loss = 999999
  best_loss = 999999
  best_epoch = 0
  epochsSinceLastImprove = 0
  
  for epoch in range(num_epochs):
    avg_loss = train(network, training_generator, loss_function, optimizer, should_output=should_output)
    test_loss = test(network, testing_generator, loss_function)
    scheduler.step(test_loss[0])
    logResults(epoch, num_epochs, avg_loss, test_loss, logging_interval, should_output)
    
    train_loss_history.append(avg_loss)
    test_loss_history.append(test_loss)
    epoch_counter.append(epoch)
    
    if (test_loss[0] < best_loss):
      best_loss = test_loss[0]
      best_epoch = epoch
      torch.save(network.state_dict(), model_name)
      epochsSinceLastImprove = 0
    else:
      epochsSinceLastImprove += 1
      
    if epochsSinceLastImprove >= early_stopping_cutoff:
      if should_output:
        print("Stopped Training Early")
      break

  if should_output:
    print(f"Best result at epoch={best_epoch} with loss={best_loss}")
    graphLoss(epoch_counter, train_loss_history, test_loss_history, graph_y_range=graph_y_range, title="Loss")
  return best_loss