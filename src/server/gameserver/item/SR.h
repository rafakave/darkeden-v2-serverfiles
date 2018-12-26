//////////////////////////////////////////////////////////////////////////////
// Filename    : SR.h
// Written By  : Elca
// Description :
//////////////////////////////////////////////////////////////////////////////

#ifndef __SR_H__
#define __SR_H__

//#include "Gun.h"
#include "ConcreteItem.h"
#include "ItemPolicies.h"
#include "ItemInfo.h"
#include "InfoClassManager.h"
#include "ItemFactory.h"
#include "ItemLoader.h"
#include "Mutex.h"

//////////////////////////////////////////////////////////////////////////////
// class SR;
//////////////////////////////////////////////////////////////////////////////

class SR : public ConcreteItem<Item::ITEM_CLASS_SR, NoStack, HasDurability, HasOption, WeaponGrade, SlayerGun, NoEnchantLevel, HasOption2, HasHeroOption, HasHeroOptionAttr> 
{
public:
	SR() ;
	SR(ItemType_t itemType, const list<OptionType_t>& optionType) ;
	~SR() ;
	
public:
	virtual void create(const string & ownerID, Storage storage, StorageID_t storageID, BYTE x, BYTE y, ItemID_t itemID=0);
	virtual void save(const string & ownerID, Storage storage, StorageID_t storageID, BYTE x, BYTE y);
	void tinysave(const string & field) 	{ tinysave(field.c_str()); }
	void tinysave(const char* field) ;
	virtual void saveBullet();
	virtual string toString() ;

	static void initItemIDRegistry(void) ;

public:
//	virtual ItemClass getItemClass()  { return Item::ITEM_CLASS_SR; }
//	virtual string getObjectTableName()  { return "SRObject"; }

/*	bool isSilverWeapon()  { return true; }

	virtual VolumeWidth_t getVolumeWidth() ;
	virtual VolumeHeight_t getVolumeHeight() ;
	virtual Weight_t getWeight() ;

	virtual Damage_t getMinDamage() ;
	virtual Damage_t getMaxDamage() ;

	virtual Range_t getRange() ;

	virtual ToHit_t getToHitBonus() ;

	virtual int getCriticalBonus(void) ;
*/
	void makePCItemInfo(PCItemInfo& result) const;

private:

	static Mutex    m_Mutex;          // 아이템 ID 관련 락
	static ItemID_t m_ItemIDRegistry; // 클래스별 고유 아이템 아이디 발급기
};


//////////////////////////////////////////////////////////////////////////////
// class SRInfo
//////////////////////////////////////////////////////////////////////////////

class SRInfo : public ItemInfo 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_SR; }

	virtual Durability_t getDurability()  { return m_Durability; }
	virtual void setDurability(Durability_t durability) throw() { m_Durability = durability; }

	virtual Damage_t getMinDamage()  { return m_MinDamage; }
	virtual void setMinDamage(Damage_t minDamage) throw() { m_MinDamage = minDamage; }

	virtual Damage_t getMaxDamage()  { return m_MaxDamage; }
	virtual void setMaxDamage(Damage_t maxDamage) throw() { m_MaxDamage = maxDamage; }

	Range_t getRange()  { return m_Range; }
	void setRange(Range_t range) throw() { m_Range = range; }

	ToHit_t getToHitBonus()  { return m_ToHitBonus; }
	void setToHitBonus(ToHit_t tohit) throw() { m_ToHitBonus = tohit; }

	virtual Speed_t getSpeed(void)  { return m_Speed; }
	virtual void setSpeed(Speed_t speed) throw() { m_Speed = speed; }

	virtual uint getItemLevel(void)  { return m_ItemLevel; }
	virtual void setItemLevel(uint level) throw() { m_ItemLevel = level; }

	virtual int getCriticalBonus(void)  { return m_CriticalBonus; }
	virtual void setCriticalBonus(int bonus) throw() { m_CriticalBonus = bonus; }

	virtual string toString() ;

private:
	Durability_t  m_Durability;
	Damage_t      m_MinDamage;
	Damage_t      m_MaxDamage;
	Range_t       m_Range;
	ToHit_t       m_ToHitBonus;
	Speed_t       m_Speed;
	uint          m_ItemLevel;
	int           m_CriticalBonus; // 아이템마다 다른 크리티컬 확률

};


//////////////////////////////////////////////////////////////////////////////
// class SRInfoManager;
//////////////////////////////////////////////////////////////////////////////

class SRInfoManager : public InfoClassManager 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_SR; }
	virtual void load();

};

// global variable declaration
extern SRInfoManager* g_pSRInfoManager;


//////////////////////////////////////////////////////////////////////////////
// class SRFactory
//////////////////////////////////////////////////////////////////////////////

class SRFactory : public ItemFactory 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_SR; }
	virtual string getItemClassName()  { return "SR"; }
	
public:
	virtual Item* createItem(ItemType_t ItemType, const list<OptionType_t>& OptionType) throw() { return new SR(ItemType,OptionType); }
};

//////////////////////////////////////////////////////////////////////////////
// class SRLoader;
//////////////////////////////////////////////////////////////////////////////

class SRLoader : public ItemLoader 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_SR; }
	virtual string getItemClassName()  { return "SR"; }

public:
	virtual void load(Creature* pCreature);
	virtual void load(Zone* pZone);
	virtual void load(StorageID_t storageID, Inventory* pInventory);
};

extern SRLoader* g_pSRLoader;

#endif
