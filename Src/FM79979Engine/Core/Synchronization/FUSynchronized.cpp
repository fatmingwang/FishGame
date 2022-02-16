#include "../stdAfx.h"
#include "FUSynchronized.h"

//
// cFUSynchronized
//

cFUSynchronized::~cFUSynchronized()
{
}

void cFUSynchronized::Lock()
{
	criticalSection.Enter();
}

void cFUSynchronized::Unlock()
{
	criticalSection.Leave();
}
