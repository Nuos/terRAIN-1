#include "DrainGraphNode.h"

namespace TR {

DrainGraphNode::DrainGraphNode():_pData(NULL)
{

}

DrainGraphNode::~DrainGraphNode()
{
	if (_pData)
		delete _pData;
}

void DrainGraphNode::addParent(DrainGraphNode & node)
{
	_parents.push_back(&node);
}

void DrainGraphNode::addChild(DrainGraphNode & node)
{
	_children.push_back(&node);
}

void DrainGraphNode::setNodeData(DrainGraphNodeData * pData)
{
	_pData = pData;
}

DrainGraphNodeData * DrainGraphNode::getNodeData()
{
	return _pData;
}

DrainGraphNode::typeNodeList & DrainGraphNode::getParents()
{
	return _parents;
}

DrainGraphNode::typeNodeList & DrainGraphNode::getChildren()
{
	return _children;
}

bool DrainGraphNode::hasChildren() const
{
	return !(_children.empty());
}

}
