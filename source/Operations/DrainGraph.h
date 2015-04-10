#ifndef DRAINGRAPH_H
#define DRAINGRAPH_H

#include "MultiflowDMatrix.h"
#include "DrainGraphNode.h"
#include "DrainGraphNodeDataFactory.h"
#include "matrix.h"

namespace TR {

class DrainGraph
{
public:
	typedef matrix<DrainGraphNode *> typeGraphNodePool;
	typedef std::list<MultiflowDMatrix::iterator> typeRootPointList;
private:
	typeRootPointList & _rootPointList;
	DrainGraphNodeDataFactory & _graphNodeDataFactory;
	double _dblLocalDrainLimit;

	typeGraphNodePool _graphNodePool;
	DrainGraphNode::typeNodeList _rootNodes;
	
public:
	DrainGraph(typeRootPointList & rootPoints, double dblLocalDrainLimit, DrainGraphNodeDataFactory & factory);
	~DrainGraph();
	DrainGraphNode::typeNodeList & getRootNodes();
	void build();
private:
	void buildGrapghNodePool();
	void clearGraphNodePool();
	void buildFromOnePoint(MultiflowDMatrix::iterator & root);
};

}

#endif