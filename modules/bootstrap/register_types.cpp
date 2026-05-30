#include "register_types.h"
#include "bootstrap.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"
#include "core/os/os.h"
#include "scene/main/window.h"


static Bootstrap *bootstrap = nullptr;
const String SERVICE_NAME = "Bootstrap";

void initialize_bootstrap_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) { return; }

	ClassDB::register_class<Bootstrap>();

	// Register setting if it doesn't exist
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::STRING, "bootstrap/script_directory", PROPERTY_HINT_DIR), "res://bootstrap");
	GLOBAL_DEF_BASIC(PropertyInfo(Variant::BOOL, "bootstrap/enabled"), false);

	bool service_enabled = GLOBAL_GET("bootstrap/enabled");

	if (service_enabled == false || ProjectSettings::get_singleton()->has_service(SERVICE_NAME)) { return; }

	bootstrap = memnew(Bootstrap);
	ProjectSettings::get_singleton()->add_service(SERVICE_NAME, *bootstrap);
}

void uninitialize_bootstrap_module(ModuleInitializationLevel p_level) {
	if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) { return; }

	ProjectSettings::get_singleton()->remove_service(SERVICE_NAME);

	if (bootstrap) {
		memdelete(bootstrap);
	}
}
