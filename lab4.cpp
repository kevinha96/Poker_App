// Lab4.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ErrorValues.h"
#include "FiveCardDraw.h"

#include <iostream>

using namespace std;

//usage function
void usage(char * program_name) {
	cout << "\nUsage: " << program_name << " <game> <player> <player> ..." << endl;
	cout << "The first argument is the game being played, while the arguments following it are the player names." << endl;
	cout << "Purpose: Start a selected game with the players given.\n" << endl;
}

int main(int argc, char * argv[]) {
	ErrorValues error;

	//check for at least 3 arguments
	if (argc < 4) {
		error.p(ErrorValues::invalid_Argc);
		//give usage statement
		usage(argv[0]);
		return error.e();
	}
	else {
		//start the game
		try { Game::start_game(argv[1]); }
		catch (ErrorValues::Error err) {
			error.p(err);
			return error.e();
		}

		//get the game instance
		try { Game::instance(); }
		catch (ErrorValues::Error err) {
			error.p(err);
			return error.e();
		}
		shared_ptr<Game> game = Game::instance();

		//add players
		for (int i = 2; i < argc; ++i) {
			try { game->add_player(argv[i]); }
			catch (ErrorValues::Error err) {
				error.p(err);
			}
		}

		//keep calling round methods
		while (game->playerCount() > 1) {
			try {
				game->before_round();
				game->round();
				game->after_round();
			}
			catch (ErrorValues::Error err) {
				error.p(err);
				break;
			}
		}
		int errorVal = error.e();
		game->stop_game();
		if (error.e() == ErrorValues::no_Error) {
			cout << "Thank you for playing" << endl;
			return 0;
		}
		else {
			return errorVal;
		}
	}

	return 0;
}

