#ifndef CARD_H
#define CARD_H

#include <string>
#include <vector>

struct Card {

	enum Suit { C, D, H, S };
	enum Rank { two, three, four, five, six, seven, eight, nine, ten, jack, queen, king, ace };

	//member variables
	Suit s_;
	Rank r_;

	//face up or down
	bool cardFacingUp_;

	//default constructor
	Card();

	//mutator
	int s(char);
	int r(char);

	//mutator with number
	int s(unsigned int);
	int r(unsigned int);

	//accessor
	Suit s() const;
	Rank r() const;

	//convert card definition strings into card structs
	int readCard(std::string);

	//less than operator
	bool operator<(const Card &) const;

	//card asString
	std::string asString() const;

	//shows cards regardless of face
	std::string alwaysShowCard() const; 

	//print card
	int printCard() const;

	//equivalence operator
	bool operator== (const Card &) const;

};

#endif /* CARD_H */