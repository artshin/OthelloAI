#ifndef OTHELLO_H
#define OTHELLO_H
#include "othelloutils.h"

const int FLIPSTATE = 1;           //game is in process of flipping over pieces
const int PICKINGSTATE = 3 ;       //game is in a state where it can wait for AI
const int ISOVERSTATE =2;          //game is over                                


class Othello{
  int board_[NUMSQUARES][NUMSQUARES];   //0 means empty, 1 is p1, -1 is p2
  int who_;                             //who's turn is it? 1 for p1, -1 for p2
  //int checkWin(int,int);
  int numonboard_;
  int state_;                          //state of the game.  This is 
                                       //used to control animations so that
                                       //we can show the flipping of the pieces
                                       //the mouse pointers and so on.
  MoveList flips_;
  MoveList p1valid_;
  MoveList p2valid_;
  int currflip_;
public:
  Othello();
  bool add(int row, int col,int who);   //adds a piece to the given row/col
                                       //does not refresh to physical screen.
                                       //returns true if it is valid, false otherwise
  bool generateLegalMoves();            //generates all possible legal moves
                                       //for both players given current board
                                       //function returns true if there is
                                       //at least one legal move, false otherwise
  void getBoard(int bb[][NUMSQUARES]); //copies internal board to parameter
                                       //I know this is slow and inefficicient
                                       //but I need to make sure that your AI 
                                       //won't mess up the board by accident.
  int whoMoves() const{return who_;}
  bool isDone();
  void setState(int state){state_=state;}
  int state() const{return state_;}
  void changePlayer(){who_=who_*-1;}  //changes who's turn it is
  void getNextFlip(int& r,int& c);
  bool isFull(){return numonboard_==NUMSQUARES*NUMSQUARES;}
  bool hasValidMove(int who);
};
#endif