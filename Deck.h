#ifndef deck_H
#define deck_H

#include "Card.h"

class Hand;

class Deck {

	friend std::ostream &operator<< (std::ostream &, const Deck &);
	friend void operator<< (Hand &, Deck &);
	friend void addCardWithFace(Hand &, Deck&, bool);
	friend void operator<< (Deck &, Deck &);

private:
	std::vector<Card> d_;

public:

	//default constructor
	Deck();

	//constructor with file name parameter
	Deck(char *);

	//Deconstructor
	~Deck();

	//Load
	int load(char *);

	//shuffle deck
	void shuffleDeck();

	//number of cards
	size_t size() const;

	//add a card to the deck
	void add_card(Card);

};

//insertion operator
std::ostream &operator<< (std::ostream &, const Deck &);

//deck to deck insertion operator
void operator<< (Deck &, Deck &);

#endif /* deck_H */