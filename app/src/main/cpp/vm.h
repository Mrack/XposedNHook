//
// Created by Mrack on 2024/4/19.
//

#ifndef XPOSEDNHOOK_VM_H
#define XPOSEDNHOOK_VM_H

#include "QBDI.h"
#include <iomanip>
#include "nhook.h"
#include <sstream>


void syn_regs(DobbyRegisterContext *ctx, QBDI::GPRState *state);

#define LOGT(...) __android_log_print(ANDROID_LOG_DEBUG, "TRACER", __VA_ARGS__)

class vm {

public:
    QBDI::VM init(void *address);

    std::stringstream logbuf;
private:
};


#endif //XPOSEDNHOOK_VM_H
