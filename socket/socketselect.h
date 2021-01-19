#ifndef OI_SVR_SOCKETSELECT_H
#define OI_SVR_SOCKETSELECT_H

#include <sys/socket.h>


class SocketSelect {

  public:
    int Select();
    int Select(int _msec);


  private:
  
};


#endif //OI_SVR_SOCKETSELECT_H
