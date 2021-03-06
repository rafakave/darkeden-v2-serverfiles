//--------------------------------------------------------------------------------
// 
// Filename    : UCRequestLoginMode.h 
// Written By  : Reiot
// 
//--------------------------------------------------------------------------------

#ifndef __UC_REQUEST_LOGIN_MODE_H__
#define __UC_REQUEST_LOGIN_MODE_H__

// include files
#include "Packet.h"
#include "PacketFactory.h"

//--------------------------------------------------------------------------------
//
// class UCRequestLoginMode;
//
//--------------------------------------------------------------------------------

class UCRequestLoginMode : public Packet {

public :

	// 입력스트림(버퍼)으로부터 데이터를 읽어서 패킷을 초기화한다.
	void read ( SocketInputStream & iStream )  { throw UnsupportedError(__PRETTY_FUNCTION__); }

    // 소켓으로부터 직접 데이터를 읽어서 패킷을 초기화한다.
    void read ( Socket * pSocket ) ;
		    
    // 출력스트림(버퍼)으로 패킷의 바이너리 이미지를 보낸다.
    void write ( SocketOutputStream & oStream )  { throw UnsupportedError(__PRETTY_FUNCTION__); }

    // 소켓으로 직접 패킷의 바이너리 이미지를 보낸다.
    void write ( Socket * pSocket ) ;

	// execute packet's handler
	void execute ( Player * pPlayer ) ;

	// get packet id
	PacketID_t getPacketID ()  { return PACKET_UC_REQUEST_LOGIN_MODE; }
	
	// get packet body size
	// *OPTIMIZATION HINT*
	// const static UCRequestLoginModePacketSize 를 정의, 리턴하라.
	PacketSize_t getPacketSize ()  
	{ 
		return szBYTE;
	}
	
	// 아무리 커도 백메가는 받지 못한다.
	static PacketSize_t getPacketMaxSize ()  
	{ 
		return szBYTE;
	}

	// get packet's name
	string getPacketName ()  { return "UCRequestLoginMode"; }
	
	// get packet's debug string
	string toString () ;


public :

	// get/set login mode
	BYTE getLoginMode() const { return m_LoginMode; }
	void setLoginMode( BYTE loginMode ) { m_LoginMode = loginMode; }

private :

	BYTE m_LoginMode;
	
};


//--------------------------------------------------------------------------------
//
// class UCRequestLoginModeFactory;
//
// Factory for UCRequestLoginMode
//
//--------------------------------------------------------------------------------

class UCRequestLoginModeFactory : public PacketFactory {

public :
	
	// create packet
	Packet * createPacket ()  { return new UCRequestLoginMode(); }

	// get packet name
	string getPacketName ()  { return "UCRequestLoginMode"; }
	
	// get packet id
	PacketID_t getPacketID ()  { return Packet::PACKET_UC_REQUEST_LOGIN_MODE; }

	// get packet's max body size
	PacketSize_t getPacketMaxSize ()  { return szBYTE; }
	
};


//--------------------------------------------------------------------------------
//
// class UCRequestLoginModeHandler;
//
//--------------------------------------------------------------------------------

class UCRequestLoginModeHandler {

public :

	// execute packet's handler
	static void execute ( UCRequestLoginMode * pPacket , Player * pPlayer ) ;

};

#endif
