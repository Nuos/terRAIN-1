#include "DrainGraph.h"

namespace TR {

DrainGraph::DrainGraph(typeRootPointList & rootPoints, double dblLocalDrainLimit, DrainGraphNodeDataFactory & factory)
:_rootPointList(_rootPointList),_graphNodeDataFactory(factory),_dblLocalDrainLimit(dblLocalDrainLimit)
{
	buildGrapghNodePool();
}

DrainGraph::~DrainGraph()
{
	clearGraphNodePool();
}

DrainGraphNode::typeNodeList & DrainGraph::getRootNodes()
{
	return _rootNodes;
}

void DrainGraph::build()
{
	typeRootPointList::iterator iter = _rootPointList.begin();
	typeRootPointList::iterator iEnd = _rootPointList.end();

	for (;iter!=iEnd; ++iter){
		buildFromOnePoint(*iter);	
	}
}

void DrainGraph::buildFromOnePoint(MultiflowDMatrix::iterator & root)
{

}
void DrainGraph::buildGrapghNodePool()
{
	if (!_rootPointList.empty()){
	
		MultiflowDMatrix::iterator & first = _rootPointList.front();

		MultiflowDMatrix mfLDD = first.getOwner();

		size_t nRows = mfLDD.getRowNr();
		size_t nCols = mfLDD.getColNr();
		
		if (nRows>0 && nCols>0) {
			_graphNodePool.init(nRows,nCols);

			size_t i = 0;
			size_t j = 0;

			for ( i = 0; i < nRows; i++ ){
				for ( j = 0; j < nCols; j++ ){
					_graphNodePool(i,j) = new DrainGraphNode();
				}
			}
		}
	}
}

void DrainGraph::clearGraphNodePool()
{
	size_t nRows = _graphNodePool.getRowNr();
	size_t nCols = _graphNodePool.getColNr();

	size_t i = 0;
	size_t j = 0;

	for (i = 0; i < nRows; i++ ) {
		for ( j = 0; j < nCols; j++ ) {
			delete _graphNodePool(i,j);
		}
	}
}


}