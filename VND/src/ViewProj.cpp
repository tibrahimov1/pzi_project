#pragma once
#include "ViewProj.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "DialogProj.h"
#include "../../common/include/Globals.h"

ViewProj::ViewProj() :
	_gl(4, 6)
	, _name(tr("ProjName"))
	//,_manName(tr("ManName"))
	//,_finish(tr("Finish"))
	, _date(tr("Date"))
	, _finDate(tr("FinDate"))
	, _num(tr("Pnum"))
	, _prog(tr("Prog"))
	, _spec(tr("Spec"))
	, _Sspec(tr("Description"))
	//,_btnProj(tr("NewProj"))
	, _db(dp::getMainDatabase())
	, _Nnum(td::int4, gui::LineEdit::Messages::DoNotSend)
	, _Vvert(6)
	, _Vvert2(3)
	, _h1(3)
	, _h2(2)
	, _h3(3)
	, _pok1(tr("ExpEnd"))
	, _pok2(tr("Prolong"))
	, _pok3(tr("BrSpeed"))
	, _pok5(td::int4)
	, _pok6(td::decimal4)
	, _Ttim(td::int4)
	, _addTim(tr("AddTeam"))
{

	_Vvert.append(_pok1);
	_Vvert.append(_pok4);
	_Vvert.append(_pok2);
	_Vvert.append(_pok5);
	_Vvert.append(_pok3);
	_Vvert.append(_pok6);

	_pok4.setAsReadOnly();
	_pok5.setAsReadOnly();
	_pok6.setAsReadOnly();

	//_h3.append(_Ggraf);
	//_h3.appendSpacer(1);
	_h3.append(_Vvert);
	//_h3.append(_Vvert2);

	_h2.appendSpacer(1);

	gui::GridComposer gc(_gl);
	gc.appendRow(_name);
	gc.appendCol(_Ename);
	_Ename.setAsReadOnly();
	//gc.appendCol(_manName);
	//gc.appendCol(_Eman);
	//gc.appendCol(_finish);
	//gc.appendCol(_Cfinish);

	gc.appendCol(_date);
	gc.appendCol(_Ddate);
	_Ddate.setAsReadOnly();
	gc.appendCol(_finDate);
	gc.appendCol(_DfinDate);
	_DfinDate.setAsReadOnly();

	gc.appendRow(_prog);
	gc.appendCol(_Pprog);
	_Pprog.setAsReadOnly();
	gc.appendCol(_spec);
	gc.appendCol(_Sspec);
	_Sspec.setAsReadOnly();
	gc.appendCol(_num);
	gc.appendCol(_Nnum);
	_Nnum.setAsReadOnly();
	
	//gc.appendRow(_h2);
	//gc.appendCol(_Ggraf);

	_h1.appendSpacer();
	_h1.append(_Ttim);
	_h1.append(_addTim);

	gc.appendRow(_h1,0);

	gc.appendRow(_Ggraf,5);
	gc.appendCol(_h3);
	
	//gc.appendCol(_h3,0);
	//gc.appendCol(_Vvert);
	
	//gc.appendEmptyCols(1);
	//gc.appendCol(_btnProj);

	//gc.appendRow(_te, 0);
	gui::View::setLayout(&_gl);

	populateComboBox("SELECT ID, Ime as Name FROM Tim WHERE Tim.ID!=-1 and Tim.ProjekatID=-1");
	populateData(0);


	//setaasreadonly
}

void ViewProj::populateComboBox(td::String naziv) {
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

bool ViewProj::onChangedSelection(gui::DBComboBox* combo) {

	if (combo == &_Ttim) {
		/*td::Variant val;
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

		dp::IStatementPtr pInsertItem(dp::getMainDatabase()->createStatement("update Tim SET ProjekatID=? WHERE ID=?"));
		dp::Params pParams3(pInsertItem->allocParams());
		pParams3 << ID1<<IDTima;

		if (!pInsertItem->execute())
		{
			return false;
		}
		pInsertItem->moveNext();
		*/
	}
	return true;
}

bool ViewProj::onClick(gui::Button* pBtn) {
	/*if (pBtn == &_btnFile) {
		gui::OpenFileDialog* s = new gui::OpenFileDialog(this, "Open file", { ".xml" });
		s->openModal(DlgID::FileSelect,this);
	}*/
	/*else if (pBtn == &_btnProj) {
		DialogProj* p = new DialogProj(this);
		p->openModal(DlgID::NewProj, this);
		p->setTitle(tr("NewProj"));
	}*/
	if (pBtn == &_addTim) {
		td::Variant val;
		_Ttim.getValue(val);
		td::INT4 IDTima = val.i4Val();

		dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("SELECT p.ID as ID1 FROM Projekti p WHERE p.MenadzerID=?");
		dp::Params pParams2(pSelect1->allocParams());
		pParams2 << Globals::_currentUserID;

		dp::Columns cols1 = pSelect1->allocBindColumns(1);
		td::INT4 ID1;
		cols1 << "ID1" << ID1;

		if (!pSelect1->execute())
			return false;
		pSelect1->moveNext();

		dp::IStatementPtr pInsertItem(dp::getMainDatabase()->createStatement("update Tim SET ProjekatID=? WHERE ID=?"));
		dp::Params pParams3(pInsertItem->allocParams());
		pParams3 << ID1 << IDTima;

		if (!pInsertItem->execute())
		{
			return false;
		}
		pInsertItem->moveNext();

		auto poz = _Ttim.getSelectedIndex();
		_Ttim.removeItem(poz);
		_Ttim.selectIndex(0);
	}
	return true;
}
bool ViewProj::onChangedValue(gui::DateEdit* dEdit) {
	return true;
}
void ViewProj::populateData(td::INT4 type) {
	dp::IStatementPtr pSelect = (_db->createStatement("SELECT a.Ime as ProjName, CASE WHEN a.Zavrsen=0 THEN 'U progresu' ELSE 'Zavrsen' END as Status, Cast(a.DatumPoc/1000000 AS Varchar(255)) || '/' || Cast((a.DatumPoc-(a.DatumPoc/1000000)*1000000)/10000 As Varchar(255))  || '/' || Cast(a.DatumPoc%10000 AS Varchar(255)) As DatumPoc, Cast(a.DatumKraj/1000000 AS Varchar(255)) || '/' || Cast((a.DatumKraj-(a.DatumKraj/1000000)*1000000)/10000 As Varchar(255))  || '/' || Cast(a.DatumKraj%10000 AS Varchar(255)) As DatumKraj, b.Ime, b.Prezime, t.BrClanova as BrClanova, b.TimID as TimID, a.Opis as Opis"
		" FROM Projekti a, Korisnik b, Tim t WHERE a.MenadzerID=b.ID and t.ProjekatID=a.ID and a.MenadzerID=?"));
	dp::Params pParams1(pSelect->allocParams());
	pParams1 << Globals::_currentUserID;
	//td::INT4 omgg = 1;
	//pParams1 << omgg;

	dp::Columns cols(pSelect->allocBindColumns(9));
	td::String Ime, Prezime, ProjName, DatumPoc, DatumKraj, Zavrsen, Opis;
	td::INT4 BrClanova,IDTima;
	//ovdje ima sila popravki                                        
	cols << "ProjName" << ProjName << "Status" << Zavrsen << "DatumPoc" << DatumPoc << "DatumKraj" << DatumKraj << "Ime" << Ime << "Prezime" << Prezime<<"BrClanova"<<BrClanova<<"TimID"<<IDTima<<"Opis"<<Opis;
	if (!pSelect->execute())
	{
		pSelect = nullptr;
		return;
	}
	pSelect->moveNext();
	_Ename.setValue(ProjName);
	_Ddate.setValue(DatumPoc);
	_DfinDate.setValue(DatumKraj);
	//_Nnum.setValue(BrClanova);
	_Sspec.setValue(Opis);

	if (type == 1) {
		dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("SELECT p.ID as ID1 FROM Projekti p, Tim t WHERE p.ID=t.ProjekatID and t.ID=?");
		dp::Params pParams2(pSelect1->allocParams());
		pParams2 << IDTima;

		dp::Columns cols1 = pSelect1->allocBindColumns(1);
		td::INT4 ID1;
		cols1 << "ID1" << ID1;

		if (!pSelect1->execute())
			return;
		pSelect1->moveNext();

		dp::IStatementPtr pSelect1d = dp::getMainDatabase()->createStatement("SELECT count(t.ProjekatID) as CountFR from Tim t where t.ProjekatID=?");
		dp::Params pParams2d(pSelect1d->allocParams());
		pParams2d << ID1;

		dp::Columns cols1d = pSelect1d->allocBindColumns(1);
		td::INT4 CountFR;
		cols1d << "CountFR" << CountFR;

		if (!pSelect1d->execute())
			return;
		pSelect1d->moveNext();
		
		if (CountFR == 0) {
			_Nnum.setValue(CountFR);
		}
		else {
			dp::IStatementPtr pSelect1t = dp::getMainDatabase()->createStatement("SELECT Sum(t.BrClanova) as BrClan from Tim t, Projekti p WHERE t.ProjekatID=p.ID and p.ID=?");
			dp::Params pParams2t(pSelect1t->allocParams());
			pParams2t << ID1;

			dp::Columns cols1t = pSelect1t->allocBindColumns(1);
			td::INT4 BrClan;
			cols1t << "BrClan" << BrClan;

			if (!pSelect1t->execute())
				return;
			pSelect1t->moveNext();

			_Nnum.setValue(BrClan);
		}
		

		dp::IStatementPtr pSelectt23 = dp::getMainDatabase()->createStatement("SELECT COUNT(s.Tezina) as brojj FROM Tiketi s WHERE s.ProjekatID=?");
		dp::Params pParamss23(pSelectt23->allocParams());
		pParamss23 << IDTima;

		td::INT4 brojj;
		dp::Columns colss23 = pSelectt23->allocBindColumns(1);
		colss23 << "brojj" << brojj;

		if (!pSelectt23->execute())
			return;
		pSelectt23->moveNext();

		if (brojj == 0) {
			_Ggraf.StaviTacke(0, 0, 0, 0, -1);
			return;
		}

		dp::IStatementPtr pSelectt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as ukupTez FROM Tiketi s WHERE s.ProjekatID=?");
		dp::Params pParamss(pSelectt->allocParams());
		pParamss << ID1;

		td::INT4 ukupTez;
		dp::Columns colss = pSelectt->allocBindColumns(1);
		colss << "ukupTez" << ukupTez;

		if (!pSelectt->execute())
			return;
		pSelectt->moveNext();

		dp::IStatementPtr pSelecttt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as sadTez FROM Tiketi s WHERE s.ProjekatID=? and s.Stanje!=2");
		dp::Params pParamsss(pSelecttt->allocParams());
		pParamsss << ID1;

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


		td::INT4 brojDana = datum.getNoOfDays() - datum1.getNoOfDays();

		td::Decimal4 k = (sadTez - ukupTez) / 1. / brojDana;
		td::INT4 ukupDana = std::round(-1. * ukupTez / k);

		if (ukupTez == sadTez) {
			_Ggraf.StaviTacke(ukupTez, sadTez, brojDana, ukupDana, 0);
			return;
		}
		else _Ggraf.StaviTacke(ukupTez, sadTez, brojDana, ukupDana,1);

		_Pprog.setValue(brojDana / 1. / ukupDana);

		td::Date novi = datum1 + ukupDana;

		td::INT4 dan1=novi.getDay(), mjesec1=novi.getMonth(), god1=novi.getYear();

		td::String datnov=std::to_string(dan1)+"/"+std::to_string(mjesec1)+"/"+std::to_string(god1);
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
		_pok6.setValue(-1*k);
	}
}