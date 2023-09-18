#pragma once
#include <gui/Dialog.h>
#include <gui/Frame.h>
#include "../../common/include/VNDLib.h"
#include "ViewDialogZah.h"
class VND_LIB_API DialogZah : public gui::Dialog
{
protected:
	ViewDialogZah _viewDialogZah;
	gui::TableEdit* _te;
public:
	DialogZah(gui::Frame* pView, gui::TableEdit* _te1) :
		gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
								{gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(400, 100)
					)
		, _te(_te1)
	{
		setCentralView(&_viewDialogZah);
	}
	bool onClick(gui::Dialog::Button::ID btnID, gui::Button* pButton) override
	{
		if (btnID == gui::Dialog::Button::ID::OK) {
			

			std::vector<cnt::SafeFullVector<td::Variant, false>> _sviRedovi1;
			gui::TableEdit _te2;

			dp:: IDataSet* _pDS = (dp::getMainDatabase()->createDataSet("SELECT k.ID as ID, k.Ime || ' ' || k.Prezime as 'Ime i prezime', Cast(z.Datum/1000000 AS Varchar(255)) || '/' || Cast((z.Datum-(z.Datum/1000000)*1000000)/10000 As Varchar(255))  || '/' || Cast(z.Datum%10000 AS Varchar(255)) As Datum, z.Opis as Zahtjev, CASE WHEN z.Status=0 THEN 'Neodgovoren' ELSE 'Odgovoren' END as Status, z.ID as ID1 FROM Korisnik k, Zahtjevi z where k.ID=z.SenderID"));

			//OVO SAM IMAO PRVOBITNO ------- and t.VlasnikID=a.MenadzerID

			dp::DSColumns cols(_pDS->allocBindColumns(6));
			td::String Name, Zahtjev, Status1, Datum1;
			td::INT4 ID, Datum, ID2;
			//ovdje ima sila popravki                                        
			cols << "ID" << ID << "Ime i prezime" << Name << "Datum" << Datum1 << "Zahtjev" << Zahtjev << "Status" << Status1 << "ID1" << ID2;
			if (!_pDS->execute())
			{
				_pDS = nullptr;
				return false;
			}
			_te2.init(_pDS, { 1,2,3,4 });

			dp::IDataSet* pDS11 = _te2.getDataSet();
			for (size_t i = 0; i < pDS11->getNumberOfRows(); ++i)
			{
				auto& row = pDS11->getRow(i);
				_sviRedovi1.push_back(row);
			}
			std::vector<td::INT4> minimum;
			for (int i = 0; i < _sviRedovi1.size(); i++) {
				minimum.push_back(_sviRedovi1[i][5].i4Val());
			}
			td::INT4 ID1 = 1;
			std::sort(minimum.begin(), minimum.end());

			for (auto x : minimum) {
				if (x == ID1)ID1++;
				else if (x > ID1) {
					break;
				}
			}

			/*dp::IStatementPtr ubaci = dp::getMainDatabase()->createStatement("WITH RECURSIVE available_ids AS (SELECT 1 AS candidate_id UNION SELECT candidate_id + 1 FROM available_ids WHERE candidate_id < (SELECT MAX(ID) FROM Zahtjevi)) SELECT MIN(candidate_id) AS ID1 FROM available_ids WHERE candidate_id >= 1 AND candidate_id NOT IN(SELECT ID FROM Zahtjevi)");
			dp::Columns kol(ubaci->allocBindColumns(1));
			kol << "ID1" << ID1;

			if (!ubaci->execute())
				return false;
			ubaci->moveNext();*/

			td::INT4 ReceiverID = _viewDialogZah.DajIDComba();
			td::INT4 SenderID = Globals::_currentUserID;
			td::String Opis = _viewDialogZah.DajText();

			td::Date datum;
			datum.now();
			td::INT4 dan=datum.getDay(), mjesec=datum.getMonth(), godina=datum.getYear();
			td::INT4 vrijed = dan * 1000000 + mjesec * 10000 + godina;
			td::INT4 Status = 0;

			dp::IStatementPtr ins = dp::getMainDatabase()->createStatement("INSERT INTO Zahtjevi Values (?,?,?,?,?,?)");
			dp::Params pParams2(ins->allocParams());
			pParams2 << ID1 << SenderID << ReceiverID << dp::toNCh(Opis,100) << Status << vrijed;

			if (!ins->execute())
			{
				return false;
			}
			//***************************
			//write to db the new project
			//***************************
			return true;
		}
		return true;
	}
};
