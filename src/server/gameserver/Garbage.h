//////////////////////////////////////////////////////////////////////////////
// Filename    : Garbage.h
// Written by  : excel96
// Description :
// 서버 DB 오류 같은 것으로 인해, 유저의 아이템이 사라질 위기에 처했을 
// 경우에 들어가게 되는 storage의 하나다.
//////////////////////////////////////////////////////////////////////////////

#ifndef __GARBAGE_H__
#define __GARBAGE_H__

#include "ObjectRegistry.h"
#include <list>

//////////////////////////////////////////////////////////////////////////////
// forward declaration
//////////////////////////////////////////////////////////////////////////////
class Item;

//////////////////////////////////////////////////////////////////////////////
// class Garbage
//////////////////////////////////////////////////////////////////////////////
class Garbage
{
public:
	Garbage();
	~Garbage();

public:
	int size(void) const { return (int)m_Items.size(); }

	void addItem(Item* pItem);
	void addItemEx(Item* pItem, const string& OwnerID);
	Item* popItem(void);

	void save(const string& OwnerID);
	void registerObject(ObjectRegistry& OR);

protected:
	list<Item*> m_Items;
};

#endif
