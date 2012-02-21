#ifndef P2_H
#define P2_H
#include "player.h"
#include "Coordinator.h"
#include "othelloutils.h"
/*put your code for your Player 2 AI here.  make sure your error 
checking is for Player 2 (-ve values on the board) in this one.  
You are welcome to add any members (both data and functions) that
you like.  Just be aware that main only will use getmove()*/

class P2:public Player{
  int row_;
  int col_;
public:
  P2();
  virtual bool getmove(int& row,int& col,int board[][NUMSQUARES],int mx,int my,bool clicked);
};
#endif