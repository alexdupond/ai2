#ifndef Q_PLAYER_H
#define Q_PLAYER_H

#include <QObject>
#include <iostream>
#include <algorithm> 
#include <vector> 
#include <random>
#include "string.h"
#include "positions_and_dice.h"

#include <fstream>

#define REWARD_IS_SAFE             -0.1
#define REWARD_PROTECT_BRICK        0.6
#define REWARD_MOVE_OUT_OF_HOME     1.2
#define REWARD_MOVE_INTO_DANGER    -0.8
#define REWARD_MOVE_OUT_OF_DANGER   0.5
#define REWARD_MOVE_IN_GOAL         0.7
#define REWARD_MOVE_INTO_SAFE_ZONE  0.5
#define REWARD_KILL                 0.9
#define REWARD_SUICIDE             -1.5
#define REWARD_MOVE                 0.0
#define REWARD_CANT_MOVE           -0.5
#define REWARD_GET_KILLED          -0.9
#define REWARD_HIT_STAR             0.4

#define REWARD_WIN                  50
#define REWARD_LOSE                 25

// States
// 0 - Brick is in safe zone            00000 00001
// 1 - Can move out of home             00000 00010
// 2 - Can move into danger zone        00000 00100
// 3 - Can move out of danger zone      00000 01000
// 4 - Can move into goal               00000 10000
// 5 - Can move into safety or Can form blockade  
// 6 - Can kill                         00010 00000
// 7 - Can suicide                      00100 00000
// 8 - Can move normal                  01000 00000
// 9 - Cant move 

using namespace std;

class q_player : public QObject {
    Q_OBJECT
private:

    bool traning = true; 

    int iterations;
    double LEARNING_RATE = 0.8;
    double LEARNING_RATE_INC; 
    double EXPLORE_RATE = 1.00;
    double EXPLORE_RATE_DECAY;
    double DISCOUNT_FACTOR = 0.95;


    // For results
    int games_won = 0;
    int games_played = 0;
    int total_games_won = 0;

    int previous_action = -1;
    int previous_decision = -1;
    int previous_state = -1;
    double previous_reward = -1;
    vector<int> previous_states = {4, -1}; 
    vector<int> previous_actions = {4, -1};

    const int static nStates =  58;
    const int static nStatesNew = 16777215;
    const int static nActions = 10;
    vector<double> *q_table = new vector<double>[nStatesNew];
    //double q_table[nStates][nActions]; // 11 states, and 4 actions

    std::vector<int> pos_start_of_turn;
    std::vector<int> pos_end_of_turn;
    int dice_roll;

    int make_decision();
    int select_action(vector<int>& state, vector<int> &actions); 

    void get_reward(vector<int> states, vector<int> actions, int decision); 
    bool inDanger(int brick, int enemy);
    bool closeToEnemy(int brick, int enemy);
    int convertToSingleState(vector<int> &states); 

    vector<int> get_states(); 
    vector<int> get_actions(); 
    //bool loadQTable(string &filename, double * result); 
    //bool saveQTable(string &filename, double * input);
public:
    q_player(int);
    void print_q_table();
signals:
    void select_piece(int);
    void turn_complete(bool);

public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(std::vector<int> relative_pos);
    void get_winner(int id);
};

#endif // Q_PLAYER_H
