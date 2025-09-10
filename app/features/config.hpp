#pragma once

#include "color.hpp"
#include "link.hpp"

#include <any>
#include <deque>
#include <filesystem>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>
#include <algorithm>
#include <nlohmann/json.hpp>
#include <windows.h>

using json = nlohmann::json;

// helper: create a deque filled with the same value
template <typename T, std::size_t N>
inline std::deque<T> make_filled_deque(const T& value) {
    return std::deque<T>(N, value);
}

#define ADDVAR(Type, szName, pDefault) \
    const std::uint32_t szName = cfg::add_variable<Type>(fnv1a::hash_const(#szName), fnv1a::hash_const(#Type), pDefault);

#define ADDVAR(Type, uSize, szName, pDefault) \
    const std::uint32_t szName = cfg::add_variable<std::deque<Type>>(fnv1a::hash_const(#szName), fnv1a::hash_const("std::deque<" #Type ">"), make_filled_deque<Type, uSize>(pDefault));

#define INVALID_VARIABLE (std::size_t)-1

// variable descriptor
struct variable_t {
    variable_t() = default;

    variable_t(fnv1a_t name_hash, fnv1a_t type_hash, std::any default_value)
        : name_hash(name_hash), type_hash(type_hash), value(std::move(default_value)) {
    }

    template <typename T>
    T& get() {
        return *std::any_cast<T>(&value);
    }

    template <typename T>
    void set(const T& new_value) {
        value.emplace<T>(new_value);
    }

    fnv1a_t name_hash{};
    fnv1a_t type_hash{};
    std::any  value{};
};

// config system
namespace cfg {

    inline std::vector<variable_t> variables{};
    inline std::deque<std::string> file_names{};
    inline std::string config_path{ "pipeline-v4" };
    inline const std::filesystem::path working_path = std::filesystem::current_path();

    bool setup(std::string_view default_file);
    bool save(std::string_view file);
    bool load(std::string_view file);

    bool import(std::string file);
    void export_file(std::string file);

    void remove(std::string_view file);
    void refresh();

    std::size_t find_variable_index(fnv1a_t name_hash);

    template <typename T>
    T& get(std::uint32_t index) {
        return variables.at(index).get<T>();
    }

    template <typename T>
    std::uint32_t add_variable(fnv1a_t name_hash, fnv1a_t type_hash, const T& default_value) {
        variables.emplace_back(name_hash, type_hash, std::make_any<T>(default_value));
        return static_cast<std::uint32_t>(variables.size() - 1);
    }
}