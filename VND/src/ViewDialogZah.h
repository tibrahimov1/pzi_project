#pragma once
#include <gui/View.h>
#include <gui/Button.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/DateEdit.h>
#include <gui/DBComboBox.h>
#include <gui/FileDialog.h>
#include "../../common/include/VNDLib.h"
#include <gui/GridComposer.h>
#include "../../common/include/Globals.h"
#include <gui/GridLayout.h>
#include <td/Date.h>
#include <cnt/StringBuilder.h>
//#include <gui/Types.h>

class VND_LIB_API ViewDialogZah : public gui::View
{
protected:
	gui::Label _prima;
	gui::DBComboBox _Pprima;
	gui::Label _zahtjev;
	gui::LineEdit _Zzahtjev;

	gui::GridLayout _gl;

public:
	ViewDialogZah() :
		_gl(2, 2) //zbog spacinga 
		, _prima(tr("NewProj"))
		, _zahtjev(tr("Request"))
		, _Pprima(td::int4)
	{
		gui::GridComposer gc(_gl);
		gc.appendRow(_prima);
		gc.appendCol(_Pprima, -1);
		gc.appendRow(_zahtjev);
		gc.appendCol(_Zzahtjev, -1);
		gui::View::setLayout(&_gl);
		populateCombo();
	}

	void populateCombo() {
		dp::IStatementPtr pStat(dp::getMainDatabase()->createStatement("SELECT ID, Ime || ' ' || Prezime as 'Ime i prezime' FROM Korisnik WHERE TipID=1"));
		dp::Columns cols(pStat->allocBindColumns(2));
		td::String name;
		td::INT4 ID;
		cols << "ID" << ID << "Ime i prezime" << name;
		if (!pStat->execute()) return;
		while (pStat->moveNext()) {
			cnt::StringBuilderSmall sb();

			_Pprima.addItem(name, ID); //NE ZNAM SABRAT STRINGOVE
		}
		_Pprima.selectIndex(0);
	};

	td::INT4 DajIDComba() {
		td::Variant val;
		_Pprima.getValue(val);
		auto vrijednost = val.i4Val();
		return vrijednost;
	}
	td::String DajText() {
		td::Variant val;
		_Zzahtjev.getValue(val);
		auto y = val.strVal();
		return y;
	}
};