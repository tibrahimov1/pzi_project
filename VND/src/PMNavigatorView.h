#pragma once
#include "gui/View.h"
#include "gui/ViewSwitcher.h"
#include "gui/Navigator.h"
#include "../../common/include/VNDLib.h"
#include "gui/GridLayout.h"
#include "gui/HorizontalLayout.h"
#include "SwitcherPMView.h"
#include "../../common/include/Globals.h"

class VND_LIB_API PMNavigatorView : public gui::View 
{
protected:
	gui::Navigator _nav;
	gui::HorizontalLayout _hl;
	PMSwitcher _switcherPM;
	gui::Image _imgProg;
	gui::Image _imgTeam;
	gui::Image _imgTick;
public:
	PMNavigatorView();
	void refresh(td::INT4 arg);
	void setNavigatorSelection(td::UINT2 sel)
	{
		_nav.setCurrentSelection(sel);
		_switcherPM.showView((int)sel);
	}
	virtual bool onChangedSelection(gui::Navigator* pNav)
	{
		td::UINT2 currSelection = pNav->getCurrentSelection();
		_switcherPM.showView((int)currSelection);
		return true;
	}
};