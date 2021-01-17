#ifndef OI_SVR_UNIX_SOCKET_H
#define OI_SVR_UNIX_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "autobuffer.h"
#include "log.h"


#define SOCKET int
#define INVALID_SOCKET -1
#define CONNECT_FAILED -2

class UnixSocket {

  public:
    
    static size_t BlockSocketReceive(SOCKET _socket, AutoBuffer &_recv_buff,
                             size_t _buff_size, int _time_out_mills = 3000) {
        size_t available = _recv_buff.AvailableSize();
        if (available < _buff_size) {
            _recv_buff.AddCapacity(_buff_size - available);
        }
        ssize_t len = recv(_socket, _recv_buff.Ptr(_recv_buff.Length()), _buff_size, 0);
        if (len == 0) {
            /*  If no messages are available to be
                received and the peer has performed an
                orderly shutdown, the value 0 is
                returned.
             */
        } else if (len > 0) {
            _recv_buff.AddLength(len);
        } else {
            Log("[BlockSocketReceive] len = %zd", len)
        }
        return len;
    }
    
};


#endif //OI_SVR_UNIX_SOCKET_H
