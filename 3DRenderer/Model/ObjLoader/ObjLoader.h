#pragma once
#include "../Object.h"
#include "../World.h"

namespace kernel {
using BasicObject = renderer::BasicObject;
void ReadAllFromDirectory(std::string path, renderer::World *world);
BasicObject ReadObject(const std::string &path);
}  // namespace kernel
