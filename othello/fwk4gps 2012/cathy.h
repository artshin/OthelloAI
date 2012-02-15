#ifndef CATHY_H
#define CATHY_H
#include "othelloutils.h"
namespace Cathy{

  struct Node{
  int score_;         //score of the node
  Node* child_[NUMSQUARES][NUMSQUARES];  /*use 2 d array of Node* so that index of array can be
                         used to correspond to where a piece was placed.  for
                         example if the piece is placed at 0,0 then the
                         pointer for that move is to 0,0*/
  Node* noMove_;   /*corresponds to a move made by not altering the board(done when
				     someone is forced to give up a move*/
  Node();
};

/*We use a 2D int array to represent a board, a value of 0 in the board means it is not occupied, a value of 1 means player 1 owns the square, -1 means player 2 owns the square.*/
class GameTree{
  int player_;
  Node* root_;
  Node* CreateTree(int board[][NUMSQUARES],int lvl);
  int bestrow_;          //row position of best move at root level
  int bestcol_;          //col position of best move at root level
  int height_;
public:
  /*given the current board, create a full game tree.  The constructor acts 
    as a wrapper that calls the CreateTree function*/
  GameTree(int board[][NUMSQUARES],int player,int height){ //remember to change the height to 4 here or
                                                                //overide the default in p1/p2
    player_=player;                                               //either 1 or -1.
    height_=height;
    root_=CreateTree(board,0);
  }
  void getBest(int& r,int&c){
    r=bestrow_;
    c=bestcol_;
  }
  ~GameTree();
  void DestroyTree(Node* root);
};
int Eval(int board[][NUMSQUARES],int player);
}
#endif