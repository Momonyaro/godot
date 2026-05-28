#include "bootstrap.h"

#include "core/config/project_settings.h"
#include "core/io/dir_access.h"
#include "core/io/resource_loader.h"
#include "core/object/script_language.h"
#include "scene/main/window.h"

Bootstrap::Bootstrap() {
}

void Bootstrap::_bind_methods() {
	ClassDB::bind_method(D_METHOD("run"), &Bootstrap::run);
}

void Bootstrap::_notification(int p_what) {
	if (p_what == NOTIFICATION_READY && !_initialized) {
		_initialized = true;
		run();
	}
}

void Bootstrap::run() {
	String script_dir = GLOBAL_GET("bootstrap/script_directory");

	if (!_is_valid_script_path(script_dir)) {
		WARN_PRINT("[BOOTSTRAP] invalid script directory: " + script_dir);
		return;
	}

	Ref<DirAccess> dir = DirAccess::open(script_dir);
	if (dir.is_null()) {
		ERR_PRINT("[BOOTSTRAP] failed to open script directory: " + script_dir);
		return;
	}
	dir->list_dir_begin();

	String file_name = dir->get_next();
	while(file_name.is_empty() == false) {
		if (!dir->current_is_dir()) {
			if (file_name.ends_with(".gd")) {
				String script_path = script_dir.path_join(file_name);
				print_line("[BOOTSTRAP] running script: " + script_path);
				execute_script(script_path);
			}
		}
        file_name = dir->get_next();
	}

	dir->list_dir_end();
	print_line("[BOOTSTRAP] completed.");
	queue_free();
}

void Bootstrap::execute_script(const String &p_path) {
	Ref<Script> script = ResourceLoader::load(p_path);

	if (script.is_null()) {
		ERR_PRINT("[BOOTSTRAP] failed to load script: " + p_path);
		return;
	}

    Node *instance = memnew(Node);
	instance->set_script(script);
	add_child(instance);

	if (instance->has_method("_exec")) {
		instance->call("_exec");
	} else {
		WARN_PRINT("[BOOTSTRAP] script does not have _exec() method: " + p_path);
	}

	instance->queue_free();
}

bool Bootstrap::_is_valid_script_path(const String &p_path) {
	// Must not be empty
	if (p_path.is_empty()) {
		return false;
	}

	// Only allow res:// or user://
	if (!p_path.begins_with("res://") && !p_path.begins_with("user://")) {
		return false;
	}

	// Directory must exist
	Ref<DirAccess> dir = DirAccess::open(p_path);
	if (dir.is_null()) {
		return false;
	}

	return true;
}
