#ifndef DRAINGRAPHNODE_H
#define DRAINGRAPHNODE_H

#include "DrainGraphNodeData.h"
#include <list>

namespace TR {

class DrainGraphNode 
{
public:
	typedef std::list<DrainGraphNode*> typeNodeList;
private:
	DrainGraphNodeData * _pData;
	typeNodeList		 _parents;
	typeNodeList		 _children;

public:
	DrainGraphNode();
	~DrainGraphNode();

	void addParent(DrainGraphNode & node);
	void addChild(DrainGraphNode & node);
	void setNodeData(DrainGraphNodeData * pData);
	DrainGraphNodeData * getNodeData();

	typeNodeList & getParents();
	typeNodeList & getChildren();

	bool hasChildren() const;

};

}

#endif