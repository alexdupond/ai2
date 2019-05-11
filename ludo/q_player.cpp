#include "q_player.h"
#include <random>
#include <sstream>
#include <fstream>

q_player::q_player():
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0)
{
    
}

int q_player::get_reward(){
    
}

bool q_player::loadQTable(string &filename, vector<vector<double>> &vec){
    std::string line;
    ifstream file(filename); 
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string str_num;
        std::vector<double> temp;
        while (std::getline(ss, str_num, ',')) {
            temp.emplace_back(std::stoi(str_num));
        }
            vec.emplace_back(temp);
    }
    return true; 
}

bool q_player::saveQTable(string &filename, vector<vector<double>> &vec){
    std::ofstream out_file(filename);
    for (const auto& i : vec) {
        for (const auto& j : i) {
            out_file << j << ", ";
        }
        out_file << '\n';
    }

    return true; 
}

int q_player::make_decision(){
    if(dice_roll == 6){
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
    } else {
        for(int i = 0; i < 4; ++i){
            if(pos_start_of_turn[i]>=0 && pos_start_of_turn[i] != 99){
                return i;
            }
        }
        for(int i = 0; i < 4; ++i){ //maybe they are all locked in
            if(pos_start_of_turn[i]<0){
                return i;
            }
        }
    }
    return -1;
}

void q_player::start_turn(positions_and_dice relative){
    pos_start_of_turn = relative.pos;
    dice_roll = relative.dice;
    int decision = make_decision();
    emit select_piece(decision);
}

void q_player::post_game_analysis(std::vector<int> relative_pos){
    pos_end_of_turn = relative_pos;
    bool game_complete = true;
    for(int i = 0; i < 4; ++i){
        if(pos_end_of_turn[i] < 99){
            game_complete = false;
        }
    }
    emit turn_complete(game_complete);
}
