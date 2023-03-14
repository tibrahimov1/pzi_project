#pragma once
#include <gui/ViewSwitcher.h>
#include "NavigatorView.h"
#include "ViewProj.h"
class  VND_LIB_API MainSwitcher : public gui::ViewSwitcher
{
	NavigatorView _viewNav;
	ViewProj _viewProj;
public:
	MainSwitcher():
		gui::ViewSwitcher(3) //CEO,PM,Employee
		, _viewNav()
		, _viewProj()
	{
		addView(&_viewNav);
		addView(&_viewProj);
		showView(1); //select the starting view here
	}
	void showCEO() {
		showView(0);
	}
	
};