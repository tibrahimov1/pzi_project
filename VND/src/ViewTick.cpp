#pragma once
#include "ViewTick.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "DialogTick.h"
#include "../../common/include/Globals.h"
ViewTick::ViewTick() :
	_gl(4, 6)
	, _tiket(tr("TickName"))
	, _Ntiket(td::int4)
	, _tim(tr("Team"))
	, _dok(tr("Document"))
	, _status(tr("Status"))
	, _tezina(tr("Difficulty"))
	, _Dtezina(td::int4)
	//, _btnDok("<path>")
	, _btnTik(tr("AddTick"))
	//, _Ttim(td::int4)
	, _Hlbtn(4)
	, _btnSac(tr("Save"))
	//, _btnUc("Ucitaj")
	, _btnPon(tr("Discard"))
	, _btnDel(tr("Delete"))
	//, _statnaz("Status")
	//, _Status(td::int4)
	, _db(dp::getMainDatabase())
{
	_Hlbtn.append(_btnSac);
	//_Hlbtn.append(_btnUc);
	_Hlbtn.append(_btnPon);
	_Hlbtn.append(_btnDel);
	//_Hlbtn.appendSpacer();
	//_Hlbtn.append(_statnaz);
	//_Hlbtn.append(_Status);

	gui::GridComposer gc(_gl);
	gc.appendRow(_tiket);
	gc.appendCol(_Ntiket);
	_Ntiket.setAsReadOnly();
	gc.appendCol(_tim);
	gc.appendCol(_Ttim);
	_Ttim.setAsReadOnly();
	gc.appendCol(_dok);
	gc.appendCol(_btnDok);
	_btnDok.setAsReadOnly();

	gc.appendRow(_status);
	gc.appendCol(_Dstat);
	_Dstat.setAsReadOnly();
	gc.appendCol(_tezina);
	gc.appendCol(_Dtezina);
	_Dtezina.setAsReadOnly();
	gc.appendEmptyCols(1);
	gc.appendCol(_btnTik);


	gc.appendRow(_te, 0);

	gc.appendRow(_Hlbtn,0);

	gui::View::setLayout(&_gl);
	//populateComboBox(_Ttim, "SELECT ID, Ime as Name FROM Tim WHERE Tim.ID!=-1");
	//populateComboBox1(_Status,"SELECT DISTINCT CASE WHEN Stanje = 0 THEN 'Nije zavrsen' ELSE 'Zavrsen' END as Name FROM Tiketi");
	//_Status.addItem("Zavrsen", 0);
	//_Status.addItem("Nije završen", 1);
	//_Status.selectIndex(0);
	populateData(0);
	//setaasreadonly
}

void ViewTick::populateComboBox(gui::DBComboBox& cmb,td::String naziv) {
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


bool ViewTick::onClick(gui::Button* pBtn) {
	
	if (pBtn == &_btnTik) {
		DialogTick* p = new DialogTick(this,&_te,&_dodaj1,&_dodaj2, &_ticksToInsert);
		p->openModal(DlgID::NewTick, this);
		p->setTitle(tr("NewTick"));


		//Ovo radi ali prvo se izvrsi ovo sto ne zelim pa je to belaj velik
		/*int iRow = _te.getFirstSelectedRow();
		if (iRow < 0)
			iRow = 0;
		if (!canAdd())
			return true;
		_te.beginUpdate();
		auto& row = _te.getEmptyRow();
		td::Variant val;
		row[0].setValue(p->dajID());
		row[1].setValue(p->dajIDTima());
		row[2].setValue(p->dajImeTIma());
		row[3].setValue(p->dajStanje());
		row[4].setValue(p->dajTezinu());
		row[5].setValue(p->dajOpis());
		//populateDSRow(row);
		_te.insertRow(iRow);
		_te.endUpdate();
		*/
	}
	else if (pBtn == &_btnSac) {
		saveData();
		return true;
	}
	else if (pBtn == &_btnPon) {
		//zbog nekog razloga samo pojede ono sto je bilo, ne znam zasto trebalo bi normalno da radi
		_te.reload();
		
		
		for (size_t i = 0; i < _sviRedovi.size(); ++i)
		{
			_te.beginUpdate();
			auto& row = _te.getEmptyRow();
			row[0] = _sviRedovi[i][0];
			row[1] = _sviRedovi[i][1];
			row[2] = _sviRedovi[i][2];
			row[3] = _sviRedovi[i][3];
			row[4] = _sviRedovi[i][4];
			row[5] = _sviRedovi[i][5];
			_te.insertRow(i);
			_te.endUpdate();
			//_te.insertRow(row);
		}

		
		_ticksToDelete.clear();
		_ticksToInsert.clear();
		_ticksToUpdate.clear();
		_dodaj1.clear();
		_dodaj2.clear();
		pomocnaa = 0;
		return true;
	}
	/*else if (pBtn == &_btnUc) {

	}*/
	else if (pBtn == &_btnDel) {
		int iRow = _te.getFirstSelectedRow();
		td::INT4 TickID = getIDfromTable(iRow);
		if (iRow < 0)
			return true;
		_te.beginUpdate();
		_te.removeRow(iRow);
		_te.endUpdate();

		_ticksToDelete.push_back(TickID);
		//kada brisemo neki item, ako smo ga ranije dodali ili azurirali i nije spaseno u bazu, nemoj ga spasiti u bazu
		_ticksToInsert.erase(std::remove(_ticksToInsert.begin(), _ticksToInsert.end(), TickID), _ticksToInsert.end());
		_ticksToUpdate.erase(std::remove(_ticksToUpdate.begin(), _ticksToUpdate.end(), TickID), _ticksToUpdate.end());
		return true;
	}
	/*else if (pBtn == &_btnProj) {
		DialogProj* p = new DialogProj(this);
		p->openModal(DlgID::NewProj, this);
		p->setTitle(tr("NewTeam"));
	}*/
	return true;
}
bool ViewTick::onChangedValue(gui::DateEdit* dEdit) {
	return true;
}
void ViewTick::populateData(td::INT4 type) {
	//_pDS = (_db->createDataSet("SELECT t.ID as ID, s.Ime as TimName, t.Stanje as Status, ((DatumKraj/1000000-DatumPoc/1000000)*24+((DatumKraj-(DatumKraj/1000000)*1000000)/10000-(DatumPoc-(DatumPoc/1000000)*1000000)/10000)*30+((DatumKraj-((DatumKraj-(DatumKraj/1000000)*1000000)/10000)*10000)%10000-(DatumPoc-((DatumPoc-(DatumPoc/1000000)*1000000)/10000)*10000)%10000)*365) as Tezina"
	//	" FROM Tiketi t, Tim s, Projekti a, Korisnik k WHERE a.ID=t.ProjekatID and t.VlasnikID=a.MenadzerID and s.ID=k.TimID"));
	_pDS = (_db->createDataSet("SELECT t.ID as ID, s.ID as TimID, s.Ime as TimName, CASE WHEN t.Stanje=0 THEN 'Dodan' WHEN t.Stanje=1 THEN 'U progresu' ELSE 'Zavrsen' END as Status, t.Tezina as Tezina, t.Opis as Opis FROM Tiketi t, Tim s, Projekti a, Korisnik k WHERE a.ID=t.ProjekatID and a.ID=s.ProjekatID and a.MenadzerID=k.ID and k.TimID=s.ID and s.ID!=-1"));
	

	//OVO SAM IMAO PRVOBITNO ------- and t.VlasnikID=a.MenadzerID
	
	dp::DSColumns cols(_pDS->allocBindColumns(6));
	td::String TimName, Status, Opis;
	td::INT4 ID, Tezina, TimID;
	//ovdje ima sila popravki                                        
	cols << "ID" << ID << "TimID" << TimID << "TimName" << TimName << "Status" << Status << "Tezina" << Tezina << "Opis" << Opis;
	if (!_pDS->execute())
	{
		_pDS = nullptr;
		return;
	}
	//td::INT4 vrijednost = (DatumKraj - DatumPoc) * 24;
	if (type == 0) {
		_te.init(_pDS, { 0,2,3,4 });
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
		_te1.init(_pDS, { 0,2,3,4 });

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


td::INT4 ViewTick::getIDfromTable(int rowID)
{
	dp::IDataSet* pDS = _te.getDataSet();
	auto& row = pDS->getRow(rowID);
	return row[0].i4Val();
}

bool ViewTick::onChangedSelection(gui::TableEdit* pTE)
{
	
	if (pTE == &_te)
	{
		int iRow = _te.getFirstSelectedRow();
		if (iRow < 0)
		{
			_Ntiket.toZero();
			_Ttim.toZero();
			_Dstat.toZero();
			_Dtezina.toZero();
			return true;
		}
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);
		_Ttim.setValue(row[2]);
		_Dstat.setValue(row[3]);
		_Ntiket.setValue(row[0]);
		_Dtezina.setValue(row[4]);
		_btnDok.setValue(row[5]);
		//Dozvoljava samo 2 zbog nekog razloga??

		//
		//auto a = row[5];
		//
		return true;
	}
	return false;
	
	//return true;
}

void ViewTick::populateDSRow(dp::IDataSet::Row& row)
{
	td::Variant val;
	_Ntiket.getValue(val);
	row[0].setValue(val);
	_btnDok.getValue(val);
	row[5].setValue(val);
	_Dstat.getValue(val);
	row[3].setValue(val);
	_Dtezina.getValue(val);
	row[4].setValue(val);
	
	
	_Ttim.getValue(val);
	row[2].setValue(val);
	//row[2].setValue(_Ttim.getSelectedText());
}

bool ViewTick::canAdd()
{
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
	}
	return true;
}

bool ViewTick::eraseTicks()
{
	td::INT4 id;
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
	}
	return true;
}

bool ViewTick::insertTicks()
{
	dp::IStatementPtr pInsertItem(_db->createStatement("insert into Tiketi(ID, ProjekatID, VlasnikID, Stanje, Tezina, Opis) values(?,?,?,?,?,?)"));
	dp::Params pParams2(pInsertItem->allocParams());
	td::INT4 id, ProjekatId, VlasnikId, Stanje, Tezina;
	td::String Opis;
	pParams2 << id << ProjekatId << VlasnikId << Stanje<<Tezina << dp::toNCh(Opis, 100);


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

bool ViewTick::updateTicks()
{
	dp::IStatementPtr pInsertItem(_db->createStatement("insert into Tiketi(ID, ProjekatID, VlasnikID, Stanje, Tezina, Opis) values(?,?,?,?,?,?)"));
	dp::Params pParams2(pInsertItem->allocParams());
	td::INT4 id, ProjekatId, VlasnikId, Stanje, Tezina;
	td::String Opis;
	pParams2 << id << ProjekatId << VlasnikId << Stanje << Tezina << dp::toNCh(Opis, 100);

	gui::TableEdit _te2;
	td::INT4 idovi;
	dp::IDataSet* dobijamo=_db->createDataSet("select ID from Tiketi");
	dp::DSColumns coll=dobijamo->allocBindColumns(1);
	coll << "ID" << idovi;
	if (!dobijamo->execute()) {
		dobijamo=nullptr;
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
		if (_sviRedovi222.size()!=0 && std::find(_sviRedovi222.begin(), _sviRedovi222.end(), id)!=_sviRedovi222.end()) {
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

bool ViewTick::saveData()
{
	dp::Transaction tr(_db);
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
	}
	return true;
}