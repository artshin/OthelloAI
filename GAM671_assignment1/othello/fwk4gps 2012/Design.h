#ifndef _DESIGN_H_
#define _DESIGN_H_

/* Design Definition - Modelling Layer
 *
 * Design.h
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Coordinator.h" // for the Coordinator class definition
#include "Player.h"
#include "othello.h"
#include "othelloutils.h"



const int BOARDOFFSET = -60;   //Visual offset of board
const int HALFWIDTH = (300/NUMSQUARES)/2 - 5;  //half the width of a piece
const float SQUARESIZE=300/NUMSQUARES;
const float HALFSQUARE=(300/NUMSQUARES)/2;
const Colour red(1,0,0);
const Colour green(0,1,0);
float visualX(float col);
float visualZ(float row);
//-------------------------------- Design -------------------------------------
//
// The Design class defines the structure of the game design
//
class iObject;
class iLight;
class iSound;
class iHUD;

class Design : public Coordinator {

    iLight*   topLight_;        // points to the spot on the top box
    iCamera*  camera_;           //we will have only one camera

	static const int MAX_OBJECTS  = 500;
	static const int MAX_TEXTURES = 500;
	static const int MAX_CHAR     = 80;
	iHUD*     hud;                 // points to the hud



    iObject*   object_[NUMSQUARES][NUMSQUARES];   
	                                   // points to the pieces in the scene
	iTexture*  texture_[MAX_TEXTURES]; // points to the textures in the scene
	int noObjects_;                    // number of objects
	int noTextures_;                   // number of textures



    Othello* thegame_;       //the game object that stores all the info about the game such as board. win/loss state etc.
	Player* players_[2];     //The players playing this game
	int who_;                //the player making the move at this point in the game
	bool firstmove_;         //is this the first move in the game?
	int forfeit_;            //if an illegal move was made the player loses automatically to avoid being stuck in
	                         //an infinite loop when playing off computer players
	int whoforfeit_;
	int oprow_;              //row and col choice
	int opcol_;
	int flipto_;
	bool msgChange_;          //used to change message so that it will 
	                          //render one frame with message before actually getting
	                          //move
	int moveTime_;            //how long it took to make a move
	/*Othello messages.  Used to display info about the game*/
    iText* whoismoving_;
    iText* action_ ;
	iText* chosen_;
	iText* timer_;
    iText* finalmsg1_ ;
    iText* finalmsg2_ ;
	iText* finalmsg3_;
	int mx_;
	int my_;
	iObject* mouse_;
	int timeLastChange;

    Design(const Design& s);            // prevents copying
    Design& operator=(const Design& s); // prevents assignment

  public:
    Design(void*, int);
    void initialize();
    void update();
	iHUD* getData() { return hud; };
};


#endif
