#pragma once
#include "TreeNodeId.h"

class TreeNodeIdService
{
public:
	TreeNodeId generateTreeNodeId()
	{
		static unsigned long long id = 0;
		return ++id;
	}
};