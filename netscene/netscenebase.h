#ifndef OI_SVR_NETSCENEBASE_H
#define OI_SVR_NETSCENEBASE_H
#include <string>
#include <string.h>
#include <map>
#include "../socket/unix_socket.h"
#include "../utils/autobuffer.h"

/**
 * Base class for all NetScenes.
 *
 * Responsible for:
 *      1. Managing the net scene type;
 *      2. Packing htp body into http message;
 *      3. Implementing the business logic by subclasses themselves.
 * Note:
 *      1. NOT responsible for any network operation(recv, send, etc.).
 */
class NetSceneBase {

  public:
    NetSceneBase();
    
    virtual int GetType() = 0;
    
    virtual NetSceneBase *NewInstance() = 0;
    
    virtual int DoSceneImpl(const std::string &_in_buffer) = 0;
    
    virtual ~NetSceneBase() {}
    
    void SetSocket(SOCKET _socket);
    
    int DoScene(const std::string &_in_buffer);
    
    int PackHttpMsg();
    
    void CopyRespToSendBody(std::string &_resp, size_t _size);
    
    AutoBuffer *GetHttpResp();
    
    SOCKET GetSocket() const;
    
  private:
    void __ShowHttpHeader(AutoBuffer &_out);
    
  protected:
    SOCKET                              socket_;
    AutoBuffer                          resp_body_;
    AutoBuffer                          resp_msg_;
    int                                 status_code_;
    std::string                         status_desc_;
    std::map <std::string, std::string> http_headers_;

};


#endif //OI_SVR_NETSCENEBASE_H
