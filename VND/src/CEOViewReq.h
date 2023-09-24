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
class VND_LIB_API CEOViewReq : public gui::View
{
protected:
	gui::GridLayout _gl;

	gui::Label _od;
	gui::LineEdit _lod;

	gui::Label _proj;
	gui::LineEdit _lproj;

	gui::Label _poz;
	gui::LineEdit _lpoz;

	gui::Label _date;
	gui::DateEdit _Ddate;

	gui::Label _z;
	gui::LineEdit _lz;

	gui::Button _ok;
	gui::Button _neok;

	gui::TableEdit _te;

	dp::IDatabasePtr _db;
	dp::IDataSetPtr _pDS;
public:
	CEOViewReq();
	bool onClick(gui::Button* pBtn) override;
	void populateData();
	virtual bool onChangedSelection(gui::TableEdit* pTE);
	td::INT4 getIDfromTable(int rowID);
};