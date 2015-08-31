#pragma once
#include <vector>
#include "TreeNodeIdService.h"
#include "TreeNodeService.h"

template <class T>
class DatabaseTree
{
public:
	DatabaseTree():
		m_pTreeNodeIdService(new TreeNodeIdService),
		m_pTreeNodeService(new TreeNodeService<T>)
	{
	}

	~DatabaseTree()
	{
		delete m_pTreeNodeIdService;
		delete m_pTreeNodeService;
	}

	TreeNodeId createRootNode(const T& content);
	TreeNodeId addNode(TreeNodeId parentId, const T& content);
	bool deleteNode(TreeNodeId nodeId);
	bool moveNodeTo(TreeNodeId nodeId, TreeNodeId newParentId);
	bool getContent(TreeNodeId nodeId, T& content);
	bool updateContent(TreeNodeId nodeId, const T& content);
	TreeNodeId searchNode(const T& content);

private:
	std::vector<TreeNodeId> m_nodes;

private:
	TreeNodeIdService* m_pTreeNodeIdService;
	TreeNodeService<T>* m_pTreeNodeService;
};