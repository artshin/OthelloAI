#ifndef PLAYER_H
#define PLAYER_H

#include "Coordinator.h"
#include "othelloutils.h"
 /*Abstract base class Player.  Your objects will be derived from this.  
  This object stores no information and has one single pure virtual function
  which your object must override.  It's job is simply to provide a
  clean interface for the main*/
class Player{
protected:

public:
	Player(){
	}
  /*The getmove function is passed references to two ints and a board. It is
    also passed a NUMSQUARESXNUMSQUARES integer array that represents the board.  A 0 
    represents an empty spot.  +1 for player 1, -1 for player 2.
    Your function will use the board information to figure out
    which row and which column to put the piece in and pass them back
    through row and col respectively.  
    Your player must pass back valid values (in terms of range and placement
    rules) or it will automatically lose the game.

	The last 3 values for the function represent the mouses current X and current Y position and 
	whether or not it was clicked.  These values are meant for writing interactive players only and
	should not be used by your code
	
	Function returns true if a move was made (it should always be if its an AI player) and false if it wasn't
	Your function for your AI players should always return true*/
  virtual bool getmove(int& row,int& col,int board[][NUMSQUARES],int mx,int my,bool clicked)=0;
};

#endif
