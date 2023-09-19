#pragma once
#include "ViewZah.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "DialogZah.h"
#include "../../common/include/Globals.h"
ViewZah::ViewZah() :
	_gl(4, 4)
	, _prima(tr("towards"))
	, _zahtjev(tr("Request"))
	, _dat(tr("rDate"))
	//, _Ddat(td::int4)
	, _status(tr("Status"))
	//, _btnDok("<path>")
	//, _btnUc("Ucitaj")
	, _btnZah(tr("NewReq"))
	, _btnOdg(tr("ReqAnswer"))
	, _Hlbtn(3)
	//, _statnaz("Status")
	//, _Status(td::int4)
	, _db(dp::getMainDatabase())
{
	_Hlbtn.appendSpacer();
	_Hlbtn.append(_btnZah);
	_Hlbtn.append(_btnOdg);



	gui::GridComposer gc(_gl);
	gc.appendRow(_prima);
	gc.appendCol(_Pprima);
	_Pprima.setAsReadOnly();
	gc.appendCol(_zahtjev);
	gc.appendCol(_Zzahtjev);
	_Zzahtjev.setAsReadOnly();

	gc.appendRow(_dat);
	gc.appendCol(_Ddat);
	_Ddat.setAsReadOnly();
	gc.appendCol(_status);
	gc.appendCol(_Dstat);
	_Dstat.setAsReadOnly();

	gc.appendRow(_Hlbtn, 0);

	gc.appendRow(_te, 0);

	gui::View::setLayout(&_gl);
	//populateComboBox(_Ttim, "SELECT ID, Ime as Name FROM Tim WHERE Tim.ID!=-1");
	//populateComboBox1(_Status,"SELECT DISTINCT CASE WHEN Stanje = 0 THEN 'Nije zavrsen' ELSE 'Zavrsen' END as Name FROM Tiketi");
	//_Status.addItem("Zavrsen", 0);
	//_Status.addItem("Nije završen", 1);
	//_Status.selectIndex(0);
	populateData(0);
	//setaasreadonly
}

void ViewZah::populateComboBox(gui::DBComboBox& cmb, td::String naziv) {
	dp::IStatementPtr pSelect = _db->createStatement(naziv.c_str());
	dp::Columns pCols = pSelect->allocBindColumns(2);
	td::String name;
	td::INT4 id;
	pCols << "ID" << id << "Name" << name;
	if (!pSelect->execute())
		assert(false);

	while (pSelect->moveNext())
	{
		cmb.addItem(name, id);
	}
	cmb.selectIndex(0);
}


bool ViewZah::onClick(gui::Button* pBtn) {

	if (pBtn == &_btnZah) {
		DialogZah* p = new DialogZah(this, &_te);
		p->openModal(DlgID::NewTick, this);
		p->setTitle(tr("NewReq"));
	}
	else if (pBtn == &_btnOdg) {
		int iRow = _te.getFirstSelectedRow();
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);
		dp::IStatementPtr pInsertItem(dp::getMainDatabase()->createStatement("update Zahtjevi SET Status=1 WHERE ID=?"));
		dp::Params pParams2(pInsertItem->allocParams());
		pParams2 << row[5];

		if (!pInsertItem->execute())
		{
			return false;
		}

		gui::TableEdit _te1;

		_pDS = (_db->createDataSet("SELECT k.ID as ID, k.Ime || ' ' || k.Prezime as 'Ime i prezime', Cast(z.Datum/1000000 AS Varchar(255)) || '/' || Cast((z.Datum-(z.Datum/1000000)*1000000)/10000 As Varchar(255))  || '/' || Cast(z.Datum%10000 AS Varchar(255)) As Datum, z.Opis as Zahtjev, CASE WHEN z.Status=0 THEN 'Neodgovoren' ELSE 'Odgovoren' END as Status, z.ID as ID1 FROM Korisnik k, Zahtjevi z where k.ID=z.SenderID and z.ReceiverID=?"));
		dp::Params pParams1(_pDS->allocParams());
		pParams1 << Globals::_currentUserID;

		//OVO SAM IMAO PRVOBITNO ------- and t.VlasnikID=a.MenadzerID

		dp::DSColumns cols(_pDS->allocBindColumns(6));
		td::String Name, Zahtjev, Status, Datum1;
		td::INT4 ID, Datum, ID1;
		//ovdje ima sila popravki                                        
		cols << "ID" << ID << "Ime i prezime" << Name << "Datum" << Datum1 << "Zahtjev" << Zahtjev << "Status" << Status << "ID1" << ID1;
		if (!_pDS->execute())
		{
			_pDS = nullptr;
			return false;
		}

		_te1.init(_pDS, { 1,2,3,4 });

		std::vector<cnt::SafeFullVector<td::Variant, false>> _sviRedovi11;

		dp::IDataSet* pDS2 = _te1.getDataSet();
		for (size_t i = 0; i < pDS2->getNumberOfRows(); ++i)
		{
			auto& row1 = pDS2->getRow(i);
			_sviRedovi11.push_back(row1);
		}
		_te.reload();
		for (int i = 0; i < _sviRedovi11.size(); i++) {
			_te.beginUpdate();
			auto& row1 = _te.getEmptyRow();
			row1[0] = _sviRedovi11[i][0];
			row1[1] = _sviRedovi11[i][1];
			row1[2] = _sviRedovi11[i][2];
			row1[3] = _sviRedovi11[i][3];
			row1[4] = _sviRedovi11[i][4];
			row1[5] = _sviRedovi11[i][5];
			_te.insertRow(i);
			_te.endUpdate();
		}
		
		return true;
	}
	return true;
}
bool ViewZah::onChangedValue(gui::DateEdit* dEdit) {
	return true;
}
void ViewZah::populateData(td::INT4 type) {
	//_pDS = (_db->createDataSet("SELECT t.ID as ID, s.Ime as TimName, t.Stanje as Status, ((DatumKraj/1000000-DatumPoc/1000000)*24+((DatumKraj-(DatumKraj/1000000)*1000000)/10000-(DatumPoc-(DatumPoc/1000000)*1000000)/10000)*30+((DatumKraj-((DatumKraj-(DatumKraj/1000000)*1000000)/10000)*10000)%10000-(DatumPoc-((DatumPoc-(DatumPoc/1000000)*1000000)/10000)*10000)%10000)*365) as Tezina"
	//	" FROM Tiketi t, Tim s, Projekti a, Korisnik k WHERE a.ID=t.ProjekatID and t.VlasnikID=a.MenadzerID and s.ID=k.TimID"));
	_pDS = (_db->createDataSet("SELECT k.ID as ID, k.Ime || ' ' || k.Prezime as 'Ime i prezime', Cast(z.Datum/1000000 AS Varchar(255)) || '/' || Cast((z.Datum-(z.Datum/1000000)*1000000)/10000 As Varchar(255))  || '/' || Cast(z.Datum%10000 AS Varchar(255)) As Datum, z.Opis as Zahtjev, CASE WHEN z.Status=0 THEN 'Neodgovoren' ELSE 'Odgovoren' END as Status, z.ID as ID1 FROM Korisnik k, Zahtjevi z where k.ID=z.SenderID and z.ReceiverID=?"));
	dp::Params pParams1(_pDS->allocParams());
	pParams1 << Globals::_currentUserID;

	//OVO SAM IMAO PRVOBITNO ------- and t.VlasnikID=a.MenadzerID

	dp::DSColumns cols(_pDS->allocBindColumns(6));
	td::String Name, Zahtjev,Status, Datum1;
	td::INT4 ID,Datum,ID1;
	//ovdje ima sila popravki                                        
	cols << "ID" << ID << "Ime i prezime" << Name << "Datum" << Datum1 <<"Zahtjev" <<Zahtjev<< "Status" << Status<<"ID1"<<ID1;
	if (!_pDS->execute())
	{
		_pDS = nullptr;
		return;
	}
	//td::INT4 vrijednost = (DatumKraj - DatumPoc) * 24;
	if (type == 0) {
		_te.init(_pDS, { 1,2,3,4 });
		_pomocni = _te;

		dp::IDataSet* pDS = _te.getDataSet();
		for (size_t i = 0; i < pDS->getNumberOfRows(); ++i)
		{
			auto& row = pDS->getRow(i);
			_sviRedovi.push_back(row);
		}

	}

	else {
		_te.reload();

		std::vector<cnt::SafeFullVector<td::Variant, false>> _sviRedovi1;

		gui::TableEdit _te1;
		_te1.init(_pDS, { 1,2,3,4 });

		dp::IDataSet* pDS1 = _te1.getDataSet();
		for (size_t i = 0; i < pDS1->getNumberOfRows(); ++i)
		{
			auto& row = pDS1->getRow(i);
			_sviRedovi1.push_back(row);
		}

		for (size_t i = 0; i < _sviRedovi1.size(); ++i)
		{
			_te.beginUpdate();
			auto& row = _te.getEmptyRow();
			row[0] = _sviRedovi1[i][0];
			row[1] = _sviRedovi1[i][1];
			row[2] = _sviRedovi1[i][2];
			row[3] = _sviRedovi1[i][3];
			row[4] = _sviRedovi1[i][4];
			row[5] = _sviRedovi1[i][5];
			_te.insertRow(i);
			_te.endUpdate();
			//_te.insertRow(row);
		}
		_ticksToDelete.clear();
		_ticksToInsert.clear();
		_ticksToUpdate.clear();
		_dodaj1.clear();
		_dodaj2.clear();
	}
}


td::INT4 ViewZah::getIDfromTable(int rowID)
{
	dp::IDataSet* pDS = _te.getDataSet();
	auto& row = pDS->getRow(rowID);
	return row[0].i4Val();
}

bool ViewZah::onChangedSelection(gui::TableEdit* pTE)
{

	if (pTE == &_te)
	{
		int iRow = _te.getFirstSelectedRow();
		if (iRow < 0)
		{
			_Pprima.toZero();
			_Zzahtjev.toZero();
			_Dstat.toZero();
			_Ddat.toZero();
			return true;
		}
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);
		_Pprima.setValue(row[1]);
		_Dstat.setValue(row[4]);
		_Zzahtjev.setValue(row[3]);
		_Ddat.setValue(row[2]);
		//Dozvoljava samo 2 zbog nekog razloga??

		//
		//auto a = row[5];
		//
		return true;
	}
	return false;

	//return true;
}

void ViewZah::populateDSRow(dp::IDataSet::Row& row)
{
	td::Variant val;
	_Pprima.getValue(val);
	row[1].setValue(val);
	_Dstat.getValue(val);
	row[4].setValue(val);
	_Zzahtjev.getValue(val);
	row[3].setValue(val);
	_Ddat.getValue(val);
	row[2].setValue(val);
}

bool ViewZah::canAdd()
{
	/*
	//cant have 2 items with same id...
	td::Variant id = _Ntiket.getValue();
	dp::IDataSet* pDS = _te.getDataSet();
	for (size_t i = 0; i < pDS->getNumberOfRows(); ++i)
	{
		auto& row = pDS->getRow(i);
		if (row[0] == id)
		{
			return false;
		}
	}*/
	return true;
}

bool ViewZah::eraseTicks()
{
	/*td::INT4 id;
	dp::IStatementPtr pDeleteItem(_db->createStatement("delete from Tiketi where ID=?"));
	dp::Params pParams2(pDeleteItem->allocParams());
	pParams2 << id;

	for (auto item : _ticksToDelete)
	{
		id = item;
		if (!pDeleteItem->execute())
		{
			return false;
		}
	}*/
	return true;
}

bool ViewZah::insertTicks()
{
	/*
	dp::IStatementPtr pInsertItem(_db->createStatement("insert into Tiketi(ID, ProjekatID, VlasnikID, Stanje, Tezina, Opis) values(?,?,?,?,?,?)"));
	dp::Params pParams2(pInsertItem->allocParams());
	td::INT4 id, ProjekatId, VlasnikId, Stanje, Tezina;
	td::String Opis;
	pParams2 << id << ProjekatId << VlasnikId << Stanje << Tezina << dp::toNCh(Opis, 100);


	dp::IDataSet* pDS = _te.getDataSet();
	auto rowCnt = pDS->getNumberOfRows();

	for (int i = 0; i < _dodaj1.size(); i++) {
		id = _dodaj1[i][0];
		ProjekatId = _dodaj1[i][1];
		VlasnikId = _dodaj1[i][2];
		Stanje = _dodaj1[i][3];
		Tezina = _dodaj1[i][4];
		Opis = _dodaj2[i];

		if (!pInsertItem->execute())
		{
			return false;
		}
	}
	/*for (size_t iRow = 0; iRow < rowCnt; ++iRow)
	{
		auto& row = pDS->getRow(iRow);
		id = row[0].i4Val();
		if (std::find(_ticksToInsert.begin(), _ticksToInsert.end(), id) == _ticksToInsert.end())//this item is not marked to insert, go next
			continue;
		ProjekatId = row[1].i4Val();
		VlasnikId = row[2].i4Val();
		Opis = row[3];

		if (!pInsertItem->execute())
		{
			return false;
		}
	}*/
	return true;
}

bool ViewZah::updateTicks()
{
	/*dp::IStatementPtr pInsertItem(_db->createStatement("insert into Tiketi(ID, ProjekatID, VlasnikID, Stanje, Tezina, Opis) values(?,?,?,?,?,?)"));
	dp::Params pParams2(pInsertItem->allocParams());
	td::INT4 id, ProjekatId, VlasnikId, Stanje, Tezina;
	td::String Opis;
	pParams2 << id << ProjekatId << VlasnikId << Stanje << Tezina << dp::toNCh(Opis, 100);

	gui::TableEdit _te2;
	td::INT4 idovi;
	dp::IDataSet* dobijamo = _db->createDataSet("select ID from Tiketi");
	dp::DSColumns coll = dobijamo->allocBindColumns(1);
	coll << "ID" << idovi;
	if (!dobijamo->execute()) {
		dobijamo = nullptr;
		return false;
	}
	_te2.init(dobijamo, { 0 });

	std::vector<cnt::SafeFullVector<td::Variant, false>> _sviRedovi22;
	dp::IDataSet* pDS22 = _te2.getDataSet();
	for (size_t i = 0; i < pDS22->getNumberOfRows(); ++i)
	{
		auto& row = pDS22->getRow(i);
		_sviRedovi22.push_back(row);
	}
	std::vector<td::INT4> _sviRedovi222;
	for (int i = 0; i < _sviRedovi22.size(); i++) {
		_sviRedovi222.push_back(_sviRedovi22[i][0].i4Val());
	}

	for (int i = 0; i < _sviRedovi.size(); i++) {
		id = _sviRedovi[i][0].i4Val();
		if (_sviRedovi222.size() != 0 && std::find(_sviRedovi222.begin(), _sviRedovi222.end(), id) != _sviRedovi222.end()) {
			continue;
		}
		dp::IStatementPtr pSelect = _db->createStatement("SELECT Projekti.ID as _projekatID from Projekti, Tim where Projekti.ID=Tim.ProjekatID and Tim.Ime=?");
		dp::Params pParams1(pSelect->allocParams());
		pParams1 << dp::toNCh(_sviRedovi[i][2], 100);
		dp::Columns cols = pSelect->allocBindColumns(1);

		cols << "_projekatID" << ProjekatId;

		if (!pSelect->execute())
			return false;
		pSelect->moveNext();

		VlasnikId = Globals::_currentUserID;
		auto x = _sviRedovi[i][3].getConstStr();
		//td::String y = _sviRedovi[i][3].toString();
		auto z = _sviRedovi[i][3].strVal();
		if (std::strcmp(z.c_str(), "Dodan") == 0) Stanje = 0;
		else if (std::strcmp(z.c_str(), "U progresu") == 0) Stanje = 1;
		else Stanje = 2;
		Tezina = _sviRedovi[i][4].i4Val();
		Opis = _sviRedovi[i][5];
		int a = 3;
		if (!pInsertItem->execute())
		{
			return false;
		}
	}
	/*dp::IStatementPtr pChangeItem(_db->createStatement("update Artikal set PDVID=?, JMJID=?, NacinSkl=? where TipID=? and ID=?"));
	dp::Params pParams2(pChangeItem->allocParams());
	pParams2 << idVat << idUnit << dp::toNCh(storageType, 100) << nt << id;


	dp::IDataSet* pDS = _table.getDataSet();
	auto rowCnt = pDS->getNumberOfRows();
	for (size_t iRow = 0; iRow < rowCnt; ++iRow)
	{
		auto& row = pDS->getRow(iRow);
		id = row[0].i4Val();
		if (std::find(_itemsToUpdate.begin(), _itemsToUpdate.end(), id) == _itemsToUpdate.end()) // this item is not marked to insert, go next
			continue;

		name = row[1];
		storageType = row[6];
		idVat = row[2].i4Val();
		idUnit = row[3].i4Val();

		if (!pChangeNaming->execute())
		{
			return false;
		}
		if (!pChangeItem->execute())
		{
			return false;
		}
	}*/
	return true;
}

bool ViewZah::saveData()
{
	/*dp::Transaction tr(_db);
	if (pomocnaa == 0) {
		if (!updateTicks())
			return false;
		pomocnaa = 1;
	}
	if (!eraseTicks())
		return false;
	if (!insertTicks())
		return false;
	if (tr.commit())
	{
		_ticksToDelete.clear();
		_ticksToInsert.clear();
		_ticksToUpdate.clear();
		_dodaj1.clear();
		_dodaj2.clear();
		pomocnaa = 1;
	}*/
	return true;
}