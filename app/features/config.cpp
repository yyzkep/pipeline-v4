#include "config.hpp"

void config::load_config(std::string cfg)
{
    
    config::load_from_file(cfg);
}

void config::save_config(std::string out_str)
{

    config::save_to_file(out_str);
}