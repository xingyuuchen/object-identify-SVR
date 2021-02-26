#ifndef OI_SVR_NETSCENEDISPATCHER_H
#define OI_SVR_NETSCENEDISPATCHER_H
#include "netscenebase.h"
#include <vector>
#include <mutex>


/**
 * 业务分发枢纽
 */
class NetSceneDispatcher {
  public:
    ~NetSceneDispatcher();
    NetSceneDispatcher(NetSceneDispatcher const &)  = delete;
    void operator=(NetSceneDispatcher const &)      = delete;
    
    static NetSceneDispatcher& Instance() {
        static NetSceneDispatcher instance;
        return instance;
    }
    
    NetSceneBase *Dispatch(SOCKET _conn_fd, const AutoBuffer *_buffer);
    
  private:
    NetSceneDispatcher();
    
    NetSceneBase *__MakeNetScene(int _type);
    
  private:
    std::vector<NetSceneBase *>     selectors_;
    std::mutex                      mutex_;
    
};


#endif //OI_SVR_NETSCENEDISPATCHER_H
