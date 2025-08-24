#include "mipsel32/lifter.h"
#include "mipsel32/lifter-private.h"

namespace rellume::mipsel32 {

bool LiftInstruction(const Instr& inst, FunctionInfo& fi, const LLConfig& cfg,
                     ArchBasicBlock& ab) noexcept {
    return Lifter(fi, cfg, ab).Lift(inst);
}

bool Lifter::Lift(const Instr& inst) {
    return false;
}

}