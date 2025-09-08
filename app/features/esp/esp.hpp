#pragma once

#include "link.hpp"

class c_esp {
public:
	void draw();
private:
	void draw_entities();
	void draw_players();
	void draw_buildings();
	void draw_world();
};

namespace features { inline c_esp esp = {}; }