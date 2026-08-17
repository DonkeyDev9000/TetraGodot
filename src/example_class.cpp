#include "example_class.h"

#include <format>

void ExampleClass::_bind_methods() {
    godot::ClassDB::bind_method(godot::D_METHOD("print_type", "variant"), &ExampleClass::print_type);
    godot::ClassDB::bind_method(godot::D_METHOD("accumulate", "delta"), &ExampleClass::accumulate);
}

void ExampleClass::print_type(const godot::Variant &p_variant) const {
    godot::print_line(godot::vformat("Type: %d", p_variant.get_type()));
}
bool ExampleClass::accumulate(const double &delta) {
    mAccumulate += delta;
    godot::print_line(std::format("maccum:{:.2f}", mAccumulate).c_str());

    return mAccumulate >= 1.0;
}
