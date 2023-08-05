#pragma once
//Ovaj bi trebao mijenjati izmedju pojedinih nivoa CEO PM i User
//Demonstriran je primjer za CEO i PM i kako koristiti
//Poslije Dialog Logina poziva se callback koji je proslijedjen
//prilikom inicijalizacije dialog logina i upravlja
//mainswitcherom koji na kraju odabira pogled na osnovu globals varijable.
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
	void showPM() {
		showView(1);
	}
	
};