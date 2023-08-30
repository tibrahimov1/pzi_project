#pragma once
#include "UserNavigatorView.h"
#include "gui/GridComposer.h"
UserNavigatorView::UserNavigatorView():
	_gl(1,2),
	_nav(3,true),
	_imgReq(":imgReq"),
	_imgStat(":imgStat"),
	_imgTick(":imgTick")
{
	gui::GridComposer _gc(_gl);
	_gc.appendRow(_nav);
	_gc.appendCol(_switcherUser);
	gui::View::setLayout(&_gl);
	setMargins(0, 0, 0, 0);
	_nav.setItem(0, &_imgReq, tr("Zahtjevi"));
	_nav.setItem(1, &_imgTick, tr("Tiketi"));
	_nav.setItem(2, &_imgStat, tr("Statistics"));
}
void UserNavigatorView::refresh(td::INT4 arg) { //arg is currentuserid
	_switcherUser.refresh(arg);
}
bool UserNavigatorView::onChangedSelection(gui::Navigator* pNav)
{
	td::UINT2 currSelection = pNav->getCurrentSelection();
	_switcherUser.onChangedView((int)currSelection);
	return true;
}