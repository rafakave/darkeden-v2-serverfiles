//////////////////////////////////////////////////////////////////////////////
// Filename    : AR.h
// Written By  : Elca
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __AR_H__
#define __AR_H__

//#include "Gun.h"
#include "ConcreteItem.h"
#include "ItemPolicies.h"
#include "ItemInfo.h"
#include "InfoClassManager.h"
#include "ItemFactory.h"
#include "ItemLoader.h"
#include "Mutex.h"

//////////////////////////////////////////////////////////////////////////////
// class AR;
//////////////////////////////////////////////////////////////////////////////

class AR : public ConcreteItem<Item::ITEM_CLASS_AR, NoStack, HasDurability, HasOption, WeaponGrade, SlayerGun, NoEnchantLevel, HasOption2, HasHeroOption, HasHeroOptionAttr>
{
public:
	AR() ;
	AR(ItemType_t itemType, const list<OptionType_t>& optionType) ;
	~AR() ;
	
public:
	virtual void create(const string & ownerID, Storage storage, StorageID_t storageID, BYTE x, BYTE y, ItemID_t itemID=0);
	virtual void save(const string & ownerID, Storage storage, StorageID_t storageID, BYTE x, BYTE y);
	virtual void saveBullet();
	void tinysave(const string & field) 	{ tinysave(field.c_str()); }
	void tinysave(const char* field) ;
	virtual string toString() ;

	static void initItemIDRegistry(void) ;

public:
//	virtual ItemClass getItemClass()  { return Item::ITEM_CLASS_AR; }
//	virtual string getObjectTableName()  { return "ARObject"; }

/*	bool isSilverWeapon()  { return true; }

	virtual VolumeWidth_t getVolumeWidth() ;
	virtual VolumeHeight_t getVolumeHeight() ;
	virtual Weight_t getWeight() ;

	virtual Damage_t getMinDamage() ;
	virtual Damage_t getMaxDamage() ;

	virtual Range_t getRange() ;

	virtual ToHit_t getToHitBonus() ;

	virtual int getCriticalBonus(void) ;*/

	void makePCItemInfo(PCItemInfo& result) const;

private:
	static Mutex    m_Mutex;          // 아이템 ID 관련 락
	static ItemID_t m_ItemIDRegistry; // 클래스별 고유 아이템 아이디 발급기
};


//////////////////////////////////////////////////////////////////////////////
// class ARInfo
//////////////////////////////////////////////////////////////////////////////

class ARInfo : public ItemInfo 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_AR; }

	virtual Durability_t getDurability()  { return m_Durability; }
	virtual void setDurability(Durability_t durability)  { m_Durability = durability; }

	virtual Damage_t getMinDamage()  { return m_MinDamage; }
	virtual void setMinDamage(Damage_t minDamage)  { m_MinDamage = minDamage; }

	virtual Damage_t getMaxDamage()  { return m_MaxDamage; }
	virtual void setMaxDamage(Damage_t maxDamage)  { m_MaxDamage = maxDamage; }

	Range_t getRange()  { return m_Range; }
	void setRange(Range_t range)  { m_Range = range; }

	ToHit_t getToHitBonus()  { return m_ToHitBonus; }
	void setToHitBonus(ToHit_t tohit)  { m_ToHitBonus = tohit; }

	virtual Speed_t getSpeed(void)  { return m_Speed; }
	virtual void setSpeed(Speed_t speed)  { m_Speed = speed; }

	virtual uint getItemLevel(void)  { return m_ItemLevel; }
	virtual void setItemLevel(uint level)  { m_ItemLevel = level; }

	virtual int getCriticalBonus(void)  { return m_CriticalBonus; }
	virtual void setCriticalBonus(int bonus)  { m_CriticalBonus = bonus; }

	virtual string toString() ;

private:
	Durability_t  m_Durability;    // 내구성
	Damage_t      m_MinDamage;     // 최소 데미지
	Damage_t      m_MaxDamage;     // 최대 데미지
	Range_t       m_Range;         // 사정거리
	ToHit_t       m_ToHitBonus;    // 명중률 보너스
	Speed_t       m_Speed;         // 공격 속도
	uint          m_ItemLevel;     // 아이템의 레벨
	int           m_CriticalBonus; // 아이템마다 다른 크리티컬 확률

};


//////////////////////////////////////////////////////////////////////////////
// class ARInfoManager;
//////////////////////////////////////////////////////////////////////////////

class ARInfoManager : public InfoClassManager 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_AR; }
	virtual void load();
};

extern ARInfoManager* g_pARInfoManager;


//////////////////////////////////////////////////////////////////////////////
// class ARFactory
//////////////////////////////////////////////////////////////////////////////

class ARFactory : public ItemFactory 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_AR; }
	virtual string getItemClassName()  { return "AR"; }
	
public:
	virtual Item* createItem(ItemType_t ItemType, const list<OptionType_t>& OptionType)  { return new AR(ItemType,OptionType); }
};


//////////////////////////////////////////////////////////////////////////////
// class ARLoader;
//////////////////////////////////////////////////////////////////////////////

class ARLoader : public ItemLoader 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_AR; }
	virtual string getItemClassName()  { return "AR"; }

public:
	virtual void load(Creature* pCreature);
	virtual void load(Zone* pZone);
	virtual void load(StorageID_t storageID, Inventory* pInventory);
};

extern ARLoader* g_pARLoader;

#endif
