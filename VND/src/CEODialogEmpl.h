#pragma once
#include <gui/Dialog.h>
#include <gui/Frame.h>
#include "../../common/include/VNDLib.h"
#include "../../common/include/Globals.h"
#include "CEOViewDialogEmpl.h"
#include <functional>
class VND_LIB_API CEODialogEmpl : public gui::Dialog
{
protected:
	CEOViewDialogEmpl _viewDialog;

	gui::TableEdit* _te;
	std::vector<std::vector<td::INT4>>* _br;
	std::vector< std::vector<td::String>>* _txt;
	td::INT4 *_idd, *_pmm, *_korr;

public:
	CEODialogEmpl(gui::Frame* pView, gui::TableEdit* _te1, std::vector<std::vector<td::INT4>>* br, std::vector< std::vector<td::String>>* txt, td::INT4 *idd, td::INT4 *pmm, td::INT4 *korr) :
		gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
								{gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(400, 100)
		)
		, _te(_te1)
		, _br(br)
		, _txt(txt)
		, _idd(idd)
		, _pmm(pmm)
		, _korr(korr)
	{
		setCentralView(&_viewDialog);
	}
	bool onClick(gui::Dialog::Button::ID btnID, gui::Button* pButton) override
	{
		if (btnID == gui::Dialog::Button::ID::OK) {
			// postavljanje vrijednosti parametara
			td::INT4 ID, TipID, Zauzet, TimID;
			td::String Ime, Prezime, Username, Sifra;

			Zauzet = -1;
			TimID = -1;
			Ime = _viewDialog.getIme();
			Prezime = _viewDialog.getPrez();
			TipID = _viewDialog.getTipID();
			if (TipID == 2) Username = "pm";
			else if (TipID == 3) Username = "kor";

			// pronalazi se sljedeci najveci ID (ima potencijalnih rupa od brisanja korisnika ali ovo svakako nije krajnja vrijednost ID-a)
			dp::IStatementPtr pS = dp::getMainDatabase()->createStatement("SELECT MAX(ID)+1 AS 'max' FROM Korisnik");
			dp::Columns kol = pS->allocBindColumns(1);
			kol << "max" << ID;
			if (!pS->execute())
				return false;
			pS->moveNext();
			
			// ako se dodaje vise korisnika prije spasavanja u bazu, treba se jos povecavati ID cija se vrijednost prati sa _idd
			if (*_idd == -1) *_idd = ID;
			else if (*_idd >= ID) {
				ID = *_idd + 1;
				*_idd = *_idd + 1;
			}
			
			// pronalazi se najveci redni broj uposlenika u ovisnosti od njihovog ranga (pm ili korisnik)
			dp::IStatementPtr pS2 = dp::getMainDatabase()->createStatement(
				"SELECT CASE WHEN TipID=2 THEN MAX(CAST(SUBSTR(Username,3) AS INTEGER))+1 "
				"ELSE MAX(CAST(SUBSTR(Username, 4) AS INTEGER)) + 1 "
				"END AS maks FROM Korisnik WHERE TipID = ?");
			dp::Params pParams2(pS2->allocParams());
			pParams2 << TipID;

			td::INT4 rbr;
			dp::Columns kol2 = pS2->allocBindColumns(1);
			kol2 << "maks" << rbr;

			if (!pS2->execute())
				return false;
			pS2->moveNext();

			// ako se dodaje vise korisnika prije spasavanja u bazu, treba se jos povecavati redni broj koji se prati sa _pmm i _korr 
			if (TipID == 2  && *_pmm == -1) *_pmm = rbr;
			else if (TipID == 2  && *_pmm >= rbr) {
				rbr = *_pmm + 1;
				*_pmm += 1;
			}
			else if (TipID == 3 && *_korr == -1) *_korr = rbr;
			else if (TipID == 3 && *_korr >= rbr) {
				rbr = *_korr + 1;
				*_korr += 1;
			}

			cnt::StringBuilderSmall sb;
			sb << Username << rbr;
			td::String lol = sb.toString();

			Username = lol;
			Sifra = Username;
			
			// spasavanje vrijednosti u vektore kako bi se tek na kraju rada spasili u bazu podataka
			std::vector < td::INT4 > brr;
			std::vector < td::String > txtt;
			
			brr.push_back(ID);
			brr.push_back(TipID);
			brr.push_back(Zauzet);
			brr.push_back(TimID);
			_br->push_back(brr);

			txtt.push_back(Ime);
			txtt.push_back(Prezime);
			txtt.push_back(Username);
			txtt.push_back(Sifra);
			_txt->push_back(txtt);	
			
			// pravljenje novog reda u tabeli
			dp::IDataSet* pDS = _te->getDataSet();
			auto vel = pDS->getNumberOfRows();
			auto& row = _te->getEmptyRow();

			_te->beginUpdate();

			row[0].setValue(Ime + " " + Prezime);
			row[1].setValue("Nezauzeti projekt");
			if (TipID == 2) row[2].setValue("Projekt menadzer");
			else if (TipID == 3) row[2].setValue("Radnik");
			row[3].setValue("0");
			row[4].setValue("0");
			row[5].setValue(ID);
			row[6].setValue(TimID);

			if (ID != 0) _te->insertRow(vel);
			else _te->insertRow(0);
			_te->endUpdate();

			return true;
		}
		else if (btnID == gui::Dialog::Button::ID::Cancel)
			return true;
	}

};