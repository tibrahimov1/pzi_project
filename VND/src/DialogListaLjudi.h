#pragma once
#include <gui/Dialog.h>
#include <gui/Frame.h>
#include "../../common/include/VNDLib.h"
#include "ViewDialogListaLjudi.h"
class VND_LIB_API DialogListaLjudi : public gui::Dialog
{
protected:
	ViewDialogListaLjudi _viewDialogListaLjudi;
	
public:
	DialogListaLjudi(gui::Frame* pView) :
		gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
								{gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(400, 100)
					)
	{
		setCentralView(&_viewDialogListaLjudi);
	}
	bool onClick(gui::Dialog::Button::ID btnID, gui::Button* pButton) override
	{
		if (btnID == gui::Dialog::Button::ID::OK) {
			td::String _filename = _viewDialogListaLjudi.getFileName();
			td::String _projName = _viewDialogListaLjudi.getName();
			td::Date _sDate = _viewDialogListaLjudi.getDate(); //start date
			td::INT4 _manID = _viewDialogListaLjudi.getID(); //manager id
			//***************************
			//write to db the new project
			//***************************
			return true;
		}
		return true;
	}
};