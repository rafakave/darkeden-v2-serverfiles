////////////////////////////////////////////////////////////////////////////////
// Filename    : ConditionCanEnterLevelWarZone.cpp
// Written By  : 
// Description :
////////////////////////////////////////////////////////////////////////////////

#include "ConditionCanEnterLevelWarZone.h"
#include "CreatureUtil.h"
#include "LevelWarZoneInfoManager.h"
#include "VariableManager.h"

////////////////////////////////////////////////////////////////////////////////
// is satisfied?
////////////////////////////////////////////////////////////////////////////////
bool ConditionCanEnterLevelWarZone::isSatisfied (Creature * pCreature1 , Creature * pCreature2, void* pParam) const 
	throw () 
{ 
	Assert(pCreature2 != NULL);
	Assert(pCreature2->isPC());

//	if ( g_pVariableManager->getVariable( ACTIVE_LEVEL_WAR ) == 0 ) return false;

	if ( g_pLevelWarZoneInfoManager->getCreatureLevelGrade( pCreature2 ) != -1 )
		return true;
	
	return false;
}

////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////
void ConditionCanEnterLevelWarZone::read (PropertyBuffer & propertyBuffer) 
	
{
}

////////////////////////////////////////////////////////////////////////////////
	// get debug string
////////////////////////////////////////////////////////////////////////////////
string ConditionCanEnterLevelWarZone::toString () const 
	throw () 
{ 
	__BEGIN_TRY

	StringStream msg;
	msg << "ConditionCanEnterLevelWarZone("
		<< ")"; 
	return msg.toString();

	__END_CATCH
}
