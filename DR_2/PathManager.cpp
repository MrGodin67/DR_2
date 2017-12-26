#include "stdafx.h"
#include "PathManager.h"
#include "Components.h"
#include "newAStar.h"
#include "Unit.h"
MyAStar* PathfindingManager::m_pathFinder = nullptr;
std::mutex PathfindingManager::lk1;

struct PathRequestDone
{
	bool operator()(PathRequest* request)
	{
		bool done = request->done;
		if (done)
			SafeDelete(&request);
		return done;
	}
};

// function for thread
 std::vector<Vec2f> PathfindingManager::_getPath(const Vec2f& start, const Vec2f& end)
{
	
	 lk1.lock();
	 std::vector<Vec2f> path  =  m_pathFinder->getPath(start, end);
	 lk1.unlock();
	
	 return path;
}

 void PathfindingManager::refreshQueue()
 {
	
	 m_queue.erase(std::remove_if(ItrStart, ItrEnd,PathRequestDone()), ItrEnd);
	 
	 ItrStart = m_queue.begin();
	 ItrEnd = m_queue.end();

	
 }

 PathfindingManager::PathfindingManager()
 {
	
 }

 PathfindingManager::~PathfindingManager()
{
	SafeDelete(&m_pathFinder );

	for (auto& it : m_queue)
		SafeDelete(&it);
}



void PathfindingManager::Initialize(const vint& mapData, const Vec2i & worldDims, const Vec2f & celldims)
{
	SafeDelete(&m_pathFinder);
	m_pathFinder = new MyAStar(worldDims, celldims);
	for (int r : Iterate(0, worldDims.y))
	{
		for (int c : Iterate(0, worldDims.x))
		{
			const int index = r * worldDims.x + c;
			newNode* n = new newNode(mapData[index] != 0);
			n->gridRow = r;
			n->gridCol = c;
			m_pathFinder->setNode(n);
		}
	}
	
}
void PathfindingManager::setNode(newNode* node)
{
	m_pathFinder->setNode(node);
}

newNode * PathfindingManager::GetPathNode(const Vec2i & location) noexcept
{
	return m_pathFinder->getNodeLocation(location);
}

void PathfindingManager::requestPath(const Vec2f& start, const Vec2f& end,GameObject * owner)
{
	
	PathRequest* req = new PathRequest();
	req->thread = std::async(_getPath, start, end);
	req->owner = owner;
	m_queue.push_back(req);
	ItrStart = m_queue.begin();
	ItrEnd = m_queue.end();
	

}

void PathfindingManager::checkForDone()
{
	if (m_queue.size() == 0)return;
	for (int i = 0; i < m_queue.size(); i++)
	{
		if (m_queue[i]->thread.valid())
		{
			std::vector<Vec2f> path = m_queue[i]->thread.get();
			if (path.size() > 0 )
			{
				Unit* unit;
				if (unit = dynamic_cast<Unit*>(m_queue[i]->owner))
				{
					unit->SetPath(path);
					m_queue[i]->done = true;
				}
				
			}
		}
	}
	refreshQueue();
}
