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
		m_pTreeNodeService(new TreeNodeService<T>),
		m_rootId(NullTreeNodeId())
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

	bool isAncestor(TreeNodeId nodeId, TreeNodeId potentialAncestorId);

private:
	void removeFromParent(TreeNodeId nodeId, TreeNodeId parentId);

private:
	TreeNodeId m_rootId;

	TreeNodeIdService* m_pTreeNodeIdService;
	TreeNodeService<T>* m_pTreeNodeService;
};

template <class T>
TreeNodeId DatabaseTree<T>::getRootNode()
{
	if (m_pTreeNodeService->getTreeNode(m_rootId) != NULL)
		return m_rootId;
	else
	{
		m_rootId = NullTreeNodeId();
		return m_rootId;
	}
}

template <class T>
TreeNodeId DatabaseTree<T>::createRootNode(const T& content)
{
	if (m_pTreeNodeService->getTreeNode(m_rootId) != NULL)
		return m_rootId;

	m_rootId = m_pTreeNodeIdService->generateTreeNodeId();
	TreeNode<T>* pRoot = new TreeNode<T>(NullTreeNodeId(), content);
	m_pTreeNodeService->registerTreeNode(m_rootId, pRoot);

	return m_rootId;
}

template <class T>
TreeNodeId DatabaseTree<T>::addNode(TreeNodeId parentId, const T& content)
{
	TreeNode<T>* pParentNode = m_pTreeNodeService->getTreeNode(parentId);
	if (pParentNode == NULL)
		return NullTreeNodeId();

	TreeNodeId newNodeId = m_pTreeNodeIdService->generateTreeNodeId();
	TreeNode<T>* pNewNode = new TreeNode<T>(parentId, content);
	m_pTreeNodeService->registerTreeNode(newNodeId, pNewNode);

	pParentNode->m_childrenIds.push_back(newNodeId);
	return newNodeId;
}

template <class T>
void DatabaseTree<T>::removeFromParent(TreeNodeId nodeId, TreeNodeId parentId)
{
	TreeNode<T>* pParentNode = m_pTreeNodeService->getTreeNode(parentId);
	if (pParentNode == NULL)
		return;

	auto iter = std::find_if(pParentNode->m_childrenIds.begin(), pParentNode->m_childrenIds.end(), [nodeId](TreeNodeId id){
		if (id == nodeId)
			return true;
		return false;
	});
	pParentNode->m_childrenIds.erase(iter);
}

template <class T>
bool DatabaseTree<T>::deleteNode(TreeNodeId nodeId)
{
	TreeNode<T>* pDelNode = m_pTreeNodeService->getTreeNode(nodeId);
	if (pDelNode == NULL)
		return false;
	if (!pDelNode->m_childrenIds.empty())//only support delete leaf
		return false;

	removeFromParent(nodeId, pDelNode->m_parentId);
	m_pTreeNodeService->unregisterTreeNode(nodeId);
	return true;
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
bool DatabaseTree<T>::isAncestor(TreeNodeId nodeId, TreeNodeId potentialAncestorId)
{
	TreeNode<T>* pNode = m_pTreeNodeService->getTreeNode(nodeId);
	if (pNode == NULL)
		return false;

	if (pNode->m_parentId == potentialAncestorId)
		return true;

	return isAncestor(pNode->m_parentId, potentialAncestorId);
}

template <class T>
bool DatabaseTree<T>::moveNodeTo(TreeNodeId nodeId, TreeNodeId newParentId)
{
	TreeNode<T>* pNodeToMove = m_pTreeNodeService->getTreeNode(nodeId);
	TreeNode<T>* pParentNode = m_pTreeNodeService->getTreeNode(newParentId);
	if (pNodeToMove == NULL || pParentNode == NULL)
		return false;
	if (isAncestor(newParentId, nodeId))
		return false;

	removeFromParent(nodeId, pNodeToMove->m_parentId);
	pNodeToMove->m_parentId = newParentId;
	pParentNode->m_childrenIds.push_back(nodeId);
	return true;
}