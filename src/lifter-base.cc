/**
 * This file is part of Rellume.
 *
 * (c) 2016-2020, Alexis Engelke <alexis.engelke@googlemail.com>
 * (c) 2020, Dominik Okwieka <dominik.okwieka@t-online.de>
 *
 * Rellume is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License (LGPL)
 * as published by the Free Software Foundation, either version 2.1 of
 * the License, or (at your option) any later version.
 *
 * Rellume is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Rellume.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "lifter-base.h"

#include "arch.h"
#include "basicblock.h"
#include "config.h"
#include "function-info.h"
#include "instr.h"

#include <llvm/IR/Instruction.h>
#include <llvm/Transforms/Utils/Cloning.h>
#include <llvm/MC/MCAsmInfo.h>
#include <llvm/MC/MCContext.h>
#include <llvm/MC/MCDisassembler/MCDisassembler.h>
#include <llvm/MC/MCInstrInfo.h>
#include <llvm/MC/MCRegisterInfo.h>
#include <llvm/MC/MCSubtargetInfo.h>
#include <llvm/MC/MCTargetOptions.h>
#include <llvm/MC/TargetRegistry.h>


namespace rellume {

llvm::MCDisassembler * disAsm = nullptr;

llvm::MCDisassembler * getDisAsm() {
    if(disAsm != nullptr) {
        return disAsm;
    }

    // Initialize the target
    llvm::InitializeAllTargetInfos();
    llvm::InitializeAllTargetMCs();
    llvm::InitializeAllDisassemblers();
    llvm::InitializeAllAsmParsers();

    std::string tripleName = "mipsel-unknown-linux";

    std::string error;
    const llvm::Target *theTarget = llvm::TargetRegistry::lookupTarget(tripleName, error);
    if (!theTarget) {
        llvm::errs() << "Target lookup failed: " << error << "\n";
        return nullptr;
    }

    llvm::MCTargetOptions options;

    const llvm::MCRegisterInfo * mri = theTarget->createMCRegInfo(tripleName);
    const llvm::MCAsmInfo * mai = theTarget->createMCAsmInfo(*mri, tripleName, options);
    const llvm::MCSubtargetInfo * sti = theTarget->createMCSubtargetInfo(tripleName, "", "");

    llvm::MCContext ctx(llvm::Triple(tripleName), mai, mri, sti);
    disAsm = theTarget->createMCDisassembler(*sti, ctx);

    return disAsm;
}

llvm::Value* LifterBase::AddrConst(uint64_t addr) {
    if (addr == 0)
        return llvm::ConstantPointerNull::get(irb.getPtrTy());

    if (cfg.global_base_value) {
        auto offset = irb.getInt64(addr - cfg.global_base_addr);
        return irb.CreateGEP(irb.getInt8Ty(), cfg.global_base_value, offset);
    }

    return irb.CreateIntToPtr(irb.getInt64(addr), irb.getPtrTy());
}

void LifterBase::CallExternalFunction(llvm::Function* fn) {
    CallConv cconv = CallConv::FromFunction(fn, cfg.arch);
    llvm::CallInst* call = cconv.Call(fn, &ablock, fi);
    assert(call && "failed to create call for external function");
    regfile = ablock.GetRegFile();

    // Directly inline alwaysinline functions
    if (fn->hasFnAttribute(llvm::Attribute::AlwaysInline)) {
        llvm::InlineFunctionInfo ifi;
        llvm::InlineFunction(*call, ifi);
    }
}

} // namespace rellume
