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
	populateComboBox("SELECT ID, Ime as Name FROM Tim WHERE Tim.ID!=-1 and Tim.ProjekatID!=-1");
	populateData(0);
	//setaasreadonly
}

void ViewStat::populateComboBox(td::String naziv) {
	dp::IStatementPtr pSelect = _db->createStatement(naziv.c_str());
	dp::Columns pCols = pSelect->allocBindColumns(2);
	td::String name;
	td::INT4 id;
	pCols << "ID" << id << "Name" << name;
	if (!pSelect->execute())
		assert(false);

	while (pSelect->moveNext())
	{
		_Ttim.addItem(name, id);
	}
	_Ttim.selectIndex(0);
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

		dp::IStatementPtr pSelectt234 = dp::getMainDatabase()->createStatement("SELECT COUNT(s.Tezina) as brojj FROM Tiketi s WHERE s.TimID=?");
		dp::Params pParamss234(pSelectt234->allocParams());
		pParamss234 << IDTima;

		td::INT4 brojj;
		dp::Columns colss234 = pSelectt234->allocBindColumns(1);
		colss234 << "brojj" << brojj;

		if (!pSelectt234->execute())
			return false;
		pSelectt234->moveNext();

		if (brojj == 0) {
			td::String msgText(tr("BurndownStatistics"));
			td::String informativeText;

			informativeText.format("Nema dodanih tiketa, nemoguce prikazati statistiku");
			//informativeText.format("Tim: %s\nBroj gotovih tiketa: %d\nBroj preostalih tiketa: %d\nBrzina obrade tiketa: %g [tezine tiketa po danu]\nPretpostavljeno preostalo vrijeme: %d", ime2, brojGotovih, brojOstalih, k * -1, ukupDana - brojDana);

			showAlert(msgText, informativeText);
			return true;
		}

		dp::IStatementPtr pSelectt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as ukupTez FROM Tiketi s WHERE s.TimID=?");
		dp::Params pParamss(pSelectt->allocParams());
		pParamss << IDTima;

		td::INT4 ukupTez;
		dp::Columns colss = pSelectt->allocBindColumns(1);
		colss << "ukupTez" << ukupTez;

		if (!pSelectt->execute())
			return false;
		pSelectt->moveNext();

		dp::IStatementPtr pSelecttt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as sadTez FROM Tiketi s WHERE s.TimID=? and s.Stanje!=2");
		dp::Params pParamsss(pSelecttt->allocParams());
		pParamsss << IDTima;

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


		//td::INT4 brojDana = datum.getNoOfDays() - datum1.getNoOfDays();

		td::INT4 brojac;
		td::Date dkraj2(datum1);
		for (brojac = 0;; brojac++) {
			dkraj2 += 1;
			if (dkraj2 == datum) {
				brojac++;
				break;
			}
		}

		td::INT4 brojDana = brojac;

		double k = (sadTez - ukupTez) / 1. / brojDana;
		td::INT4 ukupDana = std::round(-1. * ukupTez / k);

		if (ukupTez == sadTez) {
			td::String msgText(tr("BurndownStatistics"));
			td::String informativeText;

			informativeText.format("Nema zavrsenih tiketa, nemoguce prikazati statistiku");
			//informativeText.format("Tim: %s\nBroj gotovih tiketa: %d\nBroj preostalih tiketa: %d\nBrzina obrade tiketa: %g [tezine tiketa po danu]\nPretpostavljeno preostalo vrijeme: %d", ime2, brojGotovih, brojOstalih, k * -1, ukupDana - brojDana);

			showAlert(msgText, informativeText);
			return true;
		}
		
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

		dp::IStatementPtr pS = dp::getMainDatabase()->createStatement("SELECT COUNT(s.Tezina) as sadTez FROM Tiketi s WHERE s.TimID=? and s.stanje=2");
		dp::Params pP(pS->allocParams());
		pP << IDTima;

		td::INT4 brojGotovih,brojOstalih;
		dp::Columns kol = pS->allocBindColumns(1);
		kol << "sadTez" << brojGotovih;

		if (!pS->execute())
			return false;
		pS->moveNext();

		dp::IStatementPtr pS2 = dp::getMainDatabase()->createStatement("SELECT COUNT(s.Tezina) as sadTez1 FROM Tiketi s WHERE s.TimID=? and s.stanje!=2");
		dp::Params pP2(pS2->allocParams());
		pP2 << IDTima;

		dp::Columns kol2 = pS2->allocBindColumns(1);
		kol2 << "sadTez1" << brojOstalih;

		if (!pS2->execute())
			return false;
		pS2->moveNext();

		double wow = -1 * k;

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
		auto x = _Ttim.getNoOfItems();
		if (_Ttim.getNoOfItems() == 0) {
			return false;
		}

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

		dp::IStatementPtr pSelectt = dp::getMainDatabase()->createStatement("SELECT COUNT(s.Tezina) as brojj FROM Tiketi s WHERE s.TimID=?");
		dp::Params pParamss(pSelectt->allocParams());
		pParamss << IDTima;

		td::INT4 ukupTez, brojj;
		dp::Columns colss = pSelectt->allocBindColumns(1);
		colss << "brojj"<<brojj;

		if (!pSelectt->execute())
			return false;
		pSelectt->moveNext();

		if (brojj == 0) {
			_Graf.StaviTacke(0, 0, 0, 0, -1);
			return true;
		}

		dp::IStatementPtr pSelectt23 = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as ukupTez FROM Tiketi s WHERE s.TimID=?");
		dp::Params pParamss23(pSelectt23->allocParams());
		pParamss23 << IDTima;

		dp::Columns colss23 = pSelectt23->allocBindColumns(1);
		colss23 << "ukupTez" << ukupTez;

		if (!pSelectt23->execute())
			return false;
		pSelectt23->moveNext();

		dp::IStatementPtr pSelecttt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as sadTez FROM Tiketi s WHERE s.TimID=? and s.Stanje!=2");
		dp::Params pParamsss(pSelecttt->allocParams());
		pParamsss << IDTima;

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


		//td::INT4 brojDana = datum.getNoOfDays() - datum1.getNoOfDays();

		td::INT4 brojac;
		td::Date dkraj2(datum1);
		for (brojac = 0;; brojac++) {
			dkraj2 += 1;
			if (dkraj2 == datum) {
				brojac++;
				break;
			}
		}

		td::INT4 brojDana = brojac;

		td::Decimal4 k = (sadTez - ukupTez) / 1. / brojDana;
		td::INT4 ukupDana = std::round( - 1. * ukupTez / k);

		if (ukupTez==sadTez) {
			_Graf.StaviTacke(ukupTez, sadTez, brojDana, ukupDana, 0);
			return true;
		}

		_Graf.StaviTacke(ukupTez, sadTez, brojDana, ukupDana, 1);
	}
	return true;
}

void ViewStat::populateData(td::INT4 type) {
}

void ViewStat::refresh() {
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
		return;
	pSelect1->moveNext();

	dp::IStatementPtr pSelectt = dp::getMainDatabase()->createStatement("SELECT COUNT(s.Tezina) as brojj FROM Tiketi s WHERE s.TimID=?");
	dp::Params pParamss(pSelectt->allocParams());
	pParamss << IDTima;

	td::INT4 ukupTez, brojj;
	dp::Columns colss = pSelectt->allocBindColumns(1);
	colss << "brojj" << brojj;

	if (!pSelectt->execute())
		return;
	pSelectt->moveNext();

	if (brojj == 0) {
		_Graf.StaviTacke(0, 0, 0, 0, -1);
		return;
	}

	dp::IStatementPtr pSelectt23 = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as ukupTez FROM Tiketi s WHERE s.TimID=?");
	dp::Params pParamss23(pSelectt23->allocParams());
	pParamss23 << IDTima;

	dp::Columns colss23 = pSelectt23->allocBindColumns(1);
	colss23 << "ukupTez" << ukupTez;

	if (!pSelectt23->execute())
		return;
	pSelectt23->moveNext();


	dp::IStatementPtr pSelecttt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as sadTez FROM Tiketi s WHERE s.TimID=? and s.Stanje!=2");
	dp::Params pParamsss(pSelecttt->allocParams());
	pParamsss << IDTima;

	td::INT4 sadTez;
	dp::Columns colsss = pSelecttt->allocBindColumns(1);
	colsss << "sadTez" << sadTez;

	if (!pSelecttt->execute())
		return;
	pSelecttt->moveNext();

	dp::IStatementPtr pSelec = dp::getMainDatabase()->createStatement("SELECT DatumPoc as DatumPoc, DatumKraj as DatumKraj FROM Projekti WHERE Projekti.ID=?");
	dp::Params pParam(pSelec->allocParams());
	pParam << ID1;

	td::INT4 dat, datkraj;
	dp::Columns col = pSelec->allocBindColumns(2);
	col << "DatumPoc" << dat << "DatumKraj" << datkraj;

	if (!pSelec->execute())
		return;
	pSelec->moveNext();

	td::Date datum;
	datum.now();
	td::INT4 dan, mjesec, god;
	dan = dat / 1000000;
	mjesec = (dat - dan * 1000000) / 10000;
	god = dat % 10000;
	td::Date datum1(god, mjesec, dan);


	//td::INT4 brojDana = datum.getNoOfDays() - datum1.getNoOfDays();

	td::INT4 brojac;
	td::Date dkraj2(datum1);
	for (brojac = 0;; brojac++) {
		dkraj2 += 1;
		if (dkraj2 == datum) {
			brojac++;
			break;
		}
	}

	td::INT4 brojDana = brojac;

	td::Decimal4 k = (sadTez - ukupTez) / 1. / brojDana;
	td::INT4 ukupDana = std::round(-1. * ukupTez / k);

	if (ukupTez == sadTez) {
		_Graf.StaviTacke(ukupTez, sadTez, brojDana, ukupDana, 0);
		return;
	}

	_Graf.StaviTacke(ukupTez, sadTez, brojDana, ukupDana, 1);
	

	//_Ttim.clean();
	//_Ttim.toZero();

	dp::IStatementPtr pSelect234 = _db->createStatement("SELECT ID, Ime as Name FROM Tim WHERE Tim.ID!=-1 and Tim.ProjekatID!=-1");
	dp::Columns pCols234 = pSelect234->allocBindColumns(2);
	td::String name234;
	td::INT4 id234;
	pCols234 << "ID" << id234 << "Name" << name234;
	if (!pSelect234->execute())
		assert(false);

	while (pSelect234->moveNext())
	{
		_Ttim.addItem(name234, id234);
	}
	_Ttim.selectIndex(0);



}


