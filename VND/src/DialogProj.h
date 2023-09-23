#pragma once
#include <gui/Dialog.h>
#include <gui/Frame.h>
#include "../../common/include/VNDLib.h"
#include "ViewDialogProj.h"
class VND_LIB_API DialogProj : public gui::Dialog
{
protected:
	ViewDialogProj _viewDialogProj;
	dp::IDatabasePtr _db;
	dp::IDataSetPtr _pDS;

public:
	DialogProj(gui::Frame* pView) :
		gui::Dialog(pView, { {gui::Dialog::Button::ID::OK, tr("Ok"), gui::Button::Type::Default},
								{gui::Dialog::Button::ID::Cancel, tr("Cancel")} }, gui::Size(400, 100)
		)
		, _db(dp::getMainDatabase())
	{
		setCentralView(&_viewDialogProj);
	}
	bool onClick(gui::Dialog::Button::ID btnID, gui::Button* pButton) override
	{
		if (btnID == gui::Dialog::Button::ID::OK) {
			// uzimaju se podaci iz dijaloga
			td::String _filename = _viewDialogProj.getFileName();
			td::String _projName = _viewDialogProj.getName();
			td::Date _sDate = _viewDialogProj.getDate();
			td::Date _sDate2 = _viewDialogProj.getDate2();
			td::INT4 _manID = _viewDialogProj.getID();
			td::String _opis = _viewDialogProj.getOpis();

			// trazi se sljedeci slobodni ID (a kako se projekti ne brisu iz baze ne treba se brinuti za 'rupe' u ID-evima)
			dp::IStatementPtr pS = dp::getMainDatabase()->createStatement("SELECT MAX(ID) as 'max' FROM Projekti");
			td::INT4 idd;
			dp::Columns kol = pS->allocBindColumns(1);
			kol << "max" << idd;

			if (!pS->execute())
				return false;
			pS->moveNext();

			// ubacivanje novog projekta u bazu podataka
			dp::IStatementPtr pInsertItem(_db->createStatement("INSERT INTO Projekti(ID, Ime, MenadzerID, Zavrsen, DatumPoc, DatumKraj, Opis) VALUES(?,?,?,?,?,?,?)"));
			dp::Params pParams2(pInsertItem->allocParams());
			td::INT4 id, menid, zavrs, dpoc, dkraj;
			td::String ime, opis;
			pParams2 << id << dp::toNCh(ime, 100) << menid << zavrs << dpoc << dkraj << dp::toNCh(opis, 100);

			id = idd + 1;
			ime = _projName;
			opis = _opis;
			menid = _manID;
			zavrs = 0;
			td::INT4 dan = _sDate.getDay(), mjesec = _sDate.getMonth(), godina = _sDate.getYear();
			dpoc = dan * 1000000 + mjesec * 10000 + godina;

			dan = _sDate2.getDay(); mjesec = _sDate2.getMonth(); godina = _sDate2.getYear();
			dkraj = dan * 1000000 + mjesec * 10000 + godina;

			if (!pInsertItem->execute()) {
				return false;
			}

			return true;
		}
		return true;
	}
};