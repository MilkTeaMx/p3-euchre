#include <cassert>
#include "Player.hpp"
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class SimplePlayer : public Player {
  private:
    string name;
    vector<Card> hand;

  public:
    SimplePlayer(const std::string &name) : name(name) {}

    const std::string & get_name() const override {
        return name;
    }

    void add_card(const Card &c) override {
      assert(hand.size() < MAX_HAND_SIZE);
      hand.push_back(c);
    }

    void remove_card(const Card &c) {
      assert(hand.size() > 0);

      int index_to_remove = -1;
      for (int i = 0; i < hand.size(); i++) {
        if (hand[i] == c) {
          index_to_remove = i;
          break;
        }
      }

      assert(index_to_remove != -1);
      hand.erase(hand.begin() + index_to_remove);
    }

    bool make_trump(const Card &upcard, bool is_dealer,
               int round, Suit &order_up_suit) const override {
      assert(round == 1 || round == 2);

      if (round == 1) {
        int valuable_card_counter = 0;
        for (const Card &card : hand) {
          if (card.get_suit() == upcard.get_suit() &&
              card.is_face_or_ace()) {
                ++valuable_card_counter;
          }
        }

        if (valuable_card_counter >= 2) {
          order_up_suit = upcard.get_suit();
          return true;
        } else return false;
      } 
      else {
        int valuable_card_counter = 0;
        for (const Card &card : hand) {
          if (card.get_suit() == Suit_next(upcard.get_suit()) &&
              card.is_face_or_ace()) {
                ++valuable_card_counter;
          }
        }

        if (valuable_card_counter >= 1 || is_dealer) {
          order_up_suit = Suit_next(upcard.get_suit());
          return true;
        } else return false;
      }
    }

    void add_and_discard(const Card &upcard) override {
      assert(hand.size() >= 1 && hand.size() <= MAX_HAND_SIZE);
      
      hand.push_back(upcard);

      Card lowest_card = hand[0];
      int lowest_card_index = 0;
      for (int i = 0; i < hand.size(); ++i) {
        if (Card_less(hand[i], hand[lowest_card_index], upcard.get_suit())) {
          lowest_card = hand[i];
          lowest_card_index = i;
        }
      }

      hand.erase(hand.begin() + lowest_card_index);

    }

    Card lead_card(Suit trump) override {
      assert(!hand.empty());
      
      vector<Card> trump_cards;
      vector<Card> non_trump_cards;

      for (Card card : hand) {
        if (card.is_trump(trump)) { 
          trump_cards.push_back(card); 
        } 
        else { 
          non_trump_cards.push_back(card);
        }
      }

      Card high_card;
      if (!non_trump_cards.empty()) {
        high_card = non_trump_cards[0];
        for (const Card &card : non_trump_cards) {
          if (Card_less(high_card, card, trump)) {
            high_card = card;
          }
        }
      }
      else {
        high_card = trump_cards[0];
        for (const Card &card : trump_cards) {
          if (Card_less(high_card, card, trump)) {
            high_card = card;
          }
        }
      }

      remove_card(high_card);
      return high_card;
    }

    Card play_card(const Card &led_card, Suit trump) override {
      vector<Card> follow_suit;
      vector<Card> no_follow_suit;

      for (const Card &card : hand) {
        if (card.get_suit(trump) == led_card.get_suit(trump)) {
          follow_suit.push_back(card);
        } 
        else {
          no_follow_suit.push_back(card);
        }
      }

      Card card_to_play;
      if (!follow_suit.empty()) {
        card_to_play = follow_suit[0];
        for (const Card &card : follow_suit) {
          if (Card_less(card_to_play, card, trump)) {
            card_to_play = card;
          }
        }
      }
      else {
        card_to_play = no_follow_suit[0];
        for (const Card &card : no_follow_suit) {
          if (Card_less(card, card_to_play, trump)) {
            card_to_play = card;
          }
        }
      }

      remove_card(card_to_play);
      return card_to_play;
    }
};

class HumanPlayer : public Player {
  private: 
    string name;
    vector<Card> hand;

    void print_hand() const {
      for (size_t i=0; i < hand.size(); ++i)
        cout << "Human player " << name << "'s hand: "
            << "[" << i << "] " << hand[i] << "\n";
    }
  
  public: 
    HumanPlayer(const string &name) : name(name) {}

    const string & get_name() const override {
      return name;
    }

    void add_card(const Card &c) override {
      assert(hand.size() < MAX_HAND_SIZE);
      hand.push_back(c);
      sort(hand.begin(), hand.end());
    }

    bool make_trump(const Card &upcard, bool is_dealer,
                          int round, Suit &order_up_suit) const override {
      assert(round == 1 || round == 2);
      
      print_hand();
      cout << "Human player " << name << ", please enter a suit, or \"pass\":\n";

      string answer;
      cin >> answer;

      if (answer == "pass") {
        return false;
      }
      else {
        order_up_suit = string_to_suit(answer);
        return true;
      }
    }

    void add_and_discard(const Card &upcard) override {
      print_hand();
      hand.push_back(upcard);
  
      cout << "Discard upcard: [-1]\n";
      cout << "Human player " << name << ", please select a card to discard:\n";

      int answer;
      cin >> answer;
   
      if (answer == -1) { //discard upcard
        for (int i = 0; i < hand.size(); ++i) {
          if (hand[i] == upcard) {
            hand.erase(hand.begin() + i);

            break;
          }
        }
      }
      else {

        hand.erase(hand.begin() + answer);
      }
      sort(hand.begin(), hand.end());

    }

    Card lead_card(Suit trump) override {
      print_hand();
      cout << "Human player " << name << ", please select a card:\n";

      int answer;
      cin >> answer;

      Card selected_card = hand[answer];
      hand.erase(hand.begin() + answer);
      return selected_card;
    }

    Card play_card(const Card &led_card, Suit trump) override {
      print_hand();
      cout << "Human player " << name << ", please select a card:\n";

      int answer;
      cin >> answer;

      Card selected_card = hand[answer];
      hand.erase(hand.begin() + answer);
      return selected_card;
    }
};

Player * Player_factory(const string &name, const string &strategy) {
  if (strategy == "Simple") {
    return new SimplePlayer(name);
  }
  else if (strategy == "Human") {
    return new HumanPlayer(name);
  }

  assert(false);
  return nullptr;
}

ostream & operator<<(ostream &os, const Player &p) {
  os << p.get_name();
  return os;
}