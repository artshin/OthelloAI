//#include "iHUD.h"


class MessageCentre{
	static MessageCentre* address_;

	MessageCentre();
	MessageCentre(const MessageCentre& m);
	MessageCentre& operator=(const MessageCentre& m);

	//iHUD* hud_;

	~MessageCentre() { /*delete hud_;*/};

public:
	static MessageCentre* CreateMessageCentre();
	static MessageCentre* Address() { return address_; };
	void Delete() const { delete this; address_ = 0; };

};

MessageCentre* CreateMessageCentre(){ return MessageCentre::CreateMessageCentre();}