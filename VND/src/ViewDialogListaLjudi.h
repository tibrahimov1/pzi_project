#pragma once
#include <gui/View.h>
#include <gui/Button.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/DateEdit.h>
#include <gui/DBComboBox.h>
#include <gui/FileDialog.h>
#include "../../common/include/VNDLib.h"
#include <gui/GridComposer.h>
#include "../../common/include/Globals.h"
#include <gui/GridLayout.h>
#include <td/Date.h>
#include <cnt/StringBuilder.h>
//#include <gui/Types.h>

class VND_LIB_API ViewDialogListaLjudi : public gui::View
{
protected:
	gui::Label _TeamName;
	gui::DBComboBox _Combo;
	gui::GridLayout _gl;
	gui::TableEdit _te;
	td::INT4 _IDTima = 1;
	int pomocna = -1;
	std::vector<cnt::SafeFullVector<td::Variant, false>> _sviPocetni;
	std::vector<td::INT4> _PeopleToInsert, _PeopleToDelete;

	gui::DBComboBox _Korisnici;
	gui::Button _btnDodaj;
	gui::Button _btnUkloni;

public:

	ViewDialogListaLjudi() :
		_gl(3, 3)
		, _TeamName(tr("Team Name"))
		, _Combo(td::int4)
		, _Korisnici(td::int4)
		, _btnDodaj(tr("Dodaj"))
		, _btnUkloni(tr("Ukloni"))
	{
		gui::GridComposer gc(_gl);


		gc.appendRow(_TeamName);
		gc.appendCol(_Combo, -1);
		gc.appendRow(_te, 0);
		gc.appendRow(_Korisnici);
		gc.appendCol(_btnDodaj);
		gc.appendCol(_btnUkloni, -1);
		gui::View::setLayout(&_gl);
		populateCombo();
		populateCombo1();
		populateData();
		//getPocetni();
	}

	void getPocetni() {
		dp::IStatementPtr pStat(dp::getMainDatabase()->createStatement("SELECT * FROM Korisnik"));
		dp::Columns cols(pStat->allocBindColumns(8));
		td::String Ime, Prezime, Username, Sifra;
		td::INT4 ID, TipID, Zauzet, TimID;
		cols << "ID" << ID << "TipID" << TipID << "Ime" << Ime << "Prezime" << Prezime << "Username" << Username << "Sifra" << Sifra << "Zauzet" << Zauzet << "TimID" << TimID;
		if (!pStat->execute()) return;
		pStat->moveNext();

	}

	void populateData() {

		dp::IDatabase* _db1(dp::getMainDatabase());
		dp::IDataSet* _pDS1 = _db1->createDataSet("SELECT k.Ime || ' ' || k.Prezime as 'Ime i prezime', k.ID as ID, k.TimID as TimID, t.Ime as 'Team name', t.ID as TimovID from Korisnik k, Tim t where k.TimID=? and k.TimID=t.ID and k.TipID=3");

		//OVO DODAJ AKO ZABORAVIS




		//  and k.Zauzet=-1    




		//NEMOJ ZABORAVITI
		dp::Params pParams1(_pDS1->allocParams());
		pParams1 << _IDTima;

		dp::DSColumns cols(_pDS1->allocBindColumns(5));
		td::String Ime, Team_Name;
		td::INT4  TimID, ID, TimovID;
		cols << "Ime i prezime" << Ime << "ID" << ID << "TimID" << TimID << "Team name" << Team_Name << "TimovID" << TimovID;
		if (!_pDS1->execute())
		{
			_pDS1 = nullptr;
			return;
		}
		//_IDTima = TimID;
		_te.init(_pDS1, { 0 });

	}

	void populateCombo() {
		_Combo.clean();
		dp::IStatementPtr pStat(dp::getMainDatabase()->createStatement("SELECT t. ID, t.Ime FROM Tim t where (t.ID!=-1)"));
		dp::Columns cols(pStat->allocBindColumns(2));
		td::String name;
		td::INT4 ID;
		cols << "ID" << ID << "Ime" << name;
		if (!pStat->execute()) return;
		while (pStat->moveNext()) {
			cnt::StringBuilderSmall sb();

			_Combo.addItem(name, ID); //NE ZNAM SABRAT STRINGOVE
		}
		_Combo.selectIndex(0);
	};

	void populateCombo1() {
		_Korisnici.clean();
		dp::IStatementPtr pStat(dp::getMainDatabase()->createStatement("SELECT k.ID, k.Ime || ' ' || k.Prezime as 'Ime i prezime' FROM Korisnik k where (k.TimID=-1) and k.TipID=3"));
		dp::Columns cols(pStat->allocBindColumns(2));
		td::String name;
		td::INT4 ID;
		cols << "ID" << ID << "Ime i prezime" << name;
		if (!pStat->execute()) return;
		while (pStat->moveNext()) {
			cnt::StringBuilderSmall sb();

			_Korisnici.addItem(name, ID); //NE ZNAM SABRAT STRINGOVE
		}
		_Korisnici.selectIndex(0);
	};

	bool onChangedSelection(gui::DBComboBox* combo) {

		if (combo == &_Combo) {
			td::String naziv = _Combo.getSelectedText();
			dp::IStatementPtr pStat(dp::getMainDatabase()->createStatement("SELECT t. ID FROM Tim t where t.Ime=?"));
			dp::Params pParams1(pStat->allocParams());

			pParams1 << dp::toNCh(naziv, 100);

			dp::Columns cols(pStat->allocBindColumns(1));
			cols << "ID" << _IDTima;

			if (!pStat->execute())
				return false;
			pStat->moveNext();

			if (pomocna == -1)_IDTima = 1;
		}
		else if (combo == &_Korisnici) {
			return true;
		}
		if (pomocna != -1) {
			_te.reload();
			gui::TableEdit _te1;
			dp::IDatabase* _db1(dp::getMainDatabase());
			dp::IDataSet* _pDS1 = _db1->createDataSet("SELECT k.Ime || ' ' || k.Prezime as 'Ime i prezime', k.ID as ID, k.TimID as TimID, t.Ime as 'Team name' from Korisnik k, Tim t where k.TimID=? and k.TimID=t.ID and k.TipID=3");
			//I OVDJE OVO DODAJ AKO ZABORAVIS




			//  and k.Zauzet=-1




			//NEMOJ ZABORAVITI
			dp::Params pParams1(_pDS1->allocParams());
			pParams1 << _IDTima;


			dp::DSColumns cols(_pDS1->allocBindColumns(4));
			td::String Ime, Team_Name;
			td::INT4  TimID, ID, TimovID;
			cols << "Ime i prezime" << Ime << "ID" << ID << "TimID" << TimID << "Team name" << Team_Name;

			if (!_pDS1->execute())
			{
				_pDS1 = nullptr;
				return false;
			}

			_te1.init(_pDS1, { 0 });

			std::vector<cnt::SafeFullVector<td::Variant, false>> _sviRedovi;
			dp::IDataSet* pDS = _te1.getDataSet();
			for (size_t i = 0; i < pDS->getNumberOfRows(); ++i)
			{
				auto& row = pDS->getRow(i);
				_sviRedovi.push_back(row);
			}


			for (size_t i = 0; i < _sviRedovi.size(); ++i)
			{
				_te.beginUpdate();
				auto& row = _te.getEmptyRow();
				row[0] = _sviRedovi[i][0];
				row[1] = _sviRedovi[i][1];
				row[2] = _sviRedovi[i][2];
				row[3] = _sviRedovi[i][3];
				_te.insertRow(i);
				_te.endUpdate();
			}
			populateCombo1();
			_PeopleToDelete.clear();
			_PeopleToInsert.clear();
		}
		pomocna = 0;
		return true;
	}

	bool onChangedSelection(gui::TableEdit* pTE)
	{

		if (pTE == &_te)
		{
			int iRow = _te.getFirstSelectedRow();
			if (iRow < 0)
			{
				_Combo.toZero();
				return true;
			}
			dp::IDataSet* pDS = _te.getDataSet();
			auto& row = pDS->getRow(iRow);
			//_Combo.setValue(row[4].i4Val());
			return true;
		}
		return false;
	}

	void populateDSRow(dp::IDataSet::Row& row)
	{
		td::Variant val;
		_Combo.getValue(val);
		row[2].setValue(val);

		_Korisnici.getValue(val);
		row[1].setValue(val);

		row[3].setValue(_Combo.getSelectedText());
		row[0].setValue(_Korisnici.getSelectedText());
	}

	td::INT4 DajIDTima() {
		return _IDTima;
	}
	td::INT4 getIDfromTable(int rowID)
	{
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(rowID);
		return row[1].i4Val();
	}
	bool spasi() {
		saveData();
		return true;
	}

	bool ponisti() {
		_te.reload();
		gui::TableEdit _te1;
		dp::IDatabase* _db1(dp::getMainDatabase());
		dp::IDataSet* _pDS1 = _db1->createDataSet("SELECT k.Ime || ' ' || k.Prezime as 'Ime i prezime', k.ID as ID, k.TimID as TimID, t.Ime as 'Team name' from Korisnik k, Tim t where k.TimID=? and k.TimID=t.ID and k.TipID=3");
		//I OVDJE OVO DODAJ AKO ZABORAVIS




		//  and k.Zauzet=-1




		//NEMOJ ZABORAVITI
		dp::Params pParams1(_pDS1->allocParams());
		pParams1 << _IDTima;


		dp::DSColumns cols(_pDS1->allocBindColumns(4));
		td::String Ime, Team_Name;
		td::INT4  TimID, ID, TimovID;
		cols << "Ime i prezime" << Ime << "ID" << ID << "TimID" << TimID << "Team name" << Team_Name;

		if (!_pDS1->execute())
		{
			_pDS1 = nullptr;
			return false;
		}

		_te1.init(_pDS1, { 0 });

		std::vector<cnt::SafeFullVector<td::Variant, false>> _sviRedovi;
		dp::IDataSet* pDS = _te1.getDataSet();
		for (size_t i = 0; i < pDS->getNumberOfRows(); ++i)
		{
			auto& row = pDS->getRow(i);
			_sviRedovi.push_back(row);
		}


		for (size_t i = 0; i < _sviRedovi.size(); ++i)
		{
			_te.beginUpdate();
			auto& row = _te.getEmptyRow();
			row[0] = _sviRedovi[i][0];
			row[1] = _sviRedovi[i][1];
			row[2] = _sviRedovi[i][2];
			row[3] = _sviRedovi[i][3];
			_te.insertRow(i);
			_te.endUpdate();
		}

		populateCombo1();

		_PeopleToDelete.clear();
		_PeopleToInsert.clear();
		return true;
	}


	bool onClick(gui::Button* pBtn) {

		if (pBtn == &_btnDodaj) {
			int iRow = _te.getFirstSelectedRow();
			//if (iRow < 0)
			//return true;
			td::Variant val;

			//Ovdje los ID uzima zbog nekog razloga
			_Korisnici.getValue(val);
			td::INT4 itemid = val.i4Val();


			_te.beginUpdate();
			auto& row = _te.getEmptyRow();
			populateDSRow(row);
			_te.insertRow(iRow);
			_te.endUpdate();

			//_PeopleToInsert.erase(std::remove(_itemsToUpdate.begin(), _itemsToUpdate.end(), itemid), _itemsToUpdate.end());
			_PeopleToInsert.push_back(itemid);
			auto poz = _Korisnici.getSelectedIndex();
			_Korisnici.removeItem(poz);
			_Korisnici.selectIndex(0);
			return true;
		}
		else if (pBtn == &_btnUkloni) {
			int iRow = _te.getFirstSelectedRow();
			td::INT4 DelID = getIDfromTable(iRow);

			if (iRow < 0)
				return true;

			
			dp::IDataSet* pDS = _te.getDataSet();
			auto& row = pDS->getRow(iRow);
			td::String naziv = row[0].getConstStr();
			td::INT4 ZaBris = _Korisnici.getNoOfItems();
			_Korisnici.addItem(naziv, DelID);
			_Korisnici.selectIndex(0);

			_te.beginUpdate();
			_te.removeRow(iRow);
			_te.endUpdate();

			_PeopleToDelete.push_back(DelID);


			return true;
		}

	}

	bool saveData() {
		dp::Transaction tr(dp::getMainDatabase());
		if (!insertPeople())
			return false;
		if (!erasePeople())
			return false;
		if (tr.commit())
		{
			//_ticksToDelete.clear();
			_PeopleToInsert.clear();
			_PeopleToDelete.clear();
		}
		return true;
	}

	bool insertPeople() {

		auto x = _PeopleToInsert;


		for (int i = 0; i < _PeopleToInsert.size(); i++) {
			dp::IStatementPtr pInsertItem(dp::getMainDatabase()->createStatement("update Korisnik SET TimID=? WHERE ID=?"));
			dp::Params pParams2(pInsertItem->allocParams());
			pParams2 << _IDTima << _PeopleToInsert[i];

			if (!pInsertItem->execute())
			{
				return false;
			}

			dp::IStatementPtr daj(dp::getMainDatabase()->createStatement("select BrClanova FROM Tim where ID=?"));
			dp::Params pParams1(daj->allocParams());
			pParams1 << _IDTima;

			td::INT4 Brcl;

			dp::Columns cols1 = daj->allocBindColumns(1);
			cols1 << "BrClanova" << Brcl;

			if (!daj->execute())
				return false;
			daj->moveNext();

			Brcl++;
			
			dp::IStatementPtr pStat(dp::getMainDatabase()->createStatement("update Tim SET BrClanova=? WHERE ID=?"));
			dp::Params pParams(pStat->allocParams());
			pParams << Brcl << _IDTima;

			if (!pStat->execute())
			{
				return false;
			}
		}
		return true;
	}

	bool erasePeople()
	{

		for (auto item : _PeopleToDelete)
		{
			td::INT4 id;
			dp::IStatementPtr pDeleteItem(dp::getMainDatabase()->createStatement("Update Korisnik SET TimID=-1 where ID = ?"));
			dp::Params pParams2(pDeleteItem->allocParams());
			pParams2 << id;

			dp::IStatementPtr daj(dp::getMainDatabase()->createStatement("select BrClanova FROM Tim where ID=?"));
			dp::Params pParams1(daj->allocParams());
			pParams1 << _IDTima;

			td::INT4 Brcl;

			dp::Columns cols1 = daj->allocBindColumns(1);
			cols1 << "BrClanova" << Brcl;

			if (!daj->execute())
				return false;
			daj->moveNext();

			Brcl--;

			dp::IStatementPtr pStat(dp::getMainDatabase()->createStatement("update Tim SET BrClanova=? WHERE ID=?"));
			dp::Params pParams(pStat->allocParams());
			pParams << Brcl << _IDTima;

			if (!pStat->execute())
			{
				return false;
			}
			id = item;
			if (!pDeleteItem->execute())
			{
				return false;
			}
		}
		return true;
	}

};