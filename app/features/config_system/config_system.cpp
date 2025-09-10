#include "config_system.hpp"
#include <iostream>
#include <Windows.h>
#include <fstream>
#include <filesystem>

// ---------------- bool_item_t ----------------
bool_item_t::bool_item_t(bool* p, std::string c, std::string n, bool d)
    : ptr(p), category(std::move(c)), name(std::move(n)), default_value(d) {
}

void bool_item_t::load(const json& j) { if (j.contains(category) && j[category].contains(name)) *ptr = j[category][name].get<bool>(); }
void bool_item_t::save(json& j) const { j[category][name] = *ptr; }
void bool_item_t::reset() { *ptr = default_value; }

// ---------------- int_item_t ----------------
int_item_t::int_item_t(int* p, std::string c, std::string n, int d)
    : ptr(p), category(std::move(c)), name(std::move(n)), default_value(d) {
}

void int_item_t::load(const json& j) { if (j.contains(category) && j[category].contains(name)) *ptr = j[category][name].get<int>(); }
void int_item_t::save(json& j) const { j[category][name] = *ptr; }
void int_item_t::reset() { *ptr = default_value; }

// ---------------- float_item_t ----------------
float_item_t::float_item_t(float* p, std::string c, std::string n, float d)
    : ptr(p), category(std::move(c)), name(std::move(n)), default_value(d) {
}

void float_item_t::load(const json& j) { if (j.contains(category) && j[category].contains(name)) *ptr = j[category][name].get<float>(); }
void float_item_t::save(json& j) const { j[category][name] = *ptr; }
void float_item_t::reset() { *ptr = default_value; }

// ---------------- color_item_t ----------------
color_item_t::color_item_t(color_t* p, std::string c, std::string n, color_t d)
    : ptr(p), category(std::move(c)), name(std::move(n)), default_value(d) {
}

void color_item_t::load(const json& j) {
    if (j.contains(category) && j[category].contains(name)) {
        auto arr = j[category][name];
        *ptr = color_t(arr[0], arr[1], arr[2], arr[3]);
    }
}
void color_item_t::save(json& j) const { j[category][name] = { ptr->r, ptr->g, ptr->b, ptr->a }; }
void color_item_t::reset() { *ptr = default_value; }

// ---------------- config_system ----------------
void config_system::push_item(bool* ptr, const std::string& category, const std::string& name, bool default_value) {
    bool_items.emplace_back(ptr, category, name, default_value);
}

void config_system::push_item(int* ptr, const std::string& category, const std::string& name, int default_value) {
    int_items.emplace_back(ptr, category, name, default_value);
}

void config_system::push_item(float* ptr, const std::string& category, const std::string& name, float default_value) {
    float_items.emplace_back(ptr, category, name, default_value);
}

void config_system::push_item(color_t* ptr, const std::string& category, const std::string& name, color_t default_value) {
    color_items.emplace_back(ptr, category, name, default_value);
}

void config_system::read(const std::string& filename) {
    auto path = config_dir / filename;
    if (!std::filesystem::exists(path)) return;

    std::ifstream file(path);
    if (!file.is_open()) return;

    json j;
    try { file >> j; }
    catch (...) { return; }

    for (auto& item : bool_items)   item.load(j);
    for (auto& item : int_items)    item.load(j);
    for (auto& item : float_items)  item.load(j);
    for (auto& item : color_items)  item.load(j);
}

void config_system::save(const std::string& filename) {
    std::filesystem::create_directories(config_dir);
    auto path = config_dir / filename;

    json j;
    for (auto& item : bool_items)   item.save(j);
    for (auto& item : int_items)    item.save(j);
    for (auto& item : float_items)  item.save(j);
    for (auto& item : color_items)  item.save(j);

    std::ofstream file(path);
    if (file.is_open()) file << j.dump(4);
}

void config_system::remove(const std::string& filename) {
    auto path = config_dir / filename;
    try { std::filesystem::remove(path); }
    catch (...) {}
}

void config_system::refresh() {
    config_files.clear();
    std::filesystem::create_directories(config_dir);

    for (auto& entry : std::filesystem::directory_iterator(config_dir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".json") {
            config_files.push_back(entry.path().filename().string());
        }
    }
}

void config_system::reset() {
    for (auto& item : bool_items)   item.reset();
    for (auto& item : int_items)    item.reset();
    for (auto& item : float_items)  item.reset();
    for (auto& item : color_items)  item.reset();
}

void config_system::export_to_clipboard(const std::string& filename) {
    save(filename);

    auto path = config_dir / filename;
    std::ifstream in(path);
    std::string data((std::istreambuf_iterator<char>(in)), {});

    const size_t len = data.size() + 1;
    HGLOBAL mem = GlobalAlloc(GMEM_MOVEABLE, len);
    memcpy(GlobalLock(mem), data.c_str(), len);
    GlobalUnlock(mem);

    OpenClipboard(nullptr);
    EmptyClipboard();
    SetClipboardData(CF_TEXT, mem);
    CloseClipboard();
}

void config_system::import_from_clipboard(const std::string& filename) {
    OpenClipboard(nullptr);
    HANDLE h_data = GetClipboardData(CF_TEXT);
    if (!h_data) { CloseClipboard(); return; }
    char* clipboard_text = static_cast<char*>(GlobalLock(h_data));
    if (!clipboard_text) { CloseClipboard(); return; }

    std::string data(clipboard_text);
    GlobalUnlock(h_data);
    CloseClipboard();

    std::filesystem::create_directories(config_dir);
    auto path = config_dir / filename;
    std::ofstream out(path);
    if (out.is_open()) out << data;

    read(filename);
}
