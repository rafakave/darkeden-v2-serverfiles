//////////////////////////////////////////////////////////////////////////////
// Filename    : CGAddMouseToZone.h 
// Written By  : reiot@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_ADD_MOUSE_TO_ZONE_H__
#define __CG_ADD_MOUSE_TO_ZONE_H__

#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGAddMouseToZone;
//////////////////////////////////////////////////////////////////////////////

class CGAddMouseToZone : public Packet 
{
public:
	CGAddMouseToZone() ;
	~CGAddMouseToZone() ;

public:
    void read(SocketInputStream & iStream) ;
    void write(SocketOutputStream & oStream) ;
	void execute(Player* pPlayer) ;
	PacketID_t getPacketID()  { return PACKET_CG_ADD_MOUSE_TO_ZONE; }
	PacketSize_t getPacketSize()  { return szObjectID; }
	string getPacketName()  { return "CGAddMouseToZone"; }
	string toString() ;
	
public:
	ObjectID_t getObjectID()  { return m_ObjectID; }
	void setObjectID(ObjectID_t ObjectID)  { m_ObjectID = ObjectID; }

private :
	ObjectID_t m_ObjectID;
};

//////////////////////////////////////////////////////////////////////////////
// class CGAddMouseToZoneFactory;
//////////////////////////////////////////////////////////////////////////////

class CGAddMouseToZoneFactory : public PacketFactory 
{
public:
	Packet* createPacket()  { return new CGAddMouseToZone(); }
	string getPacketName()  { return "CGAddMouseToZone"; }
	PacketID_t getPacketID()  { return Packet::PACKET_CG_ADD_MOUSE_TO_ZONE; }
	PacketSize_t getPacketMaxSize()  { return szObjectID; }
};

//////////////////////////////////////////////////////////////////////////////
// class CGAddMouseToZoneHandler;
//////////////////////////////////////////////////////////////////////////////

class CGAddMouseToZoneHandler 
{
public:
	static void execute(CGAddMouseToZone* pPacket, Player* player) ;
};

#endif
