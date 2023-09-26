#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/ComboBox.h>
#include <gui/LineEdit.h>
#include <gui/TableEdit.h>
#include <gui/Button.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/DateEdit.h>
#include "../../common/include/Globals.h"
#include "../../common/include/VNDLib.h"
class VND_LIB_API ViewNewReq : public gui::View 
{
protected:
	gui::Label _towards;
	gui::Label _stowards;
	gui::Label	_req;
	gui::LineEdit _sreq;

	gui::GridLayout _gl;
public:
	ViewNewReq() :
	_towards(tr("towards")),
	_stowards(tr("PM")),
	_req(tr("Request")),
	_gl(2,2)
	{
		gui::GridComposer gc(_gl);
		gc.appendRow(_towards);
		gc.appendCol(_stowards, td::HAlignment::Center);
		gc.appendRow(_req);
		gc.appendCol(_sreq);
		gui::View::setLayout(&_gl);
	}
	bool storetoDB() {
		dp::IDatabase *_db = dp::getMainDatabase();
		dp::IStatementPtr hstat = _db->createStatement("SELECT b.ProjekatID "
			" FROM Korisnik a, Tim b "
			" WHERE a.ID = ? AND a.TimID = b.ID"
		);
		dp::Params param(hstat->allocParams());
		param << Globals::_currentUserID;
		dp::Columns cols(hstat->allocBindColumns(1));
		td::INT4 projektID;
		cols << "ProjekatID" << projektID;
		if (!hstat->execute())
			return false;
		if (!hstat->moveNext())
			return false; //ovdje failamo


		dp::IStatementPtr lstat = _db->createStatement("SELECT a.ID  "
			"FROM Korisnik a, Projekti b"
			" WHERE a.ID = b.MenadzerID AND b.ID = ?"
		);
		dp::Params params(lstat->allocParams());
		params << projektID;
		dp::Columns lcols(lstat->allocBindColumns(1));
		td::INT4 manID;
		lcols << "ID" << manID;
		if (!lstat->execute())
			return false;
		if (!lstat->moveNext())
			return false;



		dp::IStatementPtr kstat = _db->createStatement("SELECT ID "
			"FROM Zahtjevi"		);
		dp::Columns kcols(kstat->allocBindColumns(1));
		td::INT4 reqID;
		kcols << "ID" << reqID;
		if (!kstat->execute())
			return false;
		while (kstat->moveNext())
		{ }



		dp::IStatementPtr pstat = _db->createStatement("INSERT INTO Zahtjevi(ID, SenderID, ReceiverID, Opis, Datum, Status) "
											"VALUES(?,?,?,?,?,?)"
		);
		dp::Params newparams (pstat->allocParams());
		td::Variant var;
		_sreq.getValue(var);
		td::Date datum;
		datum.now();
		td::INT4 datumuint = datum.getDay() * 1000000 + datum.getMonth() * 10000 + datum.getYear();
		newparams << td::Variant(td::INT4(reqID + 1)) << Globals::_currentUserID << manID << dp::toNCh(var, 40) << datumuint << td::Variant(0);
		if (!pstat->execute())
			return false;

		pstat = nullptr;
		kstat = nullptr;
		lstat = nullptr;
		_db = nullptr;
		return true;

	}
};