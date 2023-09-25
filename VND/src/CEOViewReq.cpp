#pragma once
#include "CEOViewReq.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "DialogProj.h"
#include "../../common/include/Globals.h"
CEOViewReq::CEOViewReq() :
	_gl(4, 6)
	, _db(dp::getMainDatabase())
	, _od("Od: ")
	, _proj("Projekat: ")
	, _poz("Pozicija: ")
	, _date("Datum: ")
	, _z("Zahtjev: ")
	, _ok("Potvrdi")
	, _neok("Odbij")
{
	// postavljanje layouta
	gui::GridComposer gc(_gl);
	gc.appendRow(_od);
	gc.appendCol(_lod);
	gc.appendCol(_proj);
	gc.appendCol(_lproj);
	gc.appendCol(_poz);
	gc.appendCol(_lpoz);

	_lod.setAsReadOnly();
	_lproj.setAsReadOnly();
	_lpoz.setAsReadOnly();

	gc.appendRow(_date);
	gc.appendCol(_Ddate);
	gc.appendCol(_z);
	gc.appendCol(_lz, -1);

	_Ddate.setAsReadOnly();
	_lz.setAsReadOnly();

	_gl.insert(2, 3, _ok);
	_gl.insert(2, 5, _neok);

	_gl.insert(3, 0, _te, -1);
	gui::View::setLayout(&_gl);

	populateData();
}

bool CEOViewReq::onClick(gui::Button* pBtn) {
	int stanje = 0;
	if (pBtn == &_ok) {
		stanje = 1;
	}
	else if (pBtn == &_neok) {
		stanje = -1;
	}
	if (pBtn == &_ok || pBtn == &_neok) {
		// mjenjanje statusa u bazi podataka (-1 je odbijen, 1 je prihvacen)
		td::INT4 id, stat1;
		dp::IStatementPtr up(_db->createStatement("UPDATE Zahtjevi SET Status=? WHERE ID=?"));
		dp::Params pParams2(up->allocParams());
		pParams2  << stat1 << id;
		stat1 = stanje;
		
		int iRow = _te.getFirstSelectedRow();
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);

		id = row[5].i4Val();
		if (!up->execute()) {
			return false;
		}

		// uklanjanje reda iz tabele (tokom sljedecih ulaza u aplikaciju nece biti prikazivan zbog nove vrijednosti stanja)
		_te.beginUpdate();
		_te.removeRow(iRow);
		_te.endUpdate();
	}
	return true;
}
void CEOViewReq::populateData() {
	// kreiranje tabele
	_pDS = (_db->createDataSet("SELECT b.Ime || ' ' || b.Prezime as 'Ime i prezime', c.Ime as 'Projekat', "
		"CASE WHEN b.TipID = 2 THEN 'Projekt menadzer' WHEN b.TipID = 3 THEN 'Radnik' END AS Pozicija, a.Opis as 'Zahtjev', a.ID, b.TipID, "
		"CASE WHEN a.Datum>0 THEN CAST(a.Datum/1000000 AS TEXT) || '.' || CAST(a.Datum%100000/10000 AS TEXT) || '.' || CAST(a.Datum%10000 AS TEXT) || '.' "
		"ELSE '0' END AS 'Datum' "
		"FROM Zahtjevi a, Korisnik b, Projekti c, Tim d "
		"WHERE a.ReceiverID = 1 AND a.SenderID = b.ID AND b.TimID = d.ID AND d.ProjekatID = c.ID AND a.Status=0"));

	dp::DSColumns cols(_pDS->allocBindColumns(7));
	td::String Ime, ProjName, DatumPoc, Opis, Pozicija;
	td::INT4 zahtj, bzz1, ID, TipID;
	
	cols << "Ime i prezime" << Ime << "Projekat" << ProjName << "Pozicija" << Pozicija << "Datum" << DatumPoc << "Zahtjev"
		<< Opis << "ID" << ID << "TipID" << TipID;
	
	if (!_pDS->execute())
	{
		_pDS = nullptr;
		return;
	}

	_te.init(_pDS, { 0,1,2,3,4 });
}

td::INT4 CEOViewReq::getIDfromTable(int rowID)
{
	dp::IDataSet* pDS = _te.getDataSet();
	auto& row = pDS->getRow(rowID);
	return row[0].i4Val();
}

bool CEOViewReq::onChangedSelection(gui::TableEdit* pTE)
{
	if (pTE == &_te) {
		int iRow = _te.getFirstSelectedRow();
		if (iRow < 0) {
			_lod.toZero();
			_lproj.toZero();
			_lpoz.toZero();
			_Ddate.toZero();
			_lz.toZero();
			return true;
		}
		// ovim prikazujemo selektovani red
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);
		_lod.setValue(row[0]);
		_lproj.setValue(row[1]);
		_lpoz.setValue(row[2]);
		_lz.setValue(row[4]);

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

		return true;
	}
	return false;
}