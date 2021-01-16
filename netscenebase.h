#ifndef OI_SVR_NETSCENEBASE_H
#define OI_SVR_NETSCENEBASE_H
#include <string>
#include "unix_socket.h"


class NetSceneBase {

  public:
    virtual int GetType() = 0;
    
    virtual int DoScene(const std::string &_in_buffer) = 0;
    
    virtual ~NetSceneBase() {}
    
    void SetSocket(SOCKET _socket);

  protected:
    SOCKET      socket_;
    AutoBuffer  send_buff_;

  private:
  
};


#endif //OI_SVR_NETSCENEBASE_H
