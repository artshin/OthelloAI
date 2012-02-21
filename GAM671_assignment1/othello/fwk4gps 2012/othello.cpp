#include "othello.h"
#include "othelloutils.h"
Othello::Othello(){
  for(int i=0;i<NUMSQUARES;i++){
    for(int j=0;j<NUMSQUARES;j++){
      board_[i][j]=0;
    }
  }
  board_[NUMSQUARES/2][NUMSQUARES/2]=1;
  board_[NUMSQUARES/2-1][NUMSQUARES/2-1]=1;
  board_[NUMSQUARES/2-1][NUMSQUARES/2]=-1;
  board_[NUMSQUARES/2][NUMSQUARES/2-1]=-1;
  who_=1;

  numonboard_=4;
  state_=PICKINGSTATE;
}
/*function checks to see if the player is allowed to choose
  the move given the current board by checking the appropriate
  valid list.  If it is invalid, function returns false.  Otherwise
  it returns true and modifies the board based on the move made*/
bool Othello::add(int row, int col,int who){
  bool rc=false;
  if(who==1){
	rc=(p1valid_.isLegal(row,col)!=-1);
  }
  else{
	rc=(p2valid_.isLegal(row,col)!=-1);
  }
  if(rc){
	addPiece(board_,row,col,who,flips_);
	state_=FLIPSTATE;
	currflip_=0;
	numonboard_++;
  }

  return rc;
}
void Othello::getBoard(int bb[][NUMSQUARES]){
  for(int i=0;i<NUMSQUARES;i++){
    for(int j=0;j<NUMSQUARES;j++){
      bb[i][j]=board_[i][j];
    }
  }
}
void Othello::getNextFlip(int& r,int& c){
	if(currflip_<flips_.numMoves()){
		flips_.getRowCol(currflip_,r,c);
		currflip_++;
		if(currflip_==flips_.numMoves()){
			state_=PICKINGSTATE;
		}
	}

}

bool Othello::isDone(){
	bool rc=true;
    if(numonboard_<NUMSQUARES*NUMSQUARES){
		if(p1valid_.numMoves() || p2valid_.numMoves()){
			rc=false;
		}
	}
	if(rc){
		state_=ISOVERSTATE;
	}
	return rc;
}
bool Othello::generateLegalMoves(){
    bool rc=false;
	p1valid_.clear();
	p2valid_.clear();
	canMove(board_,p1valid_,p2valid_);
	if(p1valid_.numMoves() || p2valid_.numMoves()){
	  rc=true;
	}
	return rc;
}
bool Othello::hasValidMove(int who){
	  bool rc=false;
	  if(who==1){
		  rc=(p1valid_.numMoves()>0);
	  }
	  else{
		  rc=(p2valid_.numMoves()>0);
	  }
	  return rc;
}
