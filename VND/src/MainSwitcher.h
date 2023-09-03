#pragma once
//Ovaj bi trebao mijenjati izmedju pojedinih nivoa CEO PM i User
//Demonstriran je primjer za CEO i PM i kako koristiti
//Poslije Dialog Logina poziva se callback koji je proslijedjen
//prilikom inicijalizacije dialog logina i upravlja
//mainswitcherom koji na kraju odabira pogled na osnovu globals varijable.
#include <gui/ViewSwitcher.h>
#include "NavigatorView.h"
#include "UserNavigatorView.h"
#include "PMNavigatorView.h"
#include "../../common/include/Globals.h"
#include "ViewProj.h"
class  VND_LIB_API MainSwitcher : public gui::ViewSwitcher
{
	NavigatorView _viewNav;
	PMNavigatorView _viewPM;
	UserNavigatorView _viewUser;
	//ViewProj _viewProj;
	//ViewTeam _viewTeam;
	//ViewTick _viewTick;
	
public:
	MainSwitcher():
		gui::ViewSwitcher(3) //CEO,PM,Employee
		, _viewNav()
		, _viewPM()
		, _viewUser()
		//, _viewTick()
	{
		addView(&_viewNav);
		addView(&_viewPM);
		addView(&_viewUser);
		//addView(&_viewTick);
		showView(1); //select the starting view here
	}
	void showCEO() {
		showView(0);
	}
	void showPM() {
		showView(1);
		_viewPM.refresh(Globals::_currentUserID);
	}
	void showUser() {
		showView(2);
		_viewUser.refresh(Globals::_currentUserID);
	}
};