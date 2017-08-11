#include "stdafx.h"
#include "Card.h"
#include "ErrorValues.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

Card::Card() : s_(), r_(), cardFacingUp_(true) {}

//get enum value for suit
int Card::s(char c) {
	if (c == 'C' || c == 'c') {
		s_ = C;
		return 0;
	}
	if (c == 'D' || c == 'd') {
		s_ = D;
		return 0;
	}
	if (c == 'H' || c == 'h') {
		s_ = H;
		return 0;
	}
	if (c == 'S' || c == 's') {
		s_ = S;
		return 0;
	}
	//invalid suit
	else {
		return 1;
	}
}

//get enum value for rank
int Card::r(char c) {
	if (c == '2') {
		r_ = two;
		return 0;
	}
	if (c == '3') {
		r_ = three;
		return 0;
	}
	if (c == '4') {
		r_ = four;
		return 0;
	}
	if (c == '5') {
		r_ = five;
		return 0;
	}
	if (c == '6') {
		r_ = six;
		return 0;
	}
	if (c == '7') {
		r_ = seven;
		return 0;
	}
	if (c == '8') {
		r_ = eight;
		return 0;
	}
	if (c == '9') {
		r_ = nine;
		return 0;
	}
	if (c == '1') {
		r_ = ten;
		return 0;
	}
	if (c == 'J' || c == 'j') {
		r_ = jack;
		return 0;
	}
	if (c == 'Q' || c == 'q') {
		r_ = queen;
		return 0;
	}
	if (c == 'K' || c == 'k') {
		r_ = king;
		return 0;
	}
	if (c == 'A' || c == 'a') {
		r_ = ace;
		return 0;
	}
	//invalid rank
	else {
		return 1;
	}
}

//mutator with int
int Card::s(unsigned int i) {
	switch (i) {
	case 0: s_ = C; return 0;
	case 1: s_ = D; return 0;
	case 2: s_ = H; return 0;
	case 3: s_ = S; return 0;
	}
}

int Card::r(unsigned int i) {
	switch (i) {
	case 0: r_ = two; return 0;
	case 1: r_ = three; return 0;
	case 2: r_ = four; return 0;
	case 3: r_ = five; return 0;
	case 4: r_ = six; return 0;
	case 5: r_ = seven; return 0;
	case 6: r_ = eight; return 0;
	case 7: r_ = nine; return 0;
	case 8: r_ = ten; return 0;
	case 9: r_ = jack; return 0;
	case 10: r_ = queen; return 0;
	case 11: r_ = king; return 0;
	case 12: r_ = ace; return 0;
	}
}

//accessors
Card::Suit Card::s() const {
	return s_;
}

Card::Rank Card::r() const {
	return r_;
}

int Card::readCard(std::string cardString) {
	const char * cardC = cardString.c_str();
	//if number is ten
	if (cardString.length() == 3) {
		if (*cardC == '1' && *(cardC + 1) == '0') {
			if (r(*cardC) == 0 && s(*(cardC + 2)) == 0) {
				return 0;
			}
			else {
				return 1;
			}
		}
		else {
			return 1;
		}
	}
	//other cards
	if (cardString.length() == 2) {
		if (r(*cardC) == 0 && s(*(cardC + 1)) == 0) {
			return 0;
		}
		else {
			return 1;
		}
	}
	//invalid card
	else {
		return 1;
	}

}

//less than operator
bool Card::operator<(const Card & c) const {
	return (r_ < c.r_) || ((r_ == c.r_) && (s_ < c.s_));
}

//card asString
std::string Card::asString() const {
	if (cardFacingUp_) {
		return alwaysShowCard();
	}
	else {
		return "*";
	}
	
}

//always show card
std::string Card::alwaysShowCard() const {
	std::string card;
	std::string rank;
	std::string suit;

	//convert enum value into string
	switch (r_) {
	case two: rank = "2"; break;
	case three: rank = "3"; break;
	case four: rank = "4"; break;
	case five: rank = "5"; break;
	case six: rank = "6"; break;
	case seven: rank = "7"; break;
	case eight: rank = "8"; break;
	case nine: rank = "9"; break;
	case ten: rank = "10"; break;
	case jack: rank = "J"; break;
	case queen: rank = "Q"; break;
	case king: rank = "K"; break;
	case ace: rank = "A"; break;
	}
	switch (s_) {
	case C: suit = "C"; break;
	case D: suit = "D"; break;
	case H: suit = "H"; break;
	case S: suit = "S"; break;
	}

	return rank + suit;
}

//print card
int Card::printCard() const {
	std::string hand;
	hand = asString();
	std::cout << hand;
	return 0;
}

//equivalence operator
bool Card::operator== (const Card & c) const {
	if ((s_ == c.s_) && (r_ == c.r_)) {
		return true;
	}
	else {
		return false;
	}
}
