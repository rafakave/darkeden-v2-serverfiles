//////////////////////////////////////////////////////////////////////////////
// Filename    : CGAddItemToItemHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//////////////////////////////////////////////////////////////////////////////
/*

// 한 아이템을 들어서 다른 아이템에 넣는다.
 [Client]                                               [Server]
           -- CGAddItemToItem
              (mouseItem_ObjectID, invenX, invenY) 
			  =(마우스에 들고있는 별, upgrade할 item XY) -->

            <-- GCAddItemToItemVerify(결과코드, 관련값) ---
   
//-------------+---------------------------------------------
// 사용 packet | client 처리
//-------------+---------------------------------------------
// 불가 packet | 불가 message 출력
// 실패 packet | 실패 message 출력, 별 삭제, [아이템 제거]
// 성공 packet | 별 삭제, option변경
//-------------+---------------------------------------------
*/


#include "CGAddItemToItem.h"

#ifdef __GAME_SERVER__
	#include <stdio.h>
	#include "GamePlayer.h"
	#include "ZoneUtil.h"
	#include "PlayerCreature.h"
	#include "Inventory.h"
	#include "Item.h"
	#include "ItemInfoManager.h"
#include "ItemFactoryManager.h"
	#include "OptionInfo.h"
	#include "ItemUtil.h"
	#include "EventStar.h"
	#include "SlayerTuningItem.h"
	#include "VampireTuningItem.h"
	#include "OustersTuningItem.h"
	#include "PetItem.h"
	#include "PetEnchantItem.h"
	#include "PetInfo.h"
	#include "PetAttrInfo.h"
	#include "PetTypeInfo.h"
	#include "CreatureUtil.h"
	#include "MixingItem.h"
	#include "PacketUtil.h"
	#include "Zone.h"
	#include "CrystalOptionLoader.h"

	#include "Gpackets/GCAddItemToItemVerify.h"
	#include "Gpackets/GCDeleteInventoryItem.h"
	#include "Gpackets/GCCreateItem.h"
	#include	"Gpackets/GCUpdateInfo.h"

	#include "SystemAvailabilitiesManager.h"

	void executeUpGrade(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem);
	void executeTypeGrade(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem);
	void executeEnchantOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY);
	void executeEnchantRareOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY);
	void executeEnchantRareThreeOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY);
	void executeEnchantSetOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY);
	void executeEnchantSetRareOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY);
	void executeEnchantSetThreeOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY);
	void executeAddOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, OptionType_t addOptionType);
	void executeAddCoralOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, int addOptionType);
	void executeAddSetOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, OptionType_t addOptionType);
	void executeTransKit(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem);
	void executeRedSunCore(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem);
	void executeVampireTuningItem(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem);
	void executeOustersTuningItem(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem);
	extern bool isPossibleOptionItemClass(Item::ItemClass IClass);

	OptionInfo* getOptionInfo(OptionType_t optionType);

	void    	processUpgradeOptionType(Item* pItem, OptionType_t currentOptionType, OptionInfo* pOptionInfo);
	void    	processUpgradeSetOptionType(Item* pItem, OptionType_t currentOptionType, OptionInfo* pOptionInfo);
	void		crashItem(Item* pItem, Inventory* pInventory, CoordInven_t invenX, CoordInven_t invenY);
	void    	downgradeOptionType(Item* pItem, OptionType_t currentOptionType, OptionInfo* pOptionInfo);
	void    	downgradeSetOptionType(Item* pItem, OptionType_t currentOptionType, OptionInfo* pOptionInfo);
	void		sendEnchantImpossible(GamePlayer* pGamePlayer);
	void		sendEnchantOK(GamePlayer* pGamePlayer, DWORD optionChange);
	void		sendSetEnchantOK(GamePlayer* pGamePlayer, DWORD optionChange);
	// add by sonic 녑힛橄昑럿쀼돨룐관
	void		sendEnchantOK(GamePlayer* pGamePlayer, DWORD optionChange, DWORD optionCHange2);

	// PET 조합 인챈트 아이템의 결과다.
	void    	processUpgradePetOptionType(PetInfo* pPetInfo, Item* pItem, OptionType_t currentOptionType, OptionInfo* pOptionInfo);
	void    	downgradePetOptionType(PetInfo* pPetInfo, Item* pItem, OptionType_t currentOptionType, OptionInfo* pOptionInfo);
#endif	// __GAME_SERVER__

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void CGAddItemToItemHandler::execute (CGAddItemToItem* pPacket , Player* pPlayer)
	 
{
	__BEGIN_TRY __BEGIN_DEBUG_EX
		
#ifdef __GAME_SERVER__

	Assert(pPacket != NULL);
	Assert(pPlayer != NULL);

	//cout << pPacket->toString().c_str() << endl;

	GamePlayer* pGamePlayer = dynamic_cast<GamePlayer*>(pPlayer);

	Assert(pGamePlayer != NULL);	// by sigi

	if (pGamePlayer->getPlayerStatus() == GPS_NORMAL) 
	{
		SYSTEM_ASSERT( SYSTEM_ENCHANT );

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);
		ObjectID_t      objectID    = pPacket->getObjectID();
		Inventory* 		pInventory  = pPC->getInventory();
		CoordInven_t 	invenX      = pPacket->getX();
		CoordInven_t 	invenY      = pPacket->getY();

		// 인벤토리에 아이템이 없을때(좌표체크도 하게 된다.)
		if (!pInventory->hasItem(invenX, invenY))
		{
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );
			return;
		}

		// 마우스 아이템
		Item*           pMouseItem  = pPC->getExtraInventorySlotItem();

		// 인벤토리에 있는 아?謙?
		Item*      		pItem       = pInventory->getItem(invenX, invenY);

		// 아이템이 없거나
		// objectID가 잘못된 경우
		if (pMouseItem==NULL
			|| pItem==NULL
			|| pMouseItem->getObjectID()!=objectID
			|| pMouseItem->getNum() != 1 )
		{
			//cout << "Wrong Item" << endl;

			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );
			return;
		}


		switch (pMouseItem->getItemClass())
		{
			//---------------------------------------------------------
			//
			// 				ITEM_CLASS_EVENT_STAR
			//
			//---------------------------------------------------------
			// 아이템의 옵션을 upgrade 시킨다.
			//---------------------------------------------------------
			case Item::ITEM_CLASS_EVENT_STAR : 
			{
				ItemInfo* pItemInfo = g_pEventStarInfoManager->getItemInfo( pMouseItem->getItemType() );
				Assert(pItemInfo!=NULL);

				EventStarInfo* pEventStarInfo = dynamic_cast<EventStarInfo*>(pItemInfo);
				Assert(pEventStarInfo!=NULL);

				int optionSize = pItem->getOptionTypeSize();
				int optionSize2 = pItem->getOptionTypeSize2();

				if (pEventStarInfo->getFunctionFlag() == 21)
				{
					if(optionSize == 1){
					executeEnchantOption(pGamePlayer, pMouseItem, pItem,  invenX, invenY);
					}else if(optionSize == 2){
					executeEnchantRareOption(pGamePlayer, pMouseItem, pItem, invenX, invenY);
					}else if(optionSize == 3){
					executeEnchantRareThreeOption(pGamePlayer, pMouseItem, pItem, invenX, invenY);
					}
				}
				else if (pEventStarInfo->getFunctionFlag() == 20)
				{
					executeRedSunCore(pGamePlayer, pMouseItem, pItem);
				}else if (pEventStarInfo->getFunctionFlag() == 17)
				{
					int addOptionType = pEventStarInfo->getFunctionValue();
					executeAddSetOption(pGamePlayer, pMouseItem, pItem, addOptionType);
				}
				else if (pEventStarInfo->getFunctionFlag() == 15)
				{
					executeEnchantSetThreeOption(pGamePlayer, pMouseItem, pItem, invenX, invenY);
				}
				else if (pEventStarInfo->getFunctionFlag() == 14)
				{
					executeEnchantSetRareOption(pGamePlayer, pMouseItem, pItem, invenX, invenY);
				}
				else if (pEventStarInfo->getFunctionFlag() == 13)
				{
					executeEnchantSetOption(pGamePlayer, pMouseItem, pItem, invenX, invenY);
				}
				else if (pEventStarInfo->getFunctionFlag() == 12)
				{
					executeTypeGrade(pGamePlayer, pMouseItem, pItem);
				}
				else if (pEventStarInfo->getFunctionFlag() == 11)
				{
					int addOptionType = pEventStarInfo->getFunctionValue();
					executeAddCoralOption(pGamePlayer, pMouseItem, pItem, addOptionType);
				}
				//�薨뚱갚링횝�
				else if ( pEventStarInfo->getFunctionFlag() == 16 )
				{
					executeUpGrade(pGamePlayer, pMouseItem, pItem);
				}
				else if (pEventStarInfo->isFunctionTransKit())
				{
					executeTransKit(pGamePlayer, pMouseItem, pItem);
				}
				// Enchant Option
				else if (optionSize>1)	// by sigi. 2002.10.21
				{
					if (pEventStarInfo->isFunctionEnchantRareOption())
					{
						if(pMouseItem->getItemType()==6 || pMouseItem->getItemType()==47 || pMouseItem->getItemType()==48 || pMouseItem->getItemType()==49)// 녑힛橄昑陋구
							executeEnchantRareThreeOption(pGamePlayer, pMouseItem, pItem, invenX, invenY);
						else
							executeEnchantRareOption(pGamePlayer, pMouseItem, pItem, invenX, invenY);
					}
					else
					{
						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
						return;
					}
				}
				else if (pEventStarInfo->isFunctionEnchantOption())
				{
					executeEnchantOption(pGamePlayer, pMouseItem, pItem, invenX, invenY);
				}
				// Add Option
				else if (pEventStarInfo->isFunctionAddOption())
				{
					int addOptionType = pEventStarInfo->getFunctionValue();
					executeAddOption(pGamePlayer, pMouseItem, pItem, addOptionType);
				}
				// Coral Option
			}
			break;

			case Item::ITEM_CLASS_PET_ENCHANT_ITEM :
			{
				//cout << "Trying Pet Enchant... " << endl;
				if ( pItem->getItemClass() != Item::ITEM_CLASS_PET_ITEM )
				{
					//cout << "펫 아이템에다가 한게 아님다." << endl;
					GCAddItemToItemVerify gcAddItemToItemVerify;
					gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
					pGamePlayer->sendPacket( &gcAddItemToItemVerify );
					return;
				}
				//ItemInfo* pItemInfo2 = g_pPetEnchantItemInfoManager->getItemInfo( pMouseItem->getItemType() );
				//Assert( pItemInfo2 != NULL );

				PetEnchantItemInfo* pItemInfo = dynamic_cast<PetEnchantItemInfo*>(g_pPetEnchantItemInfoManager->getItemInfo( pMouseItem->getItemType() ));
				Assert( pItemInfo != NULL );

				PetItem* pPetItem = dynamic_cast<PetItem*>(pItem);
				PetInfo* pPetInfo = NULL;
				if ( pPetItem == NULL || ( pPetInfo = pPetItem->getPetInfo() ) == NULL )
				{
					//cout << "펫 아이템이 ?틈構킬?펫 아이템에 펫 상태가 없습니다." << endl;
					GCAddItemToItemVerify gcAddItemToItemVerify;
					gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
					pGamePlayer->sendPacket( &gcAddItemToItemVerify );
					return;
				}

				/*
					ENCHANT_FUNCTION = 0,
					SPEC_OPTION_FUNCTION = 1,
					REVIVAL_FUNCTION = 13,
					RARE_FUNCTION = 14
				*/
				switch ( pItemInfo->getFunction() )
				{
					case PetEnchantItemInfo::ENCHANT_FUNCTION :
					{
						if ( pPetInfo->getPetAttr() != 0xff )
						{
							//cout << "이미 인챈트된 아이템입니다." << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}
						//cout << "표준 인챈트 시도중입니다." << endl;
						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						if ( PetAttrInfoManager::Instance().enchantRandomAttr( pPetInfo, pItemInfo->getFunctionGrade() ) )
						{
							//cout << "인챈??성공." << endl;
							pPetItem->savePetInfo();

							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
							gcAddItemToItemVerify.setParameter( ((uint)pPetInfo->getPetAttr())<<16 | (uint)pPetInfo->getPetAttrLevel() );

							pGamePlayer->sendPacket( &gcAddItemToItemVerify );

							if ( pPC->getPetInfo() == pPetInfo )
							{
								//cout << "이미 불러져있어서 패킷 보내는 중입니다." << endl;
								sendPetInfo( pGamePlayer, true );
								pPC->initAllStatAndSend();
							}
							return;
						}
						else
						{
							//cout << "인챈트 실패" << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}
					}
					break;
					case PetEnchantItemInfo::SPEC_OPTION_FUNCTION :
					{
						if ( pPetInfo->getPetAttr() != 0xff )
						{
							//cout << "이미 인챈트된 아이템입니다." << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}
						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						if ( PetAttrInfoManager::Instance().enchantSpecAttr( pPetInfo, (PetAttr_t)pItemInfo->getFunctionGrade() ) )
						{
							pPetItem->savePetInfo();
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
							gcAddItemToItemVerify.setParameter( ((uint)pPetInfo->getPetAttr())<<16 | (uint)pPetInfo->getPetAttrLevel() );
//							gcAddItemToItemVerify.setParameter( pPetInfo->getPetAttr() );

							pGamePlayer->sendPacket( &gcAddItemToItemVerify );

							if ( pPC->getPetInfo() == pPetInfo )
							{
								sendPetInfo( pGamePlayer, true );
								pPC->initAllStatAndSend();
							}
							return;
						}
						else
						{
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}
					}
					break;
					case PetEnchantItemInfo::PET_AMPUL_FUNCTION:
					{
						//cout << "펫 레어 인챈트 중?都求?" << endl;
						if ( pPetInfo->getPetLevel() < 49 || pPetInfo->getPetVersionOption() != 0)
						{
							//cout << "레벨도 안된 넘 인챈트하지 마셈" << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}

						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						/*
						OptionType_t targetOption = rand()%87;
						targetOption++;
						*/

						OptionType_t targetOption = 0;
						int ratio = rand() % 100000;
						const list<PetEnchantOption*>& petEnchantOptions = g_pOptionInfoManager->getPetEnchantOptionList();
						list<PetEnchantOption*>::const_iterator itr = petEnchantOptions.begin();
						list<PetEnchantOption*>::const_iterator endItr = petEnchantOptions.end();

						for ( ; itr != endItr ; itr++ )
						{
							PetEnchantOption* pPetEnchantOption = *itr;
							if ( pPetEnchantOption->getRatio() > ratio )
							{
								targetOption = pPetEnchantOption->getOptionType();
								break;
							}
						}

						pPetInfo->setPetVersionOption( targetOption );
						sendPetInfo( pGamePlayer );

							list<OptionType_t> olist;

							if ( pPetInfo->getPetOption() != 0 ) 
							olist.push_back(pPetInfo->getPetOption());

							if ( pPetInfo->getPetVersionOption() != 0 ) 
							olist.push_back(pPetInfo->getPetVersionOption());
			
							list<OptionType_t> optionList = pPetInfo->getOptionType();
							list<OptionType_t>::const_iterator itr3 = optionList.begin();

							while(itr3 != optionList.end())
							{
							olist.push_back(*itr3);
							itr3++;
							}

						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_THIRD_ENCHANT_OK );
						gcAddItemToItemVerify.setParameter( 0 );
						gcAddItemToItemVerify.setThirdOptionType( olist );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );

						char query[100];
						sprintf(query, "PetOption2=%u", (uint)targetOption);
						pItem->tinysave(query);
					}
					break;
					case PetEnchantItemInfo::RARE_FUNCTION:
					{
						//cout << "펫 레어 인챈트 중입니다." << endl;
						if ( pPetInfo->getPetLevel() < 49  || pPetInfo->getPetOption() != 0)
						{
							//cout << "레벨도 안된 넘 인챈트하지 마셈" << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}

						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						/*
						OptionType_t targetOption = rand()%87;
						targetOption++;
						*/

						OptionType_t targetOption = 0;
						int ratio = rand() % 100000;
						const list<PetEnchantOption*>& petEnchantOptions = g_pOptionInfoManager->getPetEnchantOptionList();
						list<PetEnchantOption*>::const_iterator itr = petEnchantOptions.begin();
						list<PetEnchantOption*>::const_iterator endItr = petEnchantOptions.end();

						for ( ; itr != endItr ; itr++ )
						{
							PetEnchantOption* pPetEnchantOption = *itr;
							if ( pPetEnchantOption->getRatio() > ratio )
							{
								targetOption = pPetEnchantOption->getOptionType();
								break;
							}
						}

						pPetInfo->setPetOption( targetOption );
						sendPetInfo( pGamePlayer );

							list<OptionType_t> olist;

							if ( pPetInfo->getPetOption() != 0 ) 
							olist.push_back(pPetInfo->getPetOption());

							if ( pPetInfo->getPetVersionOption() != 0 ) 
							olist.push_back(pPetInfo->getPetVersionOption());
			
							list<OptionType_t> optionList = pPetInfo->getOptionType();
							list<OptionType_t>::const_iterator itr3 = optionList.begin();

							while(itr3 != optionList.end())
							{
							olist.push_back(*itr3);
							itr3++;
							}

						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_THIRD_ENCHANT_OK );
						gcAddItemToItemVerify.setParameter( 0 );
						gcAddItemToItemVerify.setThirdOptionType( olist );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );

						char query[100];
						sprintf(query, "PetOption=%u", (uint)targetOption);
						pItem->tinysave(query);
					}
					break;
					case PetEnchantItemInfo::PET_MIXING_CLEANER_FUNCTION:
					{
						if ( pPetInfo->getMixOptionTypeSize() == 0)
						{
							//cout << "레벨도 안된 넘 인챈트하지 마셈" << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}

						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						list<OptionType_t> olist;

						pPetInfo->setMixOptionType( olist );
						sendPetInfo( pGamePlayer );

						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_THIRD_ENCHANT_OK );
						gcAddItemToItemVerify.setParameter( 0 );
						gcAddItemToItemVerify.setThirdOptionType( olist );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );

						char query[100];
						sprintf(query, "OptionType2=%s", NULL);
						pItem->tinysave(query);
					break;
					}
					case PetEnchantItemInfo::PET_DIET_FUNCTION:
					{
						if(pPetInfo->getMixOptionTypeSize() == 0 || pMouseItem->getNum() != 1){
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}

						bool bFirstSucceed = false;
						uint optionChange = 0;
						uint optionChange2 = 0;
						int  shiftValue = 16;
						int  diceValue = 3500;

						list<OptionType_t> optionList = pPetInfo->getMixOptionType();
						list<OptionType_t>::const_iterator itr = optionList.begin();
		
						OptionType_t thirdOption  = *itr;
						itr++;
						OptionType_t secondOption = *itr;
						itr++;
						OptionType_t firstOption  = *itr;
						itr--;
						itr--;

						OptionInfo* pFirstOptionInfo = getOptionInfo( firstOption );
						OptionInfo* pSecondOptionInfo = getOptionInfo( secondOption );
						OptionInfo* pThirdOptionInfo = getOptionInfo( thirdOption );

		//�薨뙈鮎례轍獨泥�
						if ( pFirstOptionInfo == NULL || pSecondOptionInfo == NULL || pThirdOptionInfo== NULL)
						{
							sendEnchantImpossible( pGamePlayer );
							return;
						}
		//쇱꿎角뤠角옵�薨뙈鮎�
						bool bFirstUpgradePossible =  pFirstOptionInfo->isUpgradePossible();
						bool bSecondUpgradePossible = pSecondOptionInfo->isUpgradePossible();
						bool bThirdUpgradePossible =  pThirdOptionInfo->isUpgradePossible();
		//쇱꿎橄昑�薨뗄�콘
						if ( !bFirstUpgradePossible && !bSecondUpgradePossible && !bThirdUpgradePossible)
						{
							sendEnchantImpossible( pGamePlayer );
							return;
							}

						if ( bFirstUpgradePossible )
						{
							// 첫번째 옵션의 인챈트 확률은 블루드??2의 확률에 따른다.
							int succeedRatio = pFirstOptionInfo->getUpgradeSecondRatio() * ( pItemInfo->getUpgradeRatio() +( rand() %50) );
							//succeedRatio += getPercentValue(succeedRatio,pMouseItem->getHeroOption());
							int dice = rand() % diceValue;
							cout << "dice : " << (int)dice <<  endl;
							cout << "diceValue : " << (int)diceValue <<  endl;

							if ( dice < succeedRatio ) //흔벎繫법，橙�薨뚱갚�
							{
								//cout << "인챈트 성공" << endl;
								processUpgradePetOptionType( pPetInfo, pItem, firstOption, pFirstOptionInfo );

								OptionType_t upgradeOptionType = pFirstOptionInfo->getUpgradeType();
								optionChange = (firstOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

								bFirstSucceed = true;
							goto EnOK;
						}
						else
						{
						if (pFirstOptionInfo->getUpgradeSecondRatio() < 5)
						{
							int noChangeRatio = 33;
							int downgradeRatio = 46;
							dice = rand() % 100;

							cout << "downgradeRatio" << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

							if ( dice < noChangeRatio )
							{
							// 변화없음
							}
							else if ( dice < noChangeRatio + downgradeRatio )
							{
							// 옵션떨어짐
							downgradePetOptionType( pPetInfo, pItem, firstOption, pFirstOptionInfo );

							OptionType_t previousOptionType = pFirstOptionInfo->getPreviousType();
							optionChange = (firstOption << (shiftValue+8)) | (previousOptionType << shiftValue);
						}
					}
				}
			}
			shiftValue -= 16;

			if ( bSecondUpgradePossible )
			{
			// 두번째 옵션의 인챈트 확률은 첫번째 옵션의 성공여부에 따른다.

			int succeedRatio= g_pOptionInfoManager->getRareUpgradeRatio( secondOption, bFirstSucceed ) * ( pItemInfo->getUpgradeRatio() +( rand() %50) );
			//succeedRatio += getPercentValue(succeedRatio,pMouseItem->getHeroOption());
			//int dice = rand() %10000;
			int dice = rand() % diceValue;

			//cout << "두번째 옵션 : " << pSecondOptionInfo->getHName() << " 인챈트 확률 " << succeedRatio << endl;
			//dice =succeedRatio-1;
			if ( dice < succeedRatio )
			{
				//cout << "인챈트 성공" << endl;
								processUpgradePetOptionType( pPetInfo, pItem, firstOption, pFirstOptionInfo );

				OptionType_t upgradeOptionType = pSecondOptionInfo->getUpgradeType();
				optionChange |= (secondOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
				goto EnOK;
			}
			else
			{
				if (pSecondOptionInfo->getUpgradeSecondRatio() < 5)
				{
					int noChangeRatio = 33;
					int downgradeRatio = 46;
					dice = rand() % 100;

					cout << "downgradeRatio" << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

					if ( dice < noChangeRatio )
					{
						// 변화없음
					}
					else if ( dice < noChangeRatio + downgradeRatio )
					{
						// 옵션떨어짐
							downgradePetOptionType( pPetInfo, pItem, firstOption, pFirstOptionInfo );

						OptionType_t previousOptionType = pSecondOptionInfo->getPreviousType();
						optionChange = (secondOption << (shiftValue+8)) | (previousOptionType << shiftValue);
					}
				}
			}
		}
			shiftValue=0;
			if ( bThirdUpgradePossible )
			{
			// 두번째 옵션의 인챈트 확률은 첫번째 옵션의 성공여부에 따른다.
			int succeedRatio = g_pOptionInfoManager->getRareUpgradeRatio( thirdOption, bFirstSucceed ) * ( pItemInfo->getUpgradeRatio() +( rand() %50) );
			//succeedRatio += getPercentValue(succeedRatio,pMouseItem->getHeroOption());
			//int dice = rand() %10000;

			int dice = rand() % diceValue;
			//cout << "두번째 옵션 : " << pSecondOptionInfo->getHName() << " 인챈트 확률 " << succeedRatio << endl;
			//dice =succeedRatio-1;
			if ( dice < succeedRatio )
			{
				//cout << "인챈트 성공" << endl;
								processUpgradePetOptionType( pPetInfo, pItem, firstOption, pFirstOptionInfo );

				OptionType_t upgradeOptionType = pThirdOptionInfo->getUpgradeType();
				optionChange2 |= (thirdOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
			}
			else
			{
				if (pThirdOptionInfo->getUpgradeSecondRatio() < 5)
				{
					int noChangeRatio = 33;
					int downgradeRatio = 46;
					dice = rand() % 100;

					cout << "downgradeRatio" << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

					if ( dice < noChangeRatio )
					{
						// 변화없음
					}
					else if ( dice < noChangeRatio + downgradeRatio )
					{
						// 옵션떨어짐
							downgradePetOptionType( pPetInfo, pItem, firstOption, pFirstOptionInfo );

						OptionType_t previousOptionType = pThirdOptionInfo->getPreviousType();
						optionChange2 |= (thirdOption << (shiftValue+8)) | (previousOptionType << shiftValue);
					}
				}
			}
		}

						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );
EnOK:
		
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_THIRD_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( 0 );
			gcAddItemToItemVerify.setThirdOptionType( pPetInfo->getMixOptionType() );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

					}
					break;
					case PetEnchantItemInfo::REVIVAL_FUNCTION:
					{
						//cout << "부활 시도중 입니다."  << endl;
						if ( pPetInfo->getPetHP() != 0 )
						{
							//cout << "안 죽은 애 부활시키지 마셈" << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}

						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						VSDateTime current = VSDateTime::currentDateTime();
						int Days = pPetInfo->getLastFeedTime().daysTo(current);
						if ( Days < 0 ) Days = 0;

						if ( pItemInfo->getItemType() != 14 )
						{
							int value = rand()%100;
							if ( Days >= 8 || value >= (10-Days/2)*10 )
							{
								GCAddItemToItemVerify gcAddItemToItemVerify;
								gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL );
								pGamePlayer->sendPacket( &gcAddItemToItemVerify );
								return;
							}
						}

						pPetInfo->setPetHP( pItemInfo->getFunctionGrade() );
						pPetInfo->setFeedTime( VSDateTime::currentDateTime() );
						pPetInfo->getPetItem()->savePetInfo();

						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_REVIVAL_OK );
						gcAddItemToItemVerify.setParameter( pPetInfo->getPetHP() );

						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
					}
					break;
					case PetEnchantItemInfo::MUTANT_FUNCTION:
					{
						if ( pPetInfo->getPetType() == pItemInfo->getFunctionGrade() )
						{
							cout << "울프독이 아니랍니다." << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}

						list<OptionType_t> optionTypeList;
						Item* pItem2 = g_pItemFactoryManager->createItem( Item::ITEM_CLASS_PET_ITEM, pItemInfo->getFunctionGrade(), optionTypeList );
						Assert( pItem2 != NULL );

						_TPOINT pt;
						if (!pInventory->CheckaddItem(invenX, invenY, pItem2, pItem))
						{
							SAFE_DELETE(pItem2);

							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );		
							return;
						}

						bool summonPet = pPC->getPetInfo() == pPetInfo;

						PetTypeInfo* pPetTypeInfo = PetTypeInfoManager::getInstance()->getPetTypeInfo(pItemInfo->getFunctionGrade());
						Assert( pPetTypeInfo != NULL );

						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
						gcAddItemToItemVerify.setParameter( pItemInfo->getFunctionGrade() );

						pGamePlayer->sendPacket( &gcAddItemToItemVerify );

						// 클라이언트에 아이템을 삭제시킨다.
						GCDeleteInventoryItem gcDeleteInventoryItem;
						gcDeleteInventoryItem.setObjectID(pItem->getObjectID());
						pGamePlayer->sendPacket(&gcDeleteInventoryItem);

						// 서버 자체에서도 없애준다. 
						//nventory->deleteItem(pItem->getObjectID());
				//_TPOINT pt2;
						//CoordInven_t X2 = pt2.x;
						//CoordInven_t Y2 = pt2.y;
						pInventory->deleteItem(pItem->getObjectID());	
						cout << " 요기서 문제 " << endl;
						// 안전하게 아예 없앤다.
						//pItem->destroy();
						//pPetItem->whenPCLost(pPC);
						pItem->destroy();
						cout << " 요기서 문제2 " << endl;
						//pPetItem->destroy();
						//SAFE_DELETE( pItem );
						//SAFE_DELETE( pPetItem );
						//SAFE_DELETE( pPetItem );
						cout << " 요기서 문제2 " << endl;

						PetInfo* pPetInfo2 = new PetInfo;
						pPetInfo2->setPetType(pItemInfo->getFunctionGrade());
						pPetInfo2->setPetCreatureType( pPetTypeInfo->getPetCreatureType( pPetInfo->getPetLevel() ) );
						pPetInfo2->setPetLevel(pPetInfo->getPetLevel());
						pPetInfo2->setPetExp(pPetInfo->getPetExp());
						pPetInfo2->setPetHP(pPetInfo->getPetHP());
						pPetInfo2->setPetAttr(pPetInfo->getPetAttr());
						pPetInfo2->setPetAttrLevel(pPetInfo->getPetAttrLevel());
						pPetInfo2->setPetOption(pPetInfo->getPetOption());
						pPetInfo2->setPetVersionOption(pPetInfo->getPetVersionOption());
						pPetInfo2->setFoodType(pPetInfo->getFoodType());
						pPetInfo2->setGamble(pPetInfo->canGamble());
						pPetInfo2->setCutHead(pPetInfo->canCutHead());
						pPetInfo2->setAttack(pPetInfo->canAttack());
						pPetInfo2->setFeedTime( VSDateTime::currentDateTime() );
						pPetInfo2->setMagicRemainSec( pPetInfo->getMagicRemainSec() );
						pPetInfo2->setExpertPetChangerRemainSec( pPetInfo->getExpertPetChangerRemainSec());
						pPetInfo2->setEnchantSkillType( pPetInfo->getEnchantSkillType() );

						//list<OptionType_t> olist3;
						cout << " 옵션 체크 " << endl;
						if(pMouseItem->getOptionTypeSize() != 0){

						cout << " 옵션 있음 " << endl;

						list<OptionType_t> oList;

						list<OptionType_t> optionList2 = pMouseItem->getOptionTypeList();
						list<OptionType_t>::const_iterator itr2 = optionList2.begin();
						while(itr2 != optionList2.end())
						{
							oList.push_back(*itr2);
							itr2++;
						}
						pPetInfo2->setOptionType( oList );
						}else{
						cout << " 옵션 체크 2" << endl;
						pPetInfo2->setOptionType( pPetInfo->getOptionType() );
						}

						pPetInfo2->setMixOptionType( pPetInfo->getMixOptionType() );
						PetItem* pPetItem2 = dynamic_cast<PetItem*>(pItem2);
						if ( pPetItem2 != NULL )
						{
						pPetItem2->setPetInfo( pPetInfo2 );
						pPetInfo2->setPetItem( pPetItem2 );
						}

						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

	// 이부분은 새로 지급하는 아이템에 사용한다.
						CoordInven_t X = pt.x;
						CoordInven_t Y = pt.y;

						pPC->getZone()->getObjectRegistry().registerObject( pItem2 );

						if(pInventory->addItem(invenX, invenY, pItem2))
						{
						pItem2->create( pPC->getName(), STORAGE_INVENTORY, 0, invenX, invenY );

	// 클라이언트에 선물이 추가되었음을 알린다.
						GCCreateItem gcCreateItem;
						makeGCCreateItem( &gcCreateItem, pItem2, invenX, invenY );
						pPlayer->sendPacket(&gcCreateItem);

						}
						if ( summonPet )
						{
							pPC->setPetInfo( pPetInfo2 );
							sendPetInfo( pGamePlayer );
						}

			//transportCreature(pCreature, pCreature->getZone()->getZoneID(), pCreature->getX(), pCreature->getY(), false);

					}
					break;
					case PetEnchantItemInfo::PET_RED_FUNCTION:
					{
						//cout << "펫 레어 인챈트 중입니다." << endl;
						if ( pPetInfo->getPetLevel() < 49 )
						{
							//cout << "레벨도 안된 넘 인챈트하지 마셈" << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}

						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						/*
						OptionType_t targetOption = rand()%87;
						targetOption++;
						*/

						OptionType_t targetOption = 0;
						int ratio = rand() % 100000;
						const list<PetEnchantOption*>& petEnchantOptions = g_pOptionInfoManager->getPetEnchantOptionList();
						list<PetEnchantOption*>::const_iterator itr = petEnchantOptions.begin();
						list<PetEnchantOption*>::const_iterator endItr = petEnchantOptions.end();

						for ( ; itr != endItr ; itr++ )
						{
							PetEnchantOption* pPetEnchantOption = *itr;
							if ( pPetEnchantOption->getRatio() > ratio )
							{
								targetOption = pPetEnchantOption->getOptionType();
								break;
							}
						}

						pPetInfo->setPetOption( targetOption );
						sendPetInfo( pGamePlayer );

							list<OptionType_t> olist;

							if ( pPetInfo->getPetOption() != 0 ) 
							olist.push_back(pPetInfo->getPetOption());

							if ( pPetInfo->getPetVersionOption() != 0 ) 
							olist.push_back(pPetInfo->getPetVersionOption());
			
							list<OptionType_t> optionList = pPetInfo->getOptionType();
							list<OptionType_t>::const_iterator itr3 = optionList.begin();

							while(itr3 != optionList.end())
							{
							olist.push_back(*itr3);
							itr3++;
							}

						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_THIRD_ENCHANT_OK );
						gcAddItemToItemVerify.setParameter( 0 );
						gcAddItemToItemVerify.setThirdOptionType( olist );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );

						char query[100];
						sprintf(query, "PetOption=%u", (uint)targetOption);
						pItem->tinysave(query);
					}
					break;
					case PetEnchantItemInfo::PET_ATTRCLEAR_FUNCTION :
					{
						if ( pPetInfo->getPetAttr() == 0xff )
						{
							//cout << "이미 인챈트된 아이템입니다." << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}
						//cout << "표준 인챈트 시도중입니다." << endl;
						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						if ( PetAttrInfoManager::Instance().enchantClearAttr( pPetInfo ) )
						{
							//cout << "인챈트 성공." << endl;
							pPetItem->savePetInfo();

							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
							gcAddItemToItemVerify.setParameter( ((uint)pPetInfo->getPetAttr())<<16 | (uint)pPetInfo->getPetAttrLevel() );

							pGamePlayer->sendPacket( &gcAddItemToItemVerify );

							if ( pPC->getPetInfo() == pPetInfo )
							{
								//cout << "이미 ?念??羚底?패킷 보내는 중입니다." << endl;
								sendPetInfo( pGamePlayer, true );
								pPC->initAllStatAndSend();
							}
							return;
						}
						else
						{
							//cout << "인챈트 실패" << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}
					}
					break;
					case PetEnchantItemInfo::PET_MAGIC_FUNCTION:
					{
						//cout << "부활 시도중 입니다."  << endl;
						if ( pPetInfo->getMagicRemainSec() != 0 )
						{
							//cout << "안 죽은 애 부활시키지 마셈" << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}

						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						VSDateTime current = VSDateTime::currentDateTime().addSecs(pItemInfo->getFunctionGrade());
						char query[100];
						sprintf(query, "MagicExpireTime='%s'", current.toDateTime().c_str());
						pItem->tinysave(query);

						pPetInfo->setMagicRemainSec( pItemInfo->getFunctionGrade() );
						pPetInfo->getPetItem()->savePetInfo();

						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
						gcAddItemToItemVerify.setParameter( pPetInfo->getMagicRemainSec() );

						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
					}
					break;
					case PetEnchantItemInfo::PET_EXPERT_FUNCTION:
					{
						//cout << "부활 시도중 입니다."  << endl;
						if ( pPetInfo->getExpertPetChangerRemainSec() != 0 )
						{
							//cout << "안 죽은 애 부활시키지 마셈" << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}

						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						VSDateTime current = VSDateTime::currentDateTime().addSecs(pItemInfo->getFunctionGrade());
						char query[100];
						sprintf(query, "ExpertExpireTime='%s'", current.toDateTime().c_str());
						pItem->tinysave(query);

						pPetInfo->setExpertPetChangerRemainSec( pItemInfo->getFunctionGrade() );
						pPetInfo->getPetItem()->savePetInfo();

						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
						gcAddItemToItemVerify.setParameter( pPetInfo->getExpertPetChangerRemainSec() );

						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
					}
					break;
					case PetEnchantItemInfo::PET_SHOCK_FUNCTION:
					{
						//cout << "부활 시도중 입니다."  << endl;
						if ( pPetInfo->getEnchantSkillType() != 0 )
						{
							//cout << "안 죽은 애 부활시키지 마셈" << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}

						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						char query[100];
						sprintf(query, "EnchantSkillType=%d", pItemInfo->getFunctionGrade());
						pItem->tinysave(query);

						pPetInfo->setEnchantSkillType( pItemInfo->getFunctionGrade() );
						pPetInfo->getPetItem()->savePetInfo();

						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
						gcAddItemToItemVerify.setParameter( pPetInfo->getEnchantSkillType() );

						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
					}
					break;
					case PetEnchantItemInfo::PET_ICE_FUNCTION:
					{
						//cout << "부활 시도중 입니다."  << endl;
						if ( pPetInfo->getEnchantSkillType() == 0 )
						{
							//cout << "안 죽은 애 부활시키지 마셈" << endl;
							GCAddItemToItemVerify gcAddItemToItemVerify;
							gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
							pGamePlayer->sendPacket( &gcAddItemToItemVerify );
							return;
						}

						pPC->deleteItemFromExtraInventorySlot();
						pMouseItem->destroy();
						SAFE_DELETE( pMouseItem );

						char query[100];
						sprintf(query, "EnchantSkillType=%d", pItemInfo->getFunctionGrade());
						pItem->tinysave(query);

						pPetInfo->setEnchantSkillType( pItemInfo->getFunctionGrade() );
						pPetInfo->getPetItem()->savePetInfo();

						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
						gcAddItemToItemVerify.setParameter( pPetInfo->getEnchantSkillType() );

						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
					}
					break;
					default:
					{
						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
						return;
					}
				}
			}
			break;
			case Item::ITEM_CLASS_MIXING_ITEM :
			{
				if ( pItem->getItemClass() != Item::ITEM_CLASS_PET_ITEM )
				{
					//cout << "펫 아이템에다가 한게 아님다." << endl;
					GCAddItemToItemVerify gcAddItemToItemVerify;
					gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
					pGamePlayer->sendPacket( &gcAddItemToItemVerify );
					return;
				}

				MixingItemInfo* pItemInfo = dynamic_cast<MixingItemInfo*>(g_pMixingItemInfoManager->getItemInfo( pMouseItem->getItemType() ));
				Assert( pItemInfo != NULL );

				if ( pItemInfo->getType() != MixingItemInfo::TYPE_DETACH )
				{
					//cout << "펫 아이템에다가 한게 아님다." << endl;
					GCAddItemToItemVerify gcAddItemToItemVerify;
					gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
					pGamePlayer->sendPacket( &gcAddItemToItemVerify );
					return;
				}

				PetItem* pPetItem = dynamic_cast<PetItem*>(pItem);
				PetInfo* pPetInfo = NULL;
				if ( pPetItem == NULL || ( pPetInfo = pPetItem->getPetInfo() ) == NULL || pPetInfo->getPetOption() == 0 )
				{
					//cout << "펫 아이템이 아니거나 펫 아이템에 펫 상태가 없습니다." << endl;
					GCAddItemToItemVerify gcAddItemToItemVerify;
					gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
					pGamePlayer->sendPacket( &gcAddItemToItemVerify );
					return;
				}

				pPC->deleteItemFromExtraInventorySlot();
				pMouseItem->destroy();
				SAFE_DELETE( pMouseItem );

				pPetInfo->setPetOption(0);
				pPetItem->tinysave("PetOption=0");

				GCAddItemToItemVerify gcAddItemToItemVerify;
				gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
				gcAddItemToItemVerify.setParameter(0);

				pGamePlayer->sendPacket( &gcAddItemToItemVerify );

				if ( pPC->getPetInfo() == pPetInfo )
				{
					//cout << "이미 불러져있어서 패킷 보내는 중입니다." << endl;
					sendPetInfo( pGamePlayer, true );
					pPC->initAllStatAndSend();
				}
				return;
			}
			break;

			case Item::ITEM_CLASS_SLAYER_TUNING_ITEM :
			{
				ItemInfo* pItemInfo = g_pSlayerTuningItemInfoManager->getItemInfo( pMouseItem->getItemType() );
				Assert(pItemInfo!=NULL);

				SlayerTuningItemInfo* pSlayerTuningItemInfo = dynamic_cast<SlayerTuningItemInfo*>(pItemInfo);
				Assert(pSlayerTuningItemInfo != NULL);

					// 같은 클래스가 아니다. 그러면 에러 메시지를 보내줘야한다.
					if(pSlayerTuningItemInfo->getTunningItemClass() != pItem->getItemClass()){
						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
						return;
					}
					// 같은 타입이면 에러메시지를 보내줘야 한다.
					else if(pSlayerTuningItemInfo->getTunningItemType() == pItem->getItemType()){
						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
						return;
					}else{
						executeRedSunCore(pGamePlayer, pMouseItem, pItem);
					}
			} 
			break;

			case Item::ITEM_CLASS_VAMPIRE_TUNING_ITEM :
			{
				ItemInfo* pItemInfo = g_pVampireTuningItemInfoManager->getItemInfo( pMouseItem->getItemType() );
				Assert(pItemInfo!=NULL);

				VampireTuningItemInfo* pCreateItemInfo = dynamic_cast<VampireTuningItemInfo*>(pItemInfo);
				Assert(pCreateItemInfo != NULL);

					// 같은 클래스가 아니다. 그러면 에러 메시지를 보내줘야한다.
					if(pCreateItemInfo->getTunningItemClass() != pItem->getItemClass()){
						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
						return;
					}
					// 같은 타입이면 에러메시지를 보내줘야 한다.
					else if(pCreateItemInfo->getTunningItemType() == pItem->getItemType()){
						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
						return;
					}else{
						executeVampireTuningItem(pGamePlayer, pMouseItem, pItem);
					}
			} 
			break;

			case Item::ITEM_CLASS_OUSTERS_TUNING_ITEM :
			{
				ItemInfo* pItemInfo = g_pOustersTuningItemInfoManager->getItemInfo( pMouseItem->getItemType() );
				Assert(pItemInfo!=NULL);

				OustersTuningItemInfo* pCreateItemInfo = dynamic_cast<OustersTuningItemInfo*>(pItemInfo);
				Assert(pCreateItemInfo != NULL);

					// 같은 클래스가 아니다. 그러면 에러 메시지를 보내줘야한다.
					if(pCreateItemInfo->getTunningItemClass() != pItem->getItemClass()){
						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
						return;
					}
					// 같은 타입이면 에러메시지를 보내줘야 한다.
					else if(pCreateItemInfo->getTunningItemType() == pItem->getItemType()){
						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );
						return;
					}else{
						executeOustersTuningItem(pGamePlayer, pMouseItem, pItem);
					}
			} 
			break;

			default:
			{
				//cout << "upgrade error" << endl;

				GCAddItemToItemVerify gcAddItemToItemVerify;
				gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ERROR );
				pGamePlayer->sendPacket( &gcAddItemToItemVerify );
				return;
			}
		}
	}

#endif	// __GAME_SERVER__
		
	__END_DEBUG_EX __END_CATCH
}


#ifdef __GAME_SERVER__
	void executeUpGrade(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem)
		
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);
		Inventory* 		pInventory  = pPC->getInventory();

		const ItemInfo* pItemInfo = g_pItemInfoManager->getItemInfo( pItem->getItemClass(), pItem->getItemType() ); 

		EventStarInfo* pEventStarInfo = dynamic_cast<EventStarInfo*>(g_pItemInfoManager->getItemInfo(pMouseItem->getItemClass(), pMouseItem->getItemType()));
		Assert(pEventStarInfo!=NULL);

		int Ratio = Random(30,50);
		if(pItem->getGrade() == 1) { Ratio = 101; }
		Ratio += pEventStarInfo->getFunctionValue();
		int RandomRatio = Random(1,100);
		if ( pMouseItem->getNum() == 1 && pItem->getGrade() < 10 && RandomRatio < Ratio-pItem->getGrade() )
		{
		cout << " 확률 : " <<  RandomRatio << "  / " << Ratio << endl;
			pItem->setGrade( pItem->getGrade() + 1 );
			char buffer[80];
			snprintf(buffer, 80, "Grade=%u", pItem->getGrade() );
			pItem->tinysave(buffer);

			pPC->deleteItemFromExtraInventorySlot();
			pMouseItem->destroy();
			SAFE_DELETE(pMouseItem);

			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_UP_GRADE_OK );
			gcAddItemToItemVerify.setParameter( pItem->getGrade() );

			pGamePlayer->sendPacket( &gcAddItemToItemVerify );
		}
		else
		{
		cout << " 확률 : " <<  RandomRatio << "  / " << Ratio << endl;

			pItem->setGrade( pItem->getGrade() - 1 );
			char buffer[80];
			snprintf(buffer, 80, "Grade=%u", pItem->getGrade() );
			pItem->tinysave(buffer);

			pPC->deleteItemFromExtraInventorySlot();
			pMouseItem->destroy();
			SAFE_DELETE(pMouseItem);

			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_UP_GRADE_OK );
			gcAddItemToItemVerify.setParameter( pItem->getGrade() );

			pGamePlayer->sendPacket( &gcAddItemToItemVerify );
			return;
		}

		__END_CATCH
	}
	void executeTypeGrade(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem)
		
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);
		Inventory* 		pInventory  = pPC->getInventory();

		const ItemInfo* pItemInfo = g_pItemInfoManager->getItemInfo( pItem->getItemClass(), pItem->getItemType() ); 

		int Ratio = 20+pMouseItem->getHeroOption();
	if( pItemInfo->getNextItemType() != pItem->getItemType() && pItemInfo->getNextItemType() != 0){
		if ( pMouseItem->getNum() == 1 && pItem->getGrade() == 10 && rand()%100 < Ratio)
		{
			pItem->setItemType( pItemInfo->getNextItemType() );
			pItem->setGrade( 1 );
			char buffer[80];
			snprintf(buffer, 80, "ItemType=%u, Grade=%u", pItem->getItemType(),pItem->getGrade() );
			pItem->tinysave(buffer);

			pPC->deleteItemFromExtraInventorySlot();
			pMouseItem->destroy();
			SAFE_DELETE(pMouseItem);

			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( 0 );

			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			// 클라이언트에 변동 사항 보내기

			GCDeleteInventoryItem gcDeleteInventoryItem;
			gcDeleteInventoryItem.setObjectID(pItem->getObjectID());
			pGamePlayer->sendPacket(&gcDeleteInventoryItem);

						CoordInven_t X = -1, Y;
						pPC->getInventory()->findItemOID( pItem->getObjectID(), X, Y );
						Assert( X != -1 );

			GCCreateItem gcCreateItem;
			makeGCCreateItem( &gcCreateItem, pItem, X, Y );
			pGamePlayer->sendPacket(&gcCreateItem);
		}
		else
		{

			pPC->deleteItemFromExtraInventorySlot();
			pMouseItem->destroy();
			SAFE_DELETE(pMouseItem);

			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );
			return;
		}
	}else{
		GCAddItemToItemVerify gcAddItemToItemVerify;
		gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
		pGamePlayer->sendPacket( &gcAddItemToItemVerify );
	}
		__END_CATCH
	}
	void executeEnchantRareThreeOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY)
		
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);
//		Inventory* 		pInventory  = pPC->getInventory();

		// 실패할 확률은 itemType에 의해서 결정된다.
		const ItemInfo* pItemInfo = g_pItemInfoManager->getItemInfo( 
													pItem->getItemClass(), 
													pItem->getItemType() );

		EventStarInfo* pEventStarInfo = dynamic_cast<EventStarInfo*>(g_pItemInfoManager->getItemInfo(pMouseItem->getItemClass(), pMouseItem->getItemType()));
		Assert(pEventStarInfo!=NULL);

		// 인챈트 할려는 아이템의 현재 옵션이 2개가 아니거나
		// 유니크 아이템인 경우
		// 혹은 별을 2개 이상 들고 인챈트할라고 할 때
		if (pItem->getOptionTypeSize()!=3
			|| pItemInfo->isUnique()
			|| pMouseItem->getNum() != 1
		)
		{
			//cout << "no Option " << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		bool bFirstSucceed = false;
		uint optionChange = 0;
		uint optionChange2 = 0;
		int  shiftValue = 16;
		int  diceValue = 3500;

		list<OptionType_t> optionList = pItem->getOptionTypeList();
		list<OptionType_t>::const_iterator itr = optionList.begin();

		OptionType_t firstOption = *itr++;
		OptionType_t secondOption = *itr++;
		OptionType_t thirdOption  = *itr;

		OptionInfo* pFirstOptionInfo = getOptionInfo( firstOption );
		OptionInfo* pSecondOptionInfo = getOptionInfo( secondOption );
		OptionInfo* pThirdOptionInfo = getOptionInfo( thirdOption );

		//�薨뙈鮎례轍獨泥�
		if ( pFirstOptionInfo == NULL || pSecondOptionInfo == NULL || pThirdOptionInfo== NULL)
		{
			sendEnchantImpossible( pGamePlayer );
			return;
		}
		//쇱꿎角뤠角옵�薨뙈鮎�
		bool bFirstUpgradePossible =  pFirstOptionInfo->isUpgradePossible();
		bool bSecondUpgradePossible = pSecondOptionInfo->isUpgradePossible();
		bool bThirdUpgradePossible =  pThirdOptionInfo->isUpgradePossible();
				cout << "인챈트 시작 1" << endl;
		//쇱꿎橄昑�薨뗄�콘
		if ( !bFirstUpgradePossible && !bSecondUpgradePossible && !bThirdUpgradePossible)
		{
			sendEnchantImpossible( pGamePlayer );
			return;
		}
		//if ((rand() %6000) > diceValue)
			//diceValue*=2;
				cout << "인챈트 시작 2" << endl;
		if ( bFirstUpgradePossible )
		{
				cout << "인챈트 시작 3/" << (int)firstOption << endl;
			// 첫번째 옵션의 인챈트 확률은 블루드롭 2의 확률에 따른다.
			int succeedRatio = pFirstOptionInfo->getUpgradeSecondRatio() * ( pItemInfo->getUpgradeRatio() +( rand() %50) );
				cout << "인챈트 시작 3.1/" << (int)thirdOption  << endl;
			succeedRatio += pEventStarInfo->getFunctionValue()*100;
			//int dice = rand()%10000;
			int dice = rand() % diceValue;
			cout << "dice : " << (int)dice <<  endl;
			cout << "diceValue : " << (int)diceValue <<  endl;

			//cout << "첫번째 옵션 : " << pFirstOptionInfo->getHName() << " 인챈트 확률 " << succeedRatio << endl;
			//dice =succeedRatio-1;
			if ( dice < succeedRatio ) //흔벎繫법，橙�薨뚱갚�
			{
				cout << "인챈트 성공" << endl;
				processUpgradeOptionType( pItem, firstOption, pFirstOptionInfo );

				OptionType_t upgradeOptionType = pFirstOptionInfo->getUpgradeType();
				optionChange = (firstOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
				goto EnOK;
			}
			else
			{
				if (pFirstOptionInfo->getUpgradeSecondRatio() < 5)
				{
					int noChangeRatio = 33;
					int downgradeRatio = 46;
					dice = rand() % 100;

					cout << "downgradeRatio" << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

					if ( dice < noChangeRatio )
					{
						// 변화없음
					}
					else if ( dice < noChangeRatio + downgradeRatio )
					{
						// 옵션떨어짐
						downgradeOptionType( pItem, firstOption, pFirstOptionInfo );

						OptionType_t previousOptionType = pFirstOptionInfo->getPreviousType();
						optionChange = (firstOption << (shiftValue+8)) | (previousOptionType << shiftValue);
					}
				}
			}
		}
		shiftValue -= 16;

		if ( bSecondUpgradePossible )
		{
				cout << "인챈트 시작 4/" << (int)secondOption << endl;
			// 두?枰?옵션의 인챈트 확률은 첫번째 옵션의 성공여부에 따른다.

			int succeedRatio= g_pOptionInfoManager->getRareUpgradeRatio( secondOption, bFirstSucceed ) * ( pItemInfo->getUpgradeRatio() +( rand() %50) );
				cout << "인챈트 시작 4.1/" << (int)thirdOption  << endl;
			succeedRatio += pEventStarInfo->getFunctionValue()*100;
			//int dice = rand() %10000;
			int dice = rand() % diceValue;

			//cout << "두번째 옵션 : " << pSecondOptionInfo->getHName() << " 인챈트 확률 " << succeedRatio << endl;
			//dice =succeedRatio-1;
			if ( dice < succeedRatio )
			{
				cout << "인챈트 성공" << endl;
				processUpgradeOptionType( pItem, secondOption, pSecondOptionInfo );

				OptionType_t upgradeOptionType = pSecondOptionInfo->getUpgradeType();
				optionChange |= (secondOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
				goto EnOK;
			}
			else
			{
				if (pSecondOptionInfo->getUpgradeSecondRatio() < 5)
				{
					int noChangeRatio = 33;
					int downgradeRatio = 46;
					dice = rand() % 100;

					cout << "downgradeRatio" << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

					if ( dice < noChangeRatio )
					{
						// 변화없음
					}
					else if ( dice < noChangeRatio + downgradeRatio )
					{
						// 옵션떨어짐
						downgradeOptionType( pItem, secondOption, pSecondOptionInfo );

						OptionType_t previousOptionType = pSecondOptionInfo->getPreviousType();
						optionChange = (secondOption << (shiftValue+8)) | (previousOptionType << shiftValue);
					}
				}
			}
		}
		shiftValue=0;
		if ( bThirdUpgradePossible )
		{
				cout << "인챈트 시작 5/" << (int)thirdOption  << endl;
			// 두번째 옵션의 인챈트 확률은 첫번째 옵션의 성공여부에 따른다.
			int succeedRatio = g_pOptionInfoManager->getRareUpgradeRatio( thirdOption, bFirstSucceed ) * ( pItemInfo->getUpgradeRatio() +( rand() %50) );
				cout << "인챈트 시작 5.1/" << (int)thirdOption  << endl;
			succeedRatio += pEventStarInfo->getFunctionValue()*100;
			//int dice = rand() %10000;

			int dice = rand() % diceValue;
			//cout << "두번째 옵션 : " << pSecondOptionInfo->getHName() << " 인챈트 확률 " << succeedRatio << endl;
			//dice =succeedRatio-1;
			if ( dice < succeedRatio )
			{
				cout << "인챈트 성공" << endl;
				processUpgradeOptionType( pItem, thirdOption, pThirdOptionInfo );

				OptionType_t upgradeOptionType = pThirdOptionInfo->getUpgradeType();
				optionChange2 |= (thirdOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
			}
			else
			{
				if (pThirdOptionInfo->getUpgradeSecondRatio() < 5)
				{
					int noChangeRatio = 33;
					int downgradeRatio = 46;
					dice = rand() % 100;

					cout << "downgradeRatio" << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

					if ( dice < noChangeRatio )
					{
						// 변화없음
					}
					else if ( dice < noChangeRatio + downgradeRatio )
					{
						// 옵션떨어짐
						downgradeOptionType( pItem, thirdOption, pThirdOptionInfo );

						OptionType_t previousOptionType = pThirdOptionInfo->getPreviousType();
						optionChange2 |= (thirdOption << (shiftValue+8)) | (previousOptionType << shiftValue);
					}
				}
			}
		}
EnOK:
				cout << "인챈트 시작 5" << endl;
		//{
			// 들고 있는 별 삭제
			pPC->deleteItemFromExtraInventorySlot();

			// DB에서 삭제
			pMouseItem->destroy();

			// 메모리에서 삭제
			SAFE_DELETE(pMouseItem);

			// 결과 패킷 보내기
			cout << "optionChange : " << optionChange <<  endl;
		
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_THIRD_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( 0 );
			gcAddItemToItemVerify.setThirdOptionType( pItem->getOptionTypeList() );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

		//}

		__END_CATCH
	}
	void executeEnchantRareOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY)
		
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);
//		Inventory* 		pInventory  = pPC->getInventory();

		// 실패할 확률은 itemType에 의해서 결정된다.
		const ItemInfo* pItemInfo = g_pItemInfoManager->getItemInfo( 
													pItem->getItemClass(), 
													pItem->getItemType() );
				EventStarInfo* pEventStarInfo = dynamic_cast<EventStarInfo*>(g_pItemInfoManager->getItemInfo(pMouseItem->getItemClass(), pMouseItem->getItemType()));
				Assert(pEventStarInfo!=NULL);

		// 인챈트 할려는 아이템의 현재 옵션이 2개가 아니거나
		// 유니크 아이템인 경우
		// 혹은 별을 2개 이상 들고 인챈트할라고 할 때
/*		// add by Coffee 2006.11.2  털뙤角뤠槨힛橄昑膠틔，흔벎角앎딧痰劤속돨념힛橄昑변鑒
		if (pItem->getOptionTypeSize() ==3);
		{
			//녑힛橄昑陋구
			executeEnchantRareThreeOption(pGamePlayer, pMouseItem, pItem, invenX, invenY);
			return;
		}
		// end */
		if (pItem->getOptionTypeSize()!=2
			|| pItemInfo->isUnique()
			|| pMouseItem->getNum() != 1
		)
		{
			cout << "no Option 1" << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		bool bFirstSucceed = false;
		uint optionChange = 0;
		int  shiftValue = 16;

		list<OptionType_t> optionList = pItem->getOptionTypeList();
		list<OptionType_t>::const_iterator itr = optionList.begin();

		OptionType_t firstOption = *itr++;
		OptionType_t secondOption = *itr;

		OptionInfo* pFirstOptionInfo = getOptionInfo( firstOption );
		OptionInfo* pSecondOptionInfo = getOptionInfo( secondOption );

		if ( pFirstOptionInfo == NULL || pSecondOptionInfo == NULL )
		{
			cout << "no Option 2" << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		bool bFirstUpgradePossible = pFirstOptionInfo->isUpgradePossible();
		bool bSecondUpgradePossible = pSecondOptionInfo->isUpgradePossible();

		if ( !bFirstUpgradePossible && !bSecondUpgradePossible )
		{
			cout << "no Option 3" << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		if ( bFirstUpgradePossible )
		{
			// 첫번째 옵션의 인챈트 확률은 블루드롭 2의 확률에 따른다.
			int succeedRatio = pFirstOptionInfo->getUpgradeSecondRatio() * pItemInfo->getUpgradeRatio();
			succeedRatio += pEventStarInfo->getFunctionValue()*100;
			int dice = rand()%10000;

			//cout << "첫번째 ?��?: " << pFirstOptionInfo->getHName() << " 인챈트 확률 " << succeedRatio << endl;

			if ( dice < succeedRatio )
			{
				//cout << "인챈트 성공" << endl;
				processUpgradeOptionType( pItem, firstOption, pFirstOptionInfo );

				OptionType_t upgradeOptionType = pFirstOptionInfo->getUpgradeType();
				optionChange = (firstOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
			}
			else
			{
				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				dice = rand() % 100;

				//cout << "인챈트 아템 : " << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio )
				{
					// 옵션떨어짐
					downgradeOptionType( pItem, firstOption, pFirstOptionInfo );

					OptionType_t previousOptionType = pFirstOptionInfo->getPreviousType();
					optionChange = (firstOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType( firstOption );
					optionChange = (firstOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList(), optionField );

					char pField[80];
					sprintf(pField, "OptionType='%s'", optionField.c_str());
					pItem->tinysave(pField);
				}
			}
		}
		else
		{
			OptionType_t prev = pFirstOptionInfo->getPreviousType();

			if ( prev != 0 )
			{
				//옵션떨어질 확률 계산해야 된다.

				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				int dice = rand() % 100;

				//cout << "첫번째 옵션 : " << pFirstOptionInfo->getHName() << " 옵 떨어질 확률 : " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio )
				{
					// 옵션떨어짐
					downgradeOptionType( pItem, firstOption, pFirstOptionInfo );

					OptionType_t previousOptionType = pFirstOptionInfo->getPreviousType();
					optionChange = (firstOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType( firstOption );
					optionChange = (firstOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList(), optionField );

					char pField[80];
					sprintf(pField, "OptionType='%s'", optionField.c_str());
					pItem->tinysave(pField);

				}
			}
		}

		shiftValue -= 16;

		if ( bSecondUpgradePossible )
		{
			// 두번째 옵션의 인챈트 ??活?첫번째 옵션의 성공여부에 따른다.
			int succeedRatio = g_pOptionInfoManager->getRareUpgradeRatio( secondOption, bFirstSucceed ) * pItemInfo->getUpgradeRatio();
			succeedRatio += pEventStarInfo->getFunctionValue()*100;
			int dice = rand() %10000;

			//cout << "두번째 옵션 : " << pSecondOptionInfo->getHName() << " 인챈트 확률 " << succeedRatio << endl;

			if ( dice < succeedRatio )
			{
				//cout << "인챈트 성공" << endl;
				processUpgradeOptionType( pItem, secondOption, pSecondOptionInfo );

				OptionType_t upgradeOptionType = pSecondOptionInfo->getUpgradeType();
				optionChange |= (secondOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
			}
			else
			{
				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				dice = rand() % 100;

				//cout << "인챈트 아템 : " << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio )
				{
					// 옵션떨어짐
					downgradeOptionType( pItem, secondOption, pSecondOptionInfo );

					OptionType_t previousOptionType = pSecondOptionInfo->getPreviousType();
					optionChange |= (secondOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType( secondOption );
					optionChange |= (secondOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList(), optionField );

					char pField[80];
					sprintf(pField, "OptionType='%s'", optionField.c_str());
					pItem->tinysave(pField);
				}
			}
		}
		else
		{
			OptionType_t prev = pSecondOptionInfo->getPreviousType();

			if ( prev != 0 )
			{
				//옵션떨어질 확률 계산해야 된다.
				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				int dice = rand() % 100;

				//cout << "두번째 옵션 : " << pSecondOptionInfo->getHName() << " 옵 떨어질 확률 : " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio )
				{
					// 옵션떨어짐
					downgradeOptionType( pItem, secondOption, pSecondOptionInfo );

					OptionType_t previousOptionType = pSecondOptionInfo->getPreviousType();
					optionChange |= (secondOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType( secondOption );
					optionChange |= (secondOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList(), optionField );

					char pField[80];
					sprintf(pField, "OptionType='%s'", optionField.c_str());
					pItem->tinysave(pField);
				}
			}
		}

/*		// 옵션이 여러개인 경우는 불가능하지는 않다고 보고..
		// 들고 있는 아이템이 맞는지 확인하고
		// upgrade가능한 option인 경우에...


		//const list<OptionType_t>& optionList = pItem->getOptionTypeList();
		// 복사해서 쓴다.. 
		list<OptionType_t>::const_iterator itr = optionList.begin();

		for (; itr!=optionList.end(); itr++)
		{
			OptionType_t currentOptionType = *itr;

			OptionInfo* pOptionInfo = getOptionInfo( currentOptionType );

			if (pOptionInfo==NULL)
			{
				sendEnchantImpossible( pGamePlayer );
				return;
			}

			bool bUpgradePossible = pOptionInfo->isUpgradePossible();

			if (bUpgradePossible)
			{
				//---------------------------------------------------------
				// item개수가 1개인 경우만 할 수 있도록 해둔다.
				// 여러개인 경우도 가능하게 하려면
				// 아이템 개수를 줄여야겠지.. client도 수정해야 한다.
				// upgrade 성공할까?
				//---------------------------------------------------------
				// 옵션확률 * 타잎확률 / 10000
				//---------------------------------------------------------
				// 첫번째 옵션
				//---------------------------------------------------------
	
				// 첫번째 옵션이 성공하면 두번째 옵션확률로 계산한다.
				int upgradeRatio = (bFirstSucceed? pOptionInfo->getUpgradeSecondRatio() 
												: pOptionInfo->getUpgradeRatio());


				int succeedRatio = upgradeRatio * pItemInfo->getUpgradeRatio();
				
				int dice = rand()%10000;
				bool bSucceed = (dice < succeedRatio);

				//cout << "EnchantCheck = " 
				//	<< pOptionInfo->getUpgradeRatio() << " * "
				//	<< pItemInfo->getUpgradeRatio() << " = "
				//	<< succeedRatio << ", dice = " << dice << " , succeed=" << (int)bSucceed << endl;

				//cout << "Before = " << getOptionTypeToString( pItem->getOptionTypeList() ) << endl;
				//cout << "CurrentOptionType = " << (int)currentOptionType << endl;


				if (pMouseItem->getNum()==1
					&& bSucceed)
				{
					processUpgradeOptionType( pItem, currentOptionType, pOptionInfo );

					OptionType_t upgradeOptionType = pOptionInfo->getUpgradeType();
					optionChange = (currentOptionType << (shiftValue+8)) | (upgradeOptionType << shiftValue);

					bFirstSucceed = true;
					//cout << gcAddItemToItemVerify.toString().c_str() << endl;
				}
				//---------------------------------------------------------
				// upgrade 실패
				//---------------------------------------------------------
				else
				{
					//cout << "Failed!" << endl;
					bool bCrashItem = pItemInfo->isUpgradeCrash();

					if ( bCrashItem )
					{
						// 아이템 부수기 전에 ItemTrace Log 남기자 ㅡ.ㅡ; 
						if ( pItem != NULL && pItem->isTraceItem() )
						{
							remainTraceLog( pItem, pCreature->getName(), "GOD", ITEM_LOG_DELETE, DETAIL_ENCHANT);
						}
						crashItem(pItem, pInventory, invenX, invenY);

						GCAddItemToItemVerify gcAddItemToItemVerify;
						gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL_CRASH );
						pGamePlayer->sendPacket( &gcAddItemToItemVerify );

						return;
					}
					else
					{
						// 위에서 list를 reference로 쓰면 여기에??옵션이 제거될 수 있으므로
						// list iterator가 다이~될수도 있다.
						downgradeOptionType( pItem, currentOptionType, pOptionInfo );

						OptionType_t previousOptionType = pOptionInfo->getPreviousType();
						optionChange = (currentOptionType << (shiftValue+8)) | (previousOptionType << shiftValue);
					}
				}
			}
			else
			{
				impossibleCount ++;
			}

			shiftValue -= 16;
		}*/

		{
			// 들고 있는 별 삭제
			pPC->deleteItemFromExtraInventorySlot();

			// DB에서 삭제
			pMouseItem->destroy();

			// 메모리에서 삭제
			SAFE_DELETE(pMouseItem);

			// 결과 패킷 보내기
			sendEnchantOK(pGamePlayer, optionChange);
		}

		__END_CATCH
	}

	void executeEnchantSetRareOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY)
		
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);
//		Inventory* 		pInventory  = pPC->getInventory();

		// 실패할 확률은 itemType에 의해서 결정된다.
		const ItemInfo* pItemInfo = g_pItemInfoManager->getItemInfo( 
													pItem->getItemClass(), 
													pItem->getItemType() );

		// 인챈트 할려는 아이템의 현재 옵션이 2개가 아니거나
		// 유니크 아이템인 경우
		// 혹은 별을 2개 이상 들고 인챈트할라고 할 때
		if (pItem->getOptionTypeSize2()!=2
			|| pItemInfo->isUnique()
			|| pMouseItem->getNum() != 1
		)
		{
			cout << "no Option 1" << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		bool bFirstSucceed = false;
		uint optionChange = 0;
		int  shiftValue = 16;

		list<OptionType_t> optionList = pItem->getOptionTypeList2();
		list<OptionType_t>::const_iterator itr = optionList.begin();

		OptionType_t firstOption = *itr++;
		OptionType_t secondOption = *itr;

		OptionInfo* pFirstOptionInfo = getOptionInfo( firstOption );
		OptionInfo* pSecondOptionInfo = getOptionInfo( secondOption );

		if ( pFirstOptionInfo == NULL || pSecondOptionInfo == NULL )
		{
			cout << "no Option 2" << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		bool bFirstUpgradePossible = pFirstOptionInfo->isUpgradePossible();
		bool bSecondUpgradePossible = pSecondOptionInfo->isUpgradePossible();

		if ( !bFirstUpgradePossible && !bSecondUpgradePossible )
		{
			cout << "no Option 3" << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		if ( bFirstUpgradePossible )
		{
			// 첫번째 옵션의 인챈트 확률은 블루드롭 2??확률에 따른다.
			int succeedRatio = pFirstOptionInfo->getUpgradeSecondRatio() * pItemInfo->getUpgradeRatio();
			//succeedRatio += pMouseItem->getHeroOption()*100;
			int dice = rand()%10000;

			//cout << "첫번째 옵션 : " << pFirstOptionInfo->getHName() << " 인챈트 확률 " << succeedRatio << endl;

			if ( dice < succeedRatio )
			{
				//cout << "인챈트 성공" << endl;
				processUpgradeSetOptionType( pItem, firstOption, pFirstOptionInfo );

				OptionType_t upgradeOptionType = pFirstOptionInfo->getUpgradeType();
				optionChange = (firstOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
			}
			else
			{
				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				dice = rand() % 100;

				//cout << "인챈트 아템 : " << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio )
				{
					// 옵션떨어짐
					downgradeSetOptionType( pItem, firstOption, pFirstOptionInfo );

					OptionType_t previousOptionType = pFirstOptionInfo->getPreviousType();
					optionChange = (firstOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType2( firstOption );
					optionChange = (firstOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

					char pField[80];
					sprintf(pField, "OptionType2='%s'", optionField.c_str());
					pItem->tinysave(pField);
				}
			}
		}
		else
		{
			OptionType_t prev = pFirstOptionInfo->getPreviousType();

			if ( prev != 0 )
			{
				//옵션떨어질 확률 계산해야 된다.

				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				int dice = rand() % 100;

				//cout << "첫번째 옵션 : " << pFirstOptionInfo->getHName() << " 옵 떨어질 확률 : " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio )
				{
					// 옵션떨어짐
					downgradeSetOptionType( pItem, firstOption, pFirstOptionInfo );

					OptionType_t previousOptionType = pFirstOptionInfo->getPreviousType();
					optionChange = (firstOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType2( firstOption );
					optionChange = (firstOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

					char pField[80];
					sprintf(pField, "OptionType2='%s'", optionField.c_str());
					pItem->tinysave(pField);

				}
			}
		}

		shiftValue -= 16;

		if ( bSecondUpgradePossible )
		{
			// 두번째 옵션??인챈트 확률은 첫번째 옵션의 성공여?恝?따른다.
			int succeedRatio = g_pOptionInfoManager->getRareUpgradeRatio( secondOption, bFirstSucceed ) * pItemInfo->getUpgradeRatio();
			//succeedRatio += pMouseItem->getHeroOption()*100;
			int dice = rand() %10000;

			//cout << "두번째 옵션 : " << pSecondOptionInfo->getHName() << " 인챈트 확률 " << succeedRatio << endl;

			if ( dice < succeedRatio )
			{
				//cout << "인챈트 성공" << endl;
				processUpgradeSetOptionType( pItem, secondOption, pSecondOptionInfo );

				OptionType_t upgradeOptionType = pSecondOptionInfo->getUpgradeType();
				optionChange |= (secondOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
			}
			else
			{
				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				dice = rand() % 100;

				//cout << "인챈트 아템 : " << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio )
				{
					// 옵션떨어짐
					downgradeOptionType( pItem, secondOption, pSecondOptionInfo );

					OptionType_t previousOptionType = pSecondOptionInfo->getPreviousType();
					optionChange |= (secondOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType2( secondOption );
					optionChange |= (secondOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

					char pField[80];
					sprintf(pField, "OptionType2='%s'", optionField.c_str());
					pItem->tinysave(pField);
				}
			}
		}
		else
		{
			OptionType_t prev = pSecondOptionInfo->getPreviousType();

			if ( prev != 0 )
			{
				//옵션떨어질 확률 계산해야 된다.
				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				int dice = rand() % 100;

				//cout << "두번째 옵션 : " << pSecondOptionInfo->getHName() << " 옵 떨어질 확률 : " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio )
				{
					// 옵션떨어짐
					downgradeSetOptionType( pItem, secondOption, pSecondOptionInfo );

					OptionType_t previousOptionType = pSecondOptionInfo->getPreviousType();
					optionChange |= (secondOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType2( secondOption );
					optionChange |= (secondOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

					char pField[80];
					sprintf(pField, "OptionType2='%s'", optionField.c_str());
					pItem->tinysave(pField);
				}
			}
		}

		{
			// 들고 있는 별 삭제
			pPC->deleteItemFromExtraInventorySlot();

			// DB에서 삭제
			pMouseItem->destroy();

			// 메모리에서 삭제
			SAFE_DELETE(pMouseItem);

			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_THIRD_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( 0 );
			gcAddItemToItemVerify.setThirdOptionType( pItem->getOptionTypeList2() );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			// 결과 패킷 보내기
			//sendSetEnchantOK(pGamePlayer, optionChange);
		}

		__END_CATCH
	}

	void executeEnchantSetThreeOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY)
		
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);
//		Inventory* 		pInventory  = pPC->getInventory();

		// 실패할 확률은 itemType에 의해서 결정된다.
		const ItemInfo* pItemInfo = g_pItemInfoManager->getItemInfo( 
													pItem->getItemClass(), 
													pItem->getItemType() );

				EventStarInfo* pEventStarInfo = dynamic_cast<EventStarInfo*>(g_pItemInfoManager->getItemInfo(pMouseItem->getItemClass(), pMouseItem->getItemType()));
				Assert(pEventStarInfo!=NULL);


		// 인챈트 할?졍?아이템의 현재 옵션이 2개가 아니거나
		// 유니크 아이템인 경우
		// 혹은 별을 2개 이상 들고 인챈트할라고 할 때
		if (pItem->getOptionTypeSize2()!=3
			|| pItemInfo->isUnique()
			|| pMouseItem->getNum() != 1
		)
		{
			cout << "no Option 1" << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		bool bFirstSucceed = false;
		uint optionChange = 0;
		int  shiftValue = 16;

		list<OptionType_t> optionList = pItem->getOptionTypeList2();
		list<OptionType_t>::const_iterator itr = optionList.begin();

		OptionType_t thirdOption  = *itr;
		itr++;
		OptionType_t secondOption = *itr;
		itr++;
		OptionType_t firstOption  = *itr;
		itr--;
		itr--;

	/*	OptionType_t firstOption = *itr++;
		OptionType_t secondOption = *itr;
	*/

		OptionInfo* pFirstOptionInfo = getOptionInfo( firstOption );
		OptionInfo* pSecondOptionInfo = getOptionInfo( secondOption );
		OptionInfo* pThreeOptionInfo = getOptionInfo( thirdOption );
		if ( pFirstOptionInfo == NULL || pSecondOptionInfo == NULL || pThreeOptionInfo == NULL)
		{
			cout << "no Option 2" << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		bool bFirstUpgradePossible = pFirstOptionInfo->isUpgradePossible();
		bool bSecondUpgradePossible = pSecondOptionInfo->isUpgradePossible();
		bool bThreeUpgradePossible = pThreeOptionInfo->isUpgradePossible();

		if ( !bFirstUpgradePossible && !bSecondUpgradePossible && !bThreeUpgradePossible )
		{
			cout << "no Option 3" << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		if ( bFirstUpgradePossible )
		{
			// 첫번째 옵션의 인챈트 확률은 블루드롭 2의 확률에 따른다.
			int succeedRatio = pFirstOptionInfo->getUpgradeSecondRatio() * pItemInfo->getUpgradeRatio();
			succeedRatio += pEventStarInfo->getFunctionValue()*100;
			int dice = rand()%10000;

			//cout << "첫번째 옵션 : " << pFirstOptionInfo->getHName() << " 인챈트 확률 " << succeedRatio << endl;

			if ( dice < succeedRatio )
			{
				//cout << "인챈트 성공" << endl;
				processUpgradeSetOptionType( pItem, firstOption, pFirstOptionInfo );

				OptionType_t upgradeOptionType = pFirstOptionInfo->getUpgradeType();
				optionChange = (firstOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
			}
			else
			{
				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				dice = rand() % 100;

				//cout << "인챈트 아템 : " << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio || pMouseItem->getItemType() >= 50 && pMouseItem->getItemType() <= 52)
				{
					// 옵션떨어짐
					downgradeSetOptionType( pItem, firstOption, pFirstOptionInfo );

					OptionType_t previousOptionType = pFirstOptionInfo->getPreviousType();
					optionChange = (firstOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType2( firstOption );
					optionChange = (firstOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

					char pField[80];
					sprintf(pField, "OptionType2='%s'", optionField.c_str());
					pItem->tinysave(pField);
				}
			}
		}
		else
		{
			OptionType_t prev = pFirstOptionInfo->getPreviousType();

			if ( prev != 0 )
			{
				//옵션떨어질 확률 계산해야 된다.

				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				int dice = rand() % 100;

				//cout << "첫번째 옵션 : " << pFirstOptionInfo->getHName() << " 옵 떨어질 확률 : " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio || pMouseItem->getItemType() >= 50 && pMouseItem->getItemType() <= 52)
				{
					// 옵션떨어짐
					downgradeSetOptionType( pItem, firstOption, pFirstOptionInfo );

					OptionType_t previousOptionType = pFirstOptionInfo->getPreviousType();
					optionChange = (firstOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType2( firstOption );
					optionChange = (firstOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

					char pField[80];
					sprintf(pField, "OptionType2='%s'", optionField.c_str());
					pItem->tinysave(pField);

				}
			}
		}

		shiftValue -= 16;

		if ( bSecondUpgradePossible )
		{
			// 두번째 옵션의 인챈트 확률은 첫번째 옵션의 성공여부에 따른다.
			int succeedRatio = g_pOptionInfoManager->getRareUpgradeRatio( secondOption, bFirstSucceed ) * pItemInfo->getUpgradeRatio();
			succeedRatio += pEventStarInfo->getFunctionValue()*100;
			int dice = rand() %10000;

			//cout << "두번째 옵션 : " << pSecondOptionInfo->getHName() << " 인챈트 확률 " << succeedRatio << endl;

			if ( dice < succeedRatio )
			{
				//cout << "인챈트 성공" << endl;
				processUpgradeSetOptionType( pItem, secondOption, pSecondOptionInfo );

				OptionType_t upgradeOptionType = pSecondOptionInfo->getUpgradeType();
				optionChange |= (secondOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
			}
			else
			{
				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				dice = rand() % 100;

				//cout << "인챈트 아템 : " << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio || pMouseItem->getItemType() >= 50 && pMouseItem->getItemType() <= 52)
				{
					// 옵션떨어짐
					downgradeOptionType( pItem, secondOption, pSecondOptionInfo );

					OptionType_t previousOptionType = pSecondOptionInfo->getPreviousType();
					optionChange |= (secondOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType2( secondOption );
					optionChange |= (secondOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

					char pField[80];
					sprintf(pField, "OptionType2='%s'", optionField.c_str());
					pItem->tinysave(pField);
				}
			}
		}
		else
		{
			OptionType_t prev = pSecondOptionInfo->getPreviousType();

			if ( prev != 0 )
			{
				//옵션떨어질 확률 계산해야 된다.
				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				int dice = rand() % 100;

				//cout << "두번째 옵션 : " << pSecondOptionInfo->getHName() << " 옵 떨어질 확률 : " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio || pMouseItem->getItemType() >= 50 && pMouseItem->getItemType() <= 52)
				{
					// 옵션떨어짐
					downgradeSetOptionType( pItem, secondOption, pSecondOptionInfo );

					OptionType_t previousOptionType = pSecondOptionInfo->getPreviousType();
					optionChange |= (secondOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType2( secondOption );
					optionChange |= (secondOption << (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

					char pField[80];
					sprintf(pField, "OptionType2='%s'", optionField.c_str());
					pItem->tinysave(pField);
				}
			}
		}

		shiftValue -= 16;

		if ( bThreeUpgradePossible )
		{
			// 첫번째 옵션의 인챈트 확률은 블루드롭 2의 확률에 따른다.
			int succeedRatio = pThreeOptionInfo->getUpgradeSecondRatio() * pItemInfo->getUpgradeRatio();
			succeedRatio += pEventStarInfo->getFunctionValue()*100;
			int dice = rand()%10000;

			//cout << "첫번째 옵션 : " << pFirstOptionInfo->getHName() << " 인챈트 확??" << succeedRatio << endl;

			if ( dice < succeedRatio )
			{
				//cout << "인챈트 성공" << endl;
				processUpgradeSetOptionType( pItem, thirdOption, pThreeOptionInfo );

				OptionType_t upgradeOptionType = pThreeOptionInfo->getUpgradeType();
				optionChange = (thirdOption << (shiftValue+8)) | (upgradeOptionType << shiftValue);

				bFirstSucceed = true;
			}
			else
			{
				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				dice = rand() % 100;

				//cout << "인챈트 아템 : " << pItemInfo->getName() << " 옵션떨어질 확률 " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio || pMouseItem->getItemType() >= 50 && pMouseItem->getItemType() <= 52)
				{
					// 옵션떨어짐
					downgradeSetOptionType( pItem, thirdOption, pThreeOptionInfo );

					OptionType_t previousOptionType = pThreeOptionInfo->getPreviousType();
					optionChange = (thirdOption << (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType2( thirdOption);
					optionChange = (thirdOption<< (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

					char pField[80];
					sprintf(pField, "OptionType2='%s'", optionField.c_str());
					pItem->tinysave(pField);
				}
			}
		}
		else
		{
			OptionType_t prev = pThreeOptionInfo->getPreviousType();

			if ( prev != 0 )
			{
				//옵션떨어질 확률 계산해야 된다.

				int noChangeRatio = 33;
				int downgradeRatio = pItemInfo->getDowngradeRatio();
				int dice = rand() % 100;

				//cout << "첫번째 옵션 : " << pFirstOptionInfo->getHName() << " 옵 떨어질 확률 : " << downgradeRatio << endl;

				if ( dice < noChangeRatio )
				{
					// 변화없음
				}
				else if ( dice < noChangeRatio + downgradeRatio || pMouseItem->getItemType() >= 50 && pMouseItem->getItemType() <= 52)
				{
					// 옵션떨어짐
					downgradeSetOptionType( pItem, thirdOption, pThreeOptionInfo);

					OptionType_t previousOptionType = pThreeOptionInfo->getPreviousType();
					optionChange = (thirdOption<< (shiftValue+8)) | (previousOptionType << shiftValue);
				}
				else
				{
					// 옵션없어짐
					pItem->removeOptionType2( thirdOption);
					optionChange = (thirdOption<< (shiftValue+8));

					// DB변경
					string optionField;
					setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

					char pField[80];
					sprintf(pField, "OptionType2='%s'", optionField.c_str());
					pItem->tinysave(pField);

				}
			}
		}


		{
			// 들고 있는 별 삭제
			pPC->deleteItemFromExtraInventorySlot();

			// DB에서 삭제
			pMouseItem->destroy();

			// 메모리에서 삭제
			SAFE_DELETE(pMouseItem);

			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_THIRD_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( 0 );
			gcAddItemToItemVerify.setThirdOptionType( pItem->getOptionTypeList2() );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			// 결과 패킷 보내기
			//sendSetEnchantOK(pGamePlayer, optionChange);
		}

		__END_CATCH
	}


	void executeEnchantOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY)
		
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);
		Inventory* 		pInventory  = pPC->getInventory();

		// Item의 OptionInfo를 얻어온다.
		OptionType_t currentOptionType = //pItem->getFirstOptionType();
										pItem->getRandomOptionType();

		int optionSize = pItem->getOptionTypeSize();

		// 실패할 확률은 itemType에 의해서 결정된다.
		const ItemInfo* pItemInfo = g_pItemInfoManager->getItemInfo( 
													pItem->getItemClass(), 
													pItem->getItemType() );


				EventStarInfo* pEventStarInfo = dynamic_cast<EventStarInfo*>(g_pItemInfoManager->getItemInfo(pMouseItem->getItemClass(), pMouseItem->getItemType()));
				Assert(pEventStarInfo!=NULL);

		// 인챈트 할려는 아이템이 무옵이거나
		// 레어 아이템이거나
		// 유니크 아이템인 경우
		if (optionSize!=1
			|| pItemInfo->isUnique())
		{
			//cout << "no Option " << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		OptionInfo* pOptionInfo = getOptionInfo( currentOptionType );

		if (pOptionInfo==NULL)
		{
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		// 옵션이 여러개인 경?珥?불가능하지는 않다고 보고..
		// 들고 있는 아이템이 맞는지 확인하고
		// upgrade가능한 option인 경우에...
		bool bUpgradePossible = pOptionInfo->isUpgradePossible();
		if (optionSize>1 || bUpgradePossible)
		{
			//---------------------------------------------------------
			// item개수가 1개인 경우만 할 수 있도록 해둔다.
			// 여러개인 경우도 가능하게 하려면
			// 아이템 개수를 줄여야겠지.. client도 수정해야 한다.
			// upgrade 성공할까?
			//---------------------------------------------------------
			// 옵션확률 * 타잎확률 / 10000
			int succeedRatio = 0;

			if ( pMouseItem->getItemType() == 15 )
			{
				// 블루 드롭 2~ 2003.5.11
				succeedRatio = pOptionInfo->getUpgradeSecondRatio() * pItemInfo->getUpgradeRatio();
				succeedRatio += pEventStarInfo->getFunctionValue()*100;
			}
			else
			{
				// 기존 인챈트 아템들
				succeedRatio = pOptionInfo->getUpgradeRatio() * 1+pItemInfo->getUpgradeRatio();
				succeedRatio += pEventStarInfo->getFunctionValue()*100;
			}

//			cout << "옵션 " << pOptionInfo->getHName() << " 인챈트 확률 : " << succeedRatio << endl;
			
			int dice = rand()%10000;
			bool bSucceed = (dice < succeedRatio);

			/*
			cout << "EnchantCheck = " 
				<< pOptionInfo->getUpgradeRatio() << " * "
				<< pItemInfo->getUpgradeRatio() << " = "
				<< succeedRatio << ", dice = " << dice << " , succeed=" << (int)bSucceed << endl;
			*/

			//cout << "Before = " << getOptionTypeToString( pItem->getOptionTypeList() ) << endl;
			//cout << "CurrentOptionType = " << (int)currentOptionType << endl;

			// 옵션이 여러개 붙어있는 경우에 현재 지정한 옵션이 upgrade가 불가능하다면 실패로 본다.
			// 위의 if에서 optionSize>1 을 체크했기 때문에.. bUpgradePossible인 경우가 아니면,
			// optionSize>1 이라고 판단할 수 있다.
			if (bUpgradePossible
				&& pMouseItem->getNum()==1
				&& bSucceed)
			{
//				cout << "인챈트 성공" << endl;
				processUpgradeOptionType( pItem, currentOptionType, pOptionInfo );

				OptionType_t upgradeOptionType = pOptionInfo->getUpgradeType();
				uint optionChange = (currentOptionType << 24) | (upgradeOptionType << 16);

				sendEnchantOK( pGamePlayer, optionChange );

				//cout << gcAddItemToItemVerify.toString().c_str() << endl;
			}
			//---------------------------------------------------------
			// upgrade 실패
			//---------------------------------------------------------
			else
			{
				//cout << "Failed!" << endl;
				bool bCrashItem = pItemInfo->isUpgradeCrash();

				// 실패 packet : 아이템이 부서지는지 아닌지 결정
				GCAddItemToItemVerify gcAddItemToItemVerify;

				if ( bCrashItem )
				{
					// 아이템 부수기 전에 ItemTrace Log 남기자 ㅡ.ㅡ; 
					if ( pItem != NULL && pItem->isTraceItem() )
					{
						remainTraceLog( pItem, pCreature->getName(), "GOD", ITEM_LOG_DELETE, DETAIL_ENCHANT);
					}

					crashItem( pItem, pInventory, invenX, invenY );

					gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL_CRASH );

					//cout << "Broken" << endl;
				}
				else
				{
					downgradeOptionType( pItem, currentOptionType, pOptionInfo );

					OptionType_t previousOptionType = pOptionInfo->getPreviousType();
					uint optionChange = (currentOptionType << 24) | (previousOptionType << 16);

					gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL_DECREASE );
					gcAddItemToItemVerify.setParameter( optionChange );
				}

				pGamePlayer->sendPacket( &gcAddItemToItemVerify );

				//cout << "Upgrade Failed " << (bCrashItem? ": Crash!":" ") << endl;
			}

			// 들고 있는 별 삭제
			pPC->deleteItemFromExtraInventorySlot();

			// DB에서 삭제
			pMouseItem->destroy();

			// 메모리에서 삭제
			SAFE_DELETE(pMouseItem);
		}
		else
		{
			sendEnchantImpossible( pGamePlayer );
			//cout << "upgrade impossible" << endl;
		}

		__END_CATCH
	}

	void executeEnchantSetOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, CoordInven_t invenX, CoordInven_t invenY)
		
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);
		Inventory* 		pInventory  = pPC->getInventory();

		// Item의 OptionInfo를 얻어온다.
		OptionType_t currentOptionType = pItem->getFirstOptionType2();
										//pItem->getRandomOptionType2();

		int optionSize = pItem->getOptionTypeSize2();

		// 실패할 확률은 itemType에 의해서 결정된다.
		const ItemInfo* pItemInfo = g_pItemInfoManager->getItemInfo( 
													pItem->getItemClass(), 
													pItem->getItemType() );

		// 인챈트 할려는 아이템이 무옵이거나
		// 레어 아이템이거나
		// 유?購?아이템인 경우
		if (optionSize!=1
			|| pItemInfo->isUnique())
		{
			//cout << "no Option " << endl;
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		OptionInfo* pOptionInfo = getOptionInfo( currentOptionType );

		if (pOptionInfo==NULL)
		{
			sendEnchantImpossible( pGamePlayer );
			return;
		}

		// 옵션이 여러개인 경우는 불가능하지는 않다고 보고..
		// 들고 있는 아?謙邦??쨈쩝?확인하고
		// upgrade가능한 option인 경우에...
		bool bUpgradePossible = pOptionInfo->isUpgradePossible();
		if (optionSize>1 || bUpgradePossible)
		{
			//---------------------------------------------------------
			// item개수가 1개인 경우만 할 수 있도록 해둔다.
			// 여러개인 경우도 가능하게 하려면
			// 아이템 개수를 줄여야겠지.. client도 수정해야 한다.
			// upgrade 성공할까?
			//---------------------------------------------------------
			// 옵션확률 * 타잎확률 / 10000
			int succeedRatio = 0;

			if ( pMouseItem->getItemType() == 15 )
			{
				// 블루 드롭 2~ 2003.5.11
				succeedRatio = pOptionInfo->getUpgradeSecondRatio() * pItemInfo->getUpgradeRatio();
				//succeedRatio += pMouseItem->getHeroOption()*100;
			}
			else
			{
				// 기존 인챈트 아템들
				succeedRatio = pOptionInfo->getUpgradeRatio() * pItemInfo->getUpgradeRatio();
				//succeedRatio += pMouseItem->getHeroOption()*100;
			}

//			cout << "옵션 " << pOptionInfo->getHName() << " 인챈트 확률 : " << succeedRatio << endl;
			
			int dice = rand()%10000;
			bool bSucceed = (dice < succeedRatio);

			/*
			cout << "EnchantCheck = " 
				<< pOptionInfo->getUpgradeRatio() << " * "
				<< pItemInfo->getUpgradeRatio() << " = "
				<< succeedRatio << ", dice = " << dice << " , succeed=" << (int)bSucceed << endl;
			*/

			//cout << "Before = " << getOptionTypeToString( pItem->getOptionTypeList() ) << endl;
			//cout << "CurrentOptionType = " << (int)currentOptionType << endl;

			// 옵?퓽?여러개 붙어있는 경우에 현재 지정한 옵션??upgrade가 불가능하다면 실패로 본다.
			// 위의 if에서 optionSize>1 을 체크했기 때문에.. bUpgradePossible인 경우가 아니면,
			// optionSize>1 이라고 판단할 수 있다.
			if (bUpgradePossible
				&& pMouseItem->getNum()==1
				&& bSucceed)
			{
//				cout << "인챈트 성공" << endl;
				processUpgradeSetOptionType( pItem, currentOptionType, pOptionInfo );

				OptionType_t upgradeOptionType = pOptionInfo->getUpgradeType();
				uint optionChange = (currentOptionType << 24) | (upgradeOptionType << 16);

			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_THIRD_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( 0 );
			gcAddItemToItemVerify.setThirdOptionType( pItem->getOptionTypeList2() );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

				//cout << gcAddItemToItemVerify.toString().c_str() << endl;
			}
			//---------------------------------------------------------
			// upgrade 실패
			//---------------------------------------------------------
			else
			{
				//cout << "Failed!" << endl;
				bool bCrashItem = pItemInfo->isUpgradeCrash();

				// 실패 packet : 아이템이 부서지는지 아닌지 결정
				GCAddItemToItemVerify gcAddItemToItemVerify;

				if ( bCrashItem )
				{
					// 아이템 부수기 전에 ItemTrace Log 남기자 ㅡ.ㅡ; 
					if ( pItem != NULL && pItem->isTraceItem() )
					{
						remainTraceLog( pItem, pCreature->getName(), "GOD", ITEM_LOG_DELETE, DETAIL_ENCHANT);
					}

					crashItem( pItem, pInventory, invenX, invenY );

					gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_FAIL_CRASH );

					//cout << "Broken" << endl;
				}
				else
				{
					downgradeSetOptionType( pItem, currentOptionType, pOptionInfo );

					OptionType_t previousOptionType = pOptionInfo->getPreviousType();
					uint optionChange = (currentOptionType << 24) | (previousOptionType << 16);

			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_THIRD_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( 0 );
			gcAddItemToItemVerify.setThirdOptionType( pItem->getOptionTypeList2() );
				}

				pGamePlayer->sendPacket( &gcAddItemToItemVerify );

				//cout << "Upgrade Failed " << (bCrashItem? ": Crash!":" ") << endl;
			}

			// 들고 있는 별 삭제
			pPC->deleteItemFromExtraInventorySlot();

			// DB에서 삭제
			pMouseItem->destroy();

			// 메모리에서 삭제
			SAFE_DELETE(pMouseItem);
		}
		else
		{
			sendEnchantImpossible( pGamePlayer );
			//cout << "upgrade impossible" << endl;
		}

		__END_CATCH
	}

	// 세트 옵션을 생성할수 있다.
	void executeAddSetOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, OptionType_t addOptionType)
		
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);
		int MinID,MaxID,LoopT;
		int i = 0;
		switch(addOptionType)
		{
			case 0 : 
			{
			MinID = 1;
			MaxID = 92;
			LoopT = 1;
			break;
			}
			case 1 : 
			{
			MinID = 1;
			MaxID = 92;
			LoopT = 2;
			break;
			}
			case 2 : 
			{
			MinID = 1;
			MaxID = 92;
			LoopT = 3;
			break;
			}
			defalut:	
			break;
		}
		// 이 클래스에 붙을 수 있는 옵션클래스이고..
		// 옵션이 없고
		// 유니크가 아니고
		// 마우스에 한개만 들고 하는 경우에 옵션 추가가 가능하다.
	if(pMouseItem->getItemType() >= 40 && pMouseItem->getItemType() <= 45){
		if (isPossibleOptionItemClass(pItem->getItemClass())
			&& !pItem->isUnique()
			&& pMouseItem->getNum()==1)
		{
			list<OptionType_t> oList;
			while(i < LoopT){
			oList.push_back(g_pCrystalOptionLoaderManager->getRandomItem( MinID , MaxID ));
			i++;
			}
			// 옵션 변경
			if(pMouseItem->getItemType() >= 43 && pMouseItem->getItemType() <= 45){
			pItem->setOptionType2( oList );
			
			string optionField;
			setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

			// DB변경
			char pField[80];
			sprintf(pField, "OptionType2='%s'", optionField.c_str());
			pItem->tinysave(pField);
			}else{
			pItem->setOptionType( oList );
			
			string optionField;
			setOptionTypeToField( pItem->getOptionTypeList(), optionField );

			// DB변경
			char pField[80];
			sprintf(pField, "OptionType='%s'", optionField.c_str());
			pItem->tinysave(pField);
			}

			uint optionChange = addOptionType;

			// 들고 있는 별 삭제
			pPC->deleteItemFromExtraInventorySlot();
			pMouseItem->destroy(); 					// DB에서 삭제
			//SAFE_DELETE(pMouseItem); 				// 메모리에서 삭제


			// 성공packet : upgrade될 option을 넣어준다.
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( 0 );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			GCDeleteInventoryItem gcDeleteInventoryItem;
			gcDeleteInventoryItem.setObjectID(pItem->getObjectID());
			pGamePlayer->sendPacket(&gcDeleteInventoryItem);

						CoordInven_t X = -1, Y;
						pPC->getInventory()->findItemOID( pItem->getObjectID(), X, Y );
						Assert( X != -1 );

			GCCreateItem gcCreateItem;
			makeGCCreateItem( &gcCreateItem, pItem, X, Y );
			pGamePlayer->sendPacket(&gcCreateItem);

			//SAFE_DELETE(pItem); 				// 메모리에서 삭제


			//cout << gcAddItemToItemVerify.toString().c_str() << endl;
		}
	}
	if(pMouseItem->getItemType() >= 33 && pMouseItem->getItemType() <= 38){
		if (isPossibleOptionItemClass(pItem->getItemClass())
			&& pItem->getOptionTypeSize2()==0 || pItem->getOptionTypeSize()==0
			&& !pItem->isUnique()
			&& pMouseItem->getNum()==1)
		{
			list<OptionType_t> oList;
			while(i < LoopT){
			oList.push_back(g_pCrystalOptionLoaderManager->getRandomItem( MinID , MaxID ));
			i++;
			}
			// 옵션 변경
			if(pMouseItem->getItemType() >= 33 && pMouseItem->getItemType() <= 35){
			pItem->setOptionType2( oList );
			
			string optionField;
			setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

			// DB변경
			char pField[80];
			sprintf(pField, "OptionType2='%s'", optionField.c_str());
			pItem->tinysave(pField);
			}else{
			pItem->setOptionType( oList );
			
			string optionField;
			setOptionTypeToField( pItem->getOptionTypeList(), optionField );

			// DB변경
			char pField[80];
			sprintf(pField, "OptionType='%s'", optionField.c_str());
			pItem->tinysave(pField);
			}

			uint optionChange = addOptionType;

			// 들고 있는 별 삭제
			pPC->deleteItemFromExtraInventorySlot();
			pMouseItem->destroy(); 					// DB에서 삭제
			//SAFE_DELETE(pMouseItem); 				// 메모리에서 삭제


			// 성공packet : upgrade될 option을 넣어준다.
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( 0 );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			GCDeleteInventoryItem gcDeleteInventoryItem;
			gcDeleteInventoryItem.setObjectID(pItem->getObjectID());
			pGamePlayer->sendPacket(&gcDeleteInventoryItem);

						CoordInven_t X = -1, Y;
						pPC->getInventory()->findItemOID( pItem->getObjectID(), X, Y );
						Assert( X != -1 );

			GCCreateItem gcCreateItem;
			makeGCCreateItem( &gcCreateItem, pItem, X, Y );
			pGamePlayer->sendPacket(&gcCreateItem);

			//SAFE_DELETE(pItem); 				// 메모리?【?삭제


			//cout << gcAddItemToItemVerify.toString().c_str() << endl;
		}
		else
		{
			//cout << "Enchant Impossible" << endl;
			//cout << "impossibleOption : " << (int)currentOptionType << endl;
			// upgrade 할 수 없는 option이다.
			// 불가 packet
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			//cout << "upgrade impossible" << endl;
		}
	}
		__END_CATCH
	}

	// 무옵 아이템에 옵션을 추가시킨다.
	void executeAddOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, OptionType_t addOptionType)
		
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);

		// 프리미엄 서비스가 아닌 경우는 안되지..
/*		if (!pGamePlayer->isPayPlaying()
			&& !pGamePlayer->isPremiumPlay())
		{
			//cout << "no Option " << endl;
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE_NO_PREMIUM );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );
			return;
		}
*/

		// 이 클래스에 ?牡?수 있는 옵션클래스이고..
		// 옵션이 없고
		// 유니크가 아니고
		// 마우스에 한개만 들고 하는 경우에 옵션 추가가 가능하다.
		if (isPossibleOptionItemClass(pItem->getItemClass())
			&& pItem->getOptionTypeSize()==0 
			&& !pItem->isUnique()
			&& pMouseItem->getNum()==1)
		{
			// 옵션 변경
			pItem->addOptionType( addOptionType );
			
			string optionField;
			setOptionTypeToField( pItem->getOptionTypeList(), optionField );

			// DB변경
			char pField[80];
			sprintf(pField, "OptionType='%s'", optionField.c_str());
			pItem->tinysave(pField);

			uint optionChange = addOptionType;

			// 들고 있는 별 삭제
			pPC->deleteItemFromExtraInventorySlot();
			pMouseItem->destroy(); 					// DB에서 삭제
			SAFE_DELETE(pMouseItem); 				// 메모리에서 삭제


			// 성공packet : upgrade될 option을 넣어준다.
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( optionChange );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			//cout << gcAddItemToItemVerify.toString().c_str() << endl;
		}
		else
		{
			//cout << "Enchant Impossible" << endl;
			//cout << "impossibleOption : " << (int)currentOptionType << endl;
			// upgrade 할 수 없는 option이다.
			// 불가 packet
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			//cout << "upgrade impossible" << endl;
		}

		__END_CATCH
	}

	// 무옵션 아이템에 코랄을 바를수 있다.

	void executeAddCoralOption(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem, int addOptionType)
		
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);

		// 프리미엄 서비스가 아닌 경우는 안되지..
/*		if (!pGamePlayer->isPayPlaying()
			&& !pGamePlayer->isPremiumPlay())
		{
			//cout << "no Option " << endl;
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE_NO_PREMIUM );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );
			return;
		}
*/

		// 이 클래스에 붙을 수 있는 옵션클래스이고..
		// 옵션이 없고
		// 유니크가 아니고
		// 마우스에 한개만 들고 하는 경우에 옵션 추가가 가능하다.
		if (isPossibleOptionItemClass(pItem->getItemClass())
			&& pItem->getOptionTypeSize()==0 
			&& !pItem->isUnique()
			&& pMouseItem->getNum()==1)
		{
			OptionType_t CoralOption;
			// 옵션 변경
			switch(addOptionType){
				
				// 레드 코랄(힘1~5)
				case 0 :
				{
				CoralOption = Random(1,5);
				break;
				}
				
				// 그린 코랄(덱1~5)
				case 1 :
				{
				CoralOption = Random(6,10);
				break;
				}

				// 블루 코랄(인1~5)
				case 2 :
				{
				CoralOption = Random(11,15);
				break;
				}

				// 블루 코랄(인1~5)
				case 3 :
				{
				CoralOption = Random(48,52);
				break;
				}

				// 블루 코랄(인1~5)
				case 4 :
				{
				CoralOption = Random(183,185);
				break;
				}

				// 블루 코랄(인1~5)
				case 5 :
				{
				CoralOption = Random(1,87);
				if(CoralOption == 32){CoralOption = 33;}
				if(CoralOption == 174){CoralOption = 175;}
				break;
				}
					default:
					break;
			}
			pItem->addOptionType( CoralOption );
			
			string optionField;
			setOptionTypeToField( pItem->getOptionTypeList(), optionField );

			// DB변경
			char pField[80];
			sprintf(pField, "OptionType='%s'", optionField.c_str());
			pItem->tinysave(pField);

			uint optionChange = CoralOption;

			// 들고 있는 별 삭제
			pPC->deleteItemFromExtraInventorySlot();
			pMouseItem->destroy(); 					// DB에서 삭제
			SAFE_DELETE(pMouseItem); 				// 메모리에서 삭제


			// 성공packet : upgrade될 option을 넣어준다.
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( optionChange );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			//cout << gcAddItemToItemVerify.toString().c_str() << endl;
		}
		else
		{
			//cout << "Enchant Impossible" << endl;
			//cout << "impossibleOption : " << (int)currentOptionType << endl;
			// upgrade 할 수 없는 option이다.
			// 불가 packet
			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			//cout << "upgrade impossible" << endl;
		}

		__END_CATCH
	}

	void executeRedSunCore(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem) 
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);

		SlayerTuningItemInfo* pSlayerTuningItemInfo = dynamic_cast<SlayerTuningItemInfo*>(g_pItemInfoManager->getItemInfo(pMouseItem->getItemClass(), pMouseItem->getItemType()));
		Assert(pSlayerTuningItemInfo != NULL);

		pItem->setItemType(pSlayerTuningItemInfo->getTunningItemType());

		char pField[80];
		sprintf(pField, "ItemType=%u", pSlayerTuningItemInfo->getTunningItemType());
		pItem->tinysave(pField);

			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_CHANGE_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( pSlayerTuningItemInfo->getTunningItemType() );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			pPC->deleteItemFromExtraInventorySlot();
			pMouseItem->destroy();
			SAFE_DELETE(pMouseItem);
			cout << " 여기까지는 오네.." << endl;

		__END_CATCH
	}


	void executeVampireTuningItem(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem) 
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);

		VampireTuningItemInfo* pCreateItemInfo = dynamic_cast<VampireTuningItemInfo*>(g_pItemInfoManager->getItemInfo(pMouseItem->getItemClass(), pMouseItem->getItemType()));
		Assert(pCreateItemInfo != NULL);

		pItem->setItemType(pCreateItemInfo->getTunningItemType());

		char pField[80];
		sprintf(pField, "ItemType=%u", pCreateItemInfo->getTunningItemType());
		pItem->tinysave(pField);

			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_CHANGE_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( pCreateItemInfo->getTunningItemType() );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			pPC->deleteItemFromExtraInventorySlot();
			pMouseItem->destroy();
			SAFE_DELETE(pMouseItem);
			cout << " 여기까지는 오네.." << endl;

		__END_CATCH
	}

	void executeOustersTuningItem(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem) 
	{
		__BEGIN_TRY

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);

		OustersTuningItemInfo* pCreateItemInfo = dynamic_cast<OustersTuningItemInfo*>(g_pItemInfoManager->getItemInfo(pMouseItem->getItemClass(), pMouseItem->getItemType()));
		Assert(pCreateItemInfo != NULL);

		pItem->setItemType(pCreateItemInfo->getTunningItemType());

		char pField[80];
		sprintf(pField, "ItemType=%u", pCreateItemInfo->getTunningItemType());
		pItem->tinysave(pField);

			GCAddItemToItemVerify gcAddItemToItemVerify;
			gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_CHANGE_ENCHANT_OK );
			gcAddItemToItemVerify.setParameter( pCreateItemInfo->getTunningItemType() );
			pGamePlayer->sendPacket( &gcAddItemToItemVerify );

			pPC->deleteItemFromExtraInventorySlot();
			pMouseItem->destroy();
			SAFE_DELETE(pMouseItem);
			cout << " 여기까지는 오네.." << endl;

		__END_CATCH
	}

	void executeTransKit(GamePlayer* pGamePlayer, Item* pMouseItem, Item* pItem) 
	{
		__BEGIN_TRY

		GCAddItemToItemVerify gcResult;
		uint optionChange;

		const ItemInfo* pItemInfo = g_pItemInfoManager->getItemInfo( pItem->getItemClass(), pItem->getItemType() );
		if ( pItemInfo->getReqGender() == GENDER_BOTH || pItemInfo->getReqGender() == GENDER_MAX
			|| pItemInfo->isUnique() 
			|| pMouseItem->getNum() != 1 )
		{
			gcResult.setCode( ADD_ITEM_TO_ITEM_VERIFY_TRANS_IMPOSSIBLE );
			pGamePlayer->sendPacket(&gcResult);
			return;
		}

		ItemType_t newItemType = pItem->getItemType() ^ 1;
		optionChange = ( pItem->getItemType() << 16 ) | newItemType;
		pItem->setItemType( newItemType );

		char query[50];
		sprintf(query, "ItemType=%u", newItemType );
		pItem->tinysave(query);

		gcResult.setCode( ADD_ITEM_TO_ITEM_VERIFY_TRANS_OK );
		gcResult.setParameter( optionChange );
		pGamePlayer->sendPacket(&gcResult);

		Creature*  		pCreature   = pGamePlayer->getCreature();
		PlayerCreature* pPC         = dynamic_cast<PlayerCreature*>(pCreature);

		pPC->deleteItemFromExtraInventorySlot();
		pMouseItem->destroy();
		SAFE_DELETE(pMouseItem);

		__END_CATCH
	}

	OptionInfo* getOptionInfo(OptionType_t optionType)
	{
		OptionInfo* pOptionInfo = NULL;

		try {
			pOptionInfo = g_pOptionInfoManager->getOptionInfo( optionType );

			if (pOptionInfo==NULL)
			{
				filelog("optionError.txt", "CGAddItemToItemHandler: no option [%d]", optionType);

				return NULL;
			}
		} catch (NoSuchElementException& t) {
			filelog("optionError.txt", "CGAddItemToItemHandler: no option [%d]", optionType);

			return NULL;
		}

		return pOptionInfo;
	}


	void	processUpgradeOptionType(
					Item* pItem,
					OptionType_t currentOptionType,
					OptionInfo* pOptionInfo)
		
	{
		__BEGIN_TRY

		//cout << "Succeed!" << endl;
		// upgrage될 option
		OptionType_t upgradeOptionType = pOptionInfo->getUpgradeType();

		// 옵션 변경
		pItem->changeOptionType( currentOptionType, upgradeOptionType );

		//cout << "Succeed = " << getOptionTypeToString( pItem->getOptionTypeList() );
		
		string optionField;
		setOptionTypeToField( pItem->getOptionTypeList(), optionField );

		//list<OptionType_t> op;
		//setOptionTypeFromField( op, optionField );
		//cout << "Check = " << getOptionTypeToString( op ) << endl;

		// DB변경
		char pField[80];
		sprintf(pField, "OptionType='%s'", optionField.c_str());
		pItem->tinysave(pField);

		__END_CATCH
	}

	void	processUpgradeSetOptionType(
					Item* pItem,
					OptionType_t currentOptionType,
					OptionInfo* pOptionInfo)
		
	{
		__BEGIN_TRY

		//cout << "Succeed!" << endl;
		// upgrage될 option
		OptionType_t upgradeOptionType = pOptionInfo->getUpgradeType();

		// 옵션 변경
		pItem->changeOptionType2( currentOptionType, upgradeOptionType );

		//cout << "Succeed = " << getOptionTypeToString( pItem->getOptionTypeList() );
		
		string optionField;
		setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

		//list<OptionType_t> op;
		//setOptionTypeFromField( op, optionField );
		//cout << "Check = " << getOptionTypeToString( op ) << endl;

		// DB변경
		char pField[80];
		sprintf(pField, "OptionType2='%s'", optionField.c_str());
		pItem->tinysave(pField);

		__END_CATCH
	}

	void	crashItem(Item* pItem, Inventory* pInventory, CoordInven_t invenX, CoordInven_t invenY)
		
	{
		__BEGIN_TRY

		// inventory에서 아이템 제거
		pInventory->deleteItem( invenX, invenY );

		// DB에서 제거
		pItem->destroy();

		// 메모리에서 제거
		SAFE_DELETE(pItem);

		__END_CATCH
	}

	void	downgradeOptionType(
					Item* pItem,
					OptionType_t currentOptionType,
					OptionInfo* pOptionInfo)
		
	{
		__BEGIN_TRY

		// 아이템의 옵션을 한단계 떨어뜨린다.
		OptionType_t previousOptionType = pOptionInfo->getPreviousType();

		//cout << "FailBefore : " << getOptionTypeToString(pItem->getOptionTypeList()) << endl;

		// 옵션 변경
		// 이전 옵션이 없다면 아예 옵션을 없앤다.
		if (previousOptionType==0)
		{
			pItem->removeOptionType( currentOptionType );
			//cout << "Remove = " << getOptionTypeToString( pItem->getOptionTypeList() ) << endl;
		}
		else
		{
			pItem->changeOptionType( currentOptionType, previousOptionType );
			//cout << "Down = " << getOptionTypeToString( pItem->getOptionTypeList() ) << endl;
		}

		// DB변경
		string optionField;
		setOptionTypeToField( pItem->getOptionTypeList(), optionField );

		//cout << "FailAfter : " << getOptionTypeToString(pItem->getOptionTypeList()) << endl;

		char pField[80];
		sprintf(pField, "OptionType='%s'", optionField.c_str());
		pItem->tinysave(pField);


		__END_CATCH
	}

	void	downgradeSetOptionType(
					Item* pItem,
					OptionType_t currentOptionType,
					OptionInfo* pOptionInfo)
		
	{
		__BEGIN_TRY

		// 아이템의 옵션을 한단계 떨어뜨린다.
		OptionType_t previousOptionType = pOptionInfo->getPreviousType();

		//cout << "FailBefore : " << getOptionTypeToString(pItem->getOptionTypeList()) << endl;

		// 옵션 변경
		// 이전 옵션이 없다면 아예 옵션을 없앤다.
		if (previousOptionType==0)
		{
			pItem->removeOptionType2( currentOptionType );
			//cout << "Remove = " << getOptionTypeToString( pItem->getOptionTypeList() ) << endl;
		}
		else
		{
			pItem->changeOptionType2( currentOptionType, previousOptionType );
			//cout << "Down = " << getOptionTypeToString( pItem->getOptionTypeList() ) << endl;
		}

		// DB변경
		string optionField;
		setOptionTypeToField( pItem->getOptionTypeList2(), optionField );

		//cout << "FailAfter : " << getOptionTypeToString(pItem->getOptionTypeList()) << endl;

		char pField[80];
		sprintf(pField, "OptionType2='%s'", optionField.c_str());
		pItem->tinysave(pField);


		__END_CATCH
	}

//-----------------------------------------------------------------
// 펫 조합 인챈트 관련 소스
// 펫 인챈트시 3개 또는 개별적으로 옵션을 지정되어 넣어주어야 한다.
//-----------------------------------------------------------------
	void	processUpgradePetOptionType(
					PetInfo* pPetInfo,
					Item* pItem,
					OptionType_t currentOptionType,
					OptionInfo* pOptionInfo)
		
	{
		__BEGIN_TRY

		//cout << "Succeed!" << endl;
		// upgrage될 option
		OptionType_t upgradeOptionType = pOptionInfo->getUpgradeType();

		pPetInfo->changeMixOptionType( currentOptionType, upgradeOptionType );

		//cout << "Succeed = " << getOptionTypeToString( pItem->getOptionTypeList() );
		
		string optionField;
		setOptionTypeToField( pPetInfo->getMixOptionType(), optionField );
		//pPetInfo->setMixOptionType(pItem->getOptionTypeList2());

		//list<OptionType_t> op;
		//setOptionTypeFromField( op, optionField );
		//cout << "Check = " << getOptionTypeToString( op ) << endl;

		// DB변경
		char pField[80];
		sprintf(pField, "OptionType2='%s'", optionField.c_str());
		pItem->tinysave(pField);

		__END_CATCH
	}

	void	downgradePetOptionType(
					PetInfo* pPetInfo,
					Item* pItem,
					OptionType_t currentOptionType,
					OptionInfo* pOptionInfo)
		
	{
		__BEGIN_TRY

		// 아이템의 옵션을 한단계 떨어뜨린다.
		OptionType_t previousOptionType = pOptionInfo->getPreviousType();

		//cout << "FailBefore : " << getOptionTypeToString(pItem->getOptionTypeList()) << endl;
		pItem->setOptionType2(pPetInfo->getMixOptionType() );
		// 옵션 변경
		// 이전 옵션이 없다면 아예 옵션을 없앤다.
		if (previousOptionType==0)
		{
			pPetInfo->removeMixOptionType( currentOptionType );
			//cout << "Remove = " << getOptionTypeToString( pItem->getOptionTypeList() ) << endl;
		}
		else
		{
			pPetInfo->changeMixOptionType( currentOptionType, previousOptionType );
			//cout << "Down = " << getOptionTypeToString( pItem->getOptionTypeList() ) << endl;
		}

		// DB변경
		string optionField;
		setOptionTypeToField( pPetInfo->getMixOptionType(), optionField );
		//pPetInfo->setMixOptionType(pItem->getOptionTypeList2());
		//cout << "FailAfter : " << getOptionTypeToString(pItem->getOptionTypeList()) << endl;

		char pField[80];
		sprintf(pField, "OptionType2='%s'", optionField.c_str());
		pItem->tinysave(pField);


		__END_CATCH
	}

//--------------------------------------------------------
// 펫 조합기 인챈??끝
//--------------------------------------------------------

	void	sendEnchantImpossible(GamePlayer* pGamePlayer)
		
	{
		__BEGIN_TRY

		GCAddItemToItemVerify gcAddItemToItemVerify;
		gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_IMPOSSIBLE );
		pGamePlayer->sendPacket( &gcAddItemToItemVerify );

		__END_CATCH
	}

	void	sendEnchantOK(GamePlayer* pGamePlayer, DWORD optionChange)
		
	{
		__BEGIN_TRY

		GCAddItemToItemVerify gcAddItemToItemVerify;
		gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_ENCHANT_OK );
		gcAddItemToItemVerify.setParameter( optionChange );
		pGamePlayer->sendPacket( &gcAddItemToItemVerify );

		__END_CATCH
	}

	void	sendSetEnchantOK(GamePlayer* pGamePlayer, DWORD optionChange)
		
	{
		__BEGIN_TRY

		GCAddItemToItemVerify gcAddItemToItemVerify;
		gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_SET_ENCHANT_OK );
		gcAddItemToItemVerify.setParameter( optionChange );
		pGamePlayer->sendPacket( &gcAddItemToItemVerify );

		__END_CATCH
	}

	void	sendEnchantOK(GamePlayer* pGamePlayer, DWORD optionChange , DWORD optionChange2)
		
	{
		__BEGIN_TRY

		GCAddItemToItemVerify gcAddItemToItemVerify;
		gcAddItemToItemVerify.setCode( ADD_ITEM_TO_ITEM_VERIFY_THIRD_ENCHANT_OK );
		gcAddItemToItemVerify.setParameter( 0 );
		pGamePlayer->sendPacket( &gcAddItemToItemVerify );

		__END_CATCH
	}

#endif
