//////////////////////////////////////////////////////////////////////////////
// Filename    : VoodooRing.cpp
// Written by  : rallser
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "VoodooRing.h"
#include "EffectPoison.h"

#include "Gpackets/GCSkillToSelfOK1.h"
#include "Gpackets/GCSkillToSelfOK2.h"
#include "Gpackets/GCAddEffect.h"
#include "Gpackets/GCStatusCurrentHP.h"
VoodooRing::VoodooRing()
	throw()
{
	__BEGIN_TRY
	__END_CATCH
}

//////////////////////////////////////////////////////////////////////////////
// 슬레이어 셀프 핸들러
//////////////////////////////////////////////////////////////////////////////

void VoodooRing::execute(Vampire* pVampire, VampireSkillSlot* pVampireSkillSlot, CEffectID_t CEffectID)
	
{
	__BEGIN_TRY

	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " Begin(slayer)" << endl;

	Assert(pVampire != NULL);
	Assert(pVampireSkillSlot != NULL);

	try 
	{
						cout << "베놈테스트 START" << endl;
		Player* pPlayer = pVampire->getPlayer();
		Zone* pZone = pVampire->getZone();

		Assert(pPlayer != NULL);
		Assert(pZone != NULL);
						cout << "베놈테스트 1" << endl;
		GCSkillToSelfOK1 _GCSkillToSelfOK1;
		GCSkillToSelfOK2 _GCSkillToSelfOK2;

		SkillType_t       SkillType  = pVampireSkillSlot->getSkillType();
		SkillInfo*        pSkillInfo = g_pSkillInfoManager->getSkillInfo(SkillType);
		//SkillDomainType_t DomainType = pSkillInfo->getDomainType();
						cout << "베놈테스트 2" << endl;
		int  RequiredMP  = (int)pSkillInfo->getConsumeMP();
		bool bManaCheck  = hasEnoughMana(pVampire, RequiredMP);
		bool bTimeCheck  = verifyRunTime(pVampireSkillSlot);
		bool bRangeCheck = checkZoneLevelToUseSkill(pVampire);
						cout << "베놈테스트 3" << endl;
		ZoneCoord_t X = pVampire->getX();
		ZoneCoord_t Y = pVampire->getY();

		VSRect rect(0, 0, pZone->getWidth()-1, pZone->getHeight()-1);
						cout << "베놈테스트 4" << endl;
		if (bManaCheck && bTimeCheck && bRangeCheck)
		{
						cout << "베놈테스트 5" << endl;
			decreaseMana(pVampire, RequiredMP, _GCSkillToSelfOK1);

			// 지속 시간을 계산한다.
			SkillInput input(pVampire);
			SkillOutput output;
			computeOutput(input, output);

			//bool bHit = false;

//			Level_t maxEnemyLevel = 0;
//			uint EnemyNum = 0;
						cout << "베놈테스트 6" << endl;
			for( int ox = -2 ; ox <= 2 ; ox++ )
			for( int oy = -2 ; oy <= 2 ; oy++ )
			{
				int tileX = X+ox;
				int tileY = Y+oy;

				if( !rect.ptInRect(tileX,tileY) ) continue;

				Tile& tile = pZone->getTile( tileX, tileY );
				if ( tile.getEffect( Effect::EFFECT_CLASS_TRYING_POSITION ) ) continue;
						cout << "베놈테스트 7" << endl;
				list<Creature*> targetList;
				if (tile.hasCreature(Creature::MOVE_MODE_WALKING))
				{
					Creature* pCreature = tile.getCreature(Creature::MOVE_MODE_WALKING);
					targetList.push_back(pCreature);
				}
				if (tile.hasCreature(Creature::MOVE_MODE_FLYING))
				{
					Creature* pCreature = tile.getCreature(Creature::MOVE_MODE_FLYING);
					targetList.push_back(pCreature);
				}
				if (tile.hasCreature(Creature::MOVE_MODE_BURROWING))
				{
					Creature* pCreature = tile.getCreature(Creature::MOVE_MODE_BURROWING);
					targetList.push_back(pCreature);
				}
						cout << "베놈테스트 8" << endl;
				list<Creature*>::iterator itr = targetList.begin();
				for ( ; itr != targetList.end() ; itr++ )
				{
					Creature* pTargetCreature = (*itr);
					Assert( pTargetCreature != NULL );
						cout << "베놈테스트 9" << endl;
					// 안전지대 체크
					// 2003.1.10 by bezz, Sequoia
					// NPC도 체크
					// 2003.3.14 by Sequoia
					if ( 
						pTargetCreature->isNPC()
						|| !checkZoneLevelToHitTarget(pTargetCreature)
				  		|| !canAttack( pVampire, pTargetCreature )
						|| pTargetCreature->isFlag( Effect::EFFECT_CLASS_COMA )
					)
						continue;
						cout << "베놈테스트 10" << endl;
					if( HitRoll::isSuccessVenom( pVampire, pVampireSkillSlot, pTargetCreature ) )
					{
						HP_t RemainHP = 0;
						// 맞는 넘한테 데미지를 주고 남은 HP 를 구한다.
						if ( pTargetCreature->isSlayer() )
						{
							GCModifyInformation gcMI;
							Slayer* pSlayer = dynamic_cast<Slayer*>(pTargetCreature);
							setDamage( pSlayer, output.Damage, pVampire, SkillType, &gcMI );
							pTargetCreature->getPlayer()->sendPacket( &gcMI );
							RemainHP = pSlayer->getHP(ATTR_CURRENT);
						}
						else if ( pTargetCreature->isOusters() )
						{
						cout << "베놈테스트 11" << endl;
							GCModifyInformation gcMI;

							Ousters* pOusters = dynamic_cast<Ousters*>(pTargetCreature);
							setDamage( pOusters, output.Damage, pVampire, SkillType, &gcMI );
						cout << "베놈테스트 12" << endl;
							pTargetCreature->getPlayer()->sendPacket( &gcMI );
							RemainHP = pOusters->getHP(ATTR_CURRENT);
						}
						else if ( pTargetCreature->isMonster() )
						{
							Monster* pMonster = dynamic_cast<Monster*>(pTargetCreature);
							setDamage( pTargetCreature, output.Damage, pVampire, SkillType );
							RemainHP = pMonster->getHP(ATTR_CURRENT);
						}

						// 이미 SLEEP 이펙트가 걸려있으면 먼저 걸려있던 이펙트를 삭제한다.
						if ( pTargetCreature->isFlag( Effect::EFFECT_CLASS_POISON) )
						{
							pTargetCreature->deleteEffect( Effect::EFFECT_CLASS_POISON);
						}

						// 포이즌 이펙트를 생성해서, 타겟 크리쳐에 붙이고, 플래그를 켜준다.
						EffectPoison* pEffectPoison = new EffectPoison(pTargetCreature);
						pEffectPoison->setLevel(pSkillInfo->getLevel()/2);
						pEffectPoison->setPoint(output.Damage/2);
						pEffectPoison->setDeadline(output.Duration); // 이부분 바꿔야 한다.
						pEffectPoison->setTick(50);             // 이부분도 바꿔야 한다.
						pEffectPoison->setUserObjectID( pVampire->getObjectID() );
						pEffectPoison->affect(pTargetCreature);
						pTargetCreature->addEffect(pEffectPoison);
						pTargetCreature->setFlag(Effect::EFFECT_CLASS_POISON);

						GCAddEffect gcAddEffect;
						gcAddEffect.setObjectID( pTargetCreature->getObjectID() );
						gcAddEffect.setEffectID( Effect::EFFECT_CLASS_POISON);
						gcAddEffect.setDuration( output.Duration );
						pZone->broadcastPacket( tileX, tileY, &gcAddEffect );

					}
				}
			}

			_GCSkillToSelfOK1.setSkillType(SkillType);
			_GCSkillToSelfOK1.setCEffectID(CEffectID);
			_GCSkillToSelfOK1.setDuration(0);
		
			_GCSkillToSelfOK2.setObjectID(pVampire->getObjectID());
			_GCSkillToSelfOK2.setSkillType(SkillType);
			_GCSkillToSelfOK2.setDuration(0);
		
			pPlayer->sendPacket(&_GCSkillToSelfOK1);
			pZone->broadcastPacket(pVampire->getX(), pVampire->getY(),  &_GCSkillToSelfOK2, pVampire);

			pVampireSkillSlot->setRunTime(output.Delay);
		} 
		else 
		{
			executeSkillFailNormal(pVampire, getSkillType(), NULL);
		}
	} 
	catch (Throwable & t) 
	{
		executeSkillFailException(pVampire, getSkillType());
	}

	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " End(slayer)" << endl;

	__END_CATCH
}
/*
void VoodooRing::execute(Slayer* pSlayer, SkillSlot* pSkillSlot, CEffectID_t CEffectID)
	
{
	__BEGIN_TRY

	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " Begin(slayer)" << endl;

	Assert(pSlayer != NULL);
	Assert(pSkillSlot != NULL);

	try 
	{
		Player* pPlayer = pSlayer->getPlayer();
		Zone* pZone = pSlayer->getZone();

		Assert(pPlayer != NULL);
		Assert(pZone != NULL);

		GCSkillToSelfOK1 _GCSkillToSelfOK1;
		GCSkillToSelfOK2 _GCSkillToSelfOK2;

		SkillType_t       SkillType  = pSkillSlot->getSkillType();
		SkillInfo*        pSkillInfo = g_pSkillInfoManager->getSkillInfo(SkillType);
		SkillDomainType_t DomainType = pSkillInfo->getDomainType();

		int  RequiredMP  = (int)pSkillInfo->getConsumeMP();
		bool bManaCheck  = hasEnoughMana(pSlayer, RequiredMP);
		bool bTimeCheck  = verifyRunTime(pSkillSlot);
		bool bRangeCheck = checkZoneLevelToUseSkill(pSlayer);

		ZoneCoord_t X = pSlayer->getX();
		ZoneCoord_t Y = pSlayer->getY();

		VSRect rect(0, 0, pZone->getWidth()-1, pZone->getHeight()-1);

		if (bManaCheck && bTimeCheck && bRangeCheck)
		{
			decreaseMana(pSlayer, RequiredMP, _GCSkillToSelfOK1);

			// 지속 시간을 계산한다.
			SkillInput input(pSlayer, pSkillSlot);
			SkillOutput output;
			computeOutput(input, output);

			bool bHit = false;

			Level_t maxEnemyLevel = 0;
			uint EnemyNum = 0;

			for( int ox = -2 ; ox <= 2 ; ox++ )
			for( int oy = -2 ; oy <= 2 ; oy++ )
			{
				int tileX = X+ox;
				int tileY = Y+oy;

				if( !rect.ptInRect(tileX,tileY) ) continue;

				Tile& tile = pZone->getTile( tileX, tileY );
				if ( tile.getEffect( Effect::EFFECT_CLASS_TRYING_POSITION ) ) continue;

				list<Creature*> targetList;
				if (tile.hasCreature(Creature::MOVE_MODE_WALKING))
				{
					Creature* pCreature = tile.getCreature(Creature::MOVE_MODE_WALKING);
					targetList.push_back(pCreature);
				}
				if (tile.hasCreature(Creature::MOVE_MODE_FLYING))
				{
					Creature* pCreature = tile.getCreature(Creature::MOVE_MODE_FLYING);
					targetList.push_back(pCreature);
				}
				if (tile.hasCreature(Creature::MOVE_MODE_BURROWING))
				{
					Creature* pCreature = tile.getCreature(Creature::MOVE_MODE_BURROWING);
					targetList.push_back(pCreature);
				}

				list<Creature*>::iterator itr = targetList.begin();
				for ( ; itr != targetList.end() ; itr++ )
				{
					Creature* pTargetCreature = (*itr);
					Assert( pTargetCreature != NULL );

					// 안전지대 체크
					// 2003.1.10 by bezz, Sequoia
					// NPC도 체크
					// 2003.3.14 by Sequoia
					if ( 
						pTargetCreature->isNPC()
						|| !checkZoneLevelToHitTarget(pTargetCreature)
				  		|| !canAttack( pSlayer, pTargetCreature )
						|| pTargetCreature->isFlag( Effect::EFFECT_CLASS_COMA )
					)
						continue;

					if( HitRoll::isSuccessRebuke( pSlayer, pSkillSlot, pTargetCreature ) )
					{
						HP_t RemainHP = 0;
						// 맞는 넘한테 데미지를 주고 남은 HP 를 구한다.
						if ( pTargetCreature->isVampire() )
						{
							GCModifyInformation gcMI;
							setDamage( pTargetCreature, output.Damage, pSlayer, SkillType, &gcMI );

							pTargetCreature->getPlayer()->sendPacket( &gcMI );

							Vampire* pVampire = dynamic_cast<Vampire*>(pTargetCreature);
							RemainHP = pVampire->getHP(ATTR_CURRENT);
						}
						else if ( pTargetCreature->isMonster() )
						{
							setDamage( pTargetCreature, output.Damage, pSlayer, SkillType );
							Monster* pMonster = dynamic_cast<Monster*>(pTargetCreature);
							RemainHP = pMonster->getHP(ATTR_CURRENT);
						}

						// 이미 SLEEP 이펙트가 걸려있으면 먼저 걸려있던 이펙트를 삭제한다.
						if ( pTargetCreature->isFlag( Effect::EFFECT_CLASS_SLEEP ) )
						{
							pTargetCreature->deleteEffect( Effect::EFFECT_CLASS_SLEEP );
						}

						EffectSleep* pEffect = new EffectSleep( pTargetCreature );

						// 맞는 넘한테 SLEEP 이펙트를 걸어준다.
						pEffect->setDeadline( output.Duration );
						pTargetCreature->addEffect( pEffect );
						pTargetCreature->setFlag( Effect::EFFECT_CLASS_SLEEP );

						GCAddEffect gcAddEffect;
						gcAddEffect.setObjectID( pTargetCreature->getObjectID() );
						gcAddEffect.setEffectID( Effect::EFFECT_CLASS_SLEEP );
						gcAddEffect.setDuration( output.Duration );
						pZone->broadcastPacket( tileX, tileY, &gcAddEffect );

						bHit = true;

						// 성향을 올린다.
						// 타겟이 슬레이어이면 먼가 문제가 있다.
						if ( !pTargetCreature->isSlayer() )
						{
						//	increaseAlignment( pSlayer, pTargetCreature, &_GCSkillToSelfOK1 );
							if ( maxEnemyLevel < pTargetCreature->getLevel() ) maxEnemyLevel = pTargetCreature->getLevel();
							EnemyNum++;
						}
					}
				}
			}

			// 경험치를 올린다.
			if( bHit )
			{
				SkillGrade Grade = g_pSkillInfoManager->getGradeByDomainLevel(pSlayer->getSkillDomainLevel(DomainType));
				Exp_t ExpUp = 10* (Grade + 1);
				shareAttrExp(pSlayer, ExpUp, 1, 1, 8, _GCSkillToSelfOK1);
				increaseDomainExp(pSlayer, DomainType, pSkillInfo->getPoint(), _GCSkillToSelfOK1, maxEnemyLevel, EnemyNum);
				increaseSkillExp(pSlayer, DomainType, pSkillSlot, pSkillInfo, _GCSkillToSelfOK1);
			}

			_GCSkillToSelfOK1.setSkillType(SkillType);
			_GCSkillToSelfOK1.setCEffectID(CEffectID);
			_GCSkillToSelfOK1.setDuration(0);
		
			_GCSkillToSelfOK2.setObjectID(pSlayer->getObjectID());
			_GCSkillToSelfOK2.setSkillType(SkillType);
			_GCSkillToSelfOK2.setDuration(0);
		
			pPlayer->sendPacket(&_GCSkillToSelfOK1);
			pZone->broadcastPacket(pSlayer->getX(), pSlayer->getY(),  &_GCSkillToSelfOK2, pSlayer);

			pSkillSlot->setRunTime(output.Delay);
		} 
		else 
		{
			executeSkillFailNormal(pSlayer, getSkillType(), NULL);
		}
	} 
	catch (Throwable & t) 
	{
		executeSkillFailException(pSlayer, getSkillType());
	}

	//cout << "TID[" << Thread::self() << "]" << getSkillHandlerName() << " End(slayer)" << endl;

	__END_CATCH
}
*/
VoodooRing g_VoodooRing;