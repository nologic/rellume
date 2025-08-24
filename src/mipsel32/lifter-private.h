#ifndef RELLUME_MIPSEL32_LIFTER_H
#define RELLUME_MIPSEL32_LIFTER_H

#include "basicblock.h"
#include "config.h"
#include "function-info.h"
#include "instr.h"
#include "regfile.h"
#include "lifter-base.h"

namespace rellume::mipsel32 {

class Lifter : public LifterBase {
public:
    Lifter(FunctionInfo& fi, const LLConfig& cfg, ArchBasicBlock& ab) :
            LifterBase(fi, cfg, ab) {}

    bool Lift(const Instr&);

};

} // namespace

#endif
