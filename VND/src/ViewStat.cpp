#pragma once
#include "ViewStat.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "DialogTick.h"
#include "../../common/include/Globals.h"
ViewStat::ViewStat() :
	_gl(3, 2)
	, _tim(tr("Team"))
	, _btnGraf(tr("Burndown Graph"))
	, _Ttim(td::int4)
	, _db(dp::getMainDatabase())
{
	gui::GridComposer gc(_gl);
	gc.appendRow(_tim);
	gc.appendCol(_Ttim);

	gc.appendRow(_btnGraf,0);


	gc.appendRow(_Graf, 0);

	gui::View::setLayout(&_gl);
	populateComboBox(_Ttim, "SELECT ID, Ime as Name FROM Tim WHERE Tim.ID!=-1 and Tim.ProjekatID!=-1");
	populateData(0);
	//setaasreadonly
}

void ViewStat::populateComboBox(gui::DBComboBox& cmb, td::String naziv) {
	dp::IStatementPtr pSelect = _db->createStatement(naziv.c_str());
	dp::Columns pCols = pSelect->allocBindColumns(2);
	td::String name;
	td::INT4 id;
	pCols << "ID" << id << "Name" << name;
	if (!pSelect->execute())
		assert(false);

	while (pSelect->moveNext())
	{
		cmb.addItem(name, id);
	}
	cmb.selectIndex(0);
}


bool ViewStat::onClick(gui::Button* pBtn) {

	if (pBtn == &_btnGraf) {
		td::Variant val;
		_Ttim.getValue(val);
		td::INT4 IDTima = val.i4Val();
		td::INT4 WorkRemaining;
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

		
		dp::IStatementPtr pSelect0 = dp::getMainDatabase()->createStatement("SELECT t.Ime as Ime FROM Tim t WHERE t.ID=?");
		dp::Params pParams0(pSelect0->allocParams());
		pParams0 << IDTima;

		dp::Columns cols0 = pSelect0->allocBindColumns(1);
		td::String ImeTima;
		cols0 << "Ime" << ImeTima;

		if (!pSelect0->execute())
			return false;
		pSelect0->moveNext();

		std::string ime1 = ImeTima.c_str();
		char ime2[100];
		for (int i = 0; i < ime1.length(); i++) {
			ime2[i] = ime1[i];
		}
		ime2[ime1.length()] = '\0';

		dp::IStatementPtr pS = dp::getMainDatabase()->createStatement("SELECT COUNT(s.Tezina) as sadTez FROM Tiketi s WHERE s.ProjekatID=? and s.stanje=2");
		dp::Params pP(pS->allocParams());
		pP << ID1;

		td::INT4 brojGotovih,brojOstalih;
		dp::Columns kol = pS->allocBindColumns(1);
		kol << "sadTez" << brojGotovih;

		if (!pS->execute())
			return false;
		pS->moveNext();

		dp::IStatementPtr pS2 = dp::getMainDatabase()->createStatement("SELECT COUNT(s.Tezina) as sadTez1 FROM Tiketi s WHERE s.ProjekatID=? and s.stanje!=2");
		dp::Params pP2(pS2->allocParams());
		pP2 << ID1;

		dp::Columns kol2 = pS2->allocBindColumns(1);
		kol2 << "sadTez1" << brojOstalih;

		if (!pS2->execute())
			return false;
		pS2->moveNext();



		td::String msgText(tr("BurndownStatistics"));
		td::String informativeText;
		informativeText.format("Tim: %s\nBroj gotovih tiketa: %d\nBroj preostalih tiketa: %d\nBrzina obrade tiketa: %g [tezine tiketa po danu]\nPretpostavljeno preostalo vrijeme: %d",ime2,brojGotovih,brojOstalih,k*-1,ukupDana-brojDana);

		showAlert(msgText, informativeText);
		int a = 3;
		return true;
	}
	return true;
}

bool ViewStat::onChangedSelection(gui::DBComboBox* combo) {

	if (combo == &_Ttim) {
		td::Variant val;
		_Ttim.getValue(val);
		td::INT4 IDTima = val.i4Val();
		td::INT4 WorkRemaining;
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
		td::INT4 ukupDana = std::round( - 1. * ukupTez / k);

		_Graf.StaviTacke(ukupTez, sadTez, brojDana, ukupDana);
	}
	return true;
}

void ViewStat::populateData(td::INT4 type) {
}


