#include "Pack.hpp"
#include <cassert>
#include <array>
#include <string>
#include <iostream>

using namespace std;

Pack::Pack() : next(0){
    int current = 0;
    for (int suit = SPADES; suit <= DIAMONDS; suit++) {
        for (int rank = NINE; rank <= ACE; rank++) {
            cards[current] = Card(static_cast<Rank>(rank), static_cast<Suit>(suit));
            current++;
        }
    }
}


Pack::Pack(std::istream& pack_input) : next(0) {
    int current = 0;
    while (current < cards.size() && pack_input >> cards[current]) {
        current++;
    }
} 

Card Pack::deal_one() {
    next++;
    return cards[next-1];
}

void Pack::reset() {
    next = 0;
}

void Pack::shuffle() {
    for (int shuffle_count = 0; shuffle_count < 7; shuffle_count++) {
        array<Card, PACK_SIZE> shuffled;

        array<Card, PACK_SIZE / 2> first_half;
        array<Card, PACK_SIZE / 2> second_half;

        for (int i = 0; i < PACK_SIZE / 2; i++) {
            first_half[i] = cards[i];
            second_half[i] = cards[i + PACK_SIZE / 2];
        }

        for (int i = 0; i < PACK_SIZE / 2; i++) {
            shuffled[2*i] = second_half[i];
            shuffled[2*i+1] = first_half[i];
        }

        cards = shuffled;
    }

    next = 0;
}

bool Pack::empty() const {
    return next >= PACK_SIZE; //next starts at 0
}




