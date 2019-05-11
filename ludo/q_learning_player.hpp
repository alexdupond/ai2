#ifndef Q_LEARNING_PLAYER_H
#define Q_LEARNING_PLAYER_H
#include <QObject>
#include <iostream>
#include "positions_and_dice.h"

using namespace std; 

class q_learning_player : public QObject {
    Q_OBJECT
private:
    bool testMode = true; 
    double learning_rate = 0.5; 
    double explor_rate = 0.5; 
    double exploi_rate = 0.5; 

    vector<vector<double> q_table(10, vector<double>(4, 0)); 


    vector<int> pos_start_of_turn;
    vector<int> pos_end_of_turn;
    int dice_roll;
    int make_decision();
    int reward(); 

public:
    ludo_player();
signals:
    void select_piece(int);
    void turn_complete(bool);
public slots:
    void start_turn(positions_and_dice relative);
    void post_game_analysis(vector<int> relative_pos);
};

#endif  //Q_LEARNING_PLAYER_H

