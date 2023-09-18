#pragma once
#include <gui/Dialog.h>
#include <gui/Frame.h>
#include "../../common/include/VNDLib.h"
#include "ViewDialogTeam.h"
class VND_LIB_API DialogTeam : public gui::Dialog
{
protected:
	ViewDialogTeam _viewDialogTeam;

	gui::TableEdit* _te;

public:
	DialogTeam(gui::Frame* pView,gui::TableEdit* _te1) :
		gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
								{gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(400, 100)
		)
		, _te(_te1)
	{
		setCentralView(&_viewDialogTeam);
	}
	bool onClick(gui::Dialog::Button::ID btnID, gui::Button* pButton) override
	{
	if (btnID == gui::Dialog::Button::ID::OK) {
		if(!_viewDialogTeam.spasi())return false;

		gui::TableEdit _te2;

		dp::IDataSet* _pDS1 = dp::getMainDatabase()->createDataSet("SELECT T.ID as ID, T.Ime as TimName, T.Opis as Opis,  T.BrClanova as 'Broj ljudi', CAST(12.53 AS DECIMAL(7,2) ) as 'Brzina', COUNT(CASE WHEN Ti.stanje = 0 THEN 1 END) AS 'Dodani Tiketi',COUNT(CASE WHEN Ti.stanje = 1 THEN 1 END) AS 'Tiketi u progresu',COUNT(CASE WHEN Ti.stanje = 2 THEN 1 END) AS 'Zavrseni tiketi' FROM Tim AS T LEFT JOIN Projekti AS P ON T.ProjekatID = P.ID LEFT JOIN Tiketi AS Ti ON P.ID = Ti.ProjekatID Where t.ID!=-1 GROUP BY T.ID, T.BrCLanova");
		dp::DSColumns cols(_pDS1->allocBindColumns(8));
		td::String TimName, Opis;
		td::INT4 Brcl, Dodan, Progres, Zavrsen, ID;
		double Brzina;
		cols << "ID" << ID << "TimName" << TimName << "Opis" << Opis << "Broj ljudi" << Brcl << "Brzina" << Brzina << "Dodani Tiketi" << Dodan << "Tiketi u progresu" << Progres << "Zavrseni tiketi" << Zavrsen;
		//cols << "TimName" << TimName << "Brcl" << Brcl;// << "Dodan" << Dodan;
		/*_pDS1 = _db1->createDataSet(" SELECT T.Ime as TimName, T.BrClanova as Brcl,COUNT(CASE WHEN Ti.stanje = 0 THEN 1 END) AS Dodan,COUNT(CASE WHEN Ti.stanje = 1 THEN 1 END) AS Progres,COUNT(CASE WHEN Ti.stanje = 2 THEN 1 END) AS Zavrsen FROM Tim AS T LEFT JOIN Korisnik AS K ON T.ID = K.TimID LEFT JOIN Tiketi AS Ti ON K.ID = Ti.VlasnikID GROUP BY T.ID, T.BrCLanova");
		dp::DSColumns cols(_pDS1->allocBindColumns(5));
		td::String TimName;
		td::INT4 Brcl, Dodan, Progres, Zavrsen;
		//ovdje ima sila popravki
		cols << "TimName" << TimName << "Brcl" << Brcl<<"Dodan"<<Dodan<<"Progres"<<Progres<<"Zavrsen"<<Zavrsen;*/
		if (!_pDS1->execute())
		{
			_pDS1 = nullptr;
			return false;
		}
		//_te.init(_pDS1, { 0,1,2});
		_te2.init(_pDS1, { 1,3,4,5,6,7 });

		_te->reload();

		std::vector<cnt::SafeFullVector<td::Variant, false>> _sviRedovi;

		dp::IDataSet* pDS = _te2.getDataSet();
		for (size_t i = 0; i < pDS->getNumberOfRows(); ++i)
		{
			auto& row = pDS->getRow(i);
			_sviRedovi.push_back(row);
		}

		for (size_t i = 0; i < _sviRedovi.size(); ++i)
		{
			_te->beginUpdate();
			auto& row = _te->getEmptyRow();
			row[0] = _sviRedovi[i][0];
			row[1] = _sviRedovi[i][1];
			row[2] = _sviRedovi[i][2];
			row[3] = _sviRedovi[i][3];
			row[5] = _sviRedovi[i][5];
			row[6] = _sviRedovi[i][6];
			row[7] = _sviRedovi[i][7];

			if (i == _sviRedovi.size() - 1) {
				row[4] = 0;
				_te->insertRow(i);
				_te->endUpdate();
				break;
			}

			dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("SELECT p.DatumPoc as DatPoc, p.ID as ID1 FROM Projekti p, Tim t WHERE p.ID=t.ProjekatID and t.ID=?");
			dp::Params pParams2(pSelect1->allocParams());
			pParams2 << row[0];

			dp::Columns cols1 = pSelect1->allocBindColumns(2);
			td::INT4 dat, ID1;
			cols1 << "DatPoc" << dat << "ID1" << ID1;

			if (!pSelect1->execute())
				return false;
			pSelect1->moveNext();

			if (ID1 == -1) {
				row[4] = 0;
				_te->insertRow(i);
				_te->endUpdate();
				continue;
			}

			td::Date datum;
			datum.now();
			td::INT4 dan, mjesec, god;
			dan = dat / 1000000;
			mjesec = (dat - dan * 1000000) / 10000;
			god = dat % 10000;
			td::Date datum1(god, mjesec, dan);
			td::INT4 brojDana = datum.getNoOfDays() - datum1.getNoOfDays();

			dp::IStatementPtr pSelectt = dp::getMainDatabase()->createStatement("SELECT SUM(s.Tezina) as ukupTez FROM Tiketi s WHERE s.ProjekatID=?");
			dp::Params pParamss(pSelectt->allocParams());
			pParamss << ID1;

			td::INT4 ukupTez;
			dp::Columns colss = pSelectt->allocBindColumns(1);
			colss << "ukupTez" << ukupTez;

			if (!pSelectt->execute())
				return false;
			pSelectt->moveNext();
			// sve dijelis sa sumom tezine zavrsenih tiketa
			int a = 2;
			int b = 3;
			int c = 3;
			double x=0;
			if (ukupTez != 0)x = 1. / ukupTez * brojDana / 24.;
			else x = 0;
			row[4] = x;
			//row[4] = _sviRedovi[i][4]; OVAJ JE BITANNNN
			
			_te->insertRow(i);
			_te->endUpdate();
			//_te.insertRow(row);
		}

		return true;
	}
	else if (btnID == gui::Dialog::Button::ID::Cancel) {
		return true;
	}
	return true;
}
};