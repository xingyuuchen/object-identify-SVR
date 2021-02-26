#ifndef OI_SVR_NETSCENEBASE_H
#define OI_SVR_NETSCENEBASE_H
#include <string>
#include <string.h>
#include <map>
#include <memory>
#include "../socket/unix_socket.h"
#include "../utils/autobuffer.h"

/**
 * Base class for all NetScene.
 */
class NetSceneBase {

  public:
    NetSceneBase();
    
    virtual int GetType() = 0;
    
    virtual std::shared_ptr<NetSceneBase> NewInstance() = 0;
    
    virtual int DoSceneImpl(const std::string &_in_buffer) = 0;
    
    virtual ~NetSceneBase() {}
    
    void SetSocket(SOCKET _socket);
    
    int DoScene(const std::string &_in_buffer);
    
    int PackAndSend();
    
    void CopyRespToSendBody(std::string &_resp, size_t _size);

  private:
    void __ShowHttpHeader(AutoBuffer &_out);
    
  protected:
    SOCKET                              socket_;
    AutoBuffer                          send_body_;
    int                                 status_code_;
    std::string                         status_desc_;
    std::map <std::string, std::string> http_headers_;

};


#endif //OI_SVR_NETSCENEBASE_H
