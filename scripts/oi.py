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

# import ssl
# ssl._create_default_https_context = ssl._create_unverified_context

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
    # net = Net()
    net = torchvision.models.resnet34(pretrained=True)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(net.parameters(), lr=1e-3, momentum=0.9)
    os.system('echo "{}" > /root/cxy/trainprogress.txt'.format(epochs))
    for epoch in range(epochs):  # loop over the dataset multiple times

        running_loss = 0.0
        start = time.perf_counter()
        net.train()
        for i, data in enumerate(trainloader, 0):
            # get the inputs; data is a list of [inputs, labels]
            inputs, labels = data
            optimizer.zero_grad()
            outputs = net(inputs)
            loss = criterion(outputs, labels)
            loss.backward()
            optimizer.step()

            running_loss += loss.item()
            if i % 20 == 19:  # print every 20 mini-batches
                print('[%d, %5d] loss: %.3f' %
                      (epoch + 1, i + 1, running_loss / 20))
                running_loss = 0.0
        net.eval()
        accuracy = test(False, net)
        print(accuracy)
        os.system('echo "{}" >> /root/cxy/trainprogress.txt'.format(accuracy))

        duration = time.perf_counter() - start
        print("epoch time: {:.4f}s".format(duration))
    print('Finished Training')
    torch.save(net.state_dict(), savePath)


def test(isLoad: bool, net, modelPath: str = '') -> float:
    if isLoad:
        net = Net()
        net.load_state_dict(torch.load(modelPath))
    hit = 0
    total = 0
    for data in iter(testloader):
        images, labels = data
        batchSize = len(labels)

        outputs = net(images)
        _, predicted = torch.max(outputs, 1)
        total += batchSize
        for i in range(batchSize):
            if labels[i] == predicted[i]:
                hit += 1
    return hit / total


if __name__ == '__main__':
    transform = transforms.Compose([transforms.ToTensor(),
                                    transforms.Normalize((0.5, 0.5, 0.5), (0.5, 0.5, 0.5))])

    CIFAR_PATH = '.' if 'Darwin' in os.uname() else '/root/cxy/cifar10'
    trainset = torchvision.datasets.CIFAR10(root=CIFAR_PATH, transform=transform)
    trainloader = torch.utils.data.DataLoader(trainset, batch_size=128,
                                              shuffle=True, num_workers=2)
    testset = torchvision.datasets.CIFAR10(root=CIFAR_PATH, train=False,
                                           transform=transform)
    testloader = torch.utils.data.DataLoader(testset, batch_size=128,
                                             shuffle=False, num_workers=2)

    classes = ('plane', 'car', 'bird', 'cat',
               'deer', 'dog', 'frog', 'horse', 'ship', 'truck')

    NET_PATH = './cifar_net'

    train(NET_PATH, 100)
    # test(True, None, NET_PATH)

