#include "stdafx.h"
#include "newAStar.h"

MyAStar::MyAStar(Vec2i size, Vec2f nodeSize)
{
	rows = (int)size.y;
	columns = (int)size.x;
	this->nodeSize = nodeSize;
	pHeap = new Heap(columns*rows);
}