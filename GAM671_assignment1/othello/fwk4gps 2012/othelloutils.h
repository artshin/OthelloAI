#ifndef OTHELLOUTILS_H
#define OTHELLOUTILS_H
const int NUMSQUARES=8;
struct Move{
	int row_;
	int col_;
};
class MoveList{
protected:
	Move legalMoves_[NUMSQUARES*NUMSQUARES-4];
	int numMoves_;
public:
	MoveList(){
		numMoves_=0;
	}
	/*adds a legal move to the legal move list*/
	void add(int r,int c){
		legalMoves_[numMoves_].row_=r;
		legalMoves_[numMoves_].col_=c;
		numMoves_++;

	}
	/*returns index of the move in the legalMoves list if the x,y coordinates passed in correspond
	  to a move stored in the legalmoves_ list, -1 otherwise*/
	int isLegal(int r,int c){  
		int rc=-1;
		for(int i=0;i<numMoves_ && rc==-1;i++){
			if(legalMoves_[i].row_==r && legalMoves_[i].col_==c){
				rc=i;
			}
		}
		return rc;
	}
    void getRowCol(int i,int& r,int& c);
	int numMoves() const{return numMoves_;}
	void clear(){numMoves_=0;}
};

int owner(int board[][NUMSQUARES],int r,int c);
/*the following num* functions all do the following:
  starting at the first square in direction specified by the function name
  from startrow,startcol counts the number that are owned by who.

  Function returns:
  - 0 if there are no pieces owned by who in that direction
  - a positive number representing number of pieces owned by who in the 
    given direction if it is capped by -who
  - a negative number representing number of pieces owned by who in the
    given direction but is not capped by -who.
*/
int numUp(int board[][NUMSQUARES],int startrow,int startcol,int who);
int numDown(int board[][NUMSQUARES],int startrow,int startcol,int who);
int numLeft(int board[][NUMSQUARES],int startrow,int startcol,int who);
int numRight(int board[][NUMSQUARES],int startrow,int startcol,int who);
int numUpLeft(int board[][NUMSQUARES],int startrow,int startcol,int who);
int numUpRight(int board[][NUMSQUARES],int startrow,int startcol,int who);
int numDownRight(int board[][NUMSQUARES],int startrow,int startcol,int who);
int numDownLeft(int board[][NUMSQUARES],int startrow,int startcol,int who);

/*Checks every square to see if there are still legal moves that can be made by either player. function 
  returns 0 if neither players can make a legal move and 1 if any player can move.  The two MoveList objects
  store all the legal moves for each player.*/
int canMove(int board[][NUMSQUARES], MoveList& p1moves, MoveList& p2moves); 
/*This version only checks if player who can move. */
int canMove(int board[][NUMSQUARES],int who, MoveList& moves); 

/*
This function passes back the current "score" of the board.  This is calculated
by the number of squares owned by each player.  Note that this score is not an indication
of win or loss or even how good the board is for any player.  Furthermore, it does not have
anything to do with the evaluation function.  This is purely a count of the final score if
the game were to end.
*/
void getScores(int board[][NUMSQUARES],int& p1score, int& p2score);

/*function adds a piece to the board at r,c if possible.
  All the pieces that are flipped as a result of adding the piece are added
  to the flips list and the board is altered to reflect the addition of
  the piece.  Function returns true if the move to r,c was valid, false 
  otherwise*/
bool addPiece(int board[][NUMSQUARES], int r,int c, int who,MoveList& flips);

/*returns true if board is full, false otherwise*/
bool boardFull(int board[][NUMSQUARES]);

/*this function makes a duplicate of original board and stores it in 
  newboard*/
void copyBoard(int newboard[][NUMSQUARES], int original[][NUMSQUARES]);

#endif