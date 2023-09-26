#include "ViewReqUser.h"
ViewReqUser::ViewReqUser() :
	_towards(tr("towards")),
	_req(tr("Request")),
	_date(tr("rDate")),
	_status(tr("Status")),
	_btn(tr("NewReq")),
	_hidbtn(tr("Shh")),
	_gl(4,5),
	_db(dp::getMainDatabase())
{
	gui::GridComposer gc(_gl);

	gc.appendRow(_towards);
	gc.appendCol(_stowards);
	gc.appendCol(_req);
	gc.appendCol(_sreq, -1);

	gc.appendRow(_date);
	gc.appendCol(_sdate);
	gc.appendCol(_status);
	gc.appendCol(_sstatus);
	gc.appendEmptyCols(1);

	gc.appendRow(_hidbtn);
	gc.appendEmptyCols(3);
	gc.appendCol(_btn);

	gc.appendRow(_te,-1);
	gui::View::setLayout(&_gl);
	_hidbtn.hide(1,1);
	_stowards.setAsReadOnly();
	_sreq.setAsReadOnly();
	_sstatus.setAsReadOnly();
	populateData();
}

bool ViewReqUser::onClick(gui::Button* pBtn) {
	if (pBtn == &_btn) {
		//otvori novi dijalog
		DialogNewReq* pDlg = new DialogNewReq(this, (std::function<void()>)std::bind(&ViewReqUser::refresh, this));
		pDlg->openModal();
		pDlg->setTitle(tr("NewReq"));
	}
	return true;
}
void ViewReqUser::populateData() {
	if (Globals::_currentUserID < 0)
		return;

	_pDs = _db->createDataSet("SELECT CASE WHEN a.Datum>0 THEN CAST(a.Datum/1000000 AS TEXT) || '.' || CAST(a.Datum%100000/10000 AS TEXT) || '.' || CAST(a.Datum%10000 AS TEXT) || '.'"
		" ELSE '0' END AS Date, "
		
		
		"a.Status, b.Ime, b.Prezime, a.Opis, "
		"CASE WHEN a.Status = 0 THEN 'Primljen' "
		"WHEN a.Status = 1 THEN 'Odgovoren' "
		"ELSE 'Problem' "
		"END AS Stanje "
		"FROM Zahtjevi a, Korisnik b "
		"WHERE( a.SenderID = ? OR a.ReceiverID = ?) AND b.ID = a.ReceiverID"
	);



	/*_pDs = _db->createDataSet("SELECT a.Datum, a.Status, b.Ime, b.Prezime, a.Opis, "
													"CASE WHEN a.Status = 0 THEN 'Primljen' "
													"WHEN a.Status = 1 THEN 'Odgovoren' "
													"ELSE 'Problem' "
													"END AS Stanje "
													"FROM Zahtjevi a, Korisnik b "
													"WHERE( a.SenderID = ? OR a.ReceiverID = ?) AND b.ID = a.ReceiverID"
	);*/
	dp::Params params = _pDs->allocParams();
	params << Globals::_currentUserID << Globals::_currentUserID;
	dp::DSColumns cols(_pDs->allocBindColumns(6));
	cols << "Date" << td::string8 << "Status" << td::int4 << "Ime" << td::string8 << "Prezime" << td::string8 << "Opis" << td::string8 << "Stanje" << td::string8;
	if (!_pDs->execute()) {
		_pDs = nullptr;
		return;
	}
	_te.init(_pDs, {2, 3, 0, 4, 5});
	if (_pDs->getNumberOfRows() > 0)
		onChangedSelection(&_te);
}
bool ViewReqUser::onChangedSelection(gui::TableEdit* pTe) {

	dp::IDataSet *pDS = pTe->getDataSet();
	int irow = pTe->getFirstSelectedRow();
	dp::IDataSet::Row row = pDS->getRow(irow);

	cnt::StringBuilderSmall strbuild;
	strbuild << row[2].strVal() << " " << row[3].strVal();

	_stowards.setValue(strbuild.toString());
	_sreq.setValue(row[4]);
	_sdate.setValue(row[0]);
	_sstatus.setValue(row[5]);
	return true;
}
void ViewReqUser::refresh() {
	dp::IStatementPtr sStat = _db->createStatement("SELECT CASE WHEN a.Datum>0 THEN CAST(a.Datum/1000000 AS TEXT) || '.' || CAST(a.Datum%100000/10000 AS TEXT) || '.' || CAST(a.Datum%10000 AS TEXT) || '.'"
		" ELSE '0' END AS Date, "


		"a.Status, b.Ime, b.Prezime, a.Opis, "
		"CASE WHEN a.Status = 0 THEN 'Primljen' "
		"WHEN a.Status = 1 THEN 'Odgovoren' "
		"ELSE 'Problem' "
		"END AS Stanje "
		"FROM Zahtjevi a, Korisnik b "
		"WHERE( a.SenderID = ? OR a.ReceiverID = ?) AND b.ID = a.ReceiverID"
	);
	dp::Params params(sStat->allocParams());
	dp::Columns cols(sStat->allocBindColumns(6));
	td::String datum;
	td::INT4 status;
	td::String ime, prezime, stanje, opis;
	cols << "Date" << datum << "Status" << status << "Ime" << ime << "Prezime" << prezime << "Opis" << opis << "Stanje" << stanje;
	params << Globals::_currentUserID;
	if (!sStat->execute()) {
		return;
	}
	while(sStat->moveNext());
	dp::IDataSet::Row newrow = _te.getEmptyRow();
	newrow[0] = datum;
	newrow[1] = status;
	newrow[2] = ime;
	newrow[3] = prezime;
	newrow[4] = opis;
	newrow[5] = stanje;

	_te.beginUpdate();
	_te.insertRow(_te.getFirstSelectedRow() + 1);
	_te.endUpdate();
}