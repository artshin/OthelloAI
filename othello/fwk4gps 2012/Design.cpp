/* Design Implementation - Application Layer
 *
 * Design.cpp
 * fwk4gps version 3.0
 * gam666/dps901/gam670/dps905
 * January 14 2012
 * copyright (c) 2012 Chris Szalwinski 
 * distributed under TPL - see ../Licenses.txt
 */

#include "Design.h"          // for the Design class definition
#include "iText.h"           // for the Text Interface
#include "iHUD.h"            // for the HUD Interface
#include "iSound.h"          // for the Sound Interface
#include "iLight.h"          // for the Light Interface
#include "iObject.h"         // for the Object Interface
#include "iTexture.h"        // for the Texture Interface
#include "Camera.h"          // for the Camera Interface
#include "iGraphic.h"        // for the Graphic Interface
#include "iUtilities.h"      // for strcpy()
#include "MathDefinitions.h" // for MODEL_Z_AXIS
#include "ModellingLayer.h"  // for MOUSE_BUTTON_SCALE, ROLL_SPEED
#include "Common_Symbols.h"  // for Action and Sound enumerations
#include "player.h"
#include "p1.h"
#include "p2.h"
#include <strsafe.h>
const wchar_t* orient(wchar_t*, const iFrame*, char, unsigned = 1u);
const wchar_t* position(wchar_t*, const iFrame*, char = ' ', unsigned = 1u);
const wchar_t* onOff(wchar_t*, const iSwitch*);
float visualX(float col){return -150+BOARDOFFSET+HALFSQUARE+(col*SQUARESIZE)+2.5;}
float visualZ(float row){return 150-HALFSQUARE-(row*SQUARESIZE)-2.5;}

//-------------------------------- Design -------------------------------------
//
// The Design class implements the game design within the Modelling Layer
//
// constructor initializes the engine and the instance pointers
//
Design::Design(void* h, int s) : Coordinator(h, s) {

  for(int i=0;i<NUMSQUARES;i++){
	  for(int j=0;j<NUMSQUARES;j++){
  	       object_[i][j]=nullptr;
	  }
  }
    forfeit_=false;
}

// initialize initializes the general display design coordinator, creates the 
// primitive sets, textures, objects, lights, sounds, cameras, and text items
//
void Design::initialize() {

    players_[0]=new P1();
	players_[1]=new P2();
    // general display design
    //
    setProjection(0.9f, 1.0f, 1000.0f);
    setAmbientLight(0.9f, 0.9f, 0.9f);

	hud = CreateHUD(0.72f, 0.01f, 0.27f, 0.99f, CreateTexture(HUD_IMAGE));
	//setTimerText(CreateText(Rectf(0.0f, 0.05f, 0.2f, 0.15f), hud, L"", 
 //    TEXT_HEIGHT, TEXT_TYPEFACE, TEXT_LEFT));

    // cameras ----------------------------------------------------------------

    // camera at a distance - in lhs coordinates
    iCamera* camera = CreateCamera();
    camera->translate(0,360,0);
    camera->setRadius(17.8f);
	camera->rotatex(3.1459/2);

	//colours

    Reflectivity redish = Reflectivity(red);
	Reflectivity greenish = Reflectivity(green);

	thegame_=new Othello();
 
	iGraphic* grid=CreateGrid(-150, 150, NUMSQUARES+1);
    CreateObject(grid)->translate(BOARDOFFSET,0,0);
	
	iGraphic* piece=CreateBox(-HALFWIDTH,-2,(-HALFWIDTH)*MODEL_Z_AXIS,HALFWIDTH,2,HALFWIDTH*MODEL_Z_AXIS);
	(object_[NUMSQUARES/2][NUMSQUARES/2]=CreateObject(piece, &redish))->translate(::visualX(NUMSQUARES/2),0,::visualZ(NUMSQUARES/2));

	piece=CreateBox(-HALFWIDTH,-2,(-HALFWIDTH)*MODEL_Z_AXIS,HALFWIDTH,2,HALFWIDTH*MODEL_Z_AXIS);
	(object_[NUMSQUARES/2-1][NUMSQUARES/2-1]=CreateObject(piece, &redish))->translate(::visualX(NUMSQUARES/2-1),0,::visualZ(NUMSQUARES/2-1));

    piece=CreateBox(-HALFWIDTH,-2,(-HALFWIDTH)*MODEL_Z_AXIS,HALFWIDTH,2,HALFWIDTH*MODEL_Z_AXIS);
	(object_[NUMSQUARES/2][NUMSQUARES/2-1]=CreateObject(piece, &greenish))->translate(::visualX(NUMSQUARES/2-1),0,::visualZ(NUMSQUARES/2));

    piece=CreateBox(-HALFWIDTH,-2,(-HALFWIDTH)*MODEL_Z_AXIS,HALFWIDTH,2,HALFWIDTH*MODEL_Z_AXIS);
	(object_[NUMSQUARES/2-1][NUMSQUARES/2]=CreateObject(piece, &greenish))->translate(::visualX(NUMSQUARES/2),0,::visualZ(NUMSQUARES/2-1));

	piece=CreateBox(-1,-1,-1*MODEL_Z_AXIS,1,1,1*MODEL_Z_AXIS);
	mouse_=CreateObject(piece,&greenish);
	mx_=0;
	my_=0;
	whoismoving_=CreateText(Rectf(0.05f,0.05f,0.95f,0.1f),hud,L"",TEXT_HEIGHT,TEXT_TYPEFACE,TEXT_LEFT);
	finalmsg1_=CreateText(Rectf(0.05f,0.05f,0.95f,0.15f),hud,L"",TEXT_HEIGHT+5,TEXT_TYPEFACE,TEXT_LEFT);
	finalmsg2_=CreateText(Rectf(0.05f,0.16f,0.95f,0.26f),hud,L"",TEXT_HEIGHT+5,TEXT_TYPEFACE,TEXT_LEFT);
	action_=CreateText(Rectf(0.05f,0.11f,0.95f,0.16f),hud,L"",TEXT_HEIGHT,TEXT_TYPEFACE,TEXT_LEFT);
	finalmsg3_=CreateText(Rectf(0.05f,0.27f,0.95f,0.37f),hud,L"",TEXT_HEIGHT+5,TEXT_TYPEFACE,TEXT_LEFT);
	timer_=CreateText(Rectf(0.05f,0.17f,0.95f,0.27f),hud,L"",TEXT_HEIGHT,TEXT_TYPEFACE,TEXT_LEFT);
	chosen_=CreateText(Rectf(0.05f,0.38f,0.95f,0.48f),hud,L"",TEXT_HEIGHT,TEXT_TYPEFACE,TEXT_LEFT);
}

// update updates the position and orientation of each object according to the 
// actions initiated by the user
//
void Design::update() {
	int delta = now - lastUpdate;
	int r;
	int c;
	int who;
	int x;
	int y;
	wchar_t str[MAX_DESC + 1];
	x=change(GF_MS_DSPX);
	y=change(GF_MS_DSPY);

	if((mx_+x)<0){
	  mx_=0;
	}
	else if((mx_+x)>200){
	  mx_=200;
	}
	else{
	  mx_+=x;
	}

	if((my_+y)<0){
	  my_=0;
	}
	else if((my_+y)>200){
	  my_=200;
	}
	else{
	  my_+=y;
	}
	mouse_->translate(x,0,y);
	int board[NUMSQUARES][NUMSQUARES];
	float movetime;
    Reflectivity redish = Reflectivity(red);
	Reflectivity greenish = Reflectivity(green);
	iGraphic* piece;
	if (firstmove_) {
	  thegame_->generateLegalMoves();
	  firstmove_=false;
	  StringCbPrintfW(str,MAX_DESC,L"Player %d is moving",1);
	  whoismoving_->set(str);
	  oprow_=now;
	  msgChange_=true;
	}
	if(thegame_->state()==FLIPSTATE){
	    //animate the flips that occur as a result of
	    //putting a piece down
 	    StringCbPrintfW(str,MAX_DESC,L"Flipping...");
		action_->set(str);
		if(now-timeLastChange > 500){  //if its been more than half a second flip
			timeLastChange=now;
			thegame_->getNextFlip(r,c);
			object_[r][c]->setReflectivity((flipto_==1)?&redish:&greenish);
		}
	}
	else if(thegame_->state()==ISOVERSTATE){
		//game over, do nothing
        int p1score,p2score;
		thegame_->getBoard(board);
		getScores(board,p1score,p2score);
		whoismoving_->set(L"");
		action_->set(L"");
		timer_->set(L"");
	    StringCbPrintfW(str,MAX_DESC,L"Game Over! ");
		finalmsg1_->set(str);
		if(!forfeit_){
  		  StringCbPrintfW(str,MAX_DESC,L"P1: %d, P2:%d",p1score,p2score);
		  finalmsg2_->set(str);
		  if(p1score>p2score)
			StringCbPrintfW(str,MAX_DESC,L"Player 1 has won!");
		  else if(p1score<p2score)
	  		StringCbPrintfW(str,MAX_DESC,L"Player 2 has won!");
		  else
			StringCbPrintfW(str,MAX_DESC,L"It's a Tie!");

		  finalmsg3_->set(str);
		}
		else{
  		  StringCbPrintfW(str,MAX_DESC,L"P%d's move was invalid",whoforfeit_);
		  finalmsg2_->set(str);
		  StringCbPrintfW(str,MAX_DESC,L"Player %d has won!",(whoforfeit_==1)?2:1);
		  finalmsg3_->set(str);
		}
	}
	else{

	  if(!forfeit_ && !thegame_->isDone()){
		chosen_->set(L"");
		thegame_->getBoard(board);
		who=thegame_->whoMoves();
		if(msgChange_){
  	      StringCbPrintfW(str,MAX_DESC,L"Player %d is moving",who==1?1:2);
	      whoismoving_->set(str);
  	      StringCbPrintfW(str,MAX_CHAR,L"Thinking...");
		  action_->set(str);
		  msgChange_=false;
		  moveTime_=now;
		  timer_->set(L"");
		  chosen_->set(L"");
		}
		else{
		  if(players_[(who>0)?0:1]->getmove(r,c,board,x,y,pressed(MDL_PLUS_X))==true){ //interactive players return false to allow for device tracking
													     			 	               //until selection was made
			opcol_=c;
			moveTime_=getTime()-moveTime_;
			StringCbPrintfW(str,MAX_DESC,L"Player %d's move took %.2lf sec",((who>0)?1:2),moveTime_/1000.0);
		    timer_->set(str);
			StringCbPrintfW(str,MAX_DESC,L"chosen row: %d, col: %d",r,c);
		    chosen_->set(str);
			timeLastChange=now;
			if(thegame_->add(r,c,who)){
		       piece=CreateBox(-HALFWIDTH,-2,(-HALFWIDTH)*MODEL_Z_AXIS,HALFWIDTH,2,HALFWIDTH*MODEL_Z_AXIS);
	           (object_[r][c]=CreateObject(piece, (who==1)?&redish:&greenish))->translate(::visualX(c),0,::visualZ(r));
			   flipto_=who;
			   thegame_->generateLegalMoves();
			   msgChange_=true;
			   if(thegame_->hasValidMove(-who)){
				   thegame_->changePlayer();
  		  		   oprow_=now;
			   }
			}
			else{
				forfeit_=true;
				whoforfeit_=(who==1)?1:2;
				thegame_->setState(ISOVERSTATE);
			}
		  }
		}
	  }
	}
}
