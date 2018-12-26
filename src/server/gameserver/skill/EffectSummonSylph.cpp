//////////////////////////////////////////////////////////////////////////////
// Filename    : EffectSummonSylph.cpp
// Written by  : elca
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "EffectSummonSylph.h"
#include "Ousters.h"
#include "Player.h"

#include "Gpackets/GCRemoveEffect.h"

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
EffectSummonSylph::EffectSummonSylph(Creature* pCreature)
	
{
	__BEGIN_TRY

	Assert(pCreature != NULL);
	Assert(pCreature->isOusters());

	setTarget(pCreature);

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void EffectSummonSylph::unaffect(Creature* pCreature)
	
{
	__BEGIN_TRY

	//cout << "EffectSummonSylph" << "unaffect BEGIN" << endl;

	Assert(pCreature != NULL);
	Assert(pCreature->isOusters());

	// 플래그를 끈다.
	pCreature->removeFlag(Effect::EFFECT_CLASS_SUMMON_SYLPH);

	Zone* pZone = pCreature->getZone();
	Assert(pZone != NULL);

	Ousters* pTargetOusters = dynamic_cast<Ousters*>(pCreature);

	pTargetOusters->setWingSylphType(0);
	pTargetOusters->setWingItemType(0);
	pTargetOusters->setWingColor(0);
	pTargetOusters->setWingEffectColor(0);

	Assert( pTargetOusters != NULL );

	OUSTERS_RECORD prev;

	pTargetOusters->getOustersRecord(prev);
	pTargetOusters->initAllStat();
	pTargetOusters->sendRealWearingInfo();
	pTargetOusters->sendModifyInfo(prev);

	// 이펙트를 삭제하라고 알려준다.
	GCRemoveEffect gcRemoveEffect;
	gcRemoveEffect.setObjectID(pCreature->getObjectID());
	gcRemoveEffect.addEffectList(getSendEffectClass());
	pZone->broadcastPacket(pCreature->getX(), pCreature->getY(), &gcRemoveEffect);

	/*gcRemoveEffect.setObjectID(pCreature->getObjectID());
	gcRemoveEffect.addEffectList(556);
	pTargetOusters->getPlayer()->sendPacket(&gcRemoveEffect);*/

	//cout << "EffectSummonSylph" << "unaffect END" << endl;

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void EffectSummonSylph::unaffect()
	
{
	__BEGIN_TRY

	Creature* pCreature = dynamic_cast<Creature *>(m_pTarget);
	unaffect(pCreature);

	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
string EffectSummonSylph::toString()
	
{
	__BEGIN_TRY

	StringStream msg;
	msg << "EffectSummonSylph("
		<< "ObjectID:" << getObjectID()
		<< ")";
	return msg.toString();

	__END_CATCH

}

