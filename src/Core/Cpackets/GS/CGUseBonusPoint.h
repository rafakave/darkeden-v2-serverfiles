//////////////////////////////////////////////////////////////////////
// 
// Filename    : CGUseBonusPoint.h 
// Written By  : crazydog
// Description : vampire가 bonus point를 사용한다.
// 
//////////////////////////////////////////////////////////////////////

#ifndef __CG_USE_BONUS_POINT_H__
#define __CG_USE_BONUS_POINT_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"


#define INC_INT		0
#define INC_STR		1
#define INC_DEX		2


//////////////////////////////////////////////////////////////////////
//
// class CGUseBonusPoint;
//
//////////////////////////////////////////////////////////////////////

class CGUseBonusPoint : public Packet {

public:
	
	// constructor
	CGUseBonusPoint() ;
	
	// destructor
	~CGUseBonusPoint() ;

	
public:
	
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read(SocketInputStream & iStream) ;
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write(SocketOutputStream & oStream) ;

	// execute packet's handler
	void execute(Player* pPlayer) ;

	// get packet id
	PacketID_t getPacketID()  { return PACKET_CG_USE_BONUS_POINT; }
	
	// get packet's body size
	PacketSize_t getPacketSize()  { return szBYTE; }

	// get packet name
	string getPacketName()  { return "CGUseBonusPoint"; }

	// get/set which 
	BYTE getWhich()  { return m_Which;}
	void setWhich(BYTE w)  { m_Which = w;}

	// get packet's debug string
	string toString() ;

private :

	// which
	BYTE m_Which;

};


//////////////////////////////////////////////////////////////////////
//
// class CGUseBonusPointFactory;
//
// Factory for CGUseBonusPoint
//
//////////////////////////////////////////////////////////////////////

class CGUseBonusPointFactory : public PacketFactory {

public:
	
	// constructor
	CGUseBonusPointFactory()  {}
	
	// destructor
	virtual ~CGUseBonusPointFactory()  {}

	
public:
	
	// create packet
	Packet* createPacket()  { return new CGUseBonusPoint(); }

	// get packet name
	string getPacketName()  { return "CGUseBonusPoint"; }
	
	// get packet id
	PacketID_t getPacketID()  { return Packet::PACKET_CG_USE_BONUS_POINT; }

	// get Packet Max Size
	PacketSize_t getPacketMaxSize()  { return szBYTE; }

};


//////////////////////////////////////////////////////////////////////
//
// class CGUseBonusPointHandler;
//
//////////////////////////////////////////////////////////////////////

class CGUseBonusPointHandler {

public:

	// execute packet's handler
	static void execute(CGUseBonusPoint* pCGUseBonusPoint, Player* pPlayer);

};

#endif
