//--------------------------------------------------------------------------------
// 
// Filename    : GCSearchMotorcycleFail.h 
// Written By  : 김성민
// Description : 플레이어에게 서버 측의 상점 버전을 알려줄 때 쓰이는 패킷이다.
// 
//--------------------------------------------------------------------------------

#ifndef __GC_SEARCH_MOTORCYCLE_FAIL_H__
#define __GC_SEARCH_MOTORCYCLE_FAIL_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"


//--------------------------------------------------------------------------------
//
// class GCSearchMotorcycleFail;
//
//--------------------------------------------------------------------------------

class GCSearchMotorcycleFail : public Packet 
{
public :
	void read(SocketInputStream & iStream) ;
	void write(SocketOutputStream & oStream) ;
	void execute(Player* pPlayer) ;
	PacketID_t getPacketID()  { return PACKET_GC_SEARCH_MOTORCYCLE_FAIL; }
	PacketSize_t getPacketSize()  { return 0; }
	string getPacketName()  { return "GCSearchMotorcycleFail"; }
	string toString() ;
};


//////////////////////////////////////////////////////////////////////
//
// class GCSearchMotorcycleFailFactory;
//
// Factory for GCSearchMotorcycleFail
//
//////////////////////////////////////////////////////////////////////

class GCSearchMotorcycleFailFactory : public PacketFactory 
{

public :
	
	Packet* createPacket()  { return new GCSearchMotorcycleFail(); }
	string getPacketName()  { return "GCSearchMotorcycleFail"; }
	PacketID_t getPacketID()  { return Packet::PACKET_GC_SEARCH_MOTORCYCLE_FAIL; }
	PacketSize_t getPacketMaxSize()  { return 0; }

};


//////////////////////////////////////////////////////////////////////
//
// class GCSearchMotorcycleFailHandler;
//
//////////////////////////////////////////////////////////////////////

class GCSearchMotorcycleFailHandler 
{
	
public :
	
	// execute packet's handler
	static void execute(GCSearchMotorcycleFail* pPacket, Player* pPlayer) ;

};

#endif
