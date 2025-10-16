#include "Player.hpp"
#include "unit_test_framework.hpp"

#include <iostream>

using namespace std;

// Tests Get Name in multiple contexts
TEST(test_player_get_name) {
    // Basic Check for Get Name
    Player * alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());
    delete alice;

    Player * bob = Player_factory("Bob", "Simple");
    ASSERT_EQUAL("Bob", bob->get_name());
    delete bob;

    // Check with Const
    const Player * manav = Player_factory("Manav", "Simple");
    ASSERT_EQUAL("Manav", manav->get_name());
    delete manav;

    // Check for Reference Consistency 
    Player *max = Player_factory("Max", "Simple");
    const string &reference_check_1 = max->get_name();
    const string &reference_check_2 = max->get_name();
    ASSERT_TRUE(&reference_check_1 == &reference_check_2);
    delete max;
}

// Tests Add Card in basic functionality
TEST(test_add_card_basic) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card(FIVE, CLUBS);
    manav->add_card(card);

    Card led_card = manav->lead_card(HEARTS);
    ASSERT_EQUAL(led_card, card);

    delete manav;
}

// Test Add Card with multiple, unique cards
TEST(test_add_card_multiple) {
    Player *max = Player_factory("Max", "Simple");
    Card card1(FIVE, CLUBS);
    Card card2(NINE, DIAMONDS);
    Card card3(JACK, CLUBS);
    Card card4(QUEEN, HEARTS);
    Card card5(ACE, SPADES);

    max->add_card(card1);
    max->add_card(card2);
    max->add_card(card3);
    max->add_card(card4);
    max->add_card(card5);

    vector<Card> removed_cards;
    for (int i = 0; i < 5; ++i) {
        removed_cards.push_back(max->lead_card(HEARTS));
    }

    ASSERT_EQUAL(removed_cards.size(), 5);

    delete max;
}

// Test Add Card max hand size behavior
TEST(test_add_card_max_hand) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(FIVE, CLUBS);
    Card card2(NINE, DIAMONDS);
    Card card3(JACK, CLUBS);
    Card card4(QUEEN, HEARTS);
    Card card5(ACE, SPADES);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    // We can ensure lead card can be used 5 times'
    manav->lead_card(HEARTS);
    manav->lead_card(HEARTS);
    manav->lead_card(HEARTS);
    manav->lead_card(HEARTS);
    manav->lead_card(HEARTS);

    delete manav;
}

// Ensure add card values are not changing
TEST(test_add_card_values) {
    Player *max = Player_factory("Max", "Simple");
    Card card(ACE, SPADES);
    max->add_card(card);

    Card led_card = max->lead_card(SPADES);
    ASSERT_EQUAL(led_card.get_rank(), ACE);
    ASSERT_EQUAL(led_card.get_suit(), SPADES);

    delete max;
}

// Test multiple players and ensure add card is correct
TEST(test_add_card_multi_players) {
    Player * max = Player_factory("Max", "Simple");
    Player * manav = Player_factory("Manav", "Simple");

    Card card1(FIVE, CLUBS);
    Card card2(NINE, DIAMONDS);
    
    max->add_card(card1);
    manav->add_card(card2);

    ASSERT_EQUAL(max->lead_card(CLUBS), card1);
    ASSERT_EQUAL(manav->lead_card(CLUBS), card2);

    delete max;
    delete manav;
}

// Test Order up with 2 face/ace cards in round 1
TEST(test_make_trump_two_faceace_cards_r1) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(FIVE, CLUBS);
    Card card2(JACK, HEARTS);
    Card card3(JACK, CLUBS);
    Card card4(NINE, HEARTS);
    Card card5(ACE, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);
    
    Suit order_up_suit = CLUBS;
    Card upcard(TEN, HEARTS);
    bool answer = manav->make_trump(upcard, false, 1, order_up_suit);
    ASSERT_TRUE(answer);
    ASSERT_EQUAL(order_up_suit, HEARTS);

    delete manav;
}

// Test Order up with 3 face/ace cards in round 1
TEST(test_make_trump_three_faceace_cards_r1) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(FIVE, CLUBS);
    Card card2(JACK, HEARTS);
    Card card3(JACK, CLUBS);
    Card card4(KING, HEARTS);
    Card card5(JACK, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);
    
    Suit order_up_suit = CLUBS;
    Card upcard(TEN, HEARTS);
    bool answer = manav->make_trump(upcard, false, 1, order_up_suit);
    ASSERT_TRUE(answer);
    ASSERT_EQUAL(order_up_suit, HEARTS);

    delete manav;
}

// Test Order up with 1 face card in round 1
TEST(test_make_trump_one_faceace_card_r1) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(FIVE, CLUBS);
    Card card2(JACK, SPADES);
    Card card3(JACK, CLUBS);
    Card card4(NINE, HEARTS);
    Card card5(ACE, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);
    
    Suit order_up_suit = CLUBS;
    Card upcard(TEN, HEARTS);
    bool answer = manav->make_trump(upcard, false, 1, order_up_suit);
    ASSERT_FALSE(answer);
    ASSERT_EQUAL(order_up_suit, CLUBS);

    delete manav;
}

// Test Order up with face cards of other suits
TEST(test_make_trump_faceace_card_offsuits_r1) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(JACK, CLUBS);
    Card card2(JACK, SPADES);
    Card card3(JACK, CLUBS);
    Card card4(JACK, CLUBS);
    Card card5(ACE, SPADES);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);
    
    Suit order_up_suit = CLUBS;
    Card upcard(TEN, HEARTS);
    bool answer = manav->make_trump(upcard, false, 1, order_up_suit);
    ASSERT_FALSE(answer);
    ASSERT_EQUAL(order_up_suit, CLUBS);

    delete manav;
}

// Test Order up with same suit but not ranked high enough
TEST(test_make_trump_faceace_card_lowrank_suits_r1) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, HEARTS);
    Card card2(NINE, HEARTS);
    Card card3(NINE, HEARTS);
    Card card4(NINE, HEARTS);
    Card card5(NINE, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);
    
    Suit order_up_suit = CLUBS;
    Card upcard(TEN, HEARTS);
    bool answer = manav->make_trump(upcard, false, 1, order_up_suit);
    ASSERT_FALSE(answer);
    ASSERT_EQUAL(order_up_suit, CLUBS);

    delete manav;
}

// Test dealer vs non-dealer (should make no difference in r1)
TEST(test_make_trump_faceace_card_dealer_r1) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, HEARTS);
    Card card2(NINE, HEARTS);
    Card card3(NINE, HEARTS);
    Card card4(NINE, HEARTS);
    Card card5(NINE, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);
    
    Suit order_up_suit = CLUBS;
    Card upcard(TEN, HEARTS);
    bool answer = manav->make_trump(upcard, true, 1, order_up_suit);
    ASSERT_FALSE(answer);
    ASSERT_EQUAL(order_up_suit, CLUBS);

    delete manav;
}

// Test no face/ace of same color suit + not dealer
TEST(test_make_trump_nofaceace_nextsuit_r2) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, HEARTS);
    Card card2(NINE, HEARTS);
    Card card3(NINE, HEARTS);
    Card card4(NINE, HEARTS);
    Card card5(NINE, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Suit order_up_suit = CLUBS;
    Card upcard(JACK, HEARTS);
    bool answer = manav->make_trump(upcard, false, 2, order_up_suit);
    ASSERT_FALSE(answer);
    ASSERT_EQUAL(order_up_suit, CLUBS);

    delete manav;
}

// Test screw the dealer
TEST(test_make_trump_screwdealer_r2) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, HEARTS);
    Card card2(NINE, HEARTS);
    Card card3(NINE, HEARTS);
    Card card4(NINE, HEARTS);
    Card card5(NINE, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Suit order_up_suit = CLUBS;
    Card upcard(JACK, HEARTS);
    bool answer = manav->make_trump(upcard, true, 2, order_up_suit);
    ASSERT_TRUE(answer);
    ASSERT_EQUAL(order_up_suit, DIAMONDS);

    delete manav;
}

// Test next suit same color in round 2
TEST(test_make_trump_next_suit_r2) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(ACE, CLUBS);
    Card card2(NINE, HEARTS);
    Card card3(NINE, HEARTS);
    Card card4(NINE, HEARTS);
    Card card5(NINE, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Suit order_up_suit = HEARTS;
    Card upcard(JACK, SPADES);
    bool answer = manav->make_trump(upcard, false, 2, order_up_suit);
    ASSERT_TRUE(answer);
    ASSERT_EQUAL(order_up_suit, CLUBS);

    delete manav;
}

// Test face/ace but opposite color as orderup
TEST(test_make_trump_next_oppositecolor_r2) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(ACE, SPADES);
    Card card2(NINE, SPADES);
    Card card3(NINE, CLUBS);
    Card card4(NINE, CLUBS);
    Card card5(NINE, CLUBS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Suit order_up_suit = SPADES;
    Card upcard(NINE, HEARTS);
    bool answer = manav->make_trump(upcard, false, 2, order_up_suit);
    ASSERT_FALSE(answer);
    ASSERT_EQUAL(order_up_suit, SPADES);

    delete manav;
}

// Test order up only changes when make_trump is true
TEST(test_make_trump_only_order_up_if_true) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, SPADES);
    Card card2(NINE, SPADES);
    Card card3(NINE, CLUBS);
    Card card4(NINE, CLUBS);
    Card card5(NINE, CLUBS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Suit order_up_suit = SPADES;
    Card upcard(NINE, HEARTS);
    bool answer = manav->make_trump(upcard, false, 2, order_up_suit);
    ASSERT_FALSE(answer);
    ASSERT_EQUAL(order_up_suit, SPADES);

    delete manav;
}

// Tests basic version of removing lowest card
TEST(test_add_and_discard_basic) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, CLUBS);
    Card card2(ACE, CLUBS);
    Card card3(KING, CLUBS);
    Card card4(TEN, CLUBS);
    Card card5(JACK, CLUBS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card upcard(QUEEN, HEARTS);
    manav->add_and_discard(upcard);

    Card expected_card(NINE, CLUBS);
    for (int i = 0; i < 5; ++i) {
        Card card = manav->lead_card(HEARTS);
        ASSERT_NOT_EQUAL(card, expected_card);
    }

    delete manav;
}

// Tests if upcard itself is removed
TEST(test_add_and_discard_upcard) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(TEN, SPADES);
    Card card2(ACE, SPADES);
    Card card3(KING, SPADES);
    Card card4(TEN, SPADES);
    Card card5(JACK, SPADES);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card upcard(NINE, SPADES);
    manav->add_and_discard(upcard);

    for (int i = 0; i < 5; ++i) {
        Card card = manav->lead_card(SPADES);
        ASSERT_NOT_EQUAL(card, upcard);
    }

    delete manav;
}

// Test all trumps and lowest discarded
TEST(test_add_and_discard_all_trumps) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, HEARTS);
    Card card2(ACE, HEARTS);
    Card card3(KING, HEARTS);
    Card card4(TEN, HEARTS);
    Card card5(JACK, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card upcard(TEN, HEARTS);
    manav->add_and_discard(upcard);

    for (int i = 0; i < 5; ++i) {
        Card card = manav->lead_card(HEARTS);
        ASSERT_NOT_EQUAL(card, card1);
    }

    delete manav;
}

// Test mix of trump + non-trump
TEST(test_add_and_discard_mix_of_trumps) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, CLUBS);
    Card card2(ACE, SPADES);
    Card card3(KING, HEARTS);
    Card card4(TEN, HEARTS);
    Card card5(JACK, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card upcard(TEN, HEARTS);
    manav->add_and_discard(upcard);

    for (int i = 0; i < 5; ++i) {
        Card card = manav->lead_card(HEARTS);
        ASSERT_NOT_EQUAL(card, card1);
    }

    delete manav;
}

// Test tied lowest rank so suit is tiebreaker
TEST(test_add_and_discard_tiebreaker) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, SPADES);
    Card card2(NINE, CLUBS);
    Card card3(TEN, HEARTS);
    Card card4(QUEEN, DIAMONDS);
    Card card5(KING, DIAMONDS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card upcard(ACE, HEARTS);
    manav->add_and_discard(upcard);

    for (int i = 0; i < 5; ++i) {
        Card card = manav->lead_card(HEARTS);
        ASSERT_NOT_EQUAL(card, card1);
    }

    delete manav;
}

// Test number of cards
TEST(test_add_and_discard_num_of_cards) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, SPADES);
    Card card2(TEN, CLUBS);
    Card card3(JACK, HEARTS);
    Card card4(QUEEN, DIAMONDS);
    Card card5(KING, DIAMONDS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card upcard(ACE, HEARTS);
    manav->add_and_discard(upcard);

    for (int i = 0; i < 5; ++i) {
        manav->lead_card(HEARTS); // this should not crash!
    }

    ASSERT_TRUE(true);

    delete manav;
}

// Tests highest-nontrump with trump + non-trump cards
TEST(test_lead_card_highest_trumpmix) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, SPADES);
    Card card2(TEN, CLUBS);
    Card card3(JACK, HEARTS);
    Card card4(QUEEN, HEARTS);
    Card card5(KING, DIAMONDS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card = manav->lead_card(HEARTS);

    ASSERT_EQUAL(led_card, card5);

    delete manav;
}

// Test highest trump when only trump cards
TEST(test_lead_card_highest_trump) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, SPADES);
    Card card2(TEN, SPADES);
    Card card3(JACK, SPADES);
    Card card4(QUEEN, SPADES);
    Card card5(KING, SPADES);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card = manav->lead_card(SPADES);

    ASSERT_EQUAL(led_card, card3);

    delete manav;
}

// Tests highest card when all offsuit
TEST(test_lead_card_highest_notrump_offsuit) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, SPADES);
    Card card2(TEN, SPADES);
    Card card3(JACK, SPADES);
    Card card4(QUEEN, SPADES);
    Card card5(KING, SPADES);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card = manav->lead_card(HEARTS);

    ASSERT_EQUAL(led_card, card5);

    delete manav;
}

// Test tiebreak by suit
TEST(test_lead_card_tiebreak_by_suit) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(ACE, SPADES);
    Card card2(ACE, HEARTS);
    Card card3(ACE, CLUBS);
    Card card4(ACE, DIAMONDS);
    Card card5(KING, CLUBS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card = manav->lead_card(CLUBS);

    ASSERT_EQUAL(led_card, card4);

    delete manav;
}

// Test left bower 
// Test highest trump when only trump cards
TEST(test_lead_card_leftbower) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, SPADES);
    Card card2(TEN, SPADES);
    Card card3(JACK, CLUBS);
    Card card4(QUEEN, SPADES);
    Card card5(KING, SPADES);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card = manav->lead_card(SPADES);

    ASSERT_EQUAL(led_card, card3);

    delete manav;
}

// Test card is actually removed from leading card
TEST(test_lead_card_removal_of_card) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, SPADES);
    Card card2(TEN, SPADES);
    Card card3(JACK, CLUBS);
    Card card4(QUEEN, SPADES);
    Card card5(KING, SPADES);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card = manav->lead_card(SPADES);
    Card led_card2 = manav->lead_card(SPADES);

    ASSERT_NOT_EQUAL(led_card, led_card2);

    delete manav;
}

// Test play card, plays highest of led suit
TEST(test_play_card_follow_suit_highest) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, CLUBS);
    Card card2(TEN, CLUBS);
    Card card3(JACK, CLUBS);
    Card card4(JACK, SPADES);
    Card card5(KING, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card(NINE, CLUBS);
    Card played_card = manav->play_card(led_card, SPADES);

    ASSERT_EQUAL(played_card, card2);

    delete manav;
}

// Test play card, cannot follow suit so lowest card
TEST(test_play_card_no_follow_suit) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(NINE, SPADES);
    Card card2(TEN, DIAMONDS);
    Card card3(JACK, SPADES);
    Card card4(KING, HEARTS);
    Card card5(QUEEN, HEARTS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card(NINE, CLUBS);
    Card played_card = manav->play_card(led_card, HEARTS);

    ASSERT_EQUAL(played_card, card1);

    delete manav;
}

// Test play card, left power counts as trump
TEST(test_play_card_left_power_is_trump) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(JACK, CLUBS);
    Card card2(ACE, SPADES);
    Card card3(QUEEN, HEARTS);
    Card card4(KING, DIAMONDS);
    Card card5(TEN, CLUBS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card(ACE, SPADES);
    Card played_card = manav->play_card(led_card, SPADES);

    ASSERT_EQUAL(played_card, card1);

    delete manav;
}

// Tests left bower
TEST(test_play_card_left_bower_not_follow_nontrump) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(JACK, CLUBS);
    Card card2(KING, CLUBS);
    Card card3(QUEEN, DIAMONDS);
    Card card4(KING, HEARTS);
    Card card5(TEN, SPADES);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card(ACE, SPADES);
    Card played_card = manav->play_card(led_card, SPADES);

    ASSERT_EQUAL(played_card, card1);

    delete manav;
}

// Tests right bower beats left bower
TEST(test_play_card_right_beats_left) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(JACK, HEARTS);
    Card card2(JACK, DIAMONDS);
    Card card3(QUEEN, DIAMONDS);
    Card card4(KING, HEARTS);
    Card card5(TEN, SPADES);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card(NINE, HEARTS);
    Card played_card = manav->play_card(led_card, HEARTS);

    ASSERT_EQUAL(played_card, card1);

    delete manav;
}

// Tests lowest card played when all non-trump
TEST(test_play_card_lowest_card_non_trump) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(JACK, DIAMONDS);
    Card card2(JACK, DIAMONDS);
    Card card3(QUEEN, DIAMONDS);
    Card card4(KING, DIAMONDS);
    Card card5(TEN, DIAMONDS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card(NINE, CLUBS);
    Card played_card = manav->play_card(led_card, DIAMONDS);

    ASSERT_EQUAL(played_card, card5);

    delete manav;
}

// Tests card is removed after play_card
TEST(test_play_card_removes_card) {
    Player *manav = Player_factory("Manav", "Simple");

    Card card1(JACK, DIAMONDS);
    Card card2(JACK, DIAMONDS);
    Card card3(QUEEN, DIAMONDS);
    Card card4(KING, DIAMONDS);
    Card card5(TEN, DIAMONDS);

    manav->add_card(card1);
    manav->add_card(card2);
    manav->add_card(card3);
    manav->add_card(card4);
    manav->add_card(card5);

    Card led_card(NINE, CLUBS);
    Card played_card = manav->play_card(led_card, DIAMONDS);
    Card played_card_2 = manav->play_card(led_card, DIAMONDS);

    ASSERT_NOT_EQUAL(played_card, played_card_2);

    delete manav;
}

TEST_MAIN()