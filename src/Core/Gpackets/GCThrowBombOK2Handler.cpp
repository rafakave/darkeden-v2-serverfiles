//////////////////////////////////////////////////////////////////////
//
// Filename    : GCThrowBombOK2Handler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "GCThrowBombOK2.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCThrowBombOK2Handler::execute ( GCThrowBombOK2 * pGCThrowBombOK2 , Player * pPlayer )
	 
{
	__BEGIN_TRY __BEGIN_DEBUG_EX
		
#if __GAME_CLIENT__

	//cout << pGCThrowBombOK2->toString() << endl;
	
#elif __WINDOWS__
#endif

	__END_DEBUG_EX __END_CATCH
}
