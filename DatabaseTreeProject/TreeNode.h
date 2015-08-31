#pragma once

template <class T> class TreeNodeService;
template <class T> class DatabaseTree;
template <class T>
class TreeNode
{
	friend class TreeNodeService<T>;
	friend class DatabaseTree<T>;
public:
//private:
	TreeNode(): m_lft(-1), m_rgt(-1), m_layer(-1)
	{
	}

	TreeNode(int lft, int rgt, int layer, const T& content):
		m_lft(lft), m_rgt(rgt), m_layer(layer), m_content(content)
	{
	}

	bool isDescendantOf(TreeNode* pNode)
	{
		if (pNode == NULL)
			return false;

		if (m_lft > pNode->m_lft && m_rgt < pNode->m_rgt)
			return true;

		return false;
	}

	bool isAncestorOf(TreeNode* pNode)
	{
		if (pNode == NULL)
			return false;

		return pNode->isDescendantOf(this);
	}

private:
	int m_lft;
	int m_rgt;
	int m_layer;
	T m_content;
};