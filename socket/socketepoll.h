#ifndef OI_SVR_SOCKETEPOLL_H
#define OI_SVR_SOCKETEPOLL_H

#include <sys/epoll.h>
#include <stddef.h>
#include "unix_socket.h"


class SocketEpoll {
  public:
    void operator=(const SocketEpoll &) = delete;
    SocketEpoll(const SocketEpoll &) = delete;
    
    static SocketEpoll &Instance() {
        static SocketEpoll instance;
        return instance;
    }
    ~SocketEpoll();
    
    void SetListenFd(SOCKET _listen_fd);
    
    int EpollWait(int _max_events = kMaxFds_, int _timeout_mills = -1);
    
    int AddSocketRead(SOCKET _fd);
    
    int DelSocket(SOCKET _fd);
    
    int ModSocket(SOCKET _fd, struct epoll_event *_event);
    
    int IsReadSet(int _idx);
    int IsWriteSet(int _idx);
    int IsErrSet(int _idx);
    
    bool IsNewConnect(int _idx);
    
    int GetErrNo() const;
    
    
  private:
    SocketEpoll(int _max_fds = 1024);
    
    /**
     * @param _idx: varies from 0 to the val as specifies by
     *              return val of EpollWait().
     * @param _flag: EPOLLIN, EPOLLOUT, EPOLLERR, etc.
     * @return: -1 if not, the fd if it's ready.
     */
    int __IsFlagSet(int _idx, int _flag);
    
    int __EpollCtl(int _op, SOCKET _fd, struct epoll_event *_event = NULL);
    
  private:
    int                     epoll_fd_;
    int                     listen_fd_;
    struct epoll_event*     epoll_events_;
    int                     errno_;
    const static int        kMaxFds_;
    
};


#endif //OI_SVR_SOCKETEPOLL_H
