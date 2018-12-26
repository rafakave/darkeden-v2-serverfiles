//////////////////////////////////////////////////////////////////////
// 
// Filename    : GCSkillToObjectOK5.h 
// Written By  : elca@ewestsoft.com
// Description : 기술이 성공했을때 보내는 패킷을 위한 클래스 정의
// 				기술에 당한 사람, 기술을 사용한 사람 모두 볼수 있는 분에게 보내는 패킷
// 
//////////////////////////////////////////////////////////////////////

#ifndef __GC_SKILL_TO_OBJECT_OK_5_H__
#define __GC_SKILL_TO_OBJECT_OK_5_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "Packet.h"
#include "PacketFactory.h"

//////////////////////////////////////////////////////////////////////
//
// class GCSkillToObjectOK5;
//
// 게임서버에서 클라이언트로 자신의 기술이 성공을 알려주기 위한 클래스
//
//////////////////////////////////////////////////////////////////////

class GCSkillToObjectOK5 : public Packet {

public :
	
	// constructor
	GCSkillToObjectOK5() ;
	
	// destructor
	~GCSkillToObjectOK5() ;

	
public :
	
	
    // 입력스트림(버퍼)으로부터 데이타를 읽어서 패킷을 초기화한다.
    void read(SocketInputStream & iStream) ;
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write(SocketOutputStream & oStream) ;

	// execute packet's handler
	void execute(Player* pPlayer) ;

	// get packet id
	PacketID_t getPacketID()  { return PACKET_GC_SKILL_TO_OBJECT_OK_5; }
	
	// get packet's body size
	// 최적화시, 미리 계산된 정수를 사용한다.
	PacketSize_t getPacketSize()  { return szObjectID + szSkillType + szObjectID + szDuration + szBYTE; }

	// get packet's name
	string getPacketName()  { return "GCSkillToObjectOK5"; }
	
	// get packet's debug string
	string toString() ;

	// get / set ObjectID
	ObjectID_t getObjectID()  { return m_ObjectID; }
	void setObjectID(ObjectID_t ObjectID) throw() { m_ObjectID = ObjectID; }
	ObjectID_t getTargetObjectID()  { return m_TargetObjectID; }
	void setTargetObjectID(ObjectID_t ObjectID) throw() { m_TargetObjectID = ObjectID; }

	// get / set SkillType
	SkillType_t getSkillType()  { return m_SkillType; }
	void setSkillType(SkillType_t SkillType) throw() { m_SkillType = SkillType; }
	
/*
	// get / set Target X,Y
	Coord_t getTargetX()  { return m_TargetX; }
	Coord_t getTargetY()  { return m_TargetY; }
	void setTargetXY(Coord_t X, Coord_t Y) throw() { m_TargetX = X; m_TargetY = Y; }
*/
	
	// get / set Duration
	Duration_t getDuration()  { return m_Duration; }
	void setDuration(Duration_t Duration) throw() { m_Duration = Duration; }
	
	// get / set CEffectID 
//	CEffectID_t getCEffectID()  { return m_CEffectID; }
//	void setCEffectID(CEffectID_t e) throw() { m_CEffectID = e; }

	BYTE getGrade()  { return m_Grade; }
	void setGrade( BYTE grade ) throw() { m_Grade = grade; }

private :
	
	// ObjectID, TargetObjectID
	ObjectID_t m_ObjectID, m_TargetObjectID;

	// SkillType
	SkillType_t m_SkillType;

	// Duration
	Duration_t m_Duration;

	// CEffectID
//	CEffectID_t m_CEffectID;

	BYTE m_Grade;

};


//////////////////////////////////////////////////////////////////////
//
// class GCSkillToObjectOK5Factory;
//
// Factory for GCSkillToObjectOK5
//
//////////////////////////////////////////////////////////////////////

class GCSkillToObjectOK5Factory : public PacketFactory {

public :
	
	// constructor
	GCSkillToObjectOK5Factory() throw() {}
	
	// destructor
	virtual ~GCSkillToObjectOK5Factory() throw() {}

	
public :
	
	// create packet
	Packet* createPacket() throw() { return new GCSkillToObjectOK5(); }

	// get packet name
	string getPacketName()  { return "GCSkillToObjectOK5"; }
	
	// get packet id
	PacketID_t getPacketID()  { return Packet::PACKET_GC_SKILL_TO_OBJECT_OK_5; }

	// get Packet Max Size
	PacketSize_t getPacketMaxSize()  { return szObjectID + szSkillType + szObjectID + szDuration + szBYTE; }

};


//////////////////////////////////////////////////////////////////////
//
// class GCSkillToObjectOK5Handler;
//
//////////////////////////////////////////////////////////////////////

class GCSkillToObjectOK5Handler {

public :

	// execute packet's handler
	static void execute(GCSkillToObjectOK5* pGCSkillToObjectOK5, Player* pPlayer);

};

#endif
