#pragma once
#include "Vec2.h"
#define DEAD_VALUE_FLOAT -196719671967.0f
typedef std::vector<int> vec_int_mapLine;
typedef std::vector<vec_int_mapLine> vec_int_map;

template<typename T>
static int _compareTo(T a, T b)
{
	if (a == b)
		return 0;
	if (a > b)
		return 1;

	return -1;// a < b
}

struct nodeData
{
	Vec2f position;
	int index;
	nodeData() = default;
	nodeData(Vec2f p, int i)
		:position(p), index(i)
	{}
};

class newNode
{
public:
	int gCost = 0;
	int hCost = 0;
	int fCost() { return gCost + hCost; }
	int gridCol = 0;
	int gridRow = 0;
	int heapIndex = 0;
	bool passable = true;
	class newNode* parent = nullptr;
	int compareTo( class newNode* next)
	{
		int compare =  _compareTo<int>(fCost(), next->fCost());
		
		if (compare == -1)
		{
			compare = 1;
		}
		else if(compare == 1)
		{
			compare = -1;
		}
		if (compare == 0)
		{
			if (_compareTo<int>(hCost, next->hCost) == -1)
				compare = 1;
			else
				compare = -1;
		}
		return compare;
	}
	newNode() = default;
	newNode(const bool& passable)
		:passable(passable)
	{}
};

// heap class for fast sorting of movement cost


class MyAStar
{
	
	class Heap
	{
		int currItemCount = 0;
		int maxItems;
		std::vector<newNode*> items;
		void sortDown(newNode* item)
		{
			while (true)
			{
				int indexLeft = item->heapIndex * 2 + 1;
				int indexRight = item->heapIndex * 2 + 2;
				int swapIndex = 0;
				if (indexLeft < currItemCount)
				{
					swapIndex = indexLeft;
					if (indexRight < currItemCount)
					{
						if (items[indexLeft]->compareTo(items[indexRight]) < 0)
						{
							swapIndex = indexRight;
						}
					}
					if (item->compareTo(items[swapIndex]) < 0)
					{
						swap(item, items[swapIndex]);
					}
					else
					{
						return;
					}
				}
				else
				{
					return;
				}

			}
		}
		void sortUp(newNode* item)
		{
			int parentIndex = (item->heapIndex - 1) / 2;
			while (true)
			{
				newNode* parentItem = items[parentIndex];
				if (item->compareTo(parentItem) > 0)
				{
					swap(item, parentItem);
				}
				else
				{
					break;
				}
				parentIndex = (item->heapIndex - 1) / 2;
			}
		}
		void swap(newNode* itemA, newNode* itemB)
		{
			items[itemA->heapIndex] = itemB;
			items[itemB->heapIndex] = itemA;
			int temp = itemA->heapIndex;
			itemA->heapIndex = itemB->heapIndex;
			itemB->heapIndex = temp;
		}
	public:
		Heap() = default;
		Heap(int maxItems)
		{
			this->maxItems = maxItems;
			items.resize(maxItems);
		}
		void reset()
		{
			items.clear();
			items.resize(maxItems);
			currItemCount = 0;
		}
		newNode* removeFirst()
		{
			newNode* n = items[0];
			currItemCount--;
			items[0] = items[currItemCount];
			items[0]->heapIndex = 0;
			sortDown(items[0]);
			return n;
		}
		void updateItem(newNode* item)
		{
			sortUp(item);
		}
		int count()
		{
			return currItemCount;
		}
		bool contains(newNode* item)
		{
			return items[item->heapIndex] == item;
		}
		void add(newNode* item)
		{
			item->heapIndex = currItemCount;
			items[currItemCount] = item;
			sortUp(item);
			currItemCount++;
		}
	};
private:
	Vec2f nodeSize;
	int columns;
	int rows;
	Heap* pHeap = nullptr;
	std::vector<newNode*> closed_list;
	std::vector<newNode*> nodes;
	std::vector<Vec2f> retraceSteps(newNode* start, newNode* end)
	{
		std::vector<Vec2f> positions;

		newNode* curr = end;
		while (curr != start)
		{
			// center of cell
			Vec2f pos((float)curr->gridCol * this->nodeSize.x,
				(float)curr->gridRow * this->nodeSize.y);
			positions.push_back(pos + nodeSize* 0.5f);
			curr = curr->parent;
		}
		// add start node

		Vec2f pos((float)start->gridCol * this->nodeSize.x,
			(float)start->gridRow * this->nodeSize.y);
	
		positions.push_back(pos + nodeSize* 0.5f);

		std::reverse(positions.begin(), positions.end());
		return positions;
	}
	void addToClosedList(newNode* node)
	{
		if(!isInClosedList(node))
		   closed_list.push_back(node);
	}
	bool isInClosedList(newNode* node)
	{
		auto& it = std::find(closed_list.begin(), closed_list.end(), node);
		if (it != closed_list.end())
		{
		   return true;
		}
		return false;
	}
	
	std::vector<newNode*> getNeighbours(newNode* node)
	{
		int X = node->gridCol;
		int Y = node->gridRow;
		
		int arr[3] = { -1,0,1 };// indices to loop through
		std::vector<newNode*> n;
		bool barrier = false;
		// get all possible
		for (int r = 0; r < 3; r++)
		{
			for (int c = 0; c < 3; c++)
			{
				int x = X + arr[c];
				int y = Y + arr[r];
				if (x == X && y == Y)// don't include current cell
					continue;
				if (x < 0 || x >= columns || y < 0 || y >= rows)// ensure it's in bounds
					continue;
				
				int index = y * columns + x;
				
				if (!nodes[index]->passable)
					barrier = true;// one of the adjacent cells is a barrier,so eliminate diagonal
				n.push_back(nodes[index]);
			}
		}
		// get only N,S,E,W cells
		if (barrier)
		{
			int index;
			int N = Y - 1;
			int S = Y + 1;
			int E = X + 1;
			int W = X - 1;
			n.clear();
			if (N >= 0)
			{
				index = N * columns + X;
				n.push_back(nodes[index]);
			}
			if (S < rows)
			{
				index = S * columns + X;
				n.push_back(nodes[index]);
			}

			if (W >= 0)
			{
				index = Y * columns + W;
				n.push_back(nodes[index]);
			}
			if (E < columns)
			{
				index = Y * columns + E;
				n.push_back(nodes[index]);
			}
		}
		return n;
	};
	int getDistance(newNode* nodeA, newNode* nodeB)
	{
		int distX = abs(nodeA->gridCol - nodeB->gridCol);
		int distY = abs(nodeA->gridRow - nodeB->gridRow);
		
		if (distX > distY)
				return 14 * distY + 10 * (distX - distY);
		
		return 14 * distX + 10 * (distY - distX);
	}
public:
	MyAStar() = default;
	MyAStar(Vec2i size, Vec2f nodeSize);
	MyAStar(vec_int_map map,Vec2f nodeSize)
	{
		rows = (int)map.size();
		columns = (int)map[0].size();
		this->nodeSize = nodeSize;
		pHeap = new Heap(columns*rows);
		for (int r = 0; r < rows; r++)
		{
			for (int c = 0; c < columns; c++)
			{
				nodes.push_back(new newNode());
				nodes.back()->gridCol = c ;
				nodes.back()->gridRow = r ;
				nodes.back()->passable = map[r][c] == 1 ? true : false;
			}
		};
		
		
	}
	~MyAStar()
	{
		for (auto& it : nodes)
			SafeDelete(&it);

		SafeDelete(&pHeap);
	}
	std::vector<Vec2f> getPath(Vec2f start, Vec2f end)
	{
		pHeap->reset();
		
		newNode* startNode = getNode(start);
		newNode* endNode = getNode(end);
		// if invalid position(s)
		
		
		if (startNode == endNode)
			return std::vector<Vec2f>();

		if (!startNode || !endNode)
			return std::vector<Vec2f>();
		
		if ((startNode && !startNode->passable) || (endNode && !endNode->passable))
			return std::vector<Vec2f>();


		closed_list.clear();
		pHeap->add(startNode);
		while (pHeap->count() > 0u)
		{
			newNode* curr_node = pHeap->removeFirst();
			addToClosedList(curr_node);
			if (curr_node == endNode)
			{
				return retraceSteps(startNode,endNode);
			}
			std::vector<newNode*> neighbour = getNeighbours(curr_node);
			for (auto& it : neighbour)
			{
				// eliminate these in "getNeightbours" ??? 
				if (!it->passable || isInClosedList(it))
					continue;

				// find cheapest cost
				const int newMoveCost = curr_node->gCost + getDistance(curr_node, it);
				bool inOpenList = pHeap->contains(it);
				if (newMoveCost < it->gCost || !inOpenList)
				{
					// we are here so add and set cheapest node
					it->gCost = newMoveCost;
					it->hCost = getDistance(it, endNode);
					it->parent = curr_node;
					if (!inOpenList)
						pHeap->add(it);
					else
						pHeap->updateItem(it);
				}

			}
		}
		return std::vector<Vec2f>();
	}
	
	newNode* getNodeLocation(const Vec2i& location) noexcept
	{
		int index = location.row * columns + location.column;
		assert(index >= 0);
		assert(index < nodes.size());
		return nodes[index];
	}
	newNode* getNode(const Vec2f& pos)
	{

		int row = (int)(pos.y / nodeSize.y);
		int col = (int)(pos.x / nodeSize.x);
		int index = row * columns + col;

		if (index < 0 || index >= nodes.size())
			return nullptr;

		return nodes[index];
	}
	void setPassable(int index, bool val)
	{
		if (index < 0 || index >= (rows * columns))
			return;

		nodes[index]->passable = val;
	}
	void setNode(newNode* node)
	{
		nodes.push_back(node);
	}
};
