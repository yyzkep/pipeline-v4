//
// Created by drev on 16/08/2025.
//

#pragma once
#include "pred.hpp"

class c_game_movement
{
public:
    virtual			~c_game_movement(void) {}
    virtual void	process_movement(base_entity* pPlayer, c_move_data* pMove) = 0;
    virtual void	start_track_prediction_errors(base_entity* pPlayer) = 0;
    virtual void	finish_track_prediction_errors(base_entity* pPlayer) = 0;
    virtual void	diff_print(char const* fmt, ...) = 0;
    virtual vector_t get_player_mins(bool ducked) const = 0;
    virtual vector_t get_player_maxs(bool ducked) const = 0;
    virtual vector_t get_player_view_offset(bool ducked) const = 0;
};