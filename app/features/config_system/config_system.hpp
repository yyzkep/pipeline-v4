#pragma once

#include "color.hpp"
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include <Windows.h>

using json = nlohmann::json;

struct bool_item_t {
    bool* ptr{};
    std::string category;
    std::string name;
    bool default_value{};

    bool_item_t(bool* p, std::string c, std::string n, bool d);

    void load(const json& j);
    void save(json& j) const;
    void reset();
};

struct int_item_t {
    int* ptr{};
    std::string category;
    std::string name;
    int default_value{};

    int_item_t(int* p, std::string c, std::string n, int d);

    void load(const json& j);
    void save(json& j) const;
    void reset();
};

struct float_item_t {
    float* ptr{};
    std::string category;
    std::string name;
    float default_value{};

    float_item_t(float* p, std::string c, std::string n, float d);

    void load(const json& j);
    void save(json& j) const;
    void reset();
};

struct color_item_t {
    color_t* ptr{};
    std::string category;
    std::string name;
    color_t default_value{};

    color_item_t(color_t* p, std::string c, std::string n, color_t d);

    void load(const json& j);
    void save(json& j) const;
    void reset();
};

class config_system {
public:
    std::vector<bool_item_t> bool_items;
    std::vector<int_item_t> int_items;
    std::vector<float_item_t> float_items;
    std::vector<color_item_t> color_items;

    std::vector<std::string> config_files;
    inline static const std::filesystem::path config_dir = "configs";

    // add items
    void push_item(bool* ptr, const std::string& category, const std::string& name, bool default_value);
    void push_item(int* ptr, const std::string& category, const std::string& name, int default_value);
    void push_item(float* ptr, const std::string& category, const std::string& name, float default_value);
    void push_item(color_t* ptr, const std::string& category, const std::string& name, color_t default_value);

    // file operations
    void read(const std::string& filename);
    void save(const std::string& filename);
    void remove(const std::string& filename);
    void refresh();
    void reset();

    // clipboard operations
    void export_to_clipboard(const std::string& filename);
    void import_from_clipboard(const std::string& filename);
};
