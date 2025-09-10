#include "config.hpp"
#include <intsafe.h>


bool cfg::setup(std::string_view default_file) {
    auto dir = working_path / config_path;

    if (!std::filesystem::is_directory(dir)) {
        std::filesystem::remove(dir);
        if (!std::filesystem::create_directories(dir)) {
            return false;
        }
    }

    auto file_path = dir / default_file;
    if (!std::filesystem::exists(file_path) && !save(default_file)) {
        return false;
    }

    if (!load(default_file)) {
        return false;
    }

    refresh();
    return true;
}

bool cfg::save(std::string_view file_name) {
    std::filesystem::path file_path(file_name);
    if (file_path.extension() != ".cfg") {
        file_path.replace_extension(".cfg");
    }

    auto full_path = (working_path / config_path / file_path).string();
    json json_out;

    for (auto& var : variables) {
        json entry;
        entry["name_id"] = var.name_hash;
        entry["type_id"] = var.type_hash;

        if (var.type_hash == fnv1a::hash_const("int")) entry["value"] = var.get<int>();
        else if (var.type_hash == fnv1a::hash_const("float")) entry["value"] = var.get<float>();
        else if (var.type_hash == fnv1a::hash_const("bool")) entry["value"] = var.get<bool>();
        else if (var.type_hash == fnv1a::hash_const("color_t")) {
            auto c = var.get<color_t>();
            entry["value"] = { c.r, c.g, c.b, c.a };
        }
        else if (var.type_hash == fnv1a::hash_const("std::deque<int>")) entry["value"] = var.get<std::deque<int>>();
        else if (var.type_hash == fnv1a::hash_const("std::deque<float>")) entry["value"] = var.get<std::deque<float>>();
        else if (var.type_hash == fnv1a::hash_const("std::deque<bool>")) entry["value"] = var.get<std::deque<bool>>();
        else if (var.type_hash == fnv1a::hash_const("std::deque<std::string>")) entry["value"] = var.get<std::deque<std::string>>();

        json_out.push_back(entry);
    }

    std::ofstream out(full_path, std::ios::out | std::ios::trunc);
    if (!out.good()) return false;

    try {
        out << json_out.dump(4);
    }
    catch (...) {
        return false;
    }

    return true;
}

bool cfg::load(std::string_view file_name) {
    std::filesystem::path file_path(file_name);
    if (file_path.extension() != ".cfg") file_path.replace_extension(".cfg");

    auto full_path = (working_path / config_path / file_path).string();
    json json_in;

    std::ifstream in(full_path);
    if (!in.good()) return false;

    try { in >> json_in; }
    catch (...) { return false; }

    for (auto& entry_json : json_in) {
        auto index = find_variable_index(entry_json["name_id"].get<fnv1a_t>());
        if (index == INVALID_VARIABLE) continue;

        auto& entry = variables.at(index);
        auto type = entry_json["type_id"].get<fnv1a_t>();

        if (type == fnv1a::hash_const("bool")) entry.set<bool>(entry_json["value"].get<bool>());
        else if (type == fnv1a::hash_const("float")) entry.set<float>(entry_json["value"].get<float>());
        else if (type == fnv1a::hash_const("int")) entry.set<int>(entry_json["value"].get<int>());
        else if (type == fnv1a::hash_const("color_t")) {
            auto arr = entry_json["value"];
            entry.set<color_t>(color_t(arr[0], arr[1], arr[2], arr[3]));
        }
        else if (type == fnv1a::hash_const("std::deque<int>")) {
            auto arr = entry_json["value"].get<std::deque<int>>();
            auto& target = entry.get<std::deque<int>>();
            for (size_t i = 0; i < std::min(arr.size(), target.size()); i++) target[i] = arr[i];
        }
        else if (type == fnv1a::hash_const("std::deque<float>")) {
            auto arr = entry_json["value"].get<std::deque<float>>();
            auto& target = entry.get<std::deque<float>>();
            for (size_t i = 0; i < std::min(arr.size(), target.size()); i++) target[i] = arr[i];
        }
        else if (type == fnv1a::hash_const("std::deque<bool>")) {
            auto arr = entry_json["value"].get<std::deque<bool>>();
            auto& target = entry.get<std::deque<bool>>();
            for (size_t i = 0; i < std::min(arr.size(), target.size()); i++) target[i] = arr[i];
        }
        else if (type == fnv1a::hash_const("std::deque<std::string>")) {
            auto arr = entry_json["value"].get<std::deque<std::string>>();
            auto& target = entry.get<std::deque<std::string>>();
            for (size_t i = 0; i < std::min(arr.size(), target.size()); i++) target[i] = arr[i];
        }
    }

    return true;
}

void cfg::refresh() {
    file_names.clear();
    auto dir = working_path / config_path;

    if (!std::filesystem::exists(dir)) std::filesystem::create_directories(dir);

    for (auto& entry : std::filesystem::directory_iterator(dir)) {
        if (entry.path().extension() == ".cfg") file_names.push_back(entry.path().filename().string());
    }
}

std::size_t cfg::find_variable_index(fnv1a_t name_hash) {
    for (std::size_t i = 0; i < variables.size(); i++) {
        if (variables[i].name_hash == name_hash) return i;
    }
    return INVALID_VARIABLE;
}