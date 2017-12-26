#pragma once


#include "Components.h"



class PathRequest 
{
public:
	std::future<std::vector<Vec2f>> thread;
	GameObject* owner = nullptr;
	bool done = false;
	PathRequest() = default;
	

};
class MyAStar;
class newNode;
class PathfindingManager 
{
private:
	std::vector<PathRequest*> m_queue;
	// for threading
	static std::mutex lk1;
	static std::vector<Vec2f> _getPath(const Vec2f& start, const Vec2f& end);
	static MyAStar* m_pathFinder;
private:
	void refreshQueue();
	typedef std::vector<PathRequest*>::iterator pathRequestItr;
	pathRequestItr ItrStart;
	pathRequestItr ItrEnd;
public:
	PathfindingManager();;
	~PathfindingManager();
	void Initialize(const vint& mapData, const Vec2i& worldDims, const Vec2f& celldims);
	void setNode(class newNode* node);
	newNode* GetPathNode(const Vec2i& location)noexcept;
	void requestPath(const Vec2f& start, const Vec2f& end, GameObject* owner);
	void checkForDone();
	
};