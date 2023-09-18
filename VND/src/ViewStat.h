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

#include <gui/Canvas.h>
#include <gui/Shape.h>
#include <math/math.h>
#include <gui/Image.h>
#include <gui/Symbol.h>

#include "../../common/include/VNDLib.h"
#include <gui/TableEdit.h>

#include "ViewGrafik.h"
class VND_LIB_API ViewStat : public gui::View
{
protected:
	gui::GridLayout _gl;

	gui::Label _tim;
	gui::DBComboBox _Ttim; //combo finish

	gui::Button _btnGraf;

	ViewGrafik _Graf;
	//gui::Label _statnaz;
	//gui::DBComboBox _Status;

	dp::IDatabase* _db;
	dp::IDataSet* _pDS = nullptr;
	std::vector<td::INT4> _ticksToUpdate, _ticksToDelete, _ticksToInsert;
	std::vector<std::vector<td::INT4>> _dodaj1, poc1;
	std::vector<td::String> _dodaj2, poc2;
	std::vector<cnt::SafeFullVector<td::Variant, false>> _sviRedovi;
	int pomocnaa = 1;
public:
	ViewStat();
	bool onClick(gui::Button* pBtn) override;
	bool onChangedSelection(gui::DBComboBox* combo) override;
	void populateComboBox(gui::DBComboBox& cmb, td::String naziv);
	void populateData(td::INT4 type);
};