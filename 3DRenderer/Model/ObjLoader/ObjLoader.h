#pragma once
#include "../Object.h"
namespace kernel {
using BasicObject = renderer::BasicObject;
BasicObject ReadObject(const std::string &path);
}  // namespace kernel
