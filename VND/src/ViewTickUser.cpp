#pragma once
#include "ViewTickUser.h"
#include "td/String.h"
#include "dp/IDatabase.h"
#include "dp/IDataSet.h"
#include "gui/GridComposer.h"

ViewTickUser::ViewTickUser() :
	_gl(3, 6),
	_tick(tr("Tiket:")),
	_doc(tr("Dokument:")),
	_dif(tr("Difficulty")),
	_status(tr("Status:")),
	_stick(td::int4),
	_sdif(td::int4),
	_btn(tr("EndGet")),
	_db(dp::getMainDatabase())
{

	gui::GridComposer _gc(_gl);
	_gc.appendRow(_tick);
	_gc.appendCol(_stick);
	_gc.appendCol(_status);
	_gc.appendCol(_sstatus);
	_gc.appendCol(_doc);
	_gc.appendCol(_sdoc);
	
	_gc.appendRow(_dif);
	_gc.appendCol(_sdif);
	_gc.appendCol(_btn,-1, td::HAlignment::Center);

	_gc.appendRow(_te, 0);
	gui::View::setLayout(&_gl);
	populateData();
}

void ViewTickUser::populateData() { 
	// provjera ima li korisnik vec zaduzenih tiketa
	// type=1 ->referesh, type=0 ->init
	// ako ima samo njih ucitaj
	// ako nema prikazi sve dostupne tikete
	if (Globals::_currentUserID == -1) return;
	dp::IStatementPtr pStat = _db->createStatement("SELECT a.ID, a.Stanje, a.Tezina, a.Opis FROM Tiketi a"
		" WHERE a.VlasnikID = ? AND a.Stanje=1");
	dp::Params pParams = pStat->allocParams();
	pParams << Globals::_currentUserID;
	pStat->execute();


	td::INT4 id, stanje, Tezina;
	td::String opis, stanjeText;
	if (pStat->moveNext()) {
		_pDS = _db->createDataSet("SELECT a.ID, a.Stanje, a.Tezina, a.Opis, "
			"CASE WHEN a.Stanje = 0 THEN 'Dostupan' "
			"WHEN a.Stanje = 1 THEN 'Zauzet' "
			"WHEN a.Stanje = 2 THEN 'Zavrsen' "
			"ELSE 'Neispravan' "
			"END AS Status "
			"FROM Tiketi a WHERE a.VlasnikID = ?");
		dp::DSColumns cols(_pDS->allocBindColumns(5));
		dp::Params pParams = _pDS->allocParams();
		pParams << Globals::_currentUserID;
		cols << "ID" << td::int4 << "Stanje" << td::int4 << "Tezina" << td::int4 << "Opis" << td::string8<<"Status"<<td::string8;
		if (!_pDS->execute())
		{
			_pDS = nullptr;
			return;
		}
			_te.init(_pDS, { 0,4,2 });

	}
	else
	{
		_pDS = _db->createDataSet("SELECT a.ID, a.Stanje, a.Tezina, a.Opis, CASE "
			"WHEN a.Stanje = 0 THEN 'Dostupan' "
			"WHEN a.Stanje = 1 THEN 'Zauzet' "
			"WHEN a.Stanje = 2 THEN 'Zavrsen' "
			"ELSE 'Neispravan' "
			"END AS Status "
			"FROM Tiketi a, Korisnik b, Tim c WHERE a.Stanje = 0 AND a.ProjekatID = c.ProjekatID AND b.TimID = c.ID AND b.TipID = 3");
		dp::DSColumns cols(_pDS->allocBindColumns(5));
		cols << "ID" << td::int4 << "Stanje" << td::int4 << "Tezina" << td::int4 << "Opis" << td::string8 << "Status" << td::string8;
		if (!_pDS->execute())
		{
			_pDS = nullptr;
			return;
		}
			_te.init(_pDS, { 0,4,2 });
	}
	onChangedSelection(&_te);
}

bool ViewTickUser::onClick(gui::Button* pBtn) {
	const char* p[3] = { "Dostupan","Zauzet","Zavrsen" };
	if (pBtn == &_btn) {
		dp::IDataSet::Row &cr = _te.getCurrentRow();
		if(cr[1] < 2)
		{
			cr[1] = cr[1] + 1;
			cr[4] = p[ cr[1].i4Val() ]; //nadajmo se da ovo radi AAAAl me tpira laganini ovo ono
		}
		_te.updateRow(_te.getFirstSelectedRow());
		onChangedSelection(&_te);

		if (cr[1] == 1)
		{
			//have to uncomment this
			//savetoDB();
			dp::IDataSet* pDS = _te.getDataSet();
			//remove all but one
			for (td::INT4 i = 0; i < pDS->getNumberOfRows(); i = i + 1) {
				auto& row = pDS->getRow(i);
				if (pDS->getRow(i)[1] != 1)
				{
					_te.beginUpdate();
					_te.removeRow(i);
					_te.endUpdate();
					i = i - 1;
				}
			}
		}		
		if (cr[1] == 2) {
			dp::IDataSet* pDS = _te.getDataSet();
			//add rows like in populatedata
			_pDS = _db->createDataSet("SELECT a.ID, a.Stanje, a.Tezina, a.Opis, CASE "
				"WHEN a.Stanje = 0 THEN 'Dostupan' "
				"WHEN a.Stanje = 1 THEN 'Zauzet' "
				"WHEN a.Stanje = 2 THEN 'Zavrsen' "
				"ELSE 'Neispravan' "
				"END AS Status "
				"FROM Tiketi a, Korisnik b, Tim c WHERE a.Stanje = 0 AND a.ProjekatID = c.ProjekatID AND b.TimID = c.ID AND b.TipID = 3");
			dp::DSColumns cols(_pDS->allocBindColumns(5));
			cols << "ID" << td::int4 << "Stanje" << td::int4 << "Tezina" << td::int4 << "Opis" << td::string8 << "Status" << td::string8;
			if (!_pDS->execute())
			{
				_pDS = nullptr;
				return true;
			}
			_te.beginUpdate();
			_te.removeRow(_te.getFirstSelectedRow());
			for (td::INT4 i = 0; i < _pDS->getNumberOfRows(); i++) {
				int iRow = _te.getFirstSelectedRow() + 1;
				if (iRow < 0)
					iRow = 0;
				td::INT4 tickID = _pDS->getRow(iRow)[0].i4Val(); //o ovom razmislit
				auto& row = _te.getEmptyRow();
				auto& fullrow = _pDS->getRow(i);
				row = fullrow;

				_te.insertRow(iRow);
			}
			_te.endUpdate();
		}
	}
	return true;
}
bool ViewTickUser::onChangedSelection(gui::TableEdit* pTE)
{
	if (pTE == &_te)
	{
		int iRow = _te.getFirstSelectedRow();
		if (iRow < 0)
		{
			_stick.toZero();
			_sdoc.toZero();
			_sdif.toZero();
			_sstatus.toZero();
			return true;
		}
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);

		_stick.setValue(row[0].i4Val());
		_sdoc.setValue(row[3].strVal());
		_sdif.setValue(row[2].i4Val());
		_sstatus.setValue(row[4].strVal());

		return true;
	}
	return false;
}
void ViewTickUser::savetoDB() {
	dp::IDataSet::Row row  = _te.getCurrentRow();
	dp::IStatementPtr pStat = _db->createStatement("UPDATE Tiketi"
		" SET Stanje = ?, VlasnikID = ?"
		" WHERE	ID = ?");
	dp::Params params = pStat->allocParams();
	params << row[1] << Globals::_currentUserID << row[0];
	if (!pStat->execute())
		return;
}