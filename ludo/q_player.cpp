#include "q_player.h"
#include <random>

q_player::q_player(int itt):
    pos_start_of_turn(16),
    pos_end_of_turn(16),
    dice_roll(0)
{
   /* for (size_t i = 0; i < nStates; i++)
    {
        for (size_t j = 0; j < nActions; j++)
        {
            q_table[i][j] = 0;
        }        
    }*/
    
    vector<double> actions(nActions, 0); 

    for (size_t i = 0; i < nStatesNew; i++)
    {   
        q_table[i] = actions;
    }

    iterations = itt; 
    EXPLORE_RATE_DECAY = (EXPLORE_RATE / iterations);
    LEARNING_RATE_INC = (1.00 / iterations) / 2.00;
    
}


// Actions
// 0 - Can move out of home 
// 1 - Can hit star 
// 2 - Can protect brick = Can hit globe + Can move into goal strech + Can form blockage
// 3 - Can hit goal
// 4 - Can kill 
// 5 - Can suicide 
// 6 - Cant move
// 7 - Can move into danger 
// 8 - Can move out of danger
// 9 - Normal move

void q_player::get_reward(vector<int> states, vector<int> actions, int decision){
    double reward = 0; 
    int action = actions[decision];
    int state = convertToSingleState(states);  // states[decision]; // 
   // cout << endl << " ---- REWARD ----" << endl;

    if(decision != -1){
    
        if(action == 0){                                 // Move out of home 
            reward += REWARD_MOVE_OUT_OF_HOME; 
            //cout << "Brick " << decision << " moved out of home!" << endl;
        }else if(action == 1){                           // Hit a star
            reward += REWARD_HIT_STAR; 
            //cout << "Brick " << decision << " hit star!" << endl;
        }else if(action == 2 ){                          // Protect brick
            reward += REWARD_PROTECT_BRICK; 
            //cout << "Brick " << decision << " is protected! " << endl;
        }else if(action == 3){                      // Move in goal
            reward += REWARD_MOVE_IN_GOAL;
            //cout << "Brick " << decision << " is moved into goal! " << endl;
        }else if(action == 4){                           // Kill enemy
            reward += REWARD_KILL; 
            //cout << "Brick " << decision << " killed enemy" << endl;
        }else if(action == 5){                           // Suicide
            reward += REWARD_SUICIDE;
            //cout << "Brick " << decision << " comitted suicide" << endl;
        }else if(action == 6){                           // Cant move
            //cout << "Brick " << decision << " cant move! " << endl;
            reward += REWARD_CANT_MOVE; 
        }else if(action == 7){                           // Move into danger
            reward += REWARD_MOVE_INTO_DANGER;
            //cout << "Brick " << decision << " moved into danger!" << endl;
        }else if(action == 8){                           // Move out of danger
            reward += REWARD_MOVE_OUT_OF_DANGER;
            //cout << "Brick " << decision << " moved out of danger!" << endl;
        }else if(action == 9){
            reward += REWARD_MOVE;
            //cout << "Brick " << decision << " is moved!" << endl;
        }else{
          //  cout << endl << endl << "ERROR! ERROR!" << endl << endl;
        }

        reward = 0;
/*
        if(previous_action != 5 && pos_start_of_turn[previous_decision] == -1){
          cout << "Brick " << previous_action << " got killed! Getting negative reward for previous move! " << endl;
          reward += REWARD_GET_KILLED; 
        }
*/
        //cout << "The total reward = " << reward << endl;
        //cout << "What the the elements = " << q_table[state][actions[0]] << ", "  << q_table[state][1] << ", "  << q_table[state][2] << ", "  << q_table[state][3] << endl;
        double maxQ = -100000;
        for (size_t i = 0; i < nActions; i++)
        {
            if(q_table[state][i] > maxQ)
                maxQ = q_table[state][i]; 
        }
        
        //cout << "The maximum Q = " << maxQ << endl;
    
        if(previous_reward != -1 && previous_state != -1 && previous_action != -1 && LEARNING_RATE){
          //  cout << "UPDATING Q TABEL!" << endl;
          //  cout << q_table[previous_state][previous_action] << " + " << LEARNING_RATE << " * ( " << previous_reward << " + " << DISCOUNT_FACTOR << " * " << maxQ << " - " << q_table[previous_state][previous_action] << " )" << endl;
            q_table[previous_state][previous_action] += LEARNING_RATE * (previous_reward + DISCOUNT_FACTOR * maxQ - q_table[previous_state][previous_action]);
        //    cout << "Q value table updated = " << q_table[previous_state][previous_action] << ", with action = " << previous_action << " and state = " << previous_state << endl << endl;
        }

        previous_action = action;
        previous_state = state;
        previous_decision = decision;
        previous_reward = reward;
    }
}

void q_player::print_q_table(){
    cout << endl;
    cout << " --  Q TABLE -- " << endl;
     for (size_t i = 0; i < nStatesNew; i++)
    {
        for (size_t j = 0; j < nActions; j++)
        {
            cout << q_table[i][j] << ", ";
        }
        cout << endl;

    }

    cout << endl << endl;

}



bool q_player::inDanger(int brick, int enemy){
    if(brick != -1 && enemy != -1 && enemy != 99 && enemy <= 50){
        int enemy_max = enemy + 6; 
        if(enemy_max > 50){
            if(enemy < brick){
                enemy_max = 50; 
            }else{
                enemy_max -= 51; 
                enemy -= 51;  
            }

            if(enemy < brick && brick <= enemy_max && brick != 0){
                //cout << "Enemy < brick < enemy_max = " << enemy << " < " << brick << " < " << enemy_max << endl; 
                return true; 
            }

        }else{
            if(enemy < brick && brick <= enemy_max){
                //cout << "Enemy < brick < enemy_max = " << enemy << " < " << brick << " < " << enemy_max << endl; 
                return true;
            }
        }
    }
    return false; 
}

bool q_player::closeToEnemy(int brick, int enemy){
    if(brick != -1 && enemy != -1 && enemy != 99 && enemy <= 50){

    }

    return false;
}



vector<int> q_player::get_states(){
    vector<int> states;
    //cout << "The dice roll was = " << dice_roll << endl; 
    for (size_t i = 0; i < 4; i++) // For all 4 bricks
    {   
       int state = pos_start_of_turn[i] + 1; 
       
        if(state > 56){
            state = 57; 
        }
        states.push_back(state);
    }
    
    //cout << "States found = " << states[0] << ", " << states[1] << ", " << states[2] << ", " << states[3] << endl;
    
    return states; 
}

// Actions
// 0 - Can move out of home 
// 1 - Can hit star 
// 2 - Can protect brick = Can hit globe + Can move into goal strech + Can form blockage
// 3 - Can hit goal
// 4 - Can kill 
// 5 - Can suicide 
// 6 - Cant move
// 7 - Can move into danger 
// 8 - Can move out of danger
// 9 - Can move normal


vector<int> q_player::get_actions(){
    vector<int> actions; 
    for (size_t i = 0; i < 4; i++) 
    {   
       // cout << "Inspecting state for brick[" << i << "] " << endl;
        int start_pos = pos_start_of_turn[i]; 
        int new_pos = pos_start_of_turn[i] + dice_roll; 
    
        int action = -1; 
        // Check if cant move out of home
        if(start_pos == -1 && dice_roll == 6){
            action = 0;
          // cout << "Setting state to can be moved out from home!" << endl;
        }else if(start_pos != -1 && start_pos != 99){

            // Can form blockade
            for (size_t j = 0; j < 4; j++) {
                if (new_pos == pos_start_of_turn[j] && i != j && pos_start_of_turn[j] != 99) {
                    action = 2;
                    break;
                }
            }
            
            bool avoidedDanger = false;  
            for (size_t j = 4; j < 16; j++)
            {
                // Can move into danger
                if(!inDanger(start_pos, pos_start_of_turn[j]) && inDanger(new_pos, pos_start_of_turn[j]) || (new_pos % 13 == 0 && new_pos != 0)) {
                    action = 7;
                }
                // Can move out of danger
                if(inDanger(start_pos, pos_start_of_turn[j])){
                   if(!inDanger(new_pos, pos_start_of_turn[j])){
                       avoidedDanger = true; 
                   }else{
                       avoidedDanger = false; 
                   }
                }

            }
            
            if(avoidedDanger){
                action = 8;
            }


            // Checking for moving into same are as enemy
            bool kill = false; 
            bool suicide = false; 
            for (size_t j = 4; j < 16; j++)
            {
                // Check if can kill
                if(new_pos == pos_start_of_turn[j] && pos_start_of_turn[j] <= 50){

                    kill = true; 
                  
                  //  cout << "Inspect if kill will end with suicide = " << pos_start_of_turn[j] << " modulus 13 = " << pos_start_of_turn[j]%13 << endl; 
                    if (((pos_start_of_turn[j]-8) % 13) == 0 || ((pos_start_of_turn[j]-8) % 18) == 0) {
                        suicide = true;
                        break;
                    }

                    for (size_t k = 0; k < 4; k++)
                    {
                        int index = int(j/4) * k;
                        if( new_pos == pos_end_of_turn[index] && index != j){
                            suicide = true; 
                            break; 
                        }
                    }
                }
            }

            // Can suicide 
            if (suicide) { 
                action = 5;
            } else if (kill && !suicide) { // Can kill
                action = 4;
            }

            // Can hit a star 
            if( (((new_pos - 5) % 13 == 0 || (new_pos - 11) % 11 == 0)) && pos_start_of_turn[i] < 50){
                action = 1; 
            } 
        
            // Can move into goal 
            if (new_pos == 56 || new_pos == 50){
                action = 3;
            }

            //  Can move into goal strech or hit globe 
            if ((new_pos > 50 && start_pos <= 50) || ((new_pos - 8) % 13 == 0 && suicide == false && pos_start_of_turn[i] < 47) ) {
                action = 2;
            }
         
             // Can move normal
            if (action == -1 && new_pos > start_pos){
                action = 9;
            }
      
        }else{
            // Cant move brick
            action = 6; 
        }
        actions.push_back(action);
      
    }
        
    return actions; 
}


int q_player::convertToSingleState(vector<int> &states){
    int jointState = 0; 
    for (size_t i = 0; i < 4; i++)
    {
        int newBitState = 0; 
        newBitState = states[i] << i*6;
        jointState = jointState | newBitState; 
    }
    //cout << "Size of joint state = " << jointState << endl; 
    if(jointState > nStatesNew -2 ){
        cout << "ERROR!" << endl; 
    }
    return jointState;
}


int q_player::select_action(vector<int>& states, vector<int>& actions){

    int best_action = -1; 
    int state = convertToSingleState(states); 
    if(EXPLORE_RATE == 0 ||  ((double)(rand() % 1000) / 1000.0 > EXPLORE_RATE)){
        double max_q = -100000;
     //  cout << "Possible actions are = " << actions[0] << ", " << actions[1] << ", " << actions[2] << ", " << actions[3] << endl;
     //   cout << "Q tabular values = " << q_table[states[0]][actions[0]] << ", " << q_table[states[1]][actions[1]] << ", " << q_table[states[2]][actions[2]] << ", " << q_table[states[3]][actions[3]] << endl;
        for (size_t i = 0; i < 4; i++)
        {
            if(q_table[state][actions[i]] > max_q && actions[i] != 6){
                max_q = q_table[state][actions[i]];
                best_action = i; 
            }
        }
       // cout << "Best action was = " << best_action << endl;
        
    }else{
        bool can_move_token = false; 
        for (size_t i = 0; i < 4; i++)
        {
            if(actions[i] != 6){
                can_move_token = true; 
                break; 
            }
        }

        while(true && can_move_token){
            best_action = rand() % 4; 
            if(actions[best_action] != 6)
                break; 
        }        
    }

    return best_action;
}


int q_player::make_decision(){
    vector<int> states = get_states();
    vector<int> actions = get_actions();
    int decision = select_action(states, actions);
    if(traning)
        get_reward(states, actions, decision);
    return decision; 
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

void q_player::get_winner(int id){ 
    if(id == 0){
        games_won++;
        total_games_won++;
        if(previous_action >= 0 && previous_state >= 0 && LEARNING_RATE && traning)
            q_table[previous_state][previous_action] += LEARNING_RATE * (previous_reward + DISCOUNT_FACTOR * 0.7 - q_table[previous_state][previous_action]);

    }else{
        if(previous_action >= 0 && previous_state >= 0 && LEARNING_RATE && traning)
            q_table[previous_state][previous_action] += LEARNING_RATE * (previous_reward + DISCOUNT_FACTOR * (-0.1) - q_table[previous_state][previous_action]);
    }



    int nGames = 1000;
    games_played++;
	if(games_played % nGames == 0){
        static std::clock_t start;
        double time_duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
        cout << "games played:   " << games_played << endl
             << "games won:      " << total_games_won << endl
             << "win rate local: " << ((float)games_won / (float)nGames) * 100.0 << " %" << endl
             << "win rate total: " << ((float)total_games_won / (float)games_played) * 100.0 << " %" << endl
             << "game speed:     " << 60.0/(time_duration/nGames) << " games/min" << endl
             << "explore rate:   " << EXPLORE_RATE << endl 
             << "leraning rate:  " << LEARNING_RATE << endl; 
        start = std::clock();

        std::ofstream myfile;
        myfile.open ("../test-data-random-1mil-large-lr=0.8-dis=0.95.csv", std::ios::out | std::ios::app);
        myfile << games_played << ", " << (((float)total_games_won / (float)games_played) * 100.0) << ", " << (((float)games_won / (float)nGames) * 100.0) << ", " <<  EXPLORE_RATE << ", " <<  LEARNING_RATE << endl;  
        myfile.close(); 

        games_won = 0;
    }

    if(games_played == iterations){
        traning = false; 
    }


    if(games_played == iterations + 10000){
      //  print_q_table();
        while(true);  
    }

    EXPLORE_RATE = EXPLORE_RATE - EXPLORE_RATE_DECAY;
  //  LEARNING_RATE = LEARNING_RATE + LEARNING_RATE_INC;

    if(EXPLORE_RATE < 0.05){
        EXPLORE_RATE = 0; 
    }
    
    if(LEARNING_RATE > 0.89){
        LEARNING_RATE = 0.9; 
    }

    // Resetting states
    previous_action = -1;
    previous_state = -1; 
    previous_reward = -1;


}
