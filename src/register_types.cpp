#include "register_types.h"

#include "TetraPlayRegion.h"
#include "example_class.h"

using namespace godot;

void fun::GodotRegister::InitializeModuleAtLevel(ModuleInitializationLevel moduleLevel) {
    switch (moduleLevel) {
        case MODULE_INITIALIZATION_LEVEL_CORE:
            print_line("InitializeModuleAtLevel - module::MODULE_INITIALIZATION_LEVEL_CORE");
            break;
        case MODULE_INITIALIZATION_LEVEL_SERVERS:
            print_line("InitializeModuleAtLevel - module::MODULE_INITIALIZATION_LEVEL_SERVERS");
            break;
        case MODULE_INITIALIZATION_LEVEL_SCENE:
            print_line("InitializeModuleAtLevel - module::MODULE_INITIALIZATION_LEVEL_SCENE");
            GDREGISTER_CLASS(ExampleClass);
            GDREGISTER_CLASS(TetraPlayRegion);
            GDREGISTER_CLASS(TetraDisplayBlock);
            break;
        case MODULE_INITIALIZATION_LEVEL_EDITOR:
            print_line("InitializeModuleAtLevel - module::MODULE_INITIALIZATION_LEVEL_EDITOR");
            break;
        case MODULE_INITIALIZATION_LEVEL_MAX:
            print_line("InitializeModuleAtLevel - module::MODULE_INITIALIZATION_LEVEL_MAX");
            break;
    }
}
void fun::GodotRegister::TerminateModuleAtLevel(ModuleInitializationLevel moduleLevel) {
    switch (moduleLevel) {
        case MODULE_INITIALIZATION_LEVEL_CORE:
            print_line("TerminateModuleAtLevel - module::MODULE_INITIALIZATION_LEVEL_CORE");
            break;
        case MODULE_INITIALIZATION_LEVEL_SERVERS:
            print_line("TerminateModuleAtLevel - module::MODULE_INITIALIZATION_LEVEL_SERVERS");
            break;
        case MODULE_INITIALIZATION_LEVEL_SCENE:
            print_line("TerminateModuleAtLevel - module::MODULE_INITIALIZATION_LEVEL_SCENE");
            break;
        case MODULE_INITIALIZATION_LEVEL_EDITOR:
            print_line("TerminateModuleAtLevel - module::MODULE_INITIALIZATION_LEVEL_EDITOR");
            break;
        case MODULE_INITIALIZATION_LEVEL_MAX:
            print_line("TerminateModuleAtLevel - module::MODULE_INITIALIZATION_LEVEL_MAX");
            break;
    }
}

extern "C" { // Our Library Initialization
GDExtensionBool GDE_EXPORT example_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                                                GDExtensionClassLibraryPtr p_library,
                                                GDExtensionInitialization *r_initialization) {
    GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

    init_obj.register_initializer(fun::GodotRegister::InitializeModuleAtLevel);
    init_obj.register_terminator(fun::GodotRegister::TerminateModuleAtLevel);
    init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}