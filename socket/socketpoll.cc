#include "socketpoll.h"
#include <errno.h>



SocketPoll::SocketPoll()
    : errno_(0) {}


int SocketPoll::Poll() {
    return Poll(-1);
}


int SocketPoll::Poll(int _msec) {
    if (_msec < -1) { _msec = -1; }
    ClearEvents();
    
    int ret = poll(&pollfds_[0], (nfds_t) pollfds_.size(), _msec);
    if (ret < 0) {
        errno_ = errno;
    }
    return ret;
}

void SocketPoll::AddSocketToRead(SOCKET _socket) {
    if (_socket < 0) {
        return;
    }
    auto find = FindPollfd(_socket);
    if (find != pollfds_.end()) {
        find->events |= POLLIN;
        Log("[AddSocketToRead] socket:%d already added", _socket)
        return;
    }
    
    pollfd fd;
    fd.fd = _socket;
    fd.events = POLLIN;
    fd.revents = 0;
    pollfds_.push_back(fd);
}

void SocketPoll::AddSocketToWrite(SOCKET _socket) {
    if (_socket < 0) {
        return;
    }
    auto find = FindPollfd(_socket);
    if (find != pollfds_.end()) {
        find->events |= POLLOUT;
        Log("[AddSocketToWrite] %d already added", _socket)
        return;
    }
    
    pollfd fd;
    fd.fd = _socket;
    fd.events = POLLOUT;
    fd.revents = 0;
    pollfds_.push_back(fd);
}

void SocketPoll::ClearEvents() {
    for (pollfd &fd : pollfds_) {
        fd.revents = 0;
    }
}

bool SocketPoll::IsReadSet(SOCKET _socket) {
    auto find = FindPollfd(_socket);
    if (find == pollfds_.end()) {
        return false;
    }
    return find->revents & POLLIN;
}

bool SocketPoll::IsErrSet(SOCKET _socket) {
    auto find = FindPollfd(_socket);
    if (find == pollfds_.end()) {
        return false;
    }
    return find->revents & POLLERR;
}

void SocketPoll::RemoveSocket(SOCKET _socket) {
    auto find = FindPollfd(_socket);
    if (find != pollfds_.end()) {
        pollfds_.erase(find);
    }
}

int SocketPoll::GetErrno() const {
    return errno_;
}

