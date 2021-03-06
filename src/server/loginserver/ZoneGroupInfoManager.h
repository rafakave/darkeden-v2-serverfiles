//----------------------------------------------------------------------
//
// Filename    : ZoneGroupInfoManager.h
// Written By  : Reiot
// Description :
//
//----------------------------------------------------------------------

#ifndef __ZONE_GROUP_INFO_MANAGER_H__
#define __ZONE_GROUP_INFO_MANAGER_H__

// include files
#include "Types.h"
#include "Exception.h"
#include "ZoneGroupInfo.h"
#include <hash_map>

typedef hash_map< ZoneGroupID_t , ZoneGroupInfo * > HashMapZoneGroupInfo;

//----------------------------------------------------------------------
//
// class ZoneGroupInfoManager;
//
// 존그룹 아이디를 키값으로 하는 존 정보의 hash_map 을 가지고 있다.
//
//----------------------------------------------------------------------

class ZoneGroupInfoManager {
	
public :
	
	// constructor
	ZoneGroupInfoManager ();
	
	// destructor
	~ZoneGroupInfoManager ();

	// initialize manager
	void init ();

	// load from database
	void load ();
	
	// add info
	void addZoneGroupInfo ( ZoneGroupInfo * pZoneGroupInfo ) ;
	
	// delete info
	void deleteZoneGroupInfo ( ZoneGroupID_t zoneGroupID ) ;
	
	// get info
	ZoneGroupInfo * getZoneGroupInfo ( ZoneGroupID_t zoneGroupID ) ;

	// get count of info
	uint getSize ()  { return m_ZoneGroupInfos.size(); }

	// get debug string
	string toString () ;

private :
	
	// hash map of ZoneGroupInfo
	// key   : ZoneGroupID_t
	// value : ZoneGroupInfo *
	HashMapZoneGroupInfo m_ZoneGroupInfos;

};


// global variable declaration
extern ZoneGroupInfoManager * g_pZoneGroupInfoManager;

#endif
