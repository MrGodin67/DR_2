#pragma once

#include "keyboard.h"
#include "mouse.h"
#include "Components.h"
enum InputEvent : std::size_t
{
	ieUp,
	ieDown,
	ieLeft,
	ieRight,
	ieSpace,
	ieNumberOf
};
class Player;
class Input :
	public Component
{
	Keyboard& kbd;
	Mouse& mouse;
	
	std::bitset<ieNumberOf> flags;
	Player* player = nullptr;
public:
	Input(Keyboard& kbd,Mouse& mouse);
	~Input();
	void Init()override;
	void Update(const float& dt)override;

};

