#include "ashinkevich.h"
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
  
  wchar_t debugStr[255 + 1];

  
  int maxOrMin = lvl%2; // 0 is max, 1 is min


  if(!canMove(board, p1, p2) || boardFull(board)){   //game over, neither player can move
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
	  
	  nn->score_=Eval(board,player_, p1, p2);    //always evaluate for player at root!
  }
  else{
    if(whomoves==1){
		if(p1.numMoves()==0){
			nn->noMove_=CreateTree(board,lvl+1);
		}
		else{
			for(int i=0,prev_r=0,prev_c=0;i<p1.numMoves();i++){
				p1.getRowCol(i,r,c);
				flips.clear();
 			    addPiece(board,r,c,1,flips);
				if( i == 0 ){							// if it is the first node, then create it 
					nn->child_[r][c] = CreateTree(board,lvl+1);
					prev_r = r, prev_c = c;
				}
				else{
					if(maxOrMin == 0){					//otherwise, check before create, depending on lvl
						if(nn->child_[prev_r][prev_c]->score_ < Eval(board,player_,p1,p2) ){
							nn->child_[prev_r][prev_c] = NULL;
							//delete nn->child_[prev_r][prev_c];
							nn->child_[r][c] = CreateTree(board,lvl+1);
							prev_r = r, prev_c = c;
						}
					}
					else if(maxOrMin ==1){
						if(nn->child_[prev_r][prev_c]->score_ > Eval(board,player_,p1,p2) ){
							nn->child_[prev_r][prev_c] = NULL;
							//delete nn->child_[prev_r][prev_c];
							nn->child_[r][c] = CreateTree(board,lvl+1);
							prev_r = r, prev_c = c;
						}

					}
				}
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
			for(int i=0,prev_r=0,prev_c=0;i<p2.numMoves();i++){
				
				p2.getRowCol(i,r,c);
				flips.clear();
 			    addPiece(board,r,c,-1,flips);
				if( i == 0 ){							// if it is the first node, the create it 
					nn->child_[r][c] = CreateTree(board,lvl+1);
					prev_r = r, prev_c = c;
				}
				else{
					if(maxOrMin == 0){					//otherwise, check before create, depending on lvl
						if(nn->child_[prev_r][prev_c]->score_ < Eval(board,player_,p1,p2) ){
							nn->child_[prev_r][prev_c] = NULL;
							//delete nn->child_[prev_r][prev_c];
							nn->child_[r][c] = CreateTree(board,lvl+1);
							prev_r = r, prev_c = c;
						}
					}
					else if(maxOrMin ==1){
						if(nn->child_[prev_r][prev_c]->score_ > Eval(board,player_,p1,p2) ){
							nn->child_[prev_r][prev_c] = NULL;
							//delete nn->child_[prev_r][prev_c];
							nn->child_[r][c] = CreateTree(board,lvl+1);
							prev_r = r, prev_c = c;
						}

					}
				}
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

	//StringCbPrintfW(debugStr,255,L"Create Tree is called %d \n", lvl%2);
	//OutputDebugStringW(debugStr);

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
  } //end of tree creation loop
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
int Cathy::Eval(int board[][NUMSQUARES],int player,  MoveList p1, MoveList p2){
  /*int p1score;
  int p2score;
  int score;*/
	int result = 0;

	int tempBoard[NUMSQUARES][NUMSQUARES] = {0};
	copyBoard(tempBoard, board);
	
	int potentialMob = 0;
	int corner = 0;

	// First, we begin with taking mobility of a given player into consideration.
	// It will be a starting point for determining how good the situation is right now.
	// It is calculated from the difference between a given player and its opponent.

	// Pottential mobility should be found out. Potential mobility is a number of
	// empty squares that are adjacent to opponents pieces. 


	if(player == 1){
		

		for(int i = 0;i < NUMSQUARES; i++){             //go through each row
			for(int j = 0; j < NUMSQUARES; j++){        //go through every column in a row
				
				if( tempBoard[i][j] == -1 ){            //if field is occupied by an opponent



					for(int k = -1;  k!= 1; k++){       //loop for local row
						if( i == 0 ) k = 0;             //if row is 0, then don't check the -1 row
						for(int l = -1; l!= 1; l++){    //loop for local column
							if( j == 0 ) l = 0;         //if column is 0, then don't check for -1 column

							if(tempBoard[i+k][j+l] == 0){
								potentialMob++;
								tempBoard[i+l][j+l] = 3;
							}

							if( j == NUMSQUARES && l == 0) l++; //if column is 8, then don't check for +1 column
						}
						if( i == NUMSQUARES && k == 0) k++;  // if row is 8, then don't check +1 row
					}
				}
			}
		}

		if(p1.isLegal(0,0) != -1 || p1.isLegal(0,NUMSQUARES) != -1 ||
		   p1.isLegal(NUMSQUARES,0) != -1 || p1.isLegal(NUMSQUARES,NUMSQUARES) != -1)
		   corner++;

		result = p1.numMoves() - p2.numMoves() + potentialMob + corner;
	}
	else{
		
		for(int i = 0;i < NUMSQUARES; i++){             //go through each row
			for(int j = 0; j < NUMSQUARES; j++){        //go through every column in a row
				
				if( tempBoard[i][j] == -1 ){            //if field is occupied by an opponent



					for(int k = -1;  k!= 1; k++){       //loop for local row
						if( i == 0 ) k = 0;             //if row is 0, then don't check the -1 row
						for(int l = -1; l!= 1; l++){    //loop for local column
							if( j == 0 ) l = 0;         //if column is 0, then don't check for -1 column

							if(tempBoard[i+k][j+l] == 0){
								potentialMob++;
								tempBoard[i+l][j+l] = 3;
							}

							if( j == NUMSQUARES && l == 0) l++; //if column is 8, then don't check for +1 column
						}
						if( i == NUMSQUARES && k == 0) k++;  // if row is 8, then don't check +1 row
					}
				}
			}
		}

		if(p2.isLegal(0,0) != -1 || p2.isLegal(0,NUMSQUARES) != -1 ||
		   p2.isLegal(NUMSQUARES,0) != -1 || p2.isLegal(NUMSQUARES,NUMSQUARES) != -1)
		   corner++;

		result = p2.numMoves() - p1.numMoves() + potentialMob + corner;
	}



  // mobility 
  /*getScores(board,p1score,p2score);
  if(player==1){
	  score=p1score-p2score;
  }
  else{
	  score=p2score-p1score;
  }*/

  
  return result;
}