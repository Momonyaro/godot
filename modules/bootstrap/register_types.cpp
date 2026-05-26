#include "register_types.h"
#include "bootstrap.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"
#include "core/os/os.h"
#include "scene/main/window.h"

static Bootstrap *bootstrap = nullptr;

void initialize_bootstrap_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) { return; }

	ClassDB::register_class<Bootstrap>();

	// Register setting if it doesn't exist
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING, "bootstrap/script_directory", PROPERTY_HINT_DIR), "res://bootstrap");

	bootstrap = memnew(Bootstrap);

	Engine::get_singleton()->add_singleton(Engine::Singleton("Bootstrap", bootstrap));
}

void uninitialize_bootstrap_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) { return; }

	if (bootstrap) {
		memdelete(bootstrap);
	}
}
