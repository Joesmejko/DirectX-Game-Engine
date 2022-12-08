#pragma once
#include <Windows.h>

class Window
{
public:
	//initialize the window
	bool init();
	bool broadcast();
	//Release the window
	bool release();
	bool isRun();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	//EVENTS
	virtual void onCreate()=0;
	virtual void onUpdate()=0;
	virtual void onDestroy();
protected:
	HWND m_hwnd;
	bool m_is_run;
};

