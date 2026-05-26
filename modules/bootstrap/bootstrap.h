#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include "core/config/engine.h"
#include "scene/main/node.h"

class Bootstrap : public Node {
	GDCLASS(Bootstrap, Node);

protected:
	static bool _is_valid_script_path(const String &p_path);

public:
	Bootstrap();

	static void run_startup_scripts();
	static void execute_script(const String &p_path);
};

#endif // BOOTSTRAP_H
