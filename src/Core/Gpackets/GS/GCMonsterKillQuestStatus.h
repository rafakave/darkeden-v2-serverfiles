//////////////////////////////////////////////////////////////////////////////
// Filename    : GCMonsterKillQuestStatus.h 
// Written By  : elca@ewestsoft.com
// Description : 
// 기술이 성공했을때 보내는 패킷을 위한 클래스 정의
//////////////////////////////////////////////////////////////////////////////

#ifndef __GC_MONSTER_KILL_QUEST_STATUS_H__
#define __GC_MONSTER_KILL_QUEST_STATUS_H__

#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////////////
// class GCMonsterKillQuestStatus;
// 게임서버에서 클라이언트로 자신의 기술이 성공을 알려주기 위한 클래스
//////////////////////////////////////////////////////////////////////////////

class GCMonsterKillQuestStatus : public Packet 
{
public:
	GCMonsterKillQuestStatus() ;
	~GCMonsterKillQuestStatus() ;
	
public:
    void read(SocketInputStream & iStream) ;
    void write(SocketOutputStream & oStream) ;
	void execute(Player* pPlayer) ;
	PacketID_t getPacketID()  { return PACKET_GC_MONSTER_KILL_QUEST_STATUS; }
	PacketSize_t getPacketSize()  { return szWORD + szWORD + szDWORD; }
	string getPacketName()  { return "GCMonsterKillQuestStatus"; }
	string toString() ;

public:
	WORD getQuestID()  { return m_QuestID; }
	void setQuestID(WORD e)  { m_QuestID = e; }
	
	WORD getCurrentNum()  { return m_CurrentNum; }
	void setCurrentNum(WORD n)  { m_CurrentNum = n; }

	DWORD getRemainTime()  { return m_Time; }
	void setRemainTime(DWORD d)  { m_Time = d; }
	
private :
	WORD m_QuestID;
	WORD m_CurrentNum;
	DWORD m_Time;
};


//////////////////////////////////////////////////////////////////////////////
// class GCMonsterKillQuestStatusFactory;
//////////////////////////////////////////////////////////////////////////////

class GCMonsterKillQuestStatusFactory : public PacketFactory 
{
public :
	GCMonsterKillQuestStatusFactory()  {}
	virtual ~GCMonsterKillQuestStatusFactory()  {}
	
public:
	Packet* createPacket()  { return new GCMonsterKillQuestStatus(); }
	string getPacketName()  { return "GCMonsterKillQuestStatus"; }
	PacketID_t getPacketID()  { return Packet::PACKET_GC_MONSTER_KILL_QUEST_STATUS; }
	PacketSize_t getPacketMaxSize()  { return szWORD + szWORD + szDWORD; }
};

//////////////////////////////////////////////////////////////////////////////
// class GCMonsterKillQuestStatusHandler;
//////////////////////////////////////////////////////////////////////////////

class GCMonsterKillQuestStatusHandler 
{
public:
	static void execute(GCMonsterKillQuestStatus* pGCMonsterKillQuestStatus, Player* pPlayer);

};

#endif
