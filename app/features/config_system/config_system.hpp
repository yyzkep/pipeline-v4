#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

//vibecoding is some insane shit
//this config system might be a bit retarded.
class c_config_system {
public:
    template <typename T>
    void set(const std::string& key, const T& value) {
        data[key] = to_string(value);
    }

    template <typename T>
    T get(const std::string& key, const T& default_value = T{}) const {
        auto it = data.find(key);
        if (it == data.end()) return default_value;
        return from_string<T>(it->second, default_value);
    }

    void save(const std::string& filename) const {
        std::ofstream ofs(filename);
        if (!ofs) return;
        for (auto& [k, v] : data) {
            ofs << k << "=" << v << "\n";
        }
    }

    void load(const std::string& filename) {
        std::ifstream ifs(filename);
        if (!ifs) return;

        data.clear();
        std::string line;
        while (std::getline(ifs, line)) {
            auto pos = line.find('=');
            if (pos == std::string::npos) continue;
            std::string key = line.substr(0, pos);
            std::string val = line.substr(pos + 1);
            data[key] = val;
        }
    }

private:
    std::unordered_map<std::string, std::string> data;

    template <typename T>
    static std::string to_string(const T& value) {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    template <typename T>
    static T from_string(const std::string& str, const T& default_value) {
        std::istringstream iss(str);
        T val{};
        if (!(iss >> val)) return default_value;
        return val;
    }

    static std::string to_string(const std::string& value) { return value; }
    static std::string from_string(const std::string& str, const std::string&) { return str; }
};

namespace config {
    inline c_config_system system = {};

    inline std::string config_dir = "configs";

    inline void save_to_file(const std::string& name) {
        std::filesystem::create_directories(config_dir);
        system.save(config_dir + "/" + name + ".cfg");
    }

    inline void load_from_file(const std::string& name) {
        system.load(config_dir + "/" + name + ".cfg");
    }
}