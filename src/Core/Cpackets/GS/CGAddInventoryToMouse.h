//////////////////////////////////////////////////////////////////////////////
// Filename    : CGAddInventoryToMouse.h 
// Written By  : reiot@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __CG_ADD_INVENTORY_TO_MOUSE_H__
#define __CG_ADD_INVENTORY_TO_MOUSE_H__

#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class CGAddInventoryToMouse;
//////////////////////////////////////////////////////////////////////////////

class CGAddInventoryToMouse : public Packet 
{
public:
	CGAddInventoryToMouse() ;
	~CGAddInventoryToMouse() ;

public:
    void read(SocketInputStream & iStream) ;
    void write(SocketOutputStream & oStream) ;
	void execute(Player* pPlayer) ;
	PacketID_t getPacketID()  { return PACKET_CG_ADD_INVENTORY_TO_MOUSE; }
	PacketSize_t getPacketSize()  { return szObjectID + szObjectID + szCoordInven + szCoordInven; }
	string getPacketName()  { return "CGAddInventoryToMouse"; }
	string toString() ;
	
public:
	ObjectID_t getObjectID()  { return m_ObjectID; }
	void setObjectID(ObjectID_t ObjectID)  { m_ObjectID = ObjectID; }

	ObjectID_t getInventoryItemObjectID()  { return m_InventoryItemObjectID; }
	void setInventoryItemObjectID(ObjectID_t InventoryItemObjectID)  { m_InventoryItemObjectID = InventoryItemObjectID; }

	CoordInven_t getX()  { return m_InvenX; }
	void setX(CoordInven_t InvenX)  { m_InvenX = InvenX; }

	CoordInven_t getY()  { return m_InvenY; }
	void setY(CoordInven_t InvenY) { m_InvenY = InvenY; }

private:
	ObjectID_t m_ObjectID;

	// 보조 인벤토리 아이템의 오브젝트 아이디. 0이면 메인 인벤토리에서 꺼냄
	ObjectID_t m_InventoryItemObjectID;

	CoordInven_t m_InvenX;
	CoordInven_t m_InvenY;
};

//////////////////////////////////////////////////////////////////////////////
// class CGAddInventoryToMouseFactory;
//////////////////////////////////////////////////////////////////////////////

class CGAddInventoryToMouseFactory : public PacketFactory 
{
public:
	Packet* createPacket()  { return new CGAddInventoryToMouse(); }
	string getPacketName()  { return "CGAddInventoryToMouse"; }
	PacketID_t getPacketID()  { return Packet::PACKET_CG_ADD_INVENTORY_TO_MOUSE; }
	PacketSize_t getPacketMaxSize()  { return szObjectID + szObjectID + szCoordInven + szCoordInven; }
};

//////////////////////////////////////////////////////////////////////////////
// class CGAddInventoryToMouseHandler;
//////////////////////////////////////////////////////////////////////////////

class CGAddInventoryToMouseHandler 
{
public:
	static void execute(CGAddInventoryToMouse* pPacket, Player* player) ;
};

#endif
