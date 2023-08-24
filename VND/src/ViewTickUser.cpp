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
	populateData(0);
}

void ViewTickUser::populateData(td::INT4 type) {
	// provjera ima li korisnik vec zaduzenih tiketa
	// ako ima samo njih ucitaj
	// ako nema prikazi sve dostupne tikete
	dp::IStatementPtr pStat = _db->createStatement("SELECT a.ID, a.Stanje, a.Tezina, a.Opis FROM Tiketi a,  WHERE a.VlasnikID = ?");
	dp::Params pParams = pStat->allocParams();
	pParams << Globals::_currentUserID;
	pStat->execute();


	td::INT4 id, stanje, Tezina;
	td::String opis;
	if (pStat->moveNext()) {
		_pDS = _db->createDataSet("SELECT a.ID, a.Stanje, a.Tezina, a.Opis FROM Tiketi a WHERE a.VlasnikID = ?");
		dp::DSColumns cols(_pDS->allocBindColumns(4));
		dp::Params pParams = _pDS->allocParams();
		pParams << Globals::_currentUserID;
		cols << "ID" << id << "Stanje" << stanje << "Tezina" << Tezina << "Opis" << opis;
		if (!_pDS->execute())
		{
			_pDS = nullptr;
			return;
		}
		if (type == 0)
			_te.init(_pDS, { 0,1,2 });
		else
			_te.reload();

	}
	else
	{
		_pDS = _db->createDataSet("SELECT a.ID, a.Stanje, a.Tezina, a.Opis FROM Tiketi a, Korisnik b, Tim c WHERE a.Stanje=0 AND a.Projek"
									"atID=c.ProjektID AND b.TimID=c.ID AND b.ID = ?");
		dp::DSColumns cols(_pDS->allocBindColumns(4));
		dp::Params pParams = _pDS->allocParams();
		pParams << Globals::_currentUserID;
		cols << "ID" << id << "Stanje" << stanje << "Tezina" << Tezina << "Opis" << opis;
		if (!_pDS->execute())
		{
			_pDS = nullptr;
			return;
		}
		if (type == 0)
			_te.init(_pDS, { 0,1,2 });
		else
			_te.reload();
	}
}

void ViewTickUser::populateDSRow(dp::IDataSet::Row& row) {

}