#pragma once
#include <gui/Dialog.h>
#include <gui/Frame.h>
#include "../../common/include/VNDLib.h"
#include "ViewDialogTeam.h"
class VND_LIB_API DialogTeam : public gui::Dialog
{
protected:
	ViewDialogTeam _viewDialogTeam;
	
public:
	DialogTeam(gui::Frame* pView) :
		gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
								{gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(400, 100)
					)
	{
		setCentralView(&_viewDialogTeam);
	}
	bool onClick(gui::Dialog::Button::ID btnID, gui::Button* pButton) override
	{
		if (btnID == gui::Dialog::Button::ID::OK) {
			td::String _filename = _viewDialogTeam.getFileName();
			td::String _projName = _viewDialogTeam.getName();
			td::Date _sDate = _viewDialogTeam.getDate(); //start date
			td::INT4 _manID = _viewDialogTeam.getID(); //manager id
			//***************************
			//write to db the new project
			//***************************
			return true;
		}
		return true;
	}
};