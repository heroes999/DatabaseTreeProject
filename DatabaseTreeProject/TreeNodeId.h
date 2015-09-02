#pragma once

class TreeNodeId
{		
	friend class TreeNodeIdService;
	typedef unsigned long long ULL;
public:
	bool isNull()
	{
		return m_id == 0x0;
	}

	bool operator<(const TreeNodeId& id) const
	{
		return m_id < id.m_id;
	}

	bool operator>(const TreeNodeId& id) const
	{
		return id < *this;
	}

	bool operator>=(const TreeNodeId& id) const
	{
		return !(*this < id);
	}

	bool operator<=(const TreeNodeId& id) const
	{
		return !(*this > id);
	}

	bool operator==(const TreeNodeId& id) const
	{
		return *this <= id && *this >= id;
	}

	bool operator!=(const TreeNodeId& id) const
	{
		return !(*this == id);
	}

protected:
	TreeNodeId(ULL id): m_id(id) {}

private:
	ULL m_id;
};

class NullTreeNodeId: public TreeNodeId
{
public:
	NullTreeNodeId(): TreeNodeId(0x0)
	{}
};