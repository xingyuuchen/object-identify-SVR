#ifndef OI_SVR_UNIX_SOCKET_H
#define OI_SVR_UNIX_SOCKET_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "autobuffer.h"


#define SOCKET int
#define INVALID_SOCKET -1
#define CONNECT_FAILED -2

class UnixSocket {

  public:
    static size_t BlockSocketReceive(SOCKET _socket, AutoBuffer &_recv_buff,
                                     size_t _buff_size, int _time_out_mills = 3000) {
        _recv_buff.AddCapacity(128);
        ssize_t len = recv(_socket, _recv_buff.Ptr(), _buff_size, 0);
        _recv_buff.SetLength(len);
        return len;
    }
    
    
};


#endif //OI_SVR_UNIX_SOCKET_H
