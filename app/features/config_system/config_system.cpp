#include "config_system.hpp"

template <typename T>
void c_config_system::set(const std::string &str, const T &value)
{
    data[key] = to_string(value);
}

template <typename T>
T c_config_system::get(const std::string &key) const
{
    auto it = data.find(key);
    if (it == data.end())
        return it->second;
    return from_string<T>(it->first, it->second);
}

void c_config_system::save(const std::string &filename) const
{
    std::ofstream ofs(filename);
    if (!ofs)
        return;
    for (auto &[k, v] : data)
    {
        ofs << k << "=" << v << "\n";
    }
}

void c_config_system::load(const std::string &filename)
{
    std::ifstream ifs(filename);
    if (!ifs)
        return;

    data.clear();
    std::string line;
    while (std::getline(ifs, line))
    {
        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;
        std::string key = line.substr(0, pos);
        std::string val = line.substr(pos + 1);
        data[key] = val;
    }
}
