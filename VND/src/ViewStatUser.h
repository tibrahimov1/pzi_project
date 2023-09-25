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
	td::INT4 _ukupDana, _ukupTez, _sadTez, _brojDana;
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
		//_vel.setAsReadOnly();
	}
	void populateData() {
		//poguraj vrijednosti u team i vel

		//velo racunamo koliko je radnik do sad uradio, prodji kroz tikete i saberi tezine
		//hvatamo sve tikete na timu 

		if (Globals::_currentUserID == -1)
			return;
		dp::IStatementPtr pStat = _db->createStatement("SELECT a.Ime FROM Tim a, Korisnik b WHERE a.ID= b.TimID");
		dp::Columns cols(pStat->allocBindColumns(1));
		td::String ime;
		cols << "Ime" << ime;
		if (!pStat->execute())
			return;
		if (!pStat->moveNext())
			return;
		_team.setValue(ime);

		updateVelo();


		dp::IStatementPtr statement = _db->createStatement("SELECT TimID FROM Korisnik WHERE ID=?");
		dp::Params params(statement->allocParams());
		params << Globals::_currentUserID;
		dp::Columns colms(statement->allocBindColumns(1));
		td::INT4 timid;
		colms << "TimID" << timid;
		if (!statement->execute())
			return;
		if (!statement->moveNext())
			return;


		//pokupi informacije o tiketima tog tima

		//posla preostalo
		td::INT4 WorkRemaining;
		dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("SELECT p.ID as ID1 FROM Projekti p, Tim t WHERE p.ID=t.ProjekatID and t.ID=?");
		dp::Params pParams2(pSelect1->allocParams());
		pParams2 << timid;
		dp::Columns cols1 = pSelect1->allocBindColumns(1);
		td::INT4 ID1;
		cols1 << "ID1" << ID1;
		if (!pSelect1->execute())
			return;
		pSelect1->moveNext();

		//izbroj tikete na timu
		dp::IStatementPtr pSelectt234 = dp::getMainDatabase()->createStatement("SELECT COUNT(s.Tezina) as brojj FROM Tiketi s WHERE s.TimID=?");
		dp::Params pParamss234(pSelectt234->allocParams());
		pParamss234 << timid;
		td::INT4 brojj;
		dp::Columns colss234 = pSelectt234->allocBindColumns(1);
		colss234 << "brojj" << brojj;
		if (!pSelectt234->execute())
			return;
		pSelectt234->moveNext();
		if (brojj == 0)
			_graf.StaviTacke(100, 20, 20, 20, 0);

		//suma ukupnih tezina
		dp::IStatementPtr pSelectt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as ukupTez FROM Tiketi s WHERE s.TimID=?");
		dp::Params pParamss(pSelectt->allocParams());
		pParamss << timid;
		td::INT4 ukupTez;
		dp::Columns colss = pSelectt->allocBindColumns(1);
		colss << "ukupTez" << ukupTez;
		if (!pSelectt->execute())
			return ;
		pSelectt->moveNext();

		//suma preostalog
		dp::IStatementPtr pSelecttt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as sadTez FROM Tiketi s WHERE s.TimID=? and s.Stanje!=2");
		dp::Params pParamsss(pSelecttt->allocParams());
		pParamsss << timid;
		td::INT4 sadTez;
		dp::Columns colsss = pSelecttt->allocBindColumns(1);
		colsss << "sadTez" << sadTez;
		if (!pSelecttt->execute())
			return;
		pSelecttt->moveNext();

		//razlika dana
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
		double k = (sadTez - ukupTez) / 1. / brojDana;
		td::INT4 ukupDana = std::round(-1. * ukupTez / k);
		if (ukupTez == sadTez)
			_graf.StaviTacke(100, 30, 49, 49, 0);

		_ukupDana = ukupDana;
		_ukupTez = ukupTez;
		_sadTez = sadTez;
		_brojDana = brojDana;
		_graf.StaviTacke(ukupTez, sadTez, brojDana, ukupDana, 1);
	}


	void updateVelo() {
		dp::IStatementPtr jaStat = _db->createStatement("SELECT SUM(a.Tezina) as Suma FROM Tiketi a, Korisnik b, Tim c WHERE "
				" a.VlasnikID=? AND a.Stanje = 2 AND b.TimID = c.ID AND c.ProjekatID = a.ProjekatID AND a.VlasnikID=b.ID "
				" AND b.TimID=a.TimID");
		dp::Params pparams(jaStat->allocParams());
		pparams << Globals::_currentUserID;
		dp::Columns colums(jaStat->allocBindColumns(1));
		td::INT4 suma = 0;
		colums << "Suma" << suma;
		if (!jaStat->execute())
			return;
		jaStat->moveNext();
		_vel.setValue(td::Variant(suma));

	}
	bool onClick(gui::Button* pBtn) {

		td::Variant val;

		dp::IStatementPtr statement = _db->createStatement("SELECT TimID FROM Korisnik WHERE ID=?");
		dp::Params params(statement->allocParams());
		params << Globals::_currentUserID;
		dp::Columns colms(statement->allocBindColumns(1));
		td::INT4 timid;
		colms << "TimID" << timid;
		if (!statement->execute())
			return false;
		if (!statement->moveNext())
			return false;


		td::INT4 WorkRemaining;
		dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("SELECT p.ID as ID1 FROM Projekti p, Tim t WHERE p.ID=t.ProjekatID and t.ID=?");
		dp::Params pParams2(pSelect1->allocParams());
		pParams2 << timid;
		dp::Columns cols1 = pSelect1->allocBindColumns(1);
		td::INT4 ID1;
		cols1 << "ID1" << ID1;
		if (!pSelect1->execute())
			return false;
		pSelect1->moveNext();


		dp::IStatementPtr pSelectt234 = dp::getMainDatabase()->createStatement("SELECT COUNT(s.Tezina) as brojj FROM Tiketi s WHERE s.TimID=?");
		dp::Params pParamss234(pSelectt234->allocParams());
		pParamss234 << timid;
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

		td::INT4 ukupTez = _ukupTez, sadTez = _sadTez;


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
		pParams0 << timid;

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
		pP << timid;

		td::INT4 brojGotovih, brojOstalih;
		dp::Columns kol = pS->allocBindColumns(1);
		kol << "sadTez" << brojGotovih;

		if (!pS->execute())
			return false;
		pS->moveNext();

		dp::IStatementPtr pS2 = dp::getMainDatabase()->createStatement("SELECT COUNT(s.Tezina) as sadTez1 FROM Tiketi s WHERE s.TimID=? and s.stanje!=2");
		dp::Params pP2(pS2->allocParams());
		pP2 << timid;

		dp::Columns kol2 = pS2->allocBindColumns(1);
		kol2 << "sadTez1" << brojOstalih;

		if (!pS2->execute())
			return false;
		pS2->moveNext();

		double wow = -1 * k;

		td::String msgText(tr("BurndownStatistics"));
		td::String informativeText;
		informativeText.format("Tim: %s\nBroj gotovih tiketa: %d\nBroj preostalih tiketa: %d\nBrzina obrade tiketa: %g [tezine tiketa po danu]\nPretpostavljeno preostalo vrijeme: %d", ime2, brojGotovih, brojOstalih, k * -1, ukupDana - brojDana);

		showAlert(msgText, informativeText);
		int a = 3;
		return true;
	}
};

