#pragma once
#include "TreeNodeIdService.h"
#include "TreeNodeService.h"

template <class T>
class DatabaseTree
{
private:
	TreeNodeIdService* m_pTreeNodeIdService;
	TreeNodeService* m_pTreeNodeService;
};