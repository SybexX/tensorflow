/* Copyright 2024 The OpenXLA Authors.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef XLA_CODEGEN_MLIR_KERNEL_SOURCE_H_
#define XLA_CODEGEN_MLIR_KERNEL_SOURCE_H_

#include <memory>
#include <string>
#include <utility>

#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "mlir/IR/BuiltinOps.h"
#include "mlir/IR/MLIRContext.h"
#include "mlir/IR/OwningOpRef.h"
#include "mlir/Support/DebugStringHelper.h"
#include "xla/codegen/kernel_source.h"

namespace xla {

// Kernel JIT source that is backed by MLIR and owned by a mlir::ModuleOp.

// The MLIR source is typically created by a fusion emitter from either the CPU
// or GPU backend, e.g., ScatterFusion. The specific dialect(s) that backs the
// source is not specified but is implicit in the passed context. It is expected
// that the source will be lowered to LLVM by the corresponding backend
// compiler.
class MlirKernelSource final : public KernelSource {
 public:
  struct Storage {
    std::unique_ptr<mlir::MLIRContext> context;
    mlir::OwningOpRef<mlir::ModuleOp> module;
  };

  // Construct a MLIR kernel source from a module and take ownership of its MLIR
  // context.
  MlirKernelSource(std::unique_ptr<mlir::MLIRContext> context,
                   mlir::OwningOpRef<mlir::ModuleOp> module)
      : storage_{std::move(context), std::move(module)} {}

  // Construct a MLIR kernel source from a module but don't take any ownership
  // of the MLIR context.
  explicit MlirKernelSource(mlir::OwningOpRef<mlir::ModuleOp> module)
      : storage_{nullptr, std::move(module)} {}

  MlirKernelSource(MlirKernelSource&& other) noexcept = default;
  MlirKernelSource& operator=(MlirKernelSource&& other) noexcept = default;

  static absl::StatusOr<MlirKernelSource> ParseFromString(
      absl::string_view ir, std::unique_ptr<mlir::MLIRContext> context);

  mlir::ModuleOp module() { return *storage_.module; }

  Storage ReleaseStorage() && { return std::move(storage_); }

  std::string ToString() const final {
    return mlir::debugString(*storage_.module);
  }

 private:
  Storage storage_;
};

}  // namespace xla

#endif  // XLA_CODEGEN_MLIR_KERNEL_SOURCE_H_
