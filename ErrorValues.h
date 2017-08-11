#ifndef ERRORVALUES_H
#define ERROVALUES_H

struct ErrorValues {

	enum Error {
		no_Error,
		cant_Open_File,
		not_five_cards,
		invalid_Argc,
		not_Enough_Cards,
		invalid_Response,
		invalid_Card_Index,
		instance_Not_Available,
		game_In_Progress,
		unknown_Game,
		no_Game_In_Progress,
		existing_Player
	};

	Error e_;

	ErrorValues();

	//mutator
	void e(Error);

	//accessor
	Error e();

	//print error message
	void p(Error);


};

#endif /* ERRORVALUES_H */