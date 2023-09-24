#pragma once
#include "gui/Dialog.h"
#include "../../common/include/Globals.h"
#include "../../common/include/VNDLib.h"
#include "ViewNewReq.h"

class VND_LIB_API DialogNewReq : public gui::Dialog
{
	ViewNewReq _view;
	std::function<void()> _func;
public:
	DialogNewReq(gui::Frame* pView, std::function<void()> func) :
		_view(),
		_func(func),
		gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("OK"), gui::Button::Type::Default},
			{gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(300, 100))
	{
		setCentralView(&_view);
	}
	bool onClick(gui::Dialog::Button::ID btnID, gui::Button* pBtn) {
		if (btnID == gui::Dialog::Button::ID::OK) {
			//store to DB
			_view.storetoDB();
			_func();
			return true;
		}
		else {
			return false;
		}
	}
};