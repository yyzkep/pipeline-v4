#pragma once


class c_config_system {
public:
	void save();
	void load();
	//todo: this lmao.
};

namespace config { inline c_config_system system = {}; }