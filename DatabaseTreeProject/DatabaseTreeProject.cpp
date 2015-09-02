// DatabaseTreeProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "TreeNodeId.h"
#include "TreeNodeIdService.h"
#include "TreeNodeService.h"
#include "DatabaseTree.h"

int _tmain(int argc, _TCHAR* argv[])
{
	TreeNodeIdService srv;
	TreeNodeId id1 = srv.generateTreeNodeId();
	TreeNodeId id2 = srv.generateTreeNodeId();
	std::cout << (id1 < id2) <<  (id1 <= id2) <<  (id1 > id2) <<  (id1 >= id2) << std::endl;
	std::cout << (id1 == id1) << (id1 != id1) << (id1 == id2) << (id1!= id2) << std::endl;

	DatabaseTree<int> dt;
	TreeNodeId rootId = dt.getRootNode();
	rootId = dt.createRootNode(1);
	rootId = dt.createRootNode(2);
	rootId = dt.getRootNode();
	TreeNodeId node1 = dt.addNode(rootId, 2);
	TreeNodeId node2 = dt.addNode(rootId, 3);
	TreeNodeId node3 = dt.addNode(node1, 4);
	//dt.deleteNode(node3);
	//dt.deleteNode(node2);
	//dt.deleteNode(node1);
	//dt.addNode(rootId, 2);
	dt.moveNodeTo(node1, node2);
	return 0;
}

