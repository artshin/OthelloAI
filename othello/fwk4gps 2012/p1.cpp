#include "p1.h"
#include "cathy.h"              //change this to youruserid.h
#include "othelloutils.h"
P1::P1():Player(){

}
/*the first 3 parameters are for ai players, the last 3 are used only by the interactive player 
  to determine current mouse position and if a click was made.  AI players always
  select a move so this function never returns false*/
bool P1::getmove(int& row,int& col,int board[][NUMSQUARES],int mx,int my,bool clicked){
  
  Cathy::GameTree gt(board,1,6);   //the class GameTree was declared
                                   //in Cathy namespace (you will need to put your 
                                   //code in your namespace)
  gt.getBest(row,col);

  return true;
}
