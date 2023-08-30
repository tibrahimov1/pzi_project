#pragma once
#include "gui/View.h"
#include "gui/ViewSwitcher.h"
#include "gui/Navigator.h"
#include "../../common/include/VNDLib.h"
#include "gui/GridLayout.h"
#include "SwitcherUserView.h"
#include "../../common/include/Globals.h"

class VND_LIB_API UserNavigatorView : public gui::View 
{
protected:
	gui::Navigator _nav;
	gui::GridLayout _gl;
	UserSwitcher _switcherUser;
	gui::Image _imgReq;
	gui::Image _imgStat;
	gui::Image _imgTick;
public:
	UserNavigatorView();
	void refresh(td::INT4 arg);
	bool onChangedSelection(gui::Navigator* pNav);
};