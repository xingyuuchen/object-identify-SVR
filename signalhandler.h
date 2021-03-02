#ifndef OI_SVR_SIGNALHANDLER_H
#define OI_SVR_SIGNALHANDLER_H
#include "utils/singleton.h"
#include <vector>
#include <functional>

class SignalHandler {
    
    SINGLETON(SignalHandler, )

  public:
    ~SignalHandler();
    
    int RegisterExitCallback(void (*func)());
    
    void ProcessExit();
    
    
  private:
    static const char *const        TAG;
    std::vector<void (*)()>         exit_callbacks_;
    
};


#endif //OI_SVR_SIGNALHANDLER_H
