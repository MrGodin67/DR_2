#pragma once

#include "Components.h"

class Animation :
	public Component
{
	Transform* transform;
public:
	struct Sequence
	{
		std::vector<RectF> srcRects;
		ID2D1Bitmap* image = nullptr;
		float timer = 0.0f;
		float frameDelay = 0.0f;
		std::size_t current_index = 0ull;
		RectF GetCurrentSrcRect()const
		{
			return srcRects[current_index];
		}
	};
	std::unordered_map<std::string, Sequence> sequences;
	Sequence* currentSequence = nullptr;
	int index = 0;
	
public:
	Animation();
	~Animation();
	void AddSequence(const std::string name, const Sequence& seq);
	void StartSequenceByName(const std::string& name);
	void Draw()override;
	void Update(const float& dt)override;
	void Init()override;
	Sequence* CurrentSequence();
	
};

