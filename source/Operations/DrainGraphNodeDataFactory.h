#ifndef DRAINGRAPHNODEDATAFACTORY_H
#define DRAINGRAPHNODEDATAFACTORY_H

#include "DrainGraphNodeData.h"

namespace TR {

class DrainGraphNodeDataFactory 
{
public:
	virtual DrainGraphNodeData * create()=0;
};

class NullDrainGraphNodeDataFactory : public DrainGraphNodeDataFactory
{
public:
	virtual DrainGraphNodeData * create() {return NULL;}
};

}
#endif