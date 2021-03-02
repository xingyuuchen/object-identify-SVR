#include "signalhandler.h"
#include "utils/log.h"


const char *const SignalHandler::TAG = "SignalHandler";

SignalHandler::SignalHandler() {
    LogI(TAG, "[SignalHandler]")
}

SignalHandler::~SignalHandler() {
    LogI(TAG, "[~SignalHandler]")
    ProcessExit();
}

int SignalHandler::RegisterExitCallback(void (*func)(void)) {
    exit_callbacks_.push_back(func);
    LogI(TAG, "[RegisterExitCallback] func: %p, n_callbacks: %zd",
         func, exit_callbacks_.size())
    return 0;
}

void SignalHandler::ProcessExit() {
    LogI(TAG, "[ProcessExit]")
    static bool has_processed = false;
    if (!has_processed) {
        LogI(TAG, "[ProcessExit] n_callbacks: %zd", exit_callbacks_.size())
        for (int i = 0; i < exit_callbacks_.size(); ++i) {
            LogI(TAG, "[ProcessExit] execute %p", exit_callbacks_[i])
            exit_callbacks_[i]();
        }
        has_processed = true;
    }
}
