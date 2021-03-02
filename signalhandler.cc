#include "signalhandler.h"
#include "utils/log.h"
#include <stdlib.h>
#include <signal.h>


IProcessExitListener::IProcessExitListener(void (*_on_process_exit)())
    : OnProcessExit(_on_process_exit) { }


const char *const SignalHandler::TAG = "SignalHandler";

void SignalHandler::Init() {}

void SignalHandlerBridge(int _sig) {
    SignalHandler::Instance().Handle(_sig);
}

SignalHandler::SignalHandler() {
    LogI(TAG, "[SignalHandler]")
    
    ::signal(SIGINT, SignalHandlerBridge);
    ::signal(SIGQUIT, SignalHandlerBridge);
    ::signal(SIGSEGV, SignalHandlerBridge);
    ::signal(SIGBUS, SignalHandlerBridge);
    ::signal(SIGFPE, SignalHandlerBridge);
    ::signal(SIGSYS, SignalHandlerBridge);
}

int SignalHandler::RegisterExitCallback(IProcessExitListener *_listener) {
    process_exit_listeners_.push_back(_listener);
    LogI(TAG, "[RegisterExitCallback] n_callbacks: %zd",
         process_exit_listeners_.size())
    return 0;
}

void SignalHandler::Handle(int _sig) {
    LogI(TAG, "[ProcessExit] sig(%d)", _sig)
    switch (_sig) {
        case SIGINT:
        case SIGQUIT:
            __ProcessExitManually();
            break;
        case SIGSEGV:
        case SIGBUS:
        case SIGFPE:
            __ProcessCrash();
            break;
        default:
            __ProcessCrash();
    }
}

void SignalHandler::__InvokeCallbacks() {
    LogI(TAG, "[__InvokeCallbacks]")
    static bool invoked = false;
    if (!invoked) {
        LogI(TAG, "[__InvokeCallbacks] n_callbacks: %zd", process_exit_listeners_.size())
        for (IProcessExitListener *listener : process_exit_listeners_) {
            if (listener->OnProcessExit) {
                LogI(TAG, "[__InvokeCallbacks] execute %p",
                     listener->OnProcessExit)
                listener->OnProcessExit();
            }
        }
        invoked = true;
    }
}

void SignalHandler::__ProcessExitManually() {
    LogI(TAG, "[__ProcessExitManually] Process Exit Manually.")
    exit(EXIT_SUCCESS);
}

void SignalHandler::__ProcessCrash() {
    // TODO: print error message
    LogI(TAG, "[__ProcessCrash] Process Crash!")
    exit(EXIT_FAILURE);
}

SignalHandler::~SignalHandler() {
    LogI(TAG, "[~SignalHandler]")
    __InvokeCallbacks();
    for (IProcessExitListener *p : process_exit_listeners_) {
        delete p;
    }
}
