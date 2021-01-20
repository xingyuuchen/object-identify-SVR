#include "blocksocket.h"



size_t BlockSocketReceive(SOCKET _socket, AutoBuffer &_recv_buff,
                          SocketPoll &_socket_poll,
                          size_t _buff_size,
                          int _timeout_mills/* = 5000*/) {
    
    uint64_t start_time = GetCurrentTimeMillis();
    
    if (_timeout_mills < -1) { _timeout_mills = -1; }
    size_t available = _recv_buff.AvailableSize();
    if (available < _buff_size) {
        _recv_buff.AddCapacity(_buff_size - available);
    }
    
    ssize_t nrecv = 0;
    uint64_t cost_time = 0;
    
    
    while (true) {
        
        int poll_timeout = _timeout_mills > cost_time ? _timeout_mills - cost_time : 0;
        int ret = _socket_poll.Poll(poll_timeout);
        
        if (ret < 0) {
            int errno_ = _socket_poll.GetErrno();
            Log("[BlockSocketReceive] poll errno: %d", errno_)
            return -1;
        } else if (ret == 0) {
            Log("[BlockSocketReceive] timeout, nrecv = %zd, poll_timeout = %d", nrecv, poll_timeout)
            return nrecv;
        } else {
            Log("[BlockSocketReceive] poll ret: %d", ret)
            
            if (_socket_poll.IsReadSet(_socket)) {
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
            } else if (_socket_poll.IsErrSet(_socket)) {
                Log("[BlockSocketReceive] POLLERR, nrecv = %zd", nrecv)
                return nrecv;
            }
        }
        cost_time = GetCurrentTimeMillis() - start_time;
    }
    
}

