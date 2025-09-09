#pragma once
#include "link.hpp"

class c_menu {
public:
	void draw_menu();
private:
	void aimbot_tab();
	void visuals_tab();
	void misc_tab();
	void settings_tab();
};

namespace features { inline c_menu menu = {}; }