
#include "vm.h"
#include "assert.h"

using namespace std;
using namespace QBDI;


QBDI::VMAction
showPostInstruction(QBDI::VM *vm, QBDI::GPRState *gprState, QBDI::FPRState *fprState,
                    void *data) {

    auto thiz = (class vm *) data;
    const QBDI::InstAnalysis *instAnalysis = vm->getInstAnalysis(QBDI::ANALYSIS_INSTRUCTION
                                                                 | QBDI::ANALYSIS_SYMBOL
                                                                 | QBDI::ANALYSIS_DISASSEMBLY
                                                                 | QBDI::ANALYSIS_OPERANDS
    );

    std::stringstream output;
    for (int i = 0; i < instAnalysis->numOperands; ++i) {
        auto op = instAnalysis->operands[i];
        if (op.regAccess == REGISTER_WRITE || op.regAccess == REGISTER_READ_WRITE) {
            if (op.regCtxIdx != -1) {
                if (op.type == OPERAND_GPR) {
                    output << op.regName << "=" << std::hex << QBDI_GPR_GET(gprState, op.regCtxIdx)
                           << " ";
                    output.flush();
                }
            }
        }
    }
    if (!output.str().empty()) {
        thiz->logbuf << "\tw[" << output.str() << "]" << std::endl;
    } else {
        thiz->logbuf << std::endl;
    }
    return QBDI::VMAction::CONTINUE;
}

QBDI::VMAction
showPreInstruction(QBDI::VM *vm, QBDI::GPRState *gprState, QBDI::FPRState *fprState,
                   void *data) {
    auto thiz = (class vm *) data;
    const QBDI::InstAnalysis *instAnalysis = vm->getInstAnalysis(QBDI::ANALYSIS_INSTRUCTION
                                                                 | QBDI::ANALYSIS_SYMBOL
                                                                 | QBDI::ANALYSIS_DISASSEMBLY
                                                                 | QBDI::ANALYSIS_OPERANDS
    );


    if (instAnalysis->symbol != nullptr) {
        thiz->logbuf << instAnalysis->symbol << "[0x" << std::hex << instAnalysis->symbolOffset
                     << "]:0x" << instAnalysis->address
                     << ": " << instAnalysis->disassembly;
    } else {
        thiz->logbuf << "0x" << std::hex << instAnalysis->address
                     << ": " << instAnalysis->disassembly;
    }

    stringstream output;
    for (int i = 0; i < instAnalysis->numOperands; ++i) {
        auto op = instAnalysis->operands[i];
        if (op.regAccess == QBDI::REGISTER_READ || op.regAccess == REGISTER_READ_WRITE) {
            if (op.regCtxIdx != -1) {
                if (op.type == OPERAND_GPR) {
                    output << op.regName << "=" << std::hex << QBDI_GPR_GET(gprState, op.regCtxIdx)
                           << " ";
                    output.flush();
                }
            }
        }
    }


    if (!output.str().empty()) {
        thiz->logbuf << "\tr[" << output.str() << "]";
    }
    return QBDI::VMAction::CONTINUE;
}


QBDI::VMAction
showMemoryAccess(QBDI::VM *vm, QBDI::GPRState *gprState, QBDI::FPRState *fprState,
                 void *data) {
    auto thiz = (class vm *) data;
    if (vm->getInstMemoryAccess().empty()) {
        thiz->logbuf << std::endl;
    }
    for (const auto &acc: vm->getInstMemoryAccess()) {
        if (acc.type == MEMORY_READ) {
            thiz->logbuf << " mem[r]:" << std::hex << acc.accessAddress << " size:" << acc.size
                         << " value:" << acc.value;
        } else if (acc.type == MEMORY_WRITE) {
            thiz->logbuf << " mem[w]:" << std::hex << acc.accessAddress << " size:" << acc.size
                         << " value:" << acc.value;
        } else {
            thiz->logbuf << " mem[rw]:" << std::hex << acc.accessAddress << " size:" << acc.size
                         << " value:" << acc.value;
        }
    }
    thiz->logbuf << std::endl << std::endl;
    return QBDI::VMAction::CONTINUE;
}

QBDI::VM vm::init(void *address) {
    uint32_t cid;
    QBDI::GPRState *state;
    QBDI::VM qvm{};
    state = qvm.getGPRState();
    qvm.setOptions(QBDI::OPT_DISABLE_LOCAL_MONITOR | QBDI::OPT_BYPASS_PAUTH | QBDI::OPT_ENABLE_BTI);
    assert(state != nullptr);
    qvm.recordMemoryAccess(QBDI::MEMORY_READ_WRITE);
    cid = qvm.addCodeCB(QBDI::PREINST, showPreInstruction, this);
    assert(cid != QBDI::INVALID_EVENTID);
    cid = qvm.addCodeCB(QBDI::POSTINST, showPostInstruction, this);
    assert(cid != QBDI::INVALID_EVENTID);
    cid = qvm.addMemAccessCB(MEMORY_READ_WRITE, showMemoryAccess, this);
    assert(cid != QBDI::INVALID_EVENTID);
    bool ret = qvm.addInstrumentedModuleFromAddr(reinterpret_cast<QBDI::rword>(address));
    assert(ret == true);
    return qvm;
}

void syn_regs(DobbyRegisterContext *ctx, QBDI::GPRState *state) {
    for (int i = 0; i < 29; i++) {
        QBDI_GPR_SET(state, i, ctx->general.x[i]);
    }
    state->lr = ctx->lr;
    state->x29 = ctx->fp;
    state->sp = ctx->sp;
}