#pragma once

#include "godot_cpp/classes/ref_counted.hpp"
#include "godot_cpp/classes/wrapped.hpp"
#include "godot_cpp/variant/variant.hpp"

class ExampleClass : public godot::RefCounted {
    GDCLASS(ExampleClass, RefCounted)

    double mAccumulate{};

protected:
    static void _bind_methods();

public:
    void print_type(const godot::Variant &p_variant) const;
    bool accumulate(const double &delta);
};
