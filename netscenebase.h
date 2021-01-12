#ifndef OI_SVR_NETSCENEBASE_H
#define OI_SVR_NETSCENEBASE_H
#include "autobuffer.h"
#include "unix_socket.h"


class NetSceneBase {

  public:
    virtual int GetType() = 0;
    
    virtual int DoScene(const AutoBuffer& _buffer) = 0;
    
    virtual ~NetSceneBase() {}
    
    void SetSocket(SOCKET _socket);

  protected:
    SOCKET      socket_;
    AutoBuffer  send_body_;

  private:
  
};


#endif //OI_SVR_NETSCENEBASE_H
