#pragma once

class TreeNodeId
{		
	friend class TreeNodeIdService;
	typedef unsigned long long ULL;
public:
	bool isValid()
	{
		return m_id > 0x0;
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

private:
	TreeNodeId(ULL id = 0x0): m_id(id) {}

private:
	ULL m_id;
};