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
#include <gui/HorizontalLayout.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <dp/IDataSetDelegate.h>

#include "../../common/include/VNDLib.h"
#include <gui/TableEdit.h>
class VND_LIB_API ViewTeam : public gui::View
{
protected:
	gui::GridLayout _gl;

	gui::Label _teamName;
	gui::LineEdit _Tname;

	gui::Label _desc;
	gui::LineEdit _Ddesc;

	gui::Button _btnList;
	gui::Button _btnNew;
	gui::HorizontalLayout _Hlbtn;
	gui::TableEdit _te;

	dp::IDatabase* _db1;
	dp::IDataSet* _pDS1 = nullptr;
	std::vector<td::INT4> _projectsToUpdate;
public:
	ViewTeam();
	bool onClick(gui::Button* pBtn) override;
	void populateComboBox(gui::ComboBox& cmb);
	void populateDSRow(dp::IDataSet::Row& row);
	bool onChangedSelection(gui::TableEdit* pTe) override;
	td::INT4 getIDfromTable(int rowID);
	void populateData();


};