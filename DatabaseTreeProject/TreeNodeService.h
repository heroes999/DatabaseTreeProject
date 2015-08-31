#pragma once

#include <map>
#include "TreeNodeId.h"
#include "TreeNodeIdService.h"
#include "TreeNode.h"

template <class T>
class TreeNodeService
{
public:
	void registerTreeNode(TreeNodeId tnid, TreeNode<T>* pNode);
	TreeNode<T>* getTreeNode(TreeNodeId tnid);
	bool unregisterTreeNode(TreeNodeId tnid);

	~TreeNodeService();

private:
	std::map<TreeNodeId, TreeNode<T>* > m_treeId2Node;
};

template <class T>
void TreeNodeService<T>::registerTreeNode(TreeNodeId tnid, TreeNode<T>* pNode)
{
	auto iter = m_treeId2Node.find(tnid);
	if (iter == m_treeId2Node.end())
	{
		m_treeId2Node[tnid] = pNode;
	}
	else
	{
		delete (iter->second);
		iter->second = pNode;
	}
}

template <class T>
bool TreeNodeService<T>::unregisterTreeNode(TreeNodeId tnid)
{
	auto iter = m_treeId2Node.find(tnid);
	if (iter == m_treeId2Node.end())
		return false;

	delete iter->second;
	m_treeId2Node.erase(tnid);
	return true;
}

template <class T>
TreeNode<T>* TreeNodeService<T>::getTreeNode(TreeNodeId tnid)
{
	auto iter = m_treeId2Node.find(tnid);
	if (iter == m_treeId2Node.end())
		return NULL;

	return iter->second;
}

template <class T>
TreeNodeService<T>::~TreeNodeService()
{
	auto iter = m_treeId2Node.begin();
	while (iter != m_treeId2Node.end())
	{
		delete iter->second;
		iter->second = NULL;
		++iter;
	}
}