#include "stdafx.h"
#include "ErrorValues.h"

#include <iostream>

ErrorValues::ErrorValues() : e_(no_Error) {}

//mutator
void ErrorValues::e(Error err) {
	e_ = err;
}

//accessor
ErrorValues::Error ErrorValues::e() {
	return e_;
}

//mutate member variable and print error messages
void ErrorValues::p(Error err) {
	e(err);

	//no Error
	if (e_ == no_Error) {
		return;
	}
	//cannot open file given by argument
	if (e_ == cant_Open_File) {
		std::cout << "File cannot be opened" << std::endl;
		return;
	}
	//Not five cards
	if (e_ == not_five_cards) {
		std::cout << "Hand does not consist of 5 cards";
		return;
	}
	//No command line arguments
	if (e_ == invalid_Argc) {
		std::cout << "Invalid amount of arguments were provided" << std::endl;
		return;
	}
	//Not enough cards
	if (e_ == not_Enough_Cards) {
		std::cout << "Not enough cards" << std::endl;
		return;
	}
	//invalid response
	if (e_ == invalid_Response) {
		std::cout << "Invalid response" << std::endl;
		return;
	}
	//invalid card index
	if (e_ == invalid_Card_Index) {
		std::cout << "No card in the position" << std::endl;
		return;
	}
	//no instance available
	if (e_ == instance_Not_Available) {
		std::cout << "The game instance is not available" << std::endl;
		return;
	}
	//game has already started
	if (e_ == game_In_Progress) {
		std::cout << "The game is already in progress" << std::endl;
		return;
	}
	//unknown game
	if (e_ == unknown_Game) {
		std::cout << "Unknown game" << std::endl;
		return;
	}
	//no game in progress
	if (e_ == no_Game_In_Progress) {
		std::cout << "There is no game in progress currently" << std::endl;
		return;
	}
	//player is already in the game
	if (e_ == existing_Player) {
		std::cout << "The player is already in the game" << std::endl;
		return;
	}
}
