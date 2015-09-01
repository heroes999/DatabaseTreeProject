#pragma once
#include <vector>
#include <algorithm>
#include <set>
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

template <class T>
TreeNodeId DatabaseTree<T>::addNode(TreeNodeId parentId, const T& content)
{
	TreeNode<T>* pParentNode = m_pTreeNodeService->getTreeNode(parentId);
	if (pParentNode == NULL)
		return m_pTreeNodeIdService->generateNullId();

	TreeNodeId newNodeId = m_pTreeNodeIdService->generateTreeNodeId();
	TreeNode<T>* pNewNode = new TreeNode<T>(pParentNode->m_rgt, pParentNode->m_rgt + 1, pParentNode->m_layer + 1, content);
	m_pTreeNodeService->registerTreeNode(newNodeId, pNewNode);

	int rgtThreshold = pParentNode->m_rgt;
	std::for_each(m_nodes.begin(), m_nodes.end(), [this, rgtThreshold](TreeNodeId nodeId) {
		TreeNode<T>* pNode = m_pTreeNodeService->getTreeNode(nodeId);
		if (pNode == NULL)
			return;

		if (pNode->m_rgt >= rgtThreshold)
			pNode->m_rgt += 2;
		if (pNode->m_lft >= rgtThreshold)
			pNode->m_lft += 2;
	});

	m_nodes.push_back(newNodeId);
	return newNodeId;
}

template <class T>
bool DatabaseTree<T>::deleteNode(TreeNodeId nodeId)
{
	TreeNode<T>* pDelNode = m_pTreeNodeService->getTreeNode(nodeId);
	if (pDelNode == NULL)
		return false;
	if (pDelNode->m_lft + 1 != pDelNode->m_rgt)//only support delete leaf
		return false;

	auto iter = std::find_if(m_nodes.begin(), m_nodes.end(), [nodeId](TreeNodeId id){
		if (id == nodeId)
			return true;
		return false;
	});
	m_nodes.erase(iter);

	int rgtThreshold = pDelNode->m_rgt;
	std::for_each(m_nodes.begin(), m_nodes.end(), [this, rgtThreshold](TreeNodeId nodeId) {
		TreeNode<T>* pNode = m_pTreeNodeService->getTreeNode(nodeId);
		if (pNode == NULL)
			return;

		if (pNode->m_rgt > rgtThreshold)
			pNode->m_rgt -= 2;
		if (pNode->m_lft >= rgtThreshold)
			pNode->m_lft -= 2;
	});

	m_pTreeNodeService->unregisterTreeNode(nodeId);
}

template <class T>
bool DatabaseTree<T>::getContent(TreeNodeId nodeId, T& content)
{
	TreeNode<T>* pNode = m_pTreeNodeService->getTreeNode(nodeId);
	if (pNode == NULL)
		return false;

	content = pNode->m_content;
	return true;
}

template <class T>
bool DatabaseTree<T>::updateContent(TreeNodeId nodeId, const T& content)
{
	TreeNode<T>* pNode = m_pTreeNodeService->getTreeNode(nodeId);
	if (pNode == NULL)
		return false;

	pNode->m_content = content;
	return true;
}

template <class T>
bool DatabaseTree<T>::moveNodeTo(TreeNodeId nodeId, TreeNodeId newParentId)
{
	TreeNode<T>* pNodeToMove = m_pTreeNodeService->getTreeNode(nodeId);
	TreeNode<T>* pParentNode = m_pTreeNodeService->getTreeNode(newParentId);
	if (pNodeToMove == NULL || pParentNode == NULL)
		return false;
	if (pNodeToMove->isAncestorOf(pParentNode))
		return false;

	//collect nodes that is to moved
	std::set<TreeNodeId> nodesToBeMoved;
	for (auto iter = m_nodes.begin(); iter != m_nodes.end(); ++iter)
	{
		TreeNode<T>* pNode = m_pTreeNodeService->getTreeNode(*iter);
		if (pNode == NULL)
			continue;

		if (pNode->m_lft >= pNodeToMove->m_lft && pNode->m_rgt <= pNodeToMove->m_rgt)
		{
			nodesToBeMoved.insert(*iter);
		}
	}

	//modify rgt/lft as if nodes in nodesToBeMoved are deleted
	int rgtThreshold = pNodeToMove->m_rgt;
	int nodesToBeMovedCount = nodesToBeMoved.size();
	std::for_each(m_nodes.begin(), m_nodes.end(), [this, rgtThreshold, nodesToBeMovedCount, nodesToBeMoved](TreeNodeId nodeId) {
		if (nodesToBeMoved.find(nodeId) != nodesToBeMoved.end())
			return;
		TreeNode<T>* pNode = m_pTreeNodeService->getTreeNode(nodeId);
		if (pNode == NULL)
			return;

		if (pNode->m_rgt > rgtThreshold)
			pNode->m_rgt -= 2 * nodesToBeMovedCount;
		if (pNode->m_lft > rgtThreshold)
			pNode->m_lft -= 2 * nodesToBeMovedCount;
	});

	//modify rgt/lft as if nodes in nodesToBeMoved are moved to new parent
	rgtThreshold = pParentNode->m_rgt;
	std::for_each(m_nodes.begin(), m_nodes.end(), [this, rgtThreshold, nodesToBeMoved, nodesToBeMovedCount](TreeNodeId nodeId) {
		if (nodesToBeMoved.find(nodeId) != nodesToBeMoved.end())
			return;
		TreeNode<T>* pNode = m_pTreeNodeService->getTreeNode(nodeId);
		if (pNode == NULL)
			return;

		if (pNode->m_rgt >= rgtThreshold)
			pNode->m_rgt += 2 * nodesToBeMovedCount;
		if (pNode->m_lft >= rgtThreshold)
			pNode->m_lft += 2 * nodesToBeMovedCount;
	});

	int layerDelta = pParentNode->m_layer - pNodeToMove->m_layer + 1;
	int lftDelta = rgtThreshold - pNodeToMove->m_lft;
	int rgtDelta = lftDelta;
	for (auto nodeId : nodesToBeMoved)
	{
		TreeNode<T>* pNode = m_pTreeNodeService->getTreeNode(nodeId);
		if (pNode == NULL)
			continue;

		pNode->m_layer += layerDelta;
		pNode->m_lft += lftDelta;
		pNode->m_rgt += rgtDelta;
	}

	nodesToBeMoved.clear();

	return true;
}