#pragma once
#include <gui/Dialog.h>
#include <gui/Frame.h>
#include "../../common/include/VNDLib.h"
#include "ViewDialogProj.h"
class VND_LIB_API DialogProj : public gui::Dialog
{
protected:
	ViewDialogProj _viewDialogProj;
	
public:
	DialogProj(gui::Frame* pView) :
		gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
								{gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(400, 100)
					)
	{
		setCentralView(&_viewDialogProj);
	}
	bool onClick(gui::Dialog::Button::ID btnID, gui::Button* pButton) override
	{
		if (btnID == gui::Dialog::Button::ID::OK) {
			td::String _filename = _viewDialogProj.getFileName();
			td::String _projName = _viewDialogProj.getName();
			td::Date _sDate = _viewDialogProj.getDate(); //start date
			td::INT4 _manID = _viewDialogProj.getID(); //manager id
			//***************************
			//write to db the new project
			//***************************
			return true;
		}
		return true;
	}
};