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
    static NetSceneDispatcher& Instance() {
        static NetSceneDispatcher instance;
        return instance;
    }
    
    int Dispatch(SOCKET _conn_fd, const AutoBuffer* _buffer);
    
    ~NetSceneDispatcher();
    
    NetSceneDispatcher(NetSceneDispatcher const &)  = delete;
    
    void operator=(NetSceneDispatcher const &)      = delete;
    
    
  private:
    NetSceneDispatcher();
    
  private:
    std::vector<NetSceneBase *>     selectors_;
    std::mutex                      mutex_;
    
};


#endif //OI_SVR_NETSCENEDISPATCHER_H
