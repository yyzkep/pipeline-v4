#pragma once
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <iostream>

// vibecoding is some insane shit
// this config system might be a bit retarded.
class c_config_system
{
public:
    template <typename T>
    void set(const std::string &key, const T &value);

    template <typename T>
    T get(const std::string &key) const;

    void save(const std::string &filename) const;

    void load(const std::string &filename);

private:
    std::unordered_map<std::string, std::string> data;

    template <typename T>
    static std::string to_string(const T &value)
    {
        std::ostringstream oss;
        oss << value;
        return oss.str();
    }

    template <typename T>
    static T from_string(const std::string &str, const T &default_value)
    {
        std::istringstream iss(str);
        T val{};
        if (!(iss >> val))
            return default_value;
        return val;
    }

    static std::string to_string(const std::string &value) { return value; }
    static std::string from_string(const std::string &str, const std::string &) { return str; }
};

namespace config
{
    inline c_config_system system = {};

    inline std::string config_dir = "configs";

    inline void save_to_file(const std::string &name)
    {
        std::filesystem::create_directories(config_dir);
        system.save(config_dir + "/" + name + ".cfg");
    }

    inline void load_from_file(const std::string &name)
    {
        system.load(config_dir + "/" + name + ".cfg");
    }
}