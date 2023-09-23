#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/GridComposer.h>
#include <gui/GridLayout.h>
#include <gui/DateEdit.h>
#include <gui/ComboBox.h>
#include <gui/NumericEdit.h>
#include <gui/VerticalLayout.h>
#include <gui/HorizontalLayout.h>
#include <gui/ProgressIndicator.h>
#include <gui/Button.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>
#include <gui/DBComboBox.h>

#include "../../common/include/VNDLib.h"
#include <gui/TableEdit.h>

#include "ViewProjGrafik.h"

class VND_LIB_API CEOViewStat : public gui::View
{
protected:
	gui::GridLayout _gl;

	gui::Label _tim;
	gui::DBComboBox _Ttim;
	gui::Label _br;

	ViewProjGrafik _Ggraf;
	gui::VerticalLayout _Vvert, _Vvert2;
	gui::HorizontalLayout _h1, _h2, _h3;

	gui::Label _pok1;
	gui::Label _pok2;
	gui::Label _pok3;
	gui::LineEdit _pok4;
	gui::NumericEdit _pok5;
	gui::NumericEdit _pok6;

	dp::IDatabase* _db;
	dp::IDataSet* _pDS = nullptr;
public:
	CEOViewStat();
	void populateComboBox(gui::DBComboBox& cmb, td::String naziv);
	td::INT4 getIDfromTable(int rowID);
	bool onChangedSelection(gui::DBComboBox* combo);
};
