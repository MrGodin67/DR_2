#pragma once
#include <windows.h>
#include "mouse.h"
#include "keyboard.h"
#include <WindowsX.h>

#include "InputManager.h"
#include <memory>
const static bool FULL_SCREEN = false;
const static int SCREEN_WIDTH = 800;
const static int SCREEN_HEIGHT= 600;
const static WCHAR* APPLICATION_NAME = L"Defenders of the Cause";

class Direct3DWindow
{
	friend class Game;
public:
	Direct3DWindow();
	Direct3DWindow(int scrnWidth, int scrnHeight);
	Direct3DWindow(const Direct3DWindow&);
	~Direct3DWindow();

	
	void Shutdown();
	bool ProcessMessage();
	int ScreenWidth();
	int ScreenHeight();
	
	void ScreenHalfExtents(float& x, float& y);
	HWND& WindowHandle();
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows();
	void ShutdownWindows();
	
	
private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;
	int m_screenHeight;
	int m_screenWidth;
	
	Mouse mouse;
	Keyboard kbd;
	InputManager m_input;
};


static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


static Direct3DWindow* ApplicationHandle = 0;
