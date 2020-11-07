// Copyright (c) ONNX Project Contributors.
// Licensed under the MIT license.

#pragma once

#include "onnx-operators.pb.h"

namespace ONNX_NAMESPACE {

template <typename T>
TensorProto ToTensor(const T& value);

template <typename T>
TensorProto ToTensor(const std::vector<T>& values);

template <typename T>
const std::vector<T> ParseData(const TensorProto* tensor_proto);

} // namespace ONNX_NAMESPACE
