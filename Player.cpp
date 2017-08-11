#include "stdafx.h"
#include "Player.h"
#include "ErrorValues.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//constructor
Player::Player(const char * c) : name_(c), handsWon_(0), handsLost_(0), numChips_(20), playerBet_(0) {

	unsigned int count = 0;

	//add .txt to end of player name for filename
	std::string file = c;
	std::string fileType = ".txt";
	file.append(fileType);

	//open file stream
	std::ifstream playerFile(file);
	std::string line;

	//keep track of whether everything works
	unsigned int success = 0;

	if (playerFile.is_open()) {
		while (!playerFile.eof()) {

			std::string word;

			//get line at a time
			getline(playerFile, line);

			//delete first string
			line = line.substr(line.find(": ") + 2);
			std::istringstream iss(line);

			//get second string that holds info
			while (iss >> word) {
				//player name
				if (count == 0) {
					name_ = word;
					++success;
				}
				//hands won
				if (count == 1) {
					handsWon_ = stoi(word);
					++success;
				}
				//hands lost
				if (count == 2) {
					handsLost_ = stoi(word);
					++success;
				}
				if (count == 3) {
					numChips_ = stoi(word);
					++success;
				}
			}
			++count;
			if (success == 4) { break; }
		}

		//something didn't register
		if (success != 4) {
			name_ = c;
			handsWon_ = 0;
			handsLost_ = 0;
			numChips_ = 20;
		}
	}
}

//insertion operator
std::ostream & operator<< (std::ostream & o, const Player & p) {
	o << "Name: " << p.name_ << std::endl;
	o << "Hands won: " << p.handsWon_ << std::endl;
	o << "Hands lost: " << p.handsLost_ << std::endl;
	o << "Number of chips: " << p.numChips_ << std::endl;
	return o;
}