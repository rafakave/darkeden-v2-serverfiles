#include "EffectChainOfDemon.h"
#include "Creature.h"
#include "PlayerCreature.h"
#include "Player.h"
#include "SkillUtil.h"
#include "Zone.h"
#include "Gpackets/GCModifyInformation.h"
#include "Gpackets/GCRemoveEffect.h"

EffectChainOfDemon::EffectChainOfDemon(Creature* pCreature) 
{
	__BEGIN_TRY

	m_pTarget = pCreature;
	m_pZone = pCreature->getZone();

	__END_CATCH
}

void EffectChainOfDemon::affect() 
{
	__BEGIN_TRY

	if ( m_pTarget != NULL && m_pTarget->getObjectClass() == Object::OBJECT_CLASS_CREATURE )
	{
		affect( dynamic_cast<Creature*>(m_pTarget) );
	}

	setNextTime(10);

	__END_CATCH
}

void EffectChainOfDemon::affect(Creature* pCreature) 
{
	__BEGIN_TRY

	if ( pCreature == NULL ) return;


	__END_CATCH
}

void EffectChainOfDemon::unaffect() 
{
	__BEGIN_TRY

	if ( m_pTarget != NULL && m_pTarget->getObjectClass() == Object::OBJECT_CLASS_CREATURE )
	{
		unaffect( dynamic_cast<Creature*>(m_pTarget) );
	}

	__END_CATCH
}

void EffectChainOfDemon::unaffect( Creature* pCreature ) 
{
	__BEGIN_TRY

	pCreature->removeFlag( getEffectClass() );

	Zone* pZone = pCreature->getZone();
	Assert(pZone != NULL);

	// 이펙트가 사라졌다고 알려준다.
	GCRemoveEffect gcRemoveEffect;
	gcRemoveEffect.setObjectID(pCreature->getObjectID());
	gcRemoveEffect.addEffectList( getSendEffectClass() );
	pZone->broadcastPacket(pCreature->getX(), pCreature->getY(), &gcRemoveEffect);

	__END_CATCH
}

string EffectChainOfDemon::toString() 
{
	__BEGIN_TRY

	return "EffectChainOfDemon";

	__END_CATCH
}
