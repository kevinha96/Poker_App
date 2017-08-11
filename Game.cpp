#include "stdafx.h"
#include "Game.h"
#include "ErrorValues.h"
#include "FiveCardDraw.h"
#include "SevenCardStud.h"

#include <iostream>
#include <sstream>
#include <algorithm>
#include <fstream>

std::shared_ptr<Game> Game::gameP_;

std::shared_ptr<Game> Game::instance() {
	//check that Deck pointer points to something
	if (gameP_) {
		std::shared_ptr<Game> copy(gameP_);
		return copy;
	}
	else {
		throw(ErrorValues::instance_Not_Available);
	}
}

void Game::start_game(const std::string & s) {
	//check if game has already started
	if (gameP_) {
		throw(ErrorValues::game_In_Progress);
	}
	if (s == "FiveCardDraw") {
		gameP_ = std::make_shared<FiveCardDraw>();
	}
	else if (s == "SevenCardStud") {
		gameP_ = std::make_shared<SevenCardStud>();
	}
	else {
		throw(ErrorValues::unknown_Game);
	}
}

void Game::stop_game() {
	//no game in progress
	if (gameP_) {
		throw(ErrorValues::no_Game_In_Progress);
	}
	else {
		gameP_ = nullptr;
	}
}

void Game::add_player(const std::string & s) {
	//check for player
	if (find_player(s) != nullptr) {
		throw(ErrorValues::existing_Player);
	}
	//if not there add a player
	else {
		Player newP(s.c_str());

		//check if player has 0 chips
		if (newP.numChips_ == 0) {
			std::cout << newP.name_ << ": If you would like to join the game you must restart with 20 chips. Would you like to continue? (Y/N)" << std::endl;

			//record answer
			std::string input;
			std::string word;
			std::vector<std::string> answer;
			getline(std::cin, input);
			std::stringstream iss(input);
			while (iss >> word) {
				answer.push_back(word);
			}

			if (answer.size() == 1) {
				if (answer[0] == "YES" || answer[0] == "yes" || answer[0] == "Y" || answer[0] == "y") {
					newP.numChips_ = 20;
					pVector_.push_back(std::make_shared<Player>(newP));
				}
			}
			else {
				throw(ErrorValues::invalid_Response);
			}
		}
		else {
			pVector_.push_back(std::make_shared<Player>(newP));
		}
	}
}

std::shared_ptr<Player> Game::find_player(const std::string & s) {
	//iterate through vector
	for (auto vit = pVector_.begin(); vit != pVector_.end(); ++vit) {
		if ((**vit).name_ == s) {
			return *vit;
		}
	}
	return nullptr;
}

Game::~Game() {}

size_t Game::playerCount() {
	return pVector_.size();
}

void Game::rankPlayers(std::vector<std::shared_ptr<Player>> & temp) {
	//sort players by rank
	std::sort(temp.begin(), temp.end(), new_poker_rank);

	//vector to consider multiple winners
	std::vector<std::shared_ptr<Player>> winners;

	//best Hand
	Hand bestH = temp.front()->hand_;

	//update everything for players still in the game
	for (auto vit = temp.begin(); vit != temp.end(); ++vit) {
		//if player has hand
		if ((*vit)->hand_ == bestH) {
			winners.push_back(*vit);
		}
		//losers
		else {
			++(*vit)->handsLost_;
		}
	}

	//winners divide pot equally
	unsigned int prize = totalChipPot / winners.size();
	for (auto wit = winners.begin(); wit != winners.end(); ++wit) {
		++(*wit)->handsWon_;
		(*wit)->numChips_ += prize;
	}
	//extra chips due to a noninteger prize
	chipCarryOver = totalChipPot - (prize * winners.size());

	//print info for players still in the game
	for (auto vit = temp.begin(); vit != temp.end(); ++vit) {
		std::cout << **vit << std::endl;
		std::cout << "Final Hand: " << (*vit)->hand_.showWholeHand() << "\n" << std::endl;
	}

	//for players not in the game
	for (auto pVI = pVector_.begin(); pVI != pVector_.end(); ++pVI) {
		if ((*pVI)->status == false) {
			++(*pVI)->handsLost_;

			//print out info
			std::cout << **pVI << std::endl;
			std::cout << "Folded hand\n" << std::endl;
		}
	}
}

int Game::leave() {
	std::cout << "Would any players like to leave? (Reply 'no' if no one would like to leave)" << std::endl;

	//get input
	std::string input;
	std::string word;
	std::vector<std::string> nameV;
	std::getline(std::cin, input);
	std::stringstream iss(input);
	while (iss >> word) {
		if (word == "no") {
			return 0;
		}
		else {
			nameV.push_back(word);
		}
	}

	//check validity of names 
	for (auto vit = nameV.begin(); vit != nameV.end(); ++vit) {
		//go through pVector to check names
		for (auto pvit = pVector_.begin(); pvit != pVector_.end();) {
			//valid
			if (*vit == (*pvit)->name_) {
				//add .txt to end of player name for filename
				std::string fileType = ".txt";
				vit->append(fileType);

				//open ofstream
				std::ofstream ofs(*vit);
				if (ofs.is_open()) {
					ofs << **pvit;
				}
				ofs.close();
				pvit = pVector_.erase(pvit);
			}
			else {
				++pvit;
			}
		}
	}
	leave();
}

int Game::join() {
	ErrorValues error;

	std::cout << "Would any players like to join? (Reply 'no' if no one would like to join)" << std::endl;

	//get input
	std::string input;
	std::string word;
	std::vector<std::string> newPV;
	std::getline(std::cin, input);
	std::stringstream iss(input);
	while (iss >> word) {
		if (word == "no") {
			return 0;
		}
		else {
			newPV.push_back(word);
		}
	}
	//check if player exists, otherwise add them
	for (auto vit = newPV.begin(); vit != newPV.end(); ++vit) {
		try { add_player(*vit); }
		catch (ErrorValues::Error err) {
			std::cout << *vit << ": ";
			error.p(err);
		}
	}
	join();
}

void Game::askBet(Player & p) {
	unsigned int diff = currentBet - p.playerBet_;

	//set player as in game
	p.status = true;

	//for reading input
	std::string input;
	std::string word;
	std::vector<std::string> betV;

	//ask player if they want to bet if they have chips
	if (p.numChips_ > 0) {
		std::cout << "\n" << p.name_ << ", your current chip count is: " << p.numChips_ << std::endl;
		if (currentBet == 0) {
			std::cout << "Would you like to check or raise?" << std::endl;
		}
		else if (p.numChips_ < diff) {
			std::cout << "To call, you must bet " << diff << " additional chips. Would you like to go allin or fold?" << std::endl;
		}
		else {
			std::cout << "To call, you must bet " << diff << " additional chips. Would you like to call, raise, or fold?" << std::endl;
		}

		//read input
		getline(std::cin, input);
		std::stringstream iss(input);
		while (iss >> word) {
			betV.push_back(word);
		}

		if (betV.size() == 1) {
			if (betV[0] == "call") {
				p.numChips_ -= diff;
				p.playerBet_ += diff;
				roundChipPot += diff;
				++betCount;
				++currP;
			}
			else if (betV[0] == "allin") {
				roundChipPot += p.numChips_;
				p.numChips_ = 0;
				++betCount;
				++currP;
			}
			else if (betV[0] == "fold") {
				//set player status as not in game
				p.status = false;
				for (auto cpVI = cpVector_.begin(); cpVI != cpVector_.end(); ++cpVI) {
					if ((*cpVI)->name_ == p.name_) {
						cpVector_.erase(cpVI);
						break;
					}
				}

			}
			else if (betV[0] == "check") {
				if (currentBet == 0) {
					++betCount;
					++currP;
				}
				else {
					throw(ErrorValues::invalid_Response);
				}
			}
			else {
				throw(ErrorValues::invalid_Response);
			}
		}
		else if (betV.size() == 2) {
			if (betV[0] == "raise") {
				try { stoi(betV[1]); }
				catch (...) { throw(ErrorValues::invalid_Response); }

				unsigned int raiseAmount = stoi(betV[1]);
				//if player doesnt have enough chips
				if (p.numChips_ < (raiseAmount + diff)) {
					throw(ErrorValues::invalid_Response);

				}
				if (raiseAmount == 1 || raiseAmount == 2) {
					p.numChips_ -= (diff + raiseAmount);
					p.playerBet_ += (diff + raiseAmount);
					roundChipPot += (diff + raiseAmount);
					currentBet += raiseAmount;
					++currP;
					betCount = 1;
				}
				else {
					throw(ErrorValues::invalid_Response);
				}
			}
			else {
				throw(ErrorValues::invalid_Response);
			}
		}
		else {
			throw(ErrorValues::invalid_Response);
		}
	}
	//all in player
	else {
		std::cout << "\n" << p.name_ << ": You are currently all in\n" << std::endl;
		++betCount;
		++currP;
	}
}

void Game::askPlayerToLeave() {
	for (auto pVI = pVector_.begin(); pVI != pVector_.end();) {
		//check if player has 0 chips
		if ((*pVI)->numChips_ == 0) {
			std::cout << (*pVI)->name_ << ": If you would like to stay in the game you must restart with 20 chips. Would you like to continue? (Y/N)" << std::endl;

			//record answer
			std::string input;
			std::string word;
			std::vector<std::string> answer;
			getline(std::cin, input);
			std::stringstream iss(input);
			while (iss >> word) {
				answer.push_back(word);
			}

			if (answer.size() == 1) {
				if (answer[0] == "YES" || answer[0] == "yes" || answer[0] == "Y" || answer[0] == "y") {
					(*pVI)->numChips_ = 20;
					++pVI;
				}
				else if (answer[0] == "NO" || answer[0] == "no" || answer[0] == "N" || answer[0] == "n") {
					//add .txt to end of player name for filename
					std::string fileType = ".txt";
					std::string playerName = (*pVI)->name_;
					playerName.append(fileType);

					//open ofstream
					std::ofstream ofs(playerName);
					if (ofs.is_open()) {
						ofs << **pVI;
					}
					ofs.close();
					pVI = pVector_.erase(pVI);
				}
				else {
					throw(ErrorValues::invalid_Response);
				}
			}
			else {
				throw(ErrorValues::invalid_Response);
			}
		}
		//ignore players with more than 0 chips
		else {
			++pVI;
		}
	}
}

bool new_poker_rank(std::shared_ptr<Player> p1, std::shared_ptr<Player> p2) {
	//check that pointers aren't singular
	if (p1 != nullptr) {
		if (p2 != nullptr) {
			return poker_rank(p1->hand_, p2->hand_);
		}
		else {
			return true;
		}
	}
	else {
		return false;
	}
}