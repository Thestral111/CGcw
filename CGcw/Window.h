#pragma once
#include <Windows.h>
#include <iostream>
class Window
{
public:
	HWND hwnd;
	HINSTANCE hinstance;
	std::string name;
	int width;
	int height;
	int window_x;
	int window_y;
	bool keys[256];
	int mousex;
	int mousey;
	bool mouseButtons[3];

	void init(int window_width, int window_height, const std::string window_name);

	void updateMouse(int x, int y) {
		mousex = x;
		mousey = y;
	}

	void processMessages();

};