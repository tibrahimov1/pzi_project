#pragma once
#include "PMNavigatorView.h"
#include "gui/GridComposer.h"
PMNavigatorView::PMNavigatorView():
	_hl(2),
	_nav(5,true),
	_imgProg(":imgProj"),
	_imgTeam(":imgEmpl"),
	_imgTick(":imgTick"),
	_imgZah(":imgReq"),
	_imgStat(":imgStat")
{
	setMargins(0, 0, 0, 0);
	_nav.setItem(0, &_imgTick, tr("Tiketi"));
	_nav.setItem(1, &_imgTeam, tr("Employees"));
	_nav.setItem(2, &_imgProg, tr("Projects"));
	_nav.setItem(3, &_imgZah, tr("Requests"));
	_nav.setItem(4, &_imgStat, tr("Statistics"));
	_hl.append(_nav);
	_hl.append(_switcherPM);
	setLayout(&_hl);
}



void PMNavigatorView::refresh(td::INT4 arg) { //arg is currentuserid
	_switcherPM.refresh(arg);
}