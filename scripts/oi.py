import os
import time
from typing import Any
import matplotlib.pyplot as plt
import numpy as np
import torch
import torchvision
import torchvision.transforms as transforms
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim


def imshow(img):
    img = img / 2 + 0.5  # unnormalize
    npimg = img.numpy()
    plt.imshow(np.transpose(npimg, (1, 2, 0)))
    plt.show()


class Net(nn.Module):
    def __init__(self):
        super(Net, self).__init__()
        self.conv1 = nn.Conv2d(3, 6, 5)
        self.pool = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(6, 16, 5)
        self.fc1 = nn.Linear(16 * 5 * 5, 120)
        self.fc2 = nn.Linear(120, 84)
        self.fc3 = nn.Linear(84, 10)

    def forward(self, x):
        x = self.pool(F.relu(self.conv1(x)))
        x = self.pool(F.relu(self.conv2(x)))
        x = x.view(-1, 16 * 5 * 5)
        x = F.relu(self.fc1(x))
        x = F.relu(self.fc2(x))
        x = self.fc3(x)
        return x


def train(savePath, epochs):
    net = Net()
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(net.parameters(), lr=0.001, momentum=0.9)
    os.system('echo "{}" > /root/cxy/trainprogress.txt'.format(epochs))
    for epoch in range(epochs):  # loop over the dataset multiple times

        running_loss = 0.0
        for i, data in enumerate(trainloader, 0):
            # get the inputs; data is a list of [inputs, labels]
            inputs, labels = data

            # zero the parameter gradients
            optimizer.zero_grad()

            # forward + backward + optimize
            outputs = net(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()

            # print statistics
            running_loss += loss.item()
            # if i % 2000 == 1999:  # print every 2000 mini-batches
            #     print('[%d, %5d] loss: %.3f' %
            #           (epoch + 1, i + 1, running_loss / 2000))
            #     running_loss = 0.0
        hit_rate = test(False, net)
        os.system('echo "{}" >> /root/cxy/trainprogress.txt'.format(hit_rate))
    print('Finished Training')
    torch.save(net.state_dict(), savePath)


def test(isLoad: bool, net, modelPath: str = '') -> float:
    if isLoad:
        net = Net()
        net.load_state_dict(torch.load(modelPath))

    dataiter = iter(testloader)
    hit = 0
    total = 0
    for data in dataiter:
        images, labels = data

        outputs = net(images)
        _, predicted = torch.max(outputs, 1)
        # print(outputs)
        # print(_)
        # print(predicted)
        for i in range(4):
            total += 1
            if labels[i] == predicted[i]:
                hit += 1
        # print('GroundTruth: ', ' '.join('%5s' % classes[labels[j]] for j in range(4)))
        # print('Predicted: ', ' '.join('%5s' % classes[predicted[j]]
        #                               for j in range(4)))
    print('hit rate: {:.4f}%'.format(hit * 100 / total))
    return hit / total


if __name__ == '__main__':
    transform = transforms.Compose([transforms.ToTensor(),
                                    transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])

    CIFAR_PATH = '.' if 'Darwin' in os.uname() else '/root/cxy/cifar10'
    trainset = torchvision.datasets.CIFAR10(root=CIFAR_PATH, transform=transform)
    trainloader = torch.utils.data.DataLoader(trainset, batch_size=4,
                                              shuffle=True, num_workers=2)

    testset = torchvision.datasets.CIFAR10(root=CIFAR_PATH, train=False,
                                           transform=transform)
    testloader = torch.utils.data.DataLoader(testset, batch_size=4,
                                             shuffle=False, num_workers=2)

    classes = ('plane', 'car', 'bird', 'cat',
               'deer', 'dog', 'frog', 'horse', 'ship', 'truck')

    NET_PATH = './cifar_net'

    start = time.perf_counter()
    train(NET_PATH, 5)
    # test(True, None, NET_PATH)
    print("{:.4f}s".format(time.perf_counter() - start))

