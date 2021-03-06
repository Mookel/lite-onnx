// Copyright (c) ONNX Project Contributors.
// Licensed under the MIT license.

#pragma once

#include "defs/schema.h"

namespace ONNX_NAMESPACE {

// Declare training operators.

// Iterate over schema from ai.onnx.training version 1
class OpSet_OnnxTraining_ver1 {
 public:
  static void ForEachSchema(std::function<void(OpSchema&&)> /* fn */) {
  }
};

// Register training operators.
inline void RegisterOnnxTrainingOperatorSetSchema() {
  RegisterOpSetSchema<OpSet_OnnxTraining_ver1>();
}

} // namespace ONNX_NAMESPACE
