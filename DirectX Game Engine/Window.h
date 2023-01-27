#pragma once
#include <Windows.h>

class Window
{
public:
	//Ínitialize window
	bool init();
	bool broadcast();
	//Release window
	bool release();
	bool isRun();

	RECT getClientWindowRect();
	void setHWND(HWND hwnd);

	//EVENTS
	virtual void onCreate();
	virtual void onUpdate();
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();
protected:
	HWND m_hwnd;
	bool m_is_run;
};

