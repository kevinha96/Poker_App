#ifndef hand_H
#define hand_H

#include "Card.h"
#include "Deck.h"

#include <Vector>

class Hand {

	friend void operator<< (Hand &, Deck &);
	friend void addCardWithFace(Hand &, Deck&, bool);
	friend void operator<< (Deck &, Hand &);
	friend bool poker_rank(Hand &, Hand &);
	friend std::vector<int> notPair(const Hand &);

	enum pokerHand { noRank, onePair, twoPair, threeOfAKind, straight, flush, fullHouse, fourOfAKind, straightFlush };

private:

	std::vector<Card> h_;

	//poker hand
	pokerHand pokerH_;

public:

	//default constructor
	Hand();

	//copy constructor
	Hand(const Hand &);

	//destructor
	~Hand();

	//assignment operator
	Hand & operator= (const Hand &);

	//size
	size_t size() const;

	//equivalence operator
	bool operator== (const Hand &) const;

	//less than operator
	bool operator< (const Hand &) const;

	//hand asString
	std::string asString() const;

	//hand show all cards
	std::string showWholeHand() const;
	
	//determine hand in vector
	void h();

	//indexing operator
	Card operator[] (size_t);

	//remove a card
	void remove_card(size_t);

};

//hand insertion operator
std::ostream & operator<< (std::ostream &, const Hand &);

//deck to hand insertion operator
void operator<< (Hand &, Deck &);

//deck to hand insertion operator function taking into consideration cards facing up or down
void addCardWithFace(Hand &, Deck&, bool);

//hand to deck insertion operator
void operator<< (Deck &, Hand &);

#endif /* hand_H */
