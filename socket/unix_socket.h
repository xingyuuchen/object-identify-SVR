#ifndef OI_SVR_UNIX_SOCKET_H
#define OI_SVR_UNIX_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../autobuffer.h"
#include "../log.h"
#include "../time/timeutil.h"

#include <vector>
#include <poll.h>
#include <errno.h>


#define SOCKET int
#define INVALID_SOCKET -1
#define CONNECT_FAILED -2

class UnixSocket {

  public:
    
    static size_t BlockSocketReceive(SOCKET _socket, AutoBuffer &_recv_buff,
                             size_t _buff_size, int _timeout_mills = 3000) {
        if (_timeout_mills < -1) { _timeout_mills = -1; }
        size_t available = _recv_buff.AvailableSize();
        if (available < _buff_size) {
            _recv_buff.AddCapacity(_buff_size - available);
        }
        
        ssize_t nrecv = 0;
        uint64_t start_time = GetCurrentTimeMillis();
        uint64_t cost_time = 0;
    
        std::vector<pollfd> pollfds;
        pollfd fd;
        fd.fd = _socket;
        fd.revents = 0;
        fd.events = POLLIN;
        pollfds.push_back(fd);
        
        while (true) {
            for (pollfd &fd : pollfds) {
                fd.revents = 0;
            }
            
            int poll_timeout = _timeout_mills > cost_time ? _timeout_mills - cost_time : 0;
            int ret = poll(&pollfds[0], (nfds_t) pollfds.size(), poll_timeout);
            
            if (ret < 0) {
                int errno_ = errno;
                Log("[BlockSocketReceive] poll errno: %d", errno_)
                return -1;
            } else if (ret == 0) {
                Log("[BlockSocketReceive] timeout, nrecv = %zd, poll_timeout = %d", nrecv, poll_timeout)
                return nrecv;
            } else {
                Log("[BlockSocketReceive] poll ret: %d", ret)
                for (int i = 0; i < pollfds.size(); ++i) {
                    if (pollfds[i].revents == 0) {
                        Log("[BlockSocketReceive] revents = 0")
                        continue;
                    }
                    if (pollfds[i].revents & POLLIN) {
                        ssize_t n = recv(_socket, _recv_buff.Ptr(_recv_buff.Length()),
                                         _buff_size - nrecv, 0);
                        
                        if (n > 0) {
                            _recv_buff.AddLength(n);
                            nrecv += n;
                            if (nrecv >= _buff_size) {
                                return nrecv;
                            }
                            
                        } else if (n == 0) {
                            /*  If no messages are available to be
                                received and the peer has performed an
                                orderly shutdown, the value 0 is
                                returned.
                             */
                            Log("[BlockSocketReceive] n == 0, nrecv = %zd", nrecv)
                        } else {
                            Log("[BlockSocketReceive] n:%zd, nrecv = %zd", n, nrecv)
                            return nrecv;
                        }
                    } else if (pollfds[i].revents & POLLERR) {
                        Log("[BlockSocketReceive] POLLERR, nrecv = %zd", nrecv)
                        return nrecv;
                    } else {
                        Log("[BlockSocketReceive] revents:%d", pollfds[i].revents)
                    }
                }
            }
            cost_time = GetCurrentTimeMillis() - start_time;
        }
    }
    
};


#endif //OI_SVR_UNIX_SOCKET_H
