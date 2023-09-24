#pragma once
#pragma once
#include <gui/View.h>
#include <gui/Button.h>
#include <gui/Label.h>
#include <gui/Button.h>
#include <gui/LineEdit.h>
#include <gui/NumericEdit.h>
#include "ViewGrafik.h"
#include <gui/DateEdit.h>
#include <gui/DBComboBox.h>
#include <gui/FileDialog.h>
#include "../../common/include/VNDLib.h"
#include <gui/GridComposer.h>
#include "../../common/include/Globals.h"
#include <gui/GridLayout.h>
#include <td/Date.h>
#include <cnt/StringBuilder.h>

class VND_LIB_API ViewStatUser : public gui::View {
protected:
	gui::GridLayout gl;
	gui::Button _btn;
	gui::Label _lbl1;
	gui::Label _lbl2;
	dp::IDatabase * _db;
	gui::LineEdit _team;
	gui::NumericEdit _vel;
	ViewGrafik _graf;
public:
	ViewStatUser():
	_btn(tr("Burndown Grafik")),
	_lbl1(tr("Team")),
	_lbl2(tr("Velo:")),
	_graf(),
	_vel(td::int4),
	gl(3,4)
	{
		gui::GridComposer gc(gl);
		gc.appendRow(_lbl1);
		gc.appendCol(_team);
		gc.appendCol(_lbl2);
		gc.appendCol(_vel);
		gc.appendRow(_btn, -1);
		gc.appendRow(_graf, -1);
		gui::View::setLayout(&gl);
		_db = dp::getMainDatabase();

		populateData();
		_team.setAsReadOnly();
		_vel.setAsReadOnly();
	}
	void populateData() {
		//poguraj vrijednosti u team i vel
		dp::IStatementPtr pStat= _db->createStatement("SELECT a.Ime FROM Tim a, Korisnik b WHERE a.ID= b.TimID");
		dp::Columns cols(pStat->allocBindColumns(1));
		td::String ime;
		cols << "Ime" << ime;
		if (!pStat->execute())
			return;
		if (!pStat->moveNext())
			return;
		_team.setValue(ime);

		//velo racunamo koliko je radnik do sad uradio, prodji kroz tikete i saberi tezine
		updateVelo();
		//hvatamo sve tikete na timu 
		if (Globals::_currentUserID == -1)
			return;
		_graf.StaviTacke(100, 40, 48, 80);
	}
	void updateVelo() {
		dp::IStatementPtr pStat = _db->createStatement("SELECT a.Tezina FROM Tiketi a, Korisnik b, Tim c WHERE "
														"a.VlasnikID=? AND a.Stanje = 2 AND b.TimID = c.ID AND c.ProjekatID = a.ProjekatID");
		dp::Params params(pStat->allocParams());
		params << Globals::_currentUserID;
		dp::Columns cols(pStat->allocBindColumns(1));
		td::INT4 tezina;
		td::INT4 suma = 0;
		cols << tezina;
		if (!pStat->execute())
			return;
		while (pStat->moveNext()) {
			suma += tezina;
		}
		_vel.setValue(td::Variant(suma));

	}
	bool onClick(gui::Button* pBtn) {

		return true;
	}
};

