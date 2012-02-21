#include "p2.h"
#include "ashinkevich.h"
#include "othelloutils.h"
P2::P2():Player(){

}
bool P2::getmove(int& row,int& col,int board[][NUMSQUARES],int mx,int my,bool clicked){
  //MoveList ml1;
  //bool rc=false;
  //if(canMove(board,-1,ml1)){
	 // ml1.getRowCol(0,row,col);
	 // rc=true;
  //}
  
  Cathy::GameTree gt(board,-1,7);   //the class GameTree was declared
                                   //in Cathy namespace (you will need to put your 
                                   //code in your namespace)
  gt.getBest(row,col); 
  return true;
}
