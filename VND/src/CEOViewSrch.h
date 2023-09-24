#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/GridComposer.h>
#include <gui/GridLayout.h>
#include <gui/DateEdit.h>
#include <gui/ComboBox.h>
#include <gui/NumericEdit.h>
#include <gui/ProgressIndicator.h>
#include <gui/Button.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>

#include "../../common/include/VNDLib.h"
#include <gui/TableEdit.h>
class VND_LIB_API CEOViewSrch : public gui::View
{
protected:
	gui::GridLayout _gl;

	gui::Label _dod;
	gui::DateEdit _ldod;
	gui::Label _ddo;
	gui::DateEdit _lddo;
	gui::Label _pod;
	gui::NumericEdit _lpod;
	gui::Label _pdo;
	gui::NumericEdit _lpdo;

	gui::Label _bod;
	gui::NumericEdit _lbod;
	gui::Label _bdo;
	gui::NumericEdit _lbdo;
	gui::Label _manName;
	gui::LineEdit _Eman;

	gui::Label _name;
	gui::LineEdit _Ename;
	gui::Label _finish;
	gui::ComboBox _Cfinish;

	gui::Label _date;
	gui::DateEdit _Ddate;
	gui::Label _finDate;
	gui::DateEdit _DfinDate;
	gui::Label _num;
	gui::NumericEdit _Nnum;

	gui::Label _prog;
	gui::ProgressIndicator _Pprog;
	gui::Label _spec;
	gui::Button _btnFile;
	gui::Button _ucitaj;

	gui::TableEdit _te;

	dp::IDatabase* _db;
	dp::IDataSet* _pDS = nullptr;

	std::vector<std::vector<td::INT4>> _br;
	std::vector< std::vector<td::String>> _txt;
	int _vel;

public:
	CEOViewSrch();
	bool onClick(gui::Button* pBtn) override;
	void populateData();
	virtual bool onChangedSelection(gui::TableEdit* pTE);
	td::INT4 getIDfromTable(int rowID);

	bool jelveci(td::String s, td::Date d);
};