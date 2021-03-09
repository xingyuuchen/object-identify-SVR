#ifndef OI_SVR_NETSCENEDISPATCHER_H
#define OI_SVR_NETSCENEDISPATCHER_H
#include "netscenebase.h"
#include "singleton.h"
#include <vector>
#include <mutex>


/**
 * 业务分发枢纽
 */
class NetSceneDispatcher {
    
    SINGLETON(NetSceneDispatcher, )
    
  public:
    ~NetSceneDispatcher();
    
    NetSceneBase *Dispatch(SOCKET _conn_fd, const AutoBuffer *_buffer);
    
  private:
    
    NetSceneBase *__MakeNetScene(int _type);
    
  private:
    static const char *const        TAG;
    std::vector<NetSceneBase *>     selectors_;
    std::mutex                      mutex_;
    
};


#endif //OI_SVR_NETSCENEDISPATCHER_H
