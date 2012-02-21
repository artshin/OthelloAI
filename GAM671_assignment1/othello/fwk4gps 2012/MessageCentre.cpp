#include "MessageCentre.h"
#include "iTexture.h"			// for Texture elements
#include "ModellingLayer.h"		// for modelling elements


MessageCentre::MessageCentre(){

	address_ = this;

	//hud_ = CreateHUD(0.72f, 0.01f, 0.27f, 0.99f, CreateTexture(HUD_IMAGE));
}


MessageCentre* MessageCentre::CreateMessageCentre(){
	// if MessageCentre exists then return its address otherwise create new MessageCentre
	return MessageCentre::address_ ? MessageCentre::address_ : new MessageCentre();
}