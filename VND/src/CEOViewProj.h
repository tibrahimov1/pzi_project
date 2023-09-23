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
class VND_LIB_API CEOViewProj : public gui::View
{
protected:
	gui::GridLayout _gl;

	gui::Label _name;
	gui::LineEdit _Ename;

	gui::Label _manName;
	gui::LineEdit _Eman;

	gui::Label _finish;
	gui::LineEdit _Cfinish;

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

	gui::Button _btnProj;
	gui::TableEdit _te;
	gui::TableEdit _te2;

	dp::IDatabase *_db;
	dp::IDataSet* _pDS = nullptr;
	std::vector<td::INT4> _projectsToUpdate;
public:
	CEOViewProj();
	bool onClick(gui::Button *pBtn) override;
	void populateData();
	virtual bool onChangedSelection(gui::TableEdit* pTE);
	td::INT4 getIDfromTable(int rowID);

};