//////////////////////////////////////////////////////////////////////////////
// Filename    : GCRemoveInjuriousCreature.cpp 
// Written By  : reiot@ewestsoft.com
// Description : 
//////////////////////////////////////////////////////////////////////////////

#include "GCRemoveInjuriousCreature.h"

void GCRemoveInjuriousCreature::read (SocketInputStream & iStream) 
	 
{
	__BEGIN_TRY
		
	// 이름 읽기
	BYTE szName;

	iStream.read(szName);

	if (szName == 0)
		throw InvalidProtocolException("szName == 0");

	if (szName > 10)
		throw InvalidProtocolException("too large name length");

	iStream.read(m_Name , szName);

	__END_CATCH
}
		    
void GCRemoveInjuriousCreature::write (SocketOutputStream & oStream) const 
     
{
	__BEGIN_TRY
		
	// 이름 쓰기
	BYTE szName = m_Name.size();

	if (szName == 0)
		throw InvalidProtocolException("szName == 0");

	if (szName > 10)
		throw InvalidProtocolException("too large name length");

	oStream.write(szName);

	oStream.write(m_Name);

	__END_CATCH
}

void GCRemoveInjuriousCreature::execute (Player* pPlayer) 
	 
{
	__BEGIN_TRY
		
	GCRemoveInjuriousCreatureHandler::execute(this , pPlayer);

	__END_CATCH
}

string GCRemoveInjuriousCreature::toString () const
       
{
	__BEGIN_TRY
		
	StringStream msg;
	msg << "GCRemoveInjuriousCreature(Name :" << m_Name 
		<< ")" ;
	return msg.toString();

	__END_CATCH
}
