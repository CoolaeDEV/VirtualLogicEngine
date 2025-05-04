#pragma once

#include <../godot_cpp/classes/node.hpp>

namespace godot {
	class GDVirtualLogicEngine : public Node {
		GDCLASS(GDVirtualLogicEngine, Node);

	protected:
		static void _bind_methods();

	public:
		GDVirtualLogicEngine();
		~GDVirtualLogicEngine();

		void _process(double delta) override;
	};

}

#endif