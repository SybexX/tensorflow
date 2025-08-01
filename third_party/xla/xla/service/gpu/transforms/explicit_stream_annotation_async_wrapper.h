/* Copyright 2024 The OpenXLA Authors. All Rights Reserved.

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

#ifndef XLA_SERVICE_GPU_TRANSFORMS_EXPLICIT_STREAM_ANNOTATION_ASYNC_WRAPPER_H_
#define XLA_SERVICE_GPU_TRANSFORMS_EXPLICIT_STREAM_ANNOTATION_ASYNC_WRAPPER_H_

#include "absl/container/flat_hash_set.h"
#include "absl/status/statusor.h"
#include "absl/strings/string_view.h"
#include "xla/hlo/ir/hlo_computation.h"
#include "xla/hlo/ir/hlo_module.h"
#include "xla/hlo/pass/hlo_pass_interface.h"

namespace xla::gpu {

// This pass will find the kCall instructions that
// are annotated with explicit stream id in their frontend
// attributes. It then wraps them using AsyncStartDone pairs to achieve
// asynchronous executions.
class ExplicitStreamAnnotationAsyncWrapper : public HloModulePass {
 public:
  inline static constexpr char kMainExecutionThread[] = "main";

  absl::string_view name() const override {
    return "explicit-stream-annotation-async-wrapper";
  }

  using HloPassInterface::Run;
  absl::StatusOr<bool> Run(
      HloModule* module,
      const absl::flat_hash_set<absl::string_view>& execution_threads) override;
};

}  // namespace xla::gpu

#endif  // XLA_SERVICE_GPU_TRANSFORMS_EXPLICIT_STREAM_ANNOTATION_ASYNC_WRAPPER_H_
