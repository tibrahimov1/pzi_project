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
class VND_LIB_API CEOViewEmpl : public gui::View
{
protected:
	gui::GridLayout _gl;

	gui::Label _ime;
	gui::LineEdit _lime;
	
	gui::Label _proj;
	gui::LineEdit _lproj;

	gui::Label _poz;
	gui::LineEdit _lpoz;

	gui::Label _d1;
	gui::DateEdit _dd1;

	gui::Label _d2;
	gui::DateEdit _dd2;

	gui::Label _pod;
	gui::Button _bpod;

	gui::Button _dodaj;
	gui::Button _obrisi;
	gui::Button _sp;

	gui::TableEdit _te;

	dp::IDatabase* _db;
	dp::IDataSet* _pDS = nullptr;

	std::vector<td::INT4> _ToUpdate, _ToDelete, _ToInsert;

	std::vector<std::vector<td::INT4>> _br;
	std::vector< std::vector<td::String>> _txt;

	td::INT4 _ID, _pm, _kor;

public:
	CEOViewEmpl();
	void populateData();
	bool onClick(gui::Button* pBtn) override;
	virtual bool onChangedSelection(gui::TableEdit* pTE);
	td::INT4 getIDfromTable(int rowID);

	bool eraseE();
	bool insertE();
	bool saveData();
};