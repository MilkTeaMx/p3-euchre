#include <iostream>
#include <cassert>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <utility>


#include "Player.hpp"
#include "Card.hpp"
#include "Pack.hpp"

using namespace std;


class Game {
    private:
        vector<Player*> players;
        Card upcard;
        Pack pack;
        Suit trump;
        int points_to_win;
        int team1score;
        int team2score;
        bool bool_shuffle;
        int dealer_index; //which player is dealer 0-3
        int hand_num; //what round of game its on
        
        bool team1_ordered_up;


    public:
        Game(int points_to_win, bool shuffleOption, 
            vector<pair<string, string>> inputPlayers, ifstream& packFin) : 
            pack(packFin), points_to_win(points_to_win), team1score(0), 
            team2score(0), bool_shuffle(shuffleOption), //2long
            dealer_index(0), hand_num(0) {
                
                for (pair<string, string> p : inputPlayers) {
                    Player* player = Player_factory(p.first, p.second);
                    players.push_back(player);
                }
                
                if (bool_shuffle) {
                    shuffle();
                }
        }

        ~Game() { //destructor
            for (size_t i = 0; i < players.size(); ++i) {
                delete players[i];
            } 
        }

        void play() {
            // Game starts
            while (team1score < points_to_win && team2score < points_to_win) {
                pack.reset();
                if (bool_shuffle) { pack.shuffle(); }

                cout << "Hand " << hand_num << endl;

                cout << players[dealer_index]->get_name() << " deals" << endl;
                deal(); //gives each person 5 new cards

                cout << upcard << " turned up" << endl;
                make_trump(); //sets upcard / trump
                
                play_hand(); //play the hand out and update each teams score

                hand_num++;
                dealer_index = (dealer_index + 1) % 4;
            }       

            if (team1score > team2score) {
                cout << players[0]->get_name() << " and " << 
                players[2]->get_name() << " win!" <<endl;
            } else {
                cout << players[1]->get_name() << " and " << 
                players[3]->get_name() << " win!" <<endl;
            }
        }

        void shuffle() {
            pack.shuffle();
        }

        void deal() {
            int batches[8] = {3, 2, 3, 2, 2, 3, 2, 3};
            //deal 5 cards to each player starting from left of dealer
            for (int round = 0; round < 2; ++round) {
                for ( int i = 0; i < 4; i++) {
                    
                    int player_index = (dealer_index + 1 + i) % 4;

                    for (int j = 0; j < batches[round*4 + i]; j++){
                        players[player_index]->add_card(pack.deal_one());
                    }

                }
            }

            upcard = pack.deal_one();
        }

        void make_trump() {
            //Round 1:
            Suit orderUp = upcard.get_suit();
            for (int i = 1; i < 5; i++) {
                int current_player = (dealer_index + i) % 4;
                bool is_dealer = (current_player == dealer_index);
                if (players[current_player]->make_trump(upcard, is_dealer, 1, orderUp)){
                    trump = upcard.get_suit();
                    
                    cout << players[current_player]->get_name() << 
                     " orders up " << orderUp <<endl;

                    cout << "round1" <<endl;
                    players[dealer_index]->add_and_discard(upcard);
                    if (current_player == 0 || current_player == 2) {
                        team1_ordered_up = true;
                    } else {
                        team1_ordered_up = false;
                    }

                    return;
                }

                cout << players[current_player]->get_name() <<  " passes" <<endl; 
            }

            //Round 2:
            for (int i = 1; i < 5; i++) {
                int current_player = (dealer_index + i) % 4;
                bool is_dealer = (current_player == dealer_index);
                if (players[current_player]->make_trump(upcard, is_dealer, 2, orderUp)){
                    trump = orderUp;

                    cout << players[current_player]->get_name() << 
                     " orders up " << orderUp <<endl;

                    cout << "round2" <<endl;

                    if (current_player == 0 || current_player == 2) {
                        team1_ordered_up = true;
                    } else {
                        team1_ordered_up = false;
                    }

                    return;
                }
                cout << players[current_player]->get_name() <<  " passes" <<endl; 
            }
        }

        void play_hand() {
            int team1_tricks_won = 0;
            int team2_tricks_won = 0; 

            int lead_player_index = (dealer_index + 1) % 4;
            
            int round_number = 0;

            while (round_number < 5) {
                Card led_card = players[lead_player_index]->lead_card(trump);
                string player_name = players[lead_player_index]->get_name();
                cout << led_card << " led by " << player_name << endl;

                int trick_player_index = lead_player_index;
                Card trick_card = led_card;
                Player* trick_player = players[lead_player_index];

                for (int i = 1; i < 4; ++i) {
                    int next_player_index = (lead_player_index + i) % 4;

                    Player* curr_player = players[next_player_index];
                    Card curr_card = curr_player->play_card(led_card, trump);

                    string curr_player_name = curr_player->get_name();

                    cout << curr_card << " played by " << curr_player_name << endl;

                    if (Card_less(trick_card, curr_card, led_card, trump)) {
                        trick_card = curr_card;
                        trick_player_index = next_player_index;
                        trick_player = curr_player;
                    }
                }

                cout << trick_player->get_name() << " takes the trick" << endl;
                lead_player_index = trick_player_index;

                if (trick_player_index % 2 == 0) {
                    team1_tricks_won += 1;
                } 
                else {
                    team2_tricks_won += 1;
                }

                round_number += 1;
            }

            award_score(team1_tricks_won, team2_tricks_won);
        }


        void award_score(int team1wins, int team2wins) {

            if (team1wins > team2wins) {
                cout << players[0]->get_name() << " and " << players[2]->get_name()
                << " win the hand" << endl;

                if (team1_ordered_up) {
                    if (team1wins == 3 || team1wins == 4) {
                        team1score += 1;
                    }
                    else if (team1wins == 5) { //march
                        team1score += 2;
                            cout << "march!" <<endl;
                        }
                } else {
                    team1score += 2;
                    cout << "euchred!" <<endl;
                }
               
            }
            
            if (team2wins > team1wins) {
                cout << players[1]->get_name() << " and " << players[3]->get_name() << " win the hand" << endl;
                if (!team1_ordered_up) {
                    if (team2wins == 3 || team2wins == 4) {
                        team2score += 1;
                    }
                    else if (team2wins == 5) { //march
                        team2score += 2;
                        cout << "march!" <<endl;
                    }
                } else {
                    team2score += 2;
                    cout << "euchred!" <<endl;
                }
            }

            cout << players[0]->get_name() << " and " << players[2]->get_name() 
            << " have " << team1score << " points" << endl;

            cout << players[1]->get_name() << " and " << players[3]->get_name() 
            << " have " << team2score << " points" << endl;
        }

};

int incorrect_usage() {
    cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
        << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
        << "NAME4 TYPE4" << endl;
    return 1;
}

int main(int argc, char* argv[]) {
    if (argc != 12) {
        return incorrect_usage();
    } else if (stoi(argv[3]) < 1 && stoi(argv[3]) > 100){
        cout << "1";
        return incorrect_usage();
    } else if (string(argv[2]) != "shuffle" && string(argv[2]) != "noshuffle") {
        cout << "2";
        return incorrect_usage();
    } else if (string(argv[5]) != "Simple" && string(argv[5]) != "Human") {
        cout << "3";
        return incorrect_usage();
    } else if (string(argv[7]) != "Simple" && string(argv[7]) != "Human") {
        cout << "4";
        return incorrect_usage();
    } else if (string(argv[9]) != "Simple" && string(argv[9]) != "Human") {
        cout << "5";
        return incorrect_usage();
    } else if (string(argv[11]) != "Simple" && string(argv[11]) != "Human") {
        cout << "6";
        return incorrect_usage();
    }

    cout << string(argv[0]) << " " << string(argv[1]) << " " << string(argv[2]) << " " 
        << string(argv[3]) << " " << string(argv[4]) << " " << string(argv[5]) << " "  //Too long
        << string(argv[6]) << " " << string(argv[7]) << " " << string(argv[8]) << " " 
        << string(argv[9]) << " " << string(argv[10]) << " " << string(argv[11]) << " " <<endl; //Too long

    ifstream fin(argv[1]);

    
    if(!fin.is_open()) {
        cout << "Error opening " << string(argv[1]) << endl;
        return 1;
    }

    bool shuffle = (string(argv[2]) == "shuffle");
    int points_to_win = stoi(argv[3]);
    vector<pair<string, string>> inputPlayers;
    inputPlayers.push_back(make_pair(string(argv[4]), string(argv[5])));
    inputPlayers.push_back(make_pair(string(argv[6]), string(argv[7])));
    inputPlayers.push_back(make_pair(string(argv[8]), string(argv[9])));
    inputPlayers.push_back(make_pair(string(argv[10]), string(argv[11])));

    Game game(points_to_win, shuffle, inputPlayers, fin);
    game.play();
}


