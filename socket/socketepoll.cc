#include "socketepoll.h"
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "../utils/log.h"


const int SocketEpoll::kMaxFds_ = 1024;

SocketEpoll::SocketEpoll(int _max_fds)
        : epoll_fd_(-1)
        , listen_fd_(-1)
        , epoll_events_(NULL)
        , errno_(0) {
    if (_max_fds <= 0) { return; }
    
    epoll_events_ = new struct epoll_event[_max_fds];
    
    int ret = epoll_create(_max_fds);
    if (ret < 0) {
        LogE("[SocketEpoll::SocketEpoll] epoll_create, ret = %d", ret)
        return;
    }
    epoll_fd_ = ret;
}


int SocketEpoll::AddSocketRead(int _fd) {
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = _fd;
    return __EpollCtl(EPOLL_CTL_ADD, _fd, &event);
}

int SocketEpoll::DelSocket(int _fd) { return __EpollCtl(EPOLL_CTL_DEL, _fd); }

int SocketEpoll::ModSocket(int _fd, struct epoll_event *_event) {
    return __EpollCtl(EPOLL_CTL_MOD, _fd, _event);
}

int SocketEpoll::__EpollCtl(int _op, int _fd, struct epoll_event *_event/* = NULL*/) {
    if (_fd < 0) {
        LogE("[SocketEpoll::__EpollCtl] fd_ < 0")
        return -1;
    }
    int ret = epoll_ctl(epoll_fd_, _op, _fd, _event);
    if (ret < 0) {
        errno_ = errno;
        LogE("[SocketEpoll::__EpollCtl] errno(%d): %s", errno_, strerror(errno))
    }
    return ret;
}


int SocketEpoll::EpollWait(int _max_events/* = kMaxFds_*/,
                           int _timeout_mills/* = -1*/) {
    if (_timeout_mills < -1) { _timeout_mills = -1; }
    
    int nfds = epoll_wait(epoll_fd_, epoll_events_, _max_events, _timeout_mills);
    if (nfds < 0) {
        errno_ = errno;
        LogE("[SocketEpoll::EpollWait] errno(%d): %s", errno_, strerror(errno))
    }
    return nfds;
}

bool SocketEpoll::IsNewConnect(int _idx) {
    if (_idx < 0 || _idx >= kMaxFds_) {
        LogE("[SocketEpoll::IsNewConnect] invalid _idx: %d", _idx)
        return false;
    }
    return epoll_events_[_idx].data.fd == listen_fd_;
}

int SocketEpoll::IsWriteSet(int _idx) { return __IsFlagSet(_idx, EPOLLOUT); }

int SocketEpoll::IsReadSet(int _idx) { return __IsFlagSet(_idx, EPOLLIN); }

int SocketEpoll::IsErrSet(int _idx) { return __IsFlagSet(_idx, EPOLLERR); }

int SocketEpoll::__IsFlagSet(int _idx, int _flag) {
    if (_idx < 0 || _idx >= kMaxFds_) {
        LogE("[SocketEpoll::__IsFlagSet] invalid _idx: %d", _idx)
        return -1;
    }
    if (epoll_events_[_idx].events & _flag) {
        return epoll_events_[_idx].data.fd;
    }
    return -1;
}


void SocketEpoll::SetListenFd(int _listen_fd) {
    if (_listen_fd < 0) {
        LogE("[SocketEpoll::SetListenFd] _listen_fd: %d", _listen_fd)
        return;
    }
    AddSocketRead(_listen_fd);
    listen_fd_ = _listen_fd;
}

int SocketEpoll::GetErrNo() const { return errno_; }

SocketEpoll::~SocketEpoll() {
    if (epoll_events_ != NULL) {
        delete[] epoll_events_;
    }
    if (epoll_fd_ != -1) {
        LogI("[~SocketEpoll] close epfd")
        ::close(epoll_fd_);
    }
}
