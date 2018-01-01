#include "stdafx.h"
#include "Input.h"
#include"Player.h"

Input::Input(Keyboard& kbd, Mouse& mouse)
	:kbd(kbd),mouse(mouse)
{
}


Input::~Input()
{
}

void Input::Init()
{
	
	player = dynamic_cast<Player*>(owner);
	assert(player);
}

void Input::Update(const float & dt)
{
	flags[ieLeft]  = kbd.KeyIsPressed(VK_LEFT);
	flags[ieRight] = kbd.KeyIsPressed(VK_RIGHT);
	flags[ieUp]    = kbd.KeyIsPressed(VK_UP);
	flags[ieDown]  = kbd.KeyIsPressed(VK_DOWN);
	flags[ieSpace] = kbd.KeyIsPressed(VK_SPACE);
	player->SetInputFlags(flags);
	

}
