#include "Card.hpp"
#include "unit_test_framework.hpp"
#include <sstream>

using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

// Add more test cases here


TEST(test_card_ctor_default) {
    Card c;
    ASSERT_EQUAL(TWO, c.get_rank());
    ASSERT_EQUAL(SPADES, c.get_suit());
}

TEST(test_get_suit_with_trump) {
    Card c(JACK, CLUBS);
    ASSERT_EQUAL(c.get_suit(SPADES), SPADES);

    Card c2(JACK, CLUBS);
    ASSERT_EQUAL(c2.get_suit(HEARTS), CLUBS);

    Card c3(JACK, DIAMONDS);
    ASSERT_EQUAL(c3.get_suit(HEARTS), HEARTS);

    Card c4(TWO, HEARTS);
    ASSERT_EQUAL(c4.get_suit(CLUBS), HEARTS);
}

TEST(test_is_face_or_ace) {
    Card c1(JACK, DIAMONDS);
    Card c2(THREE, SPADES);
    ASSERT_EQUAL(c1.is_face_or_ace(), true);
    ASSERT_EQUAL(c2.is_face_or_ace(), false);
};

TEST(test_is_right_bower) {
    Card c1(JACK, HEARTS);
    ASSERT_EQUAL(c1.is_right_bower(HEARTS), true);
    ASSERT_EQUAL(c1.is_right_bower(DIAMONDS), false);

    Card c2(QUEEN, HEARTS);
    ASSERT_EQUAL(c2.is_right_bower(HEARTS), false);
    ASSERT_EQUAL(c2.is_right_bower(DIAMONDS), false);
}


TEST(test_is_left_bower) {
    Card c1(JACK, DIAMONDS);
    ASSERT_EQUAL(c1.is_left_bower(HEARTS), true);
    ASSERT_EQUAL(c1.is_left_bower(DIAMONDS), false);

    Card c2(JACK, SPADES);
    ASSERT_EQUAL(c2.is_left_bower(CLUBS), true);
    ASSERT_EQUAL(c2.is_left_bower(SPADES), false);

    Card c3(THREE, CLUBS);
    ASSERT_EQUAL(c3.is_left_bower(SPADES), false);
}

TEST(test_is_trump) {
    Card c1(JACK, DIAMONDS);
    ASSERT_EQUAL(c1.is_trump(HEARTS), true);
    ASSERT_EQUAL(c1.is_trump(DIAMONDS), true);
    ASSERT_EQUAL(c1.is_trump(SPADES), false);

    Card c2(JACK, SPADES);
    ASSERT_EQUAL(c2.is_trump(SPADES), true);
    ASSERT_EQUAL(c2.is_trump(CLUBS), true);
    ASSERT_EQUAL(c2.is_trump(HEARTS), false);

    Card c3(THREE, HEARTS);
    ASSERT_EQUAL(c3.is_trump(HEARTS), true);
    ASSERT_EQUAL(c3.is_trump(DIAMONDS), false);
}

TEST(test_operators) {
    Card c1(JACK, SPADES);
    Card c2(KING, SPADES);
    Card c3(JACK, SPADES);
    Card c4(JACK, DIAMONDS); //highest rank in jacks

    ASSERT_EQUAL((c1 < c2), true); 
    ASSERT_EQUAL((c2 > c1), true);
    ASSERT_EQUAL((c1 <= c2), true);
    ASSERT_EQUAL((c2 >= c1), true);
    ASSERT_EQUAL((c1 <= c3), true);
    ASSERT_EQUAL((c1 >= c3), true);

    ASSERT_EQUAL((c4 < c1), false); //same rank different suit
    ASSERT_EQUAL((c1 < c4), true);    
    ASSERT_EQUAL((c1 == c4), false); 

    ASSERT_EQUAL((c1 == c3), true); //equality
    ASSERT_EQUAL((c1 != c2), true);
}

TEST(test_operator_input_output) {
    Card c1(JACK, SPADES);
    Card c2(ACE, HEARTS);

    ostringstream os;
    os << c1 << " " << c2;
    ASSERT_EQUAL(os.str(), "Jack of Spades Ace of Hearts");

    Card c3;
    Card c4;
    istringstream is("Jack of Spades Ace of Hearts");
    is >> c3 >> c4;
    ASSERT_EQUAL(c3, c1);
    ASSERT_EQUAL(c4, c2);
}

TEST(test_suit_next) {
    ASSERT_EQUAL(Suit_next(CLUBS), SPADES);
    ASSERT_EQUAL(Suit_next(SPADES), CLUBS);
    ASSERT_EQUAL(Suit_next(HEARTS), DIAMONDS);
    ASSERT_EQUAL(Suit_next(DIAMONDS), HEARTS);
}

TEST(test_Card_less_no_led) {
    Card right_bower(JACK, HEARTS);
    Card left_bower(JACK, DIAMONDS);
    Card ace_trump(ACE, HEARTS);
    Card nine_trump(NINE, HEARTS);
    Card ace_non_trump(ACE, SPADES);
    Card king_non_trump(KING, CLUBS);
    
    //right bower is highest
    ASSERT_EQUAL(Card_less(left_bower, right_bower, HEARTS), true);
    ASSERT_EQUAL(Card_less(ace_trump, right_bower, HEARTS), true);
    ASSERT_EQUAL(Card_less(right_bower, left_bower, HEARTS), false);
    
    //left bower is 2nd
    ASSERT_EQUAL(Card_less(ace_trump, left_bower, HEARTS), true);
    ASSERT_EQUAL(Card_less(left_bower, ace_trump, HEARTS), false);
    
    //trumps are 3rd
    ASSERT_EQUAL(Card_less(nine_trump, ace_non_trump, HEARTS), false);
    ASSERT_EQUAL(Card_less(ace_non_trump, nine_trump, HEARTS), true);
    ASSERT_EQUAL(Card_less(nine_trump, ace_trump, HEARTS), true);
    
    //non trumps are 4th
    ASSERT_EQUAL(Card_less(king_non_trump, ace_non_trump, HEARTS), true);
}

TEST(test_Card_less_yes_led) {
    Card right_bower(JACK, SPADES);
    Card left_bower(JACK, CLUBS);
    Card king_trump(KING, SPADES);
    Card queen_trump(QUEEN, SPADES);
    Card ace_non_anything(ACE, HEARTS);
    Card three_led(THREE, HEARTS);
    Card ace_led(ACE, HEARTS);
    Card jack_non_anything(JACK, DIAMONDS);

    Card led_card(TWO, HEARTS);

    //trump is going to be spades, hearts led


    //right bower highest
    ASSERT_EQUAL(Card_less(ace_non_anything, right_bower, led_card, SPADES), true);
    ASSERT_EQUAL(Card_less(right_bower, ace_led, led_card, SPADES), false);

    // left bower 2nd
    ASSERT_EQUAL(Card_less(left_bower, right_bower, led_card, SPADES), true);
    ASSERT_EQUAL(Card_less(left_bower, left_bower, led_card, SPADES), false);
    ASSERT_EQUAL(Card_less(king_trump, left_bower, led_card, SPADES), true);
    ASSERT_EQUAL(Card_less(left_bower, ace_led, led_card, SPADES), false);

    // trumps 3rd
    ASSERT_EQUAL(Card_less(ace_led, king_trump, led_card, SPADES), true);
    ASSERT_EQUAL(Card_less(king_trump, ace_led, led_card, SPADES), false);
    ASSERT_EQUAL(Card_less(queen_trump, king_trump, led_card, SPADES), true);
    ASSERT_EQUAL(Card_less(king_trump, ace_non_anything, led_card, SPADES), false);

    // leds 4th
    ASSERT_EQUAL(Card_less(three_led, ace_led, led_card, SPADES), true);
    ASSERT_EQUAL(Card_less(ace_led, three_led, led_card, SPADES), false);
    ASSERT_EQUAL(Card_less(three_led, queen_trump, led_card, SPADES), true);
    ASSERT_EQUAL(Card_less(three_led, right_bower, led_card, SPADES), true);
    ASSERT_EQUAL(Card_less(ace_led, queen_trump, led_card, SPADES), true);

    // normals 5th
    ASSERT_EQUAL(Card_less(jack_non_anything, ace_non_anything, led_card, SPADES), true);
    ASSERT_EQUAL(Card_less(ace_non_anything, jack_non_anything, led_card, SPADES), false);
}



TEST_MAIN()