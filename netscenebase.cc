#include "netscenebase.h"

void NetSceneBase::SetSocket(SOCKET _socket) {
    if (_socket > 0) {
        socket_ = _socket;
    }
}
