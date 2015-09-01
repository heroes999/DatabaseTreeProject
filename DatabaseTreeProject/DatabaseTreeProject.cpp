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

	TreeNodeService<int> tnSrv;
	auto pNode = tnSrv.getTreeNode(id1);
	_ASSERT(pNode == NULL);
	tnSrv.registerTreeNode(id1, new TreeNode<int>);
	tnSrv.registerTreeNode(id2, new TreeNode<int>);
	auto pNode1 = tnSrv.getTreeNode(id2);
	tnSrv.unregisterTreeNode(id1);
	tnSrv.unregisterTreeNode(id2);

	DatabaseTree<int> dt;
	TreeNodeId rootId = dt.getRootNode();
	rootId = dt.createRootNode(1);
	rootId = dt.createRootNode(2);
	rootId = dt.getRootNode();
	TreeNodeId node1 = dt.addNode(rootId, 2);
	TreeNodeId node2 = dt.addNode(rootId, 3);
	TreeNodeId node3 = dt.addNode(node1, 4);
	dt.deleteNode(node3);
	dt.deleteNode(node2);
	dt.deleteNode(node1);
	dt.addNode(rootId, 2);
	return 0;
}

