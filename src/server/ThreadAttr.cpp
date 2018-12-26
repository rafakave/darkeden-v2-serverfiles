//////////////////////////////////////////////////////////////////////
//
// ThreadAttr.cpp
//
// by Reiot, the Fallen Lord of MUDMANIA(TM)
//
// Last Updated : 1999. 06. 30.
//
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// include
//////////////////////////////////////////////////////////////////////

#include "ThreadAttr.h"
#include "pthreadAPI.h"

using namespace pthreadAPI;

//////////////////////////////////////////////////////////////////////
//
// constructor
//
//////////////////////////////////////////////////////////////////////
ThreadAttr::ThreadAttr () 
    
{
	__BEGIN_TRY

	pthread_attr_init_ex( &attr );
	
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// destructor
//
//////////////////////////////////////////////////////////////////////
ThreadAttr::~ThreadAttr () 
    
{
	__BEGIN_TRY

	pthread_attr_destroy_ex( &attr );
	
	__END_CATCH
}


//////////////////////////////////////////////////////////////////////
//
// 쓰레드의 DETACHED attribute를 리턴한다.
//
//////////////////////////////////////////////////////////////////////
int ThreadAttr::getDetachState () const 
    
{
	__BEGIN_TRY

	int state;
	pthread_attr_getdetachstate_ex( &attr , &state );
	return state;
	
	__END_CATCH
}
	

//////////////////////////////////////////////////////////////////////
//
// 쓰레드의 DETACHED attribute를 설정한다.
//
//////////////////////////////////////////////////////////////////////
void ThreadAttr::setDetachState ( int state ) 
     
{
	__BEGIN_TRY

	pthread_attr_setdetachstate_ex( &attr , state );
	
	__END_CATCH
}
