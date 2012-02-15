#include "cathy.h"
#include "othelloutils.h"
#define NULL 0
#define MAXSCORE 1000

Cathy::Node::Node(){
  noMove_=NULL;
  for(int i=0;i<NUMSQUARES;i++){
    for(int j=0;j<NUMSQUARES;j++){
      child_[i][j]=NULL;
    }
  }
}

/*board is the board right now.  lvl is the lvl of the node we are creating,
  row and col is the position of the most recently added piece. row and col are
  not relevant when lvl is 0 and are invalid*/
Cathy::Node* Cathy::GameTree::CreateTree(int board[][NUMSQUARES],int lvl){
  Node* nn=new Node();
  int whowon;
  int whomoves=(lvl%2)?-player_:player_;
  MoveList p1;
  MoveList p2;
  MoveList flips;
  int nump1,nump2;
  int r,c;
  int newboard[NUMSQUARES][NUMSQUARES];
  if(!canMove(board,p1,p2) || boardFull(board)){   //game over, neither player can move
	  getScores(board,nump1,nump2);
	  if(nump1==nump2)
		  whowon=0;
	  else if(nump1>nump2)
		  whowon=1;
	  else
		  whowon=-1;		  
     //Base case 1: someone has won the game, game is over score is 100
    if(whowon) {
      nn->score_=whowon * player_* MAXSCORE; //if player_ is 1 and they won the whowon would be +ve so
                                             //we have score being +1, otherwise if player_ is -1 and 
                                             //player 2 won, then whowon would be -1 so score will still
                                             //be +1.
    }
  }
  else if(lvl==height_-1){             //tree has reached its max height
    nn->score_=Eval(board,player_);    //always evaluate for player at root!
  }
  else{
    if(whomoves==1){
		if(p1.numMoves()==0){
			nn->noMove_=CreateTree(board,lvl+1);
		}
		else{
			for(int i=0;i<p1.numMoves();i++){
				p1.getRowCol(i,r,c);
				flips.clear();
 			    addPiece(board,r,c,1,flips);
				nn->child_[r][c]=CreateTree(board,lvl+1);
				/*undo the changes before testing the next move*/
				board[r][c]=0;
				for(int j=0;j<flips.numMoves();j++){
					flips.getRowCol(j,r,c);
					board[r][c]=-1;
				}

			}
		}
	}
	else{
		if(p2.numMoves()==0){
			nn->noMove_=CreateTree(board,lvl+1);
		}
		else{
			for(int i=0;i<p2.numMoves();i++){
				p2.getRowCol(i,r,c);
				flips.clear();
 			    addPiece(board,r,c,-1,flips);
				nn->child_[r][c]=CreateTree(board,lvl+1);
				/*undo the move before testing the next move*/
				board[r][c]=0;
				for(int j=0;j<flips.numMoves();j++){
					flips.getRowCol(j,r,c);
					board[r][c]=1;
				}
			}
		}
	}

	
	bool setgood=false;

    //NOTE:  If you implement alpha beta pruning, you will have to track max/min in the
    //tree creation loop.
    if(lvl%2){
	  if(nn->noMove_){
		  nn->score_=nn->noMove_->score_;
	  }
	  else{
 	    for(int i=0;i<NUMSQUARES;i++){
	      for(int j=0;j<NUMSQUARES;j++){
		    if(nn->child_[i][j]){
		  	  if(!setgood || nn->child_[i][j]->score_ < nn->score_){
			   nn->score_=nn->child_[i][j]->score_;
			   setgood=true;
		  	  }
		    }
		  }//for
	    }//for
	  }
    }
    else{
	  if(nn->noMove_){
		  nn->score_=nn->noMove_->score_;
	  }
	  else{
        for(int i=0;i<NUMSQUARES;i++){
          for(int j=0;j<NUMSQUARES;j++){
            if(nn->child_[i][j]){
              if(!setgood || nn->child_[i][j]->score_ > nn->score_){
                nn->score_=nn->child_[i][j]->score_;
                setgood=true;
                if(lvl==0){
                  bestrow_=i;
                  bestcol_=j;
                }
              }
            }
          }//for
        }//for
	  }
    }
  }
  return nn;
}
/*PLEASE PLEASE PLEASE REMEMBER YOUR DESTRUCTORS!  with the size of the trees
  we are creating, a lack of one will be a major issue*/
Cathy::GameTree::~GameTree(){
  DestroyTree(root_);
}

void Cathy::GameTree::DestroyTree(Node* root){
  if (root){
    for(int i=0;i<NUMSQUARES;i++){
      for(int j=0;j<NUMSQUARES;j++){
        DestroyTree(root->child_[i][j]);
      }
    }
    delete root;
  }/*if*/
}
/*this function returns a board score for the given board for the player
  indicated.  well.. actually it just randomly generates a number 
  and returns that right now.  You will need to make it better.*/
int Cathy::Eval(int board[][NUMSQUARES],int player){
  int p1score;
  int p2score;
  int score;
  getScores(board,p1score,p2score);
  if(player==1){
	  score=p1score-p2score;
  }
  else{
	  score=p2score-p1score;
  }
  return score;
}