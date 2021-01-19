#ifndef OI_SVR_UNIX_SOCKET_H
#define OI_SVR_UNIX_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../autobuffer.h"
#include "../log.h"

#include <vector>
#include <poll.h>
#include <errno.h>


#define SOCKET int
#define INVALID_SOCKET -1
#define CONNECT_FAILED -2

class UnixSocket {

  public:
    
    static size_t BlockSocketReceive(SOCKET _socket, AutoBuffer &_recv_buff,
                             size_t _buff_size, int _time_out_mills = 3000) {
        if (_time_out_mills < -1) { _time_out_mills = -1; }
        
        size_t available = _recv_buff.AvailableSize();
        if (available < _buff_size) {
            _recv_buff.AddCapacity(_buff_size - available);
        }
        ssize_t nrecv = 0;
    
        std::vector<pollfd> pollfds;
        pollfd fd;
        fd.fd = _socket;
        fd.revents = 0;
        fd.events = POLLIN;
        pollfds.push_back(fd);
//        while (true) {
            for (pollfd fd : pollfds) {
                fd.revents = 0;
            }
            // poll函数跟select一样，可以处理多路复用。可以通过设置关注的描述符事件，灵活等待事件的到来。
            int ret = poll(&pollfds[0], (nfds_t) pollfds.size(), _time_out_mills);
            if (ret < 0) {
                int errno_ = errno;
                Log("[BlockSocketReceive] poll errno: %d", errno_)
                return -1;
            } else if (ret == 0) {
                // 超时, 没有事件发生
                Log("[BlockSocketReceive] poll timeout")
                return nrecv;
            } else {
                Log("[BlockSocketReceive] poll ret: %d", ret)
                for (int i = 0; i < pollfds.size(); ++i) {
                    if (pollfds[i].revents == 0) {
                        continue;
                    }
                    if (pollfds[i].revents & POLLIN) {
                        nrecv = recv(_socket, _recv_buff.Ptr(_recv_buff.Length()), _buff_size, 0);
                        if (nrecv > 0) {
                            _recv_buff.AddLength(nrecv);
        
                        } else if (nrecv == 0) {
                            /*  If no messages are available to be
                                received and the peer has performed an
                                orderly shutdown, the value 0 is
                                returned.
                             */
                        } else {
                            Log("[BlockSocketReceive] len = %zd", nrecv)
                        }
                    } else {
                        Log("[BlockSocketReceive] revents[%d]: %d", i, pollfds[i].revents)
                    }
                }
            }
//        }
        return nrecv;
    }
    
};


#endif //OI_SVR_UNIX_SOCKET_H
