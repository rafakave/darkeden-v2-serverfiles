//////////////////////////////////////////////////////////////////////////////
// Filename    : EffectHolyArmor.cpp
// Written by  : elca
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "EffectHolyArmor2.h"
#include "Slayer.h"
#include "Player.h"

#include "Gpackets/GCModifyInformation.h"
#include "Gpackets/GCStatusCurrentHP.h"
#include "Gpackets/GCRemoveEffect.h"

EffectHolyArmor2::EffectHolyArmor2(Creature* pCreature)
	
{
	__BEGIN_TRY

	setTarget(pCreature);

	__END_CATCH
}

void EffectHolyArmor2::affect(Creature* pCreature)
	
{
	__BEGIN_TRY
	__END_CATCH
}

void EffectHolyArmor2::unaffect()
	    
{
    __BEGIN_TRY

	//cout << "EffectHolyArmor2 " << "unaffect BEGIN" << endl;

    Creature* pCreature = dynamic_cast<Creature *>(m_pTarget);
    unaffect(pCreature);

	//cout << "EffectHolyArmor2 " << "unaffect END" << endl;

    __END_CATCH
}

void EffectHolyArmor2::unaffect(Creature* pCreature)
	
{
	__BEGIN_TRY
	__BEGIN_DEBUG

	//cout << "EffectHolyArmor2 " << "unaffect BEGIN" << endl;

	Assert(pCreature != NULL);
	Assert(pCreature->isSlayer());

	pCreature->removeFlag(Effect::EFFECT_CLASS_HOLY_ARMOR2);

	Slayer* pSlayer = dynamic_cast<Slayer*>(pCreature);

	SLAYER_RECORD prev;
	pSlayer->getSlayerRecord(prev);
	pSlayer->initAllStat();
	pSlayer->sendRealWearingInfo();
	pSlayer->sendModifyInfo(prev);

	Zone* pZone = pSlayer->getZone();
	Assert(pZone != NULL);

	// 이펙트가 사라졌다고 알려준다.
	GCRemoveEffect gcRemoveEffect;
	gcRemoveEffect.setObjectID(pSlayer->getObjectID());
	gcRemoveEffect.addEffectList(Effect::EFFECT_CLASS_HOLY_ARMOR2);
	pZone->broadcastPacket(pSlayer->getX(), pSlayer->getY(), &gcRemoveEffect);

	//cout << "EffectHolyArmor2 " << "unaffect END" << endl;

	__END_DEBUG
	__END_CATCH
}

string EffectHolyArmor2::toString() const 
	
{
	__BEGIN_TRY

	StringStream msg;
	msg << "EffectHolyArmor2("
		<< "ObjectID:" << getObjectID()
		<< ")";
	return msg.toString();

	__END_CATCH
}
