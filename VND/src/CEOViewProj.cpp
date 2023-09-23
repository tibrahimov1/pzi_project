#pragma once
#include "CEOViewProj.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "DialogProj.h"
#include "../../common/include/Globals.h"
CEOViewProj::CEOViewProj():
	_gl(4,6)
	,_name(tr("ProjName"))
	,_manName(tr("ManName"))
	,_finish(tr("Finish"))
	,_date(tr("Date"))
	,_finDate(tr("FinDate"))
	,_num(tr("Pnum"))
	,_prog(tr("Prog"))
	,_spec(tr("Spec"))
	,_btnFile(tr("OpFile"))
	,_btnProj(tr("NewProj"))
	,_db(dp::getMainDatabase())
	,_Nnum(td::int4, gui::LineEdit::Messages::DoNotSend)
{
	// postavljanje layouta
	gui::GridComposer gc(_gl);
	gc.appendRow(_name);
	gc.appendCol(_Ename);
	gc.appendCol(_manName);
	gc.appendCol(_Eman);
	gc.appendCol(_finish);
	gc.appendCol(_Cfinish);

	_Ename.setAsReadOnly();
	_Eman.setAsReadOnly();
	_Cfinish.setAsReadOnly();

	gc.appendRow(_date);
	gc.appendCol(_Ddate);
	gc.appendCol(_finDate);
	gc.appendCol(_DfinDate);
	gc.appendCol(_num);
	gc.appendCol(_Nnum);

	_Ddate.setAsReadOnly();
	_DfinDate.setAsReadOnly();
	_Nnum.setAsReadOnly();

	gc.appendRow(_prog);
	gc.appendCol(_Pprog);
	gc.appendCol(_spec);
	gc.appendCol(_btnFile);
	gc.appendEmptyCols(1);
	gc.appendCol(_btnProj);
	_Pprog.setAsReadOnly();

	gc.appendRow(_te, 0);
	gui::View::setLayout(&_gl);

	populateData();
}

bool CEOViewProj::onClick(gui::Button* pBtn) {
	if (pBtn == &_btnFile) {
		gui::OpenFileDialog* s = new gui::OpenFileDialog(this, "Open file", { ".xml" });
		s->openModal(DlgID::FileSelect,this);
	}
	else if (pBtn == &_btnProj) {
		DialogProj* p = new DialogProj(this);
		p->openModal(DlgID::NewProj, this);
		p->setTitle(tr("NewProj"));
	}
	return true;
}
void CEOViewProj::populateData() {
	// kreiranje tabele
	_pDS = (_db->createDataSet("SELECT a.Ime as 'Naziv', "
		"CASE WHEN a.Zavrsen = 0 THEN 'Ne' WHEN a.Zavrsen = 1 THEN 'Da' END AS Zavrsen, "
		"CASE WHEN a.DatumPoc>0 THEN CAST(a.DatumPoc/1000000 AS TEXT) || '.' || CAST(a.DatumPoc%100000/10000 AS TEXT) || '.' || CAST(a.DatumPoc%10000 AS TEXT) || '.' "
		"ELSE '0' END AS 'Datum pocetka', "
		"CASE WHEN a.DatumKraj>0 THEN CAST(a.DatumKraj/1000000 AS TEXT) || '.' || CAST(a.DatumKraj%100000/10000 AS TEXT) || '.' || CAST(a.DatumKraj%10000 AS TEXT) || '.' "
		"ELSE '0' END AS 'Datum zavrsetka', "
		"b.Ime || ' ' || b.Prezime as 'Project manager', c.BrClanova as 'Broj ljudi', "
		" COALESCE( ( SELECT SUM(T.Tezina) FROM Tiketi T WHERE T.Stanje = 2 AND T.ProjekatID = a.ID ), 0 ) AS 'bzz2', "
		" COALESCE( ( SELECT SUM(T.Tezina) FROM Tiketi T WHERE T.ProjekatID = a.ID ), 0	) AS 'bzz1'"
		" FROM Projekti a INNER JOIN Korisnik b ON a.MenadzerID = b.ID INNER JOIN Tim c ON c.ProjekatID = a.ID WHERE a.ID > 0"));
	dp::DSColumns cols(_pDS->allocBindColumns(8));
	td::String Ime, Prezime, ProjName, DatumPoc, DatumKraj, Zavrsen;
	td::INT4 BrClanova, bzz1, bzz2;
	cols << "Naziv" << ProjName << "Project manager" << Ime << "Zavrsen" << Zavrsen << "Datum pocetka"
		<< DatumPoc << "Datum zavrsetka" << DatumKraj << "Broj ljudi" << BrClanova << "bzz1" << bzz1 << "bzz2" << bzz2;
	if (!_pDS->execute())
	{
		_pDS = nullptr;
		return;
	}
	_te.init(_pDS, { 0,1,2,3,4,5 });
}

td::INT4 CEOViewProj::getIDfromTable(int rowID)
{
	dp::IDataSet* pDS = _te.getDataSet();
	auto& row = pDS->getRow(rowID);
	return row[0].i4Val();
}

bool CEOViewProj::onChangedSelection(gui::TableEdit* pTE)
{
	if (pTE == &_te) {
		int iRow = _te.getFirstSelectedRow();
		if (iRow < 0) {
			_Ename.toZero();
			_Eman.toZero();
			_Cfinish.toZero();
			_Ddate.toZero();
			_DfinDate.toZero();
			_Nnum.toZero();
			_Pprog.toZero();
			return true;
		}
		// ovim prikazujemo selektovani red
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);
		_Ename.setValue(row[0]);
		_Eman.setValue(row[1]);
		_Cfinish.setValue(row[2]);
		_Nnum.setValue(row[5]);

		auto p = row[7]*100 / row[6];
		int mrs = p.i4Val();
		double w = mrs/100.;
		_Pprog.setValue(w);

		td::INT4 dan, mjesec, god;
		td::String a, a1;
		a = row[3];
		a1 = a.subStr(0, 1);
		dan = a1.toINT4();
		a1 = a.subStr(3, 4);
		mjesec = a1.toINT4();
		a1 = a.subStr(5, 8);
		god = a1.toINT4();
		td::Date datum(god, mjesec, dan);
		_Ddate.setValue(datum);

		a = row[4];
		a1 = a.subStr(0, 1);
		dan = a1.toINT4();
		a1 = a.subStr(3, 4);
		mjesec = a1.toINT4();
		a1 = a.subStr(5, 8);
		god = a1.toINT4();
		td::Date datum1(god, mjesec, dan);
		_DfinDate.setValue(datum1);

		return true;
	}
	return false;
}