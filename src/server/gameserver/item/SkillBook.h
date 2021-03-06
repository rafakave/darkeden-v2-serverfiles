//////////////////////////////////////////////////////////////////////////////
// Filename    : DyePotion.h
// Written By  : excel96
// Description : 
//////////////////////////////////////////////////////////////////////////////

#ifndef __SKILLBOOK_H__
#define __SKILLBOOK_H__

#include "Item.h"
#include "ConcreteItem.h"
#include "ItemPolicies.h"
#include "ItemInfo.h"
#include "InfoClassManager.h"
#include "ItemFactory.h"
#include "ItemLoader.h"
#include "Mutex.h"

//////////////////////////////////////////////////////////////////////////////
// class DyePotion;
//////////////////////////////////////////////////////////////////////////////

class SkillBook : public ConcreteItem<Item::ITEM_CLASS_SKILLBOOK, Stackable, NoDurability, HasOption, NoGrade, NoAttacking, NoEnchantLevel>
{
public:
	SkillBook() ;
	SkillBook(ItemType_t itemType, const list<OptionType_t>& optionType, ItemNum_t Num) ;
	
public:
	virtual void create(const string & ownerID, Storage storage, StorageID_t storageID, BYTE x, BYTE y, ItemID_t itemID=0);
	virtual void save(const string & ownerID, Storage storage, StorageID_t storageID, BYTE x, BYTE y);
	void tinysave(const string & field) 	{ tinysave(field.c_str()); }
	void tinysave(const char* field) ;
	virtual string toString() ;

	static void initItemIDRegistry(void) ;

public:
//	virtual ItemClass getItemClass()  { return Item::ITEM_CLASS_SKILLBOOK; }
//	virtual string getObjectTableName()  { return "SkillBookObject"; }

/*	virtual ItemType_t getItemType()  { return m_ItemType; }
	virtual void setItemType(ItemType_t itemType)  { m_ItemType = itemType; }

	virtual VolumeWidth_t getVolumeWidth() ;
	virtual VolumeHeight_t getVolumeHeight() ;
	virtual Weight_t getWeight() ;

public:
	virtual ItemNum_t getNum()  { return m_Num; }
	virtual void setNum(ItemNum_t Num)  { m_Num = Num; }

	bool    isStackable()  { return false; }
*/
private:
//	ItemType_t m_ItemType;
//	ItemNum_t  m_Num;
	
	static Mutex    m_Mutex;          // 아이템 ID 관련 락
	static ItemID_t m_ItemIDRegistry; // 클래스별 고유 아이템 아이디 발급기
};

//////////////////////////////////////////////////////////////////////////////
// class SkillBookInfo
//////////////////////////////////////////////////////////////////////////////

class SkillBookInfo : public ItemInfo 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_SKILLBOOK; }
	virtual string toString() ;

public :

	virtual int getFunctionValue()  { return m_FunctionValue; }
	virtual void setFunctionValue(int value)  { m_FunctionValue = value; }

	virtual int getRaceValue()  { return m_RaceValue; }
	virtual void setRaceValue(int Racevalue)  { m_RaceValue = Racevalue; }

	int        m_FunctionValue;	// 기능과 관련된 값
	int 	m_RaceValue; // 어떤 종족이 사용가능한가.
};

//////////////////////////////////////////////////////////////////////////////
// class SkillBookInfoManager;
//////////////////////////////////////////////////////////////////////////////

class SkillBookInfoManager : public InfoClassManager 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_SKILLBOOK; }
	virtual void load();
};

extern SkillBookInfoManager* g_pSkillBookInfoManager;

//////////////////////////////////////////////////////////////////////////////
// class SkillBookFactory
//////////////////////////////////////////////////////////////////////////////

class SkillBookFactory : public ItemFactory 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_SKILLBOOK; }
	virtual string getItemClassName()  { return "SkillBook"; }
	
public:
	virtual Item* createItem(ItemType_t ItemType, const list<OptionType_t>& OptionType)  { return new SkillBook(ItemType,OptionType,1); }
};

//////////////////////////////////////////////////////////////////////////////
// class SkillBookLoader;
//////////////////////////////////////////////////////////////////////////////

class SkillBookLoader : public ItemLoader 
{
public:
	virtual Item::ItemClass getItemClass()  { return Item::ITEM_CLASS_SKILLBOOK; }
	virtual string getItemClassName()  { return "SkillBook"; }

public:
	virtual void load(Creature* pCreature);
	virtual void load(Zone* pZone);
	virtual void load(StorageID_t storageID, Inventory* pInventory);
};

extern SkillBookLoader* g_pSkillBookLoader;

#endif
