#pragma once

#include "godot_cpp/godot.hpp"

namespace fun {
class GodotRegister {
public:
    static void InitializeModuleAtLevel(godot::ModuleInitializationLevel moduleLevel);
    static void TerminateModuleAtLevel(godot::ModuleInitializationLevel moduleLevel);
};
}