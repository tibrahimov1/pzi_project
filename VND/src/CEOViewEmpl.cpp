#pragma once
#include "CEOViewEmpl.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "CEODialogEmpl.h"
#include "../../common/include/Globals.h"
#include <string>
CEOViewEmpl::CEOViewEmpl() :
	_gl(5, 6)
	, _ime("Ime i prezime: ")
	, _proj("Projekat: ")
	, _poz("Pozicija: ")
	, _d1("StartDate")
	, _d2(tr("EndDate"))
	, _pod("Podaci: ")
	, _bpod("Otvori file")
	, _dodaj ("Novi uposlenik")
	, _obrisi("Obrisi uposlenika")
	, _db(dp::getMainDatabase())
	, _sp("Spasi promjene")
	, _ID(-1)
	, _pm(-1)
	, _kor(-1)
{
	// postavljanje layouta
	gui::GridComposer gc(_gl);
	gc.appendRow(_ime);
	gc.appendCol(_lime);
	gc.appendCol(_proj);
	gc.appendCol(_lproj);
	gc.appendCol(_poz);
	gc.appendCol(_lpoz);

	_lime.setAsReadOnly();
	_lproj.setAsReadOnly();
	_lpoz.setAsReadOnly();

	gc.appendRow(_d1);
	gc.appendCol(_dd1);
	gc.appendCol(_d2);
	gc.appendCol(_dd2);
	gc.appendCol(_pod);
	gc.appendCol(_bpod);

	_dd1.setAsReadOnly();
	_dd2.setAsReadOnly();

	_gl.insert(2, 3, _dodaj);
	_gl.insert(2, 5, _obrisi);

	_gl.insert(3, 0, _te, -1);
	_gl.insert(4, 5, _sp);
	gui::View::setLayout(&_gl);

	populateData();

}

bool CEOViewEmpl::onClick(gui::Button* pBtn) {
	if (pBtn == &_bpod) {
		gui::OpenFileDialog* s = new gui::OpenFileDialog(this, "Open file", { ".xml" });
		s->openModal(DlgID::FileSelect, this);
	}
	else if (pBtn == &_dodaj) {
		// sve vrijednosti se salju kao reference kako bi se pamtio njihov sadrzaj kroz koristenje aplikacije
		CEODialogEmpl* p = new CEODialogEmpl(this, &_te, &_br, &_txt, &_ID, &_pm, &_kor);
		p->openModal(DlgID::NewTick, this);
		p->setTitle(tr("Novi uposlenik"));
	}
	else if (pBtn == &_obrisi) {
		// prvo se sklanja red iz tabele ali ne iz baze podataka
		int iRow = _te.getFirstSelectedRow();
		td::INT4 ID = getIDfromTable(iRow);
		if (iRow < 0)
			return true;

		// nema smisla brisati radnika koji je trenutno u nekom timu, ceka se da se projekat zavrsi da ga nogira ko pravi privatnik
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);
		if (row[6].i4Val() != -1) {
			showAlert("Nemoguce brisanje", "Korisnik trenutno radi na projektu, ne moze biti obrisan!");
			return true;
		}

		_te.beginUpdate();
		_te.removeRow(iRow);
		_te.endUpdate();

		_ToDelete.push_back(ID);

		// ako se brise novododani red, nek se nikako ne unosi u datoteku na kraju
		for (auto it = _br.begin(); it != _br.end(); ++it) {
			if ((*it)[0] == ID) {
				size_t indexToRemove = std::distance(_br.begin(), it);

				_br.erase(it);
				_txt.erase(_txt.begin() + indexToRemove);
				break;
			}
		}

		return true;
	}
	else if (pBtn == &_sp) {
		saveData();
	}
	return true;
}

void CEOViewEmpl::populateData() {
	// kreiranje tabele
	_pDS = (_db->createDataSet(
		"SELECT a.Ime || ' ' || a.Prezime as 'Ime i prezime', b.Ime as ProjName, "
		"CASE WHEN b.DatumPoc>0 THEN CAST(b.DatumPoc/1000000 AS TEXT) || '.' || CAST(b.DatumPoc%100000/10000 AS TEXT) || '.' || CAST(b.DatumPoc%10000 AS TEXT) || '.' " 
		"ELSE '0' END AS 'Datum pocetka', "
		"CASE WHEN b.DatumKraj>0 THEN CAST(b.DatumKraj/1000000 AS TEXT) || '.' || CAST(b.DatumKraj%100000/10000 AS TEXT) || '.' || CAST(b.DatumKraj%10000 AS TEXT) || '.' "
		"ELSE '0' END AS 'Datum zavrsetka', "
		"CASE WHEN a.TipID = 2 THEN 'Projekt menadzer' WHEN a.TipID = 3 THEN 'Radnik' END AS Pozicija, a.ID, a.TimID "
		"FROM Projekti b, Korisnik a, Tim c WHERE a.TipID > 1 AND a.TimID = c.ID AND c.ProjekatID = b.ID"
	));
	dp::DSColumns cols(_pDS->allocBindColumns(7));
	td::INT4 ID, TimID;
	td::String Prezime, ProjName, DatumPoc, DatumKraj, Zavrsen;                                       
	cols << "Ime i prezime" << Prezime << "ProjName" << ProjName << "Pozicija" << Zavrsen << "Datum pocetka" 
		<< DatumPoc << "Datum zavrsetka" << DatumKraj << "ID" << ID << "TimID" << TimID;

	if (!_pDS->execute())
	{
		_pDS = nullptr;
		return;
	}
	_te.init(_pDS, { 0,1,2,3,4 });
}

td::INT4 CEOViewEmpl::getIDfromTable(int rowID)
{
	dp::IDataSet* pDS = _te.getDataSet();
	auto& row = pDS->getRow(rowID);
	return row[5].i4Val();
}

bool CEOViewEmpl::onChangedSelection(gui::TableEdit* pTE)
{
	if (pTE == &_te) {
		int iRow = _te.getFirstSelectedRow();
		if (iRow < 0) {
			_lime.toZero();
			_lproj.toZero();
			_lpoz.toZero();
			_dd1.toZero();
			_dd2.toZero();
			return true;
		}
		// ovim prikazujemo selektovani red
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);
		_lime.setValue(row[0]);
		_lproj.setValue(row[1]);
		_lpoz.setValue(row[2]);

		td::INT4 dan, mjesec, god;
		td::String a, a1;
		a = row[3];
		if (a.length() == 1) {
			td::Date datum;
			datum.now();
			_dd1.setValue(datum);
		}
		else {
			a1 = a.subStr(0, 1);
			dan = a1.toINT4();
			a1 = a.subStr(3, 4);
			mjesec= a1.toINT4();
			a1 = a.subStr(5, 8);
			god= a1.toINT4();
			td::Date datum(god, mjesec, dan);
			_dd1.setValue(datum);
		}
		
		a = row[4];
		if (a.length() == 1) {
			td::Date datum;
			datum.now();
			_dd2.setValue(datum);
		}
		else {
			a1 = a.subStr(0, 1);
			dan = a1.toINT4();
			a1 = a.subStr(3, 4);
			mjesec = a1.toINT4();
			a1 = a.subStr(5, 8);
			god = a1.toINT4();
			td::Date datum(god, mjesec, dan);
			_dd2.setValue(datum);
		}

		return true;
	}
	return false;
}

bool CEOViewEmpl::eraseE()
{
	// konstruisemo query
	td::INT4 id;
	dp::IStatementPtr pDeleteItem(_db->createStatement("delete from Korisnik where ID = ?"));
	dp::Params pParams2(pDeleteItem->allocParams());
	pParams2 << id;

	// brisemo red po red
	for (auto item : _ToDelete) {
		id = item;
		if (!pDeleteItem->execute()) {
			return false;
		}
	}
	return true;
}

bool CEOViewEmpl::insertE()
{
	// konstruisemo query
	td::INT4 ID, TipID, Zauzet, TimID;
	td::String Ime, Prezime, Username, Sifra;
	dp::IStatementPtr pS3 = dp::getMainDatabase()->createStatement("insert into Korisnik(ID,TipID,Ime,Prezime,Username,Sifra,Zauzet,TimID) values(?,?,?,?,?,?,?,?)");
	dp::Params pParams3(pS3->allocParams());
	pParams3 << ID << TipID << dp::toNCh(Ime, 100) << dp::toNCh(Prezime, 100) << dp::toNCh(Username, 100) << dp::toNCh(Sifra, 100) << Zauzet << TimID;

	for (int i = 0; i < _br.size(); i++) {

		// ovim se trazi najmanji nezauzeti ID koji ce se dati novom korisniku (sto se dosad nije radilo zbog potencijalnih problema s brisanjem)
		dp::IStatementPtr pS = dp::getMainDatabase()->createStatement("SELECT MIN(ID + 1) AS x FROM Korisnik WHERE ID + 1 NOT IN(SELECT ID FROM Korisnik)");
		td::INT4 x;
		dp::Columns kol = pS->allocBindColumns(1);
		kol << "x" << x;
		if (!pS->execute())
			return false;
		pS->moveNext();

		ID = x;
		TipID = _br[i][1];
		Zauzet = _br[i][2];
		TimID = _br[i][3];
		Ime = _txt[i][0]; 
		Prezime = _txt[i][1];
		Username = _txt[i][2];
		Sifra = _txt[i][3];

		// ubacujemo red po red
		if (!pS3->execute()) {
			return false;
		}
	}
	return true;
}

bool CEOViewEmpl::saveData()
{
	dp::Transaction tr(_db);
	if (!eraseE())
		return false;
	if (!insertE())
		return false;
	if (tr.commit()) {
		_ToDelete.clear();
	}
	return true;
}