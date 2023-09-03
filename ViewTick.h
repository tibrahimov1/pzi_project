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
#include <gui/DBComboBox.h>

#include "../../common/include/VNDLib.h"
#include <gui/TableEdit.h>
class VND_LIB_API ViewTick : public gui::View
{
protected:
	gui::GridLayout _gl;

	gui::Label _tiket;
	gui::NumericEdit _Ntiket; //edit name

	gui::Label _tim;
	gui::DBComboBox _Ttim; //combo finish

	gui::Label _dok;
	gui::LineEdit _btnDok;

	gui::Label _status;
	gui::LineEdit _Dstat; //date edit date

	gui::Label _tezina;
	gui::NumericEdit _Dtezina; //date findate
	
	gui::Button _btnTik;

	gui::Button _btnSac;
	//gui::Button _btnUc;
	gui::Button _btnPon;
	gui::Button _btnDel;
	//gui::Label _statnaz;
	//gui::DBComboBox _Status;
	gui::TableEdit _te;
	gui::TableEdit _pomocni;

	gui::HorizontalLayout _Hlbtn;

	dp::IDatabase* _db;
	dp::IDataSet* _pDS = nullptr;
	std::vector<td::INT4> _ticksToUpdate, _ticksToDelete, _ticksToInsert;
	std::vector<std::vector<td::INT4>> _dodaj1,poc1;
	std::vector<td::String> _dodaj2,poc2;
	std::vector<cnt::SafeFullVector<td::Variant,false>> _sviRedovi;
	int pomocnaa = 1;
public:
	ViewTick();
	bool onClick(gui::Button* pBtn) override;
	void populateComboBox(gui::DBComboBox& cmb,td::String naziv);
	void populateDSRow(dp::IDataSet::Row& row);
	bool onChangedValue(gui::DateEdit* dEdit) override;
	virtual bool onChangedSelection(gui::TableEdit* pTE);
	td::INT4 getIDfromTable(int rowID);
	void populateData(td::INT4 type);
	bool canAdd();
	bool eraseTicks();
	bool insertTicks();
	bool updateTicks();
	bool saveData();
};