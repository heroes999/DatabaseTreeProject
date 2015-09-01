#pragma once
#include <vector>
#include <algorithm>
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
	TreeNodeId getRootNode();
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

template <class T>
TreeNodeId DatabaseTree<T>::getRootNode()
{
	if (m_nodes.empty())
		return m_pTreeNodeIdService->generateNullId();

	auto iter = std::find_if(m_nodes.begin(), m_nodes.end(), [this](TreeNodeId nodeId){
		TreeNode<T>* pNode = m_pTreeNodeService->getTreeNode(nodeId);
		if (pNode && pNode->m_lft == 1)
			return true;
		return false;
	});

	if (iter == m_nodes.end())
		return m_pTreeNodeIdService->generateNullId();

	return *iter;
}

template <class T>
TreeNodeId DatabaseTree<T>::createRootNode(const T& content)
{
	if (!m_nodes.empty())
		return getRootNode();

	TreeNodeId	rootNodeId = m_pTreeNodeIdService->generateTreeNodeId();
	TreeNode<T>* pRoot = new TreeNode<T>(1, 2, 1, content);
	m_pTreeNodeService->registerTreeNode(rootNodeId, pRoot);
	m_nodes.push_back(rootNodeId);

	return rootNodeId;
}