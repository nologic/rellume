

/**
 * \file
 **/

#ifndef RELLUME_PUBLIC_MIPSEL32_LIFTER_H
#define RELLUME_PUBLIC_MIPSEL32_LIFTER_H

namespace rellume {

class ArchBasicBlock;
struct FunctionInfo;
class Instr;
struct LLConfig;

namespace mipsel32 {

bool LiftInstruction(const Instr& inst, FunctionInfo& fi, const LLConfig& cfg,
                     ArchBasicBlock& ab) noexcept;

} // namespace mipsel32

}

#endif
