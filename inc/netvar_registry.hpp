//
// Created by drev on 30/08/2025.
//

#include <string>
#include <vector>
#include <unordered_map>
#include <typeinfo>

// metadata holder
struct netvar_info {
    std::string member;   // variable name
    std::string table;    // network class
    std::string var;      // property name
    std::string type;     // from typeid(T).name()
    int offset;
};

class c_netvar_registry {
public:
    std::vector<netvar_info> netvars;

    void register_netvar(const netvar_info& info) {
        netvars.push_back(info);
    }

    const std::vector<netvar_info>& get_all() const {
        return netvars;
    }
};

inline c_netvar_registry g_netvar_registry;
