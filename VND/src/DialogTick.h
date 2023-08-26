#pragma once
#include <gui/Dialog.h>
#include <gui/Frame.h>
#include "../../common/include/VNDLib.h"
#include "../../common/include/Globals.h"
#include "ViewDialogTick.h"
#include <functional>
class VND_LIB_API DialogTick : public gui::Dialog
{
protected:
	ViewDialogTick _viewDialogTick;
	
	gui::TableEdit* _te;
	td::INT4 ID;
	td::INT4 _projekatID;
	td::INT4 IDtim;
	td::INT4 _vlasnikID = Globals::_currentUserID; //korisnikov id
	td::INT4 stanje = 0;//pocetno stanje
	td::INT4 tezina;
	td::String _opis;
	td::String _imeTima;
public:
	DialogTick(gui::Frame* pView, gui::TableEdit *_te1) :
		gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
								{gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(400, 100)
		)
		, _te(_te1)
	{
		setCentralView(&_viewDialogTick);
	}
	bool onClick(gui::Dialog::Button::ID btnID, gui::Button* pButton) override
	{
		if (btnID == gui::Dialog::Button::ID::OK) {

			_imeTima = _viewDialogTick.getImeTima();
			dp::IDatabase* _db(dp::getMainDatabase());
			dp::IStatementPtr pSelect = _db->createStatement("SELECT Projekti.ID as _projekatID, Tim.ID as IDtim from Projekti, Tim where Projekti.ID=Tim.ProjekatID and Tim.Ime=?");
			dp::Params pParams1(pSelect->allocParams());
			pParams1 << dp::toNCh(_imeTima, 100);


			dp::Columns cols=pSelect->allocBindColumns(2);
			
			cols << "_projekatID" << _projekatID<<"IDtim"<<IDtim;

			if (!pSelect->execute())
				return false;
			pSelect->moveNext();
			//return (_projekatID > 0);
			//return true;
			//td::String _filename = _viewDialogTick.getFileName();
			dp::IStatementPtr pSelect1 = _db->createStatement("SELECT MAX(ID)+1 as ID FROM Tiketi");
			dp::Columns cols1=pSelect1->allocBindColumns(1);
			
			cols1 << "ID" << ID;

			if (!pSelect1->execute())
				return false;
			pSelect1->moveNext();
			//return (ID > 0);

			
			tezina = _viewDialogTick.getTezina();
			_opis = _viewDialogTick.getFileName();

			
			dp::IStatementPtr pInsertNaming(_db->createStatement("insert into Tiketi (ID, ProjekatID, VlasnikID, Stanje, Tezina,  Opis) values(?,?,?,?,?,?)"));
			dp::Params pParams(pInsertNaming->allocParams());
			pParams << ID << _projekatID << _vlasnikID<<stanje<<tezina<<dp::toNCh(_opis, 100);
				if (!pInsertNaming->execute())
				{
					return false;
				}

				_te->beginUpdate();
				auto& row = _te->getEmptyRow();
				td::Variant val;
				row[0].setValue(ID);
				row[1].setValue(IDtim);
				row[2].setValue(_imeTima);
				row[3].setValue("Dodan");
				row[4].setValue(tezina);
				row[5].setValue(_opis);
				//populateDSRow(row);
				if (ID != 0) _te->insertRow(ID - 1);
				else _te->insertRow(0);
				_te->endUpdate();

				return true;
			}
			//***************************
			//write to db the new project
			//***************************
		else if (btnID == gui::Dialog::Button::ID::Cancel)
			return true;
	}
	td::INT4 dajID() {
		return ID;
	}
	td::INT4 dajProjekatID() {
		return _projekatID;
	}
	td::INT4 dajTezinu() {
		return tezina;
	}
	td::String dajOpis() {
		return _opis;
	}
	td::String dajImeTIma() {
		return _imeTima;
	}
	td::String dajStanje() {
		return "Nije zavrsen";
	}
	td::INT4 dajIDTima() {
		return IDtim;
	}
};