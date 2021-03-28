#include "signalhandler.h"
#include "utils/log.h"
#include <stdlib.h>

    
void SignalHandler::Init() {}

static void SignalHandlerBridge(int _sig) {
    SignalHandler::Instance().Handle(_sig);
}

SignalHandler::SignalHandler() = default;


int SignalHandler::RegisterCallback(int _sig, std::function<void()> _callback) {
    if (_sig < 1 || _sig > 31) {
        return -1;
    }
    ScopedLock lock(mutex_);
    auto find = callbacks_.find(_sig);
    if (find == callbacks_.end()) {
        ::signal(_sig, SignalHandlerBridge);
    }
    callbacks_[_sig].push_back(_callback);
    return 0;
}

void SignalHandler::Handle(int _sig) {
    ScopedLock lock(mutex_);
    auto find = callbacks_.find(_sig);
    if (find != callbacks_.end()) {
        for (auto &callback : find->second) {
            callback();
        }
    }
}

// deprecated
void SignalHandler::__ProcessCrash() {
    // FIXME: LogI is not-reentrant
    LogI(__FILE__, "[__ProcessCrash] Process Crash!")
    exit(EXIT_FAILURE);
}

SignalHandler::~SignalHandler() {
    LogI(__FILE__, "[~SignalHandler]")
    
}
