#pragma once
#include <gui/View.h>
#include "MainSwitcher.h"
#include "gui/HorizontalLayout.h"
class InitView : public gui::View 
{
	MainSwitcher _mainSW;
	gui::HorizontalLayout _hl;
public:
	InitView() : 
		_hl(1),
		_mainSW()
	{
		_hl.append(_mainSW);
		setLayout(&_hl);
	}
	void showCEO() 
	{
		_mainSW.showCEO();
	}

};
