//////////////////////////////////////////////////////////////////////
//
// Filename    : GCTakeOffHandler.cc
// Written By  : elca@ewestsoft.com
// Description :
//
//////////////////////////////////////////////////////////////////////

// include files
#include "GCTakeOff.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
void GCTakeOffHandler::execute ( GCTakeOff * pGCTakeOff , Player * pPlayer )
	 
{
	__BEGIN_TRY __BEGIN_DEBUG_EX
		
#if __TEST_CLIENT__

	//cout << pGCTakeOff->toString() << endl;
	
#elif __WINDOWS__

#endif

	__END_DEBUG_EX __END_CATCH
}
