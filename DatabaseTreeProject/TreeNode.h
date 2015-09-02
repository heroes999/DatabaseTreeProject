#pragma once

#include <vector>

template <class T> class TreeNodeService;
template <class T> class DatabaseTree;
template <class T>
class TreeNode
{
	friend class TreeNodeService<T>;
	friend class DatabaseTree<T>;
private:
	TreeNode(TreeNodeId parentId, const T& content): m_parentId(parentId), m_content(content)
	{
	}

private:
	TreeNodeId m_parentId;
	std::vector<TreeNodeId> m_childrenIds;
	T m_content;
};