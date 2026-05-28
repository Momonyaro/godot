#ifndef BOOTSTRAP_H
#define BOOTSTRAP_H

#include "core/config/engine.h"
#include "scene/main/node.h"

class Bootstrap : public Node {
	GDCLASS(Bootstrap, Node);

    static void _bind_methods();
	bool _initialized = false;

protected:
	bool _is_valid_script_path(const String &p_path);
	void _notification(int p_what);

public:
	Bootstrap();

	void run();
	void execute_script(const String &p_path);
};

#endif // BOOTSTRAP_H
