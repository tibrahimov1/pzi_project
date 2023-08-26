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
class VND_LIB_API ViewProj : public gui::View
{
protected:
	gui::GridLayout _gl;

	gui::Label _name;
	gui::LineEdit _Ename; //edit name

	//gui::Label _manName;
	//gui::LineEdit _Eman;

	//gui::Label _finish;
	//gui::ComboBox _Cfinish; //combo finish

	gui::Label _date;
	gui::DateEdit _Ddate; //date edit date

	gui::Label _finDate;
	gui::DateEdit _DfinDate; //date findate

	gui::Label _num;
	gui::NumericEdit _Nnum; //numericedit num

	gui::Label _prog;
	gui::ProgressIndicator _Pprog; //progr bar prog

	gui::Label _spec;
	gui::Button _btnFile;

	//gui::Button _btnProj;
	gui::TableEdit _te;

	dp::IDatabase *_db;
	dp::IDataSet* _pDS = nullptr;
	std::vector<td::INT4> _projectsToUpdate;
public:
	ViewProj();
	bool onClick(gui::Button *pBtn) override;
	void populateComboBox(gui::ComboBox &cmb);
	void populateDSRow(dp::IDataSet::Row& row);
	bool onChangedValue(gui::DateEdit* dEdit) override;
	td::INT4 getIDfromTable(int rowID);
	void populateData();


};