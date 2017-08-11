#include "stdafx.h"
#include "Deck.h"
#include "Card.h"
#include "ErrorValues.h"

#include <fstream>
#include <sstream>
#include <algorithm>
#include <random>
#include <iostream>

//default constructor
Deck::Deck() : d_() {}

//constructor
Deck::Deck(char * c) {

	//check to see if file opens
	try { load(c); }
	catch (ErrorValues::Error err) {
		throw(err);
	}

}

//destructor
Deck::~Deck() {};

//load
int Deck::load(char * c) {

	std::string line;
	std::string word;

	//open and read file
	std::ifstream ifs;
	ifs.open(c);
	if (ifs.is_open()) {
		while (!ifs.eof()) {

			Card card;

			//read file one line at a time
			getline(ifs, line);
			std::istringstream iss(line);

			while (iss >> word) {
				//if a valid card
				if (card.readCard(word) == 0) {
					d_.push_back(card);
				}
			}
		}
	}
	else {
		throw(ErrorValues::cant_Open_File);
	}
	return 0;
}

//shuffle
void Deck::shuffleDeck() {

	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(d_.begin(), d_.end(), g);
}

//size
size_t Deck::size() const {
	return d_.size();
}

//add card
void Deck::add_card(Card c) {
	d_.push_back(c);
}

//insertion operator
std::ostream &operator<< (std::ostream & os, const Deck & d) {
	for (auto it = d.d_.begin(); it != d.d_.end(); ++it) {
		os << (*it).asString() << std::endl;
	}
	return os;
}

void operator<<(Deck & d1, Deck & d2) {
	//no more cards in deck
	if (d2.size() < 1) {
		throw(ErrorValues::not_Enough_Cards);
	}
	else {
		//add last card from deck to deck
		d1.add_card(d2.d_.back());
		//remove last card from deck
		d2.d_.pop_back();
	}
}