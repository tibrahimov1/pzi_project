#pragma once
#include "CEOViewStat.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "DialogTick.h"
#include "../../common/include/Globals.h"

CEOViewStat::CEOViewStat() :
	_gl(3, 6)
	, _db(dp::getMainDatabase())
	, _Vvert(3)
	, _Vvert2(3)
	, _h1(2)
	, _h2(2)
	, _h3(3)
	, _pok1("Ocekivano vrijeme zavrsetka: ")
	, _pok2("Zakasnjenje: ")
	, _pok3("Burdown brzina: ")
	, _pok5(td::int4)
	, _pok6(td::decimal4)
	, _tim("Projekat: ")
	, _Ttim(td::int4)
	, _br("Burndown grafik")
{
	_Vvert.append(_pok1);
	_Vvert.append(_pok2);
	_Vvert.append(_pok3);

	_Vvert2.append(_pok4);
	_Vvert2.append(_pok5);
	_Vvert2.append(_pok6);
	_pok4.setAsReadOnly();
	_pok5.setAsReadOnly();
	_pok6.setAsReadOnly();

	_h3.append(_Vvert);
	_h3.append(_Vvert2);
	_h2.appendSpacer(1);

	gui::GridComposer gc(_gl);
	gc.appendRow(_tim);
	gc.appendCol(_Ttim);
	gc.appendRow(_Ggraf, 5);
	gc.appendCol(_h3);

	gui::View::setLayout(&_gl);
	populateComboBox(_Ttim, "SELECT a.ID, a.Ime as Name FROM Projekti a, Tim c ON c.ProjekatID = a.ID WHERE a.ID > 0");
}

void CEOViewStat::populateComboBox(gui::DBComboBox& cmb, td::String naziv) {
	dp::IStatementPtr pSelect = _db->createStatement(naziv.c_str());
	dp::Columns pCols = pSelect->allocBindColumns(2);
	td::String name;
	td::INT4 id;
	pCols << "ID" << id << "Name" << name;
	if (!pSelect->execute())
		assert(false);

	while (pSelect->moveNext()) {
		cmb.addItem(name, id);
	}
	cmb.selectIndex(0);
}

bool CEOViewStat::onChangedSelection(gui::DBComboBox* combo) {

	if (combo == &_Ttim) {
		td::Variant val;
		_Ttim.getValue(val);
		td::INT4 IDTima = val.i4Val();

		dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("SELECT p.ID as ID1 FROM Projekti p, Tim t WHERE p.ID=t.ProjekatID and t.ID=?");
		dp::Params pParams2(pSelect1->allocParams());
		pParams2 << IDTima;
		dp::Columns cols1 = pSelect1->allocBindColumns(1);
		td::INT4 ID1;
		cols1 << "ID1" << ID1;
		if (!pSelect1->execute())
			return false;
		pSelect1->moveNext();

		dp::IStatementPtr pSelectt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as ukupTez FROM Tiketi s WHERE s.ProjekatID=?");
		dp::Params pParamss(pSelectt->allocParams());
		pParamss << ID1;
		td::INT4 ukupTez;
		dp::Columns colss = pSelectt->allocBindColumns(1);
		colss << "ukupTez" << ukupTez;
		if (!pSelectt->execute())
			return false;
		pSelectt->moveNext();

		dp::IStatementPtr pSelecttt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as sadTez FROM Tiketi s WHERE s.ProjekatID=? and s.Stanje!=2");
		dp::Params pParamsss(pSelecttt->allocParams());
		pParamsss << ID1;
		td::INT4 sadTez;
		dp::Columns colsss = pSelecttt->allocBindColumns(1);
		colsss << "sadTez" << sadTez;
		if (!pSelecttt->execute())
			return false;
		pSelecttt->moveNext();

		dp::IStatementPtr pSelec = dp::getMainDatabase()->createStatement("SELECT DatumPoc as DatumPoc, DatumKraj as DatumKraj FROM Projekti WHERE Projekti.ID=?");
		dp::Params pParam(pSelec->allocParams());
		pParam << ID1;
		td::INT4 dat, datkraj;
		dp::Columns col = pSelec->allocBindColumns(2);
		col << "DatumPoc" << dat << "DatumKraj" << datkraj;
		if (!pSelec->execute())
			return false;
		pSelec->moveNext();

		td::Date datum;
		datum.now();
		td::INT4 dan, mjesec, god;
		dan = dat / 1000000;
		mjesec = (dat - dan * 1000000) / 10000;
		god = dat % 10000;
		td::Date datum1(god, mjesec, dan);
		td::INT4 brojDana = datum.getNoOfDays() - datum1.getNoOfDays();
		double k = (sadTez - ukupTez) / 1. / brojDana;
		td::INT4 ukupDana = std::round(-1. * ukupTez / k);

		_Ggraf.StaviTacke(ukupTez, sadTez, brojDana, ukupDana, 0);

		{
			td::Variant val;
			_Ttim.getValue(val);
			td::INT4 IDTima = val.i4Val();
			td::INT4 ID1 = IDTima;

			dp::IStatementPtr pSelec = dp::getMainDatabase()->createStatement("SELECT DatumPoc as DatumPoc, DatumKraj as DatumKraj FROM Projekti WHERE Projekti.ID=?");
			dp::Params pParam(pSelec->allocParams());
			pParam << ID1;

			td::INT4 dat, datkraj;
			dp::Columns col = pSelec->allocBindColumns(2);
			col << "DatumPoc" << dat << "DatumKraj" << datkraj;

			if (!pSelec->execute())
				return false;
			pSelec->moveNext();

			td::Date datum;
			datum.now();
			td::INT4 dan, mjesec, god;
			dan = dat / 1000000;
			mjesec = (dat - dan * 1000000) / 10000;
			god = dat % 10000;
			td::Date datum1(god, mjesec, dan);
			td::INT4 brojDana = datum.getNoOfDays() - datum1.getNoOfDays();
			td::Decimal4 k = (sadTez - ukupTez) / 1. / brojDana;
			td::INT4 ukupDana = std::round(-1. * ukupTez / k);
			_Ggraf.StaviTacke(ukupTez, sadTez, brojDana, ukupDana, 0);

			td::Date novi = datum1 + ukupDana;
			td::INT4 dan1 = novi.getDay(), mjesec1 = novi.getMonth(), god1 = novi.getYear();
			td::String datnov = std::to_string(dan1) + "." + std::to_string(mjesec1) + "." + std::to_string(god1);
			_pok4.setValue(datnov);

			td::INT4 dan2, mjesec2, god2;
			dan2 = datkraj / 1000000;
			mjesec2 = (datkraj - dan2 * 1000000) / 10000;
			god2 = datkraj % 10000;
			td::Date dkraj(god2, mjesec2, dan2);

			td::INT4 brojac;
			td::Date dkraj2(dkraj);
			for (brojac = 0;; brojac++) {
				dkraj2 += 1;
				if (dkraj2 == novi) {
					brojac++;
					break;
				}
			}
			_pok5.setValue(brojac);
			_pok6.setValue(-1 * k);
		}

	}
	return true;
}