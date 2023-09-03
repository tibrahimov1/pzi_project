#pragma once
#include "ViewTeam.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "DialogProj.h"
#include "DialogListaLjudi.h"
#include "DialogTeam.h"
#include "../../common/include/Globals.h"
#include <gui/HorizontalLayout.h>
ViewTeam::ViewTeam() :
	_gl(3,6)
	,_teamName(tr("TeamName"))
	,_desc(tr("Description"))
	,_Hlbtn(3)
	,_btnList(tr("ListOfPeople"))
	,_btnNew(tr("NewTeam"))
	,_db1(dp::getMainDatabase())
{
	_Hlbtn.appendSpacer();
	_Hlbtn.append(_btnList);
	_Hlbtn.append(_btnNew);

	gui::GridComposer gc(_gl);
	gc.appendRow(_teamName);
	gc.appendCol(_Tname);
	gc.appendEmptyCols(2);
	gc.appendCol(_desc);
	gc.appendCol(_Ddesc);
	
	gc.appendRow(_Hlbtn,0);
	gc.appendRow(_te, 0);
	gui::View::setLayout(&_gl);

	populateData(0);

	//setaasreadonly
}

bool ViewTeam::onClick(gui::Button* pBtn) {
	if (pBtn == &_btnList) {
		DialogListaLjudi* p = new DialogListaLjudi(this, &_te);
		p->openModal(DlgID::NewProj, this);
		p->setTitle(tr("ListOfPeople"));
	}
	else if (pBtn == &_btnNew) {
		DialogTeam* p = new DialogTeam(this, &_te);
		p->openModal(DlgID::NewProj, this);
		p->setTitle(tr("NewTeam"));
	}

	int a = 23;
	return true;
}
bool ViewTeam::onChangedSelection(gui::TableEdit* pTE)
{

	if (pTE == &_te)
	{
		int iRow = _te.getFirstSelectedRow();
		if (iRow < 0)
		{
			_Tname.toZero();
			_Ddesc.toZero();
			return true;
		}
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);
		_Tname.setValue(row[1]);
		_Ddesc.setValue(row[2]);
		//Dozvoljava samo 2 zbog nekog razloga??
		//
		//auto a = row[5];
		//
		return true;
	}
	return false;

	//return true;
}

void ViewTeam::populateDSRow(dp::IDataSet::Row& row)
{
	/*td::Variant val;
	_Ntiket.getValue(val);
	row[0].setValue(val);
	_btnDok.getValue(val);
	row[5].setValue(val);
	_Dstat.getValue(val);
	row[3].setValue(val);
	_Dtezina.getValue(val);
	row[4].setValue(val);


	_Ttim.getValue(val);
	row[1].setValue(val);
	row[2].setValue(_Ttim.getSelectedText());*/
}

void ViewTeam::populateData(td::INT4 type) {


	//NEKI OGROMNI PROBLEM KOJI NE ZNAM RIJESITI
	//, COUNT(CASE WHEN Ti.stanje = 0 THEN 1 END) AS Dodan
	//_pDS1 = _db1->createDataSet("SELECT T.Ime as TimName, T.BrClanova as Brcl FROM Tim AS T LEFT JOIN Korisnik AS K ON T.ID = K.TimID LEFT JOIN Tiketi AS Ti ON K.ID = Ti.VlasnikID GROUP BY T.ID, T.BrCLanova");
	_pDS1 = _db1->createDataSet("SELECT T.ID as ID, T.Ime as TimName, T.Opis as Opis,  T.BrClanova as 'Broj ljudi', CAST(12.53 AS DECIMAL(7,2) ) as 'Brzina', COUNT(CASE WHEN Ti.stanje = 0 THEN 1 END) AS 'Dodani Tiketi',COUNT(CASE WHEN Ti.stanje = 1 THEN 1 END) AS 'Tiketi u progresu',COUNT(CASE WHEN Ti.stanje = 2 THEN 1 END) AS 'Zavrseni tiketi' FROM Tim AS T LEFT JOIN Projekti AS P ON T.ProjekatID = P.ID LEFT JOIN Tiketi AS Ti ON P.ID = Ti.ProjekatID Where t.ID!=-1 GROUP BY T.ID, T.BrCLanova");
	dp::DSColumns cols(_pDS1->allocBindColumns(8));
	td::String TimName, Opis;
	td::INT4 Brcl, Dodan, Progres, Zavrsen, ID;
	double Brzina;
	cols <<"ID"<<ID << "TimName" << TimName <<"Opis" <<Opis<< "Broj ljudi" << Brcl <<"Brzina"<<Brzina << "Dodani Tiketi" << Dodan << "Tiketi u progresu" << Progres << "Zavrseni tiketi" << Zavrsen;
	//cols << "TimName" << TimName << "Brcl" << Brcl;// << "Dodan" << Dodan;
	/*_pDS1 = _db1->createDataSet(" SELECT T.Ime as TimName, T.BrClanova as Brcl,COUNT(CASE WHEN Ti.stanje = 0 THEN 1 END) AS Dodan,COUNT(CASE WHEN Ti.stanje = 1 THEN 1 END) AS Progres,COUNT(CASE WHEN Ti.stanje = 2 THEN 1 END) AS Zavrsen FROM Tim AS T LEFT JOIN Korisnik AS K ON T.ID = K.TimID LEFT JOIN Tiketi AS Ti ON K.ID = Ti.VlasnikID GROUP BY T.ID, T.BrCLanova");
	dp::DSColumns cols(_pDS1->allocBindColumns(5));
	td::String TimName;
	td::INT4 Brcl, Dodan, Progres, Zavrsen;
	//ovdje ima sila popravki                                        
	cols << "TimName" << TimName << "Brcl" << Brcl<<"Dodan"<<Dodan<<"Progres"<<Progres<<"Zavrsen"<<Zavrsen;*/
	if (!_pDS1->execute())
	{
		_pDS1 = nullptr;
		return;
	}
	//_te.init(_pDS1, { 0,1,2});
	if (type == 0) {
		_te.init(_pDS1, { 1,3,4,5,6,7 });
		return;
	}
	
	gui::TableEdit _te1;
	_te1.init(_pDS1, { 1,3,4,5,6,7 });
	std::vector<cnt::SafeFullVector<td::Variant, false>> _sviRedovi;

	dp::IDataSet* pDS = _te1.getDataSet();
	for (size_t i = 0; i < pDS->getNumberOfRows(); ++i)
	{
		auto& row = pDS->getRow(i);
		_sviRedovi.push_back(row);
	}

	_te.reload();
	for (size_t i = 0; i < _sviRedovi.size(); ++i)
	{
		_te.beginUpdate();
		auto& row = _te.getEmptyRow();
		row[0] = _sviRedovi[i][0];
		row[1] = _sviRedovi[i][1];
		row[2] = _sviRedovi[i][2];
		row[3] = _sviRedovi[i][3];
		row[5] = _sviRedovi[i][5];
		row[6] = _sviRedovi[i][6];
		row[7] = _sviRedovi[i][7];

		dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("SELECT p.DatumPoc as DatPoc, p.ID as ID1 FROM Projekti p, Tim t WHERE p.ID=t.ProjekatID and t.ID=?");
		dp::Params pParams2(pSelect1->allocParams());
		pParams2 << row[0];
		
		dp::Columns cols1 = pSelect1->allocBindColumns(2);
		td::INT4 dat,ID1;
		cols1 << "DatPoc"<<dat<<"ID1"<<ID1;

		if (!pSelect1->execute())
			return;
		pSelect1->moveNext();

		if (ID1 == -1) {
			row[4] = 0;
			break;
		}
		
		td::Date datum;
		datum.now();
		td::INT4 dan, mjesec, god;
		dan = dat / 1000000;
		mjesec = (dat - dan * 1000000) / 10000;
		god = dat % 10000;
		td::Date datum1(god,mjesec,dan);
		td::Date razlika;
		razlika = datum - datum1;
		td::INT4 brojDana = razlika.getNoOfDays();

		dp::IStatementPtr pSelectt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as ukupTez FROM Tiketi s WHERE s.ProjekatID=?");
		dp::Params pParamss(pSelectt->allocParams());
		pParamss << ID1;

		td::INT4 ukupTez;
		dp::Columns colss = pSelectt->allocBindColumns(1);
		colss << "ukupTez"<<ukupTez;

		if (!pSelectt->execute())
			return;
		pSelectt->moveNext();
		// sve dijelis sa sumom tezine zavrsenih tiketa

		double x;
			if (ukupTez != 0)x = 1. / ukupTez * brojDana / 24.;
			else x = 0;
		row[4] = x;
		//row[4] = _sviRedovi[i][4]; OVAJ JE BITANNNN
		
		_te.insertRow(i);
		_te.endUpdate();
		//_te.insertRow(row);
	}


	
	
}