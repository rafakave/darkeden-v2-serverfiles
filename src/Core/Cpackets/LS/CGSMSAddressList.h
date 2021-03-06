//////////////////////////////////////////////////////////////////////////////
// Filename    : CGSMSAddressList.h 
// Written By  : elca@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_SMS_ADDRESS_LIST_H__
#define __CG_SMS_ADDRESS_LIST_H__

#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGSMSAddressList;
//////////////////////////////////////////////////////////////////////////////

class CGSMSAddressList : public Packet 
{
public:
	void read(SocketInputStream & iStream) ;
	void write(SocketOutputStream & oStream) ;
	void execute(Player* pPlayer) ;
	PacketID_t getPacketID()  { return PACKET_CG_SMS_ADDRESS_LIST; }
	PacketSize_t getPacketSize()  { return 0; }
	string getPacketName()  { return "CGSMSAddressList"; }
	string toString() ;

};

//////////////////////////////////////////////////////////////////////
// class CGSMSAddressListFactory;
//////////////////////////////////////////////////////////////////////

class CGSMSAddressListFactory : public PacketFactory 
{
public:
	Packet* createPacket()  { return new CGSMSAddressList(); }
	string getPacketName()  { return "CGSMSAddressList"; }
	PacketID_t getPacketID()  { return Packet::PACKET_CG_SMS_ADDRESS_LIST; }
	PacketSize_t getPacketMaxSize()  { return 0; }
};


//////////////////////////////////////////////////////////////////////
// class CGSMSAddressListHandler;
//////////////////////////////////////////////////////////////////////

class CGSMSAddressListHandler 
{
public:
	static void execute(CGSMSAddressList* pCGSMSAddressList, Player* pPlayer) ;
};

#endif
