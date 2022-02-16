#include "stdafx.h"
#include "MapSystem.h"

cMapSystem::cMapSystem()
{

}

cMapSystem::~cMapSystem()
{
	DELETE_VECTOR(m_StagePointDataVector,sStagePointData*);
}