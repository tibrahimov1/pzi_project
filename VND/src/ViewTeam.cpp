#pragma once
#include "ViewTeam.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "DialogProj.h"
#include "../../common/include/Globals.h"
#include <gui/HorizontalLayout.h>
ViewTeam::ViewTeam() :
	_gl(3,6)
	,_teamName(tr("TeamName"))
	,_desc("Description")
	,_Hlbtn(3)
	,_btnList("Lista ljudi")
	,_btnNew("Novi tim")
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

	populateData();

	//setaasreadonly
}

bool ViewTeam::onClick(gui::Button* pBtn) {
	if (pBtn == &_btnList) {
		gui::OpenFileDialog* s = new gui::OpenFileDialog(this, "Open file", { ".xml" });
		s->openModal(DlgID::FileSelect, this);
	}
	else if (pBtn == &_btnNew) {
		DialogProj* p = new DialogProj(this);
		p->openModal(DlgID::NewProj, this);
		p->setTitle(tr("NewTeam"));
	}
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

void ViewTeam::populateData() {

	//NEKI OGROMNI PROBLEM KOJI NE ZNAM RIJESITI
	//, COUNT(CASE WHEN Ti.stanje = 0 THEN 1 END) AS Dodan
	//_pDS1 = _db1->createDataSet("SELECT T.Ime as TimName, T.BrClanova as Brcl FROM Tim AS T LEFT JOIN Korisnik AS K ON T.ID = K.TimID LEFT JOIN Tiketi AS Ti ON K.ID = Ti.VlasnikID GROUP BY T.ID, T.BrCLanova");
	_pDS1 = _db1->createDataSet("SELECT T.ID as ID, T.Ime as TimName, T.Opis as Opis,  T.BrClanova as 'Broj ljudi', COUNT(CASE WHEN Ti.stanje = 0 THEN 1 END) AS 'Dodani Tiketi',COUNT(CASE WHEN Ti.stanje = 1 THEN 1 END) AS 'Tiketi u progresu',COUNT(CASE WHEN Ti.stanje = 2 THEN 1 END) AS 'Zavrseni tiketi' FROM Tim AS T LEFT JOIN Korisnik AS K ON T.ID = K.TimID LEFT JOIN Tiketi AS Ti ON K.ID = Ti.VlasnikID GROUP BY T.ID, T.BrCLanova");
	dp::DSColumns cols(_pDS1->allocBindColumns(7));
	td::String TimName, Opis;
	td::INT4 Brcl, Dodan, Progres, Zavrsen, ID;
	cols <<"ID"<<ID << "TimName" << TimName <<"Opis" <<Opis<< "Broj ljudi" << Brcl << "Dodani Tiketi" << Dodan << "Tiketi u progresu" << Progres << "Zavrseni tiketi" << Zavrsen;
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
	_te.init(_pDS1, { 1,3,4,5,6 });
	
}