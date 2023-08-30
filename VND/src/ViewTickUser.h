#pragma once
#include "gui/View.h"
#include "gui/TableEdit.h"
#pragma once
#include "gui/LineEdit.h"
#include "gui/Button.h"
#include <gui/GridComposer.h>
#include <gui/NumericEdit.h>
#include <gui/GridLayout.h>
#include "gui/Label.h"
#include "gui/DBComboBox.h"
#include "../../common/include/VNDLib.h"
#include "../../common/include/Globals.h"
class VND_LIB_API ViewTickUser : public gui::View 
{
protected:
	gui::Label _tick;
	gui::Label _doc;
	gui::Label _status;
	gui::Label _dif;
	gui::NumericEdit _stick; //see ticket name
	gui::LineEdit _sdoc;  //see doc path
	gui::NumericEdit _sdif;
	gui::Button _btn;  //zavrsi zaduzi
	gui::TableEdit _te;
	gui::LineEdit _sstatus;
	dp::IDatabase* _db;
	dp::IDataSet* _pDS = nullptr;
	gui::GridLayout _gl;
public:
	ViewTickUser();
	bool onClick(gui::Button* pBtn) override;
	//td::INT4 getIDfromTable(int rowID);
	void populateData();
	void savetoDB();
	bool onChangedSelection(gui::TableEdit* pTE) override;


};