#pragma once
#include <gui/View.h>
#include <gui/Button.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/NumericEdit.h>
#include <gui/DateEdit.h>
#include <gui/DBComboBox.h>
#include <gui/FileDialog.h>
#include "../../common/include/VNDLib.h"
#include <gui/GridComposer.h>
#include "../../common/include/Globals.h"
#include <gui/GridLayout.h>
#include <td/Date.h>
#include <cnt/StringBuilder.h>
#include <gui/Types.h>
#include <dp/IDatabase.h>
#include <dp/IDataSet.h>

class VND_LIB_API CEOViewDialogEmpl : public gui::View
{
protected:
	gui::GridLayout _gl;

	gui::Label _ime;
	gui::LineEdit _lime;
	gui::Label _prez;
	gui::LineEdit _lprez;
	gui::Label _pod;
	gui::Button _bpod;
	gui::Label _poz;
	gui::ComboBox _cpoz;
	td::String _fajl;

public:
	CEOViewDialogEmpl() :
		_gl(4, 2)
		, _ime("Ime: ")
		, _prez("Prezime: ")
		, _pod("Podaci: ")
		, _bpod("Otvori file")
		, _poz("Pozicija: ")
	{
		gui::GridComposer gc(_gl);
		gc.appendRow(_ime);
		gc.appendCol(_lime);
		gc.appendRow(_prez);
		gc.appendCol(_lprez);
		gc.appendRow(_pod);
		gc.appendCol(_bpod);
		gc.appendRow(_poz);
		gc.appendCol(_cpoz);
		gui::View::setLayout(&_gl);

		_cpoz.addItem("Projekt menadzer");
		_cpoz.addItem("Radnik");
		_cpoz.selectIndex(0);
	}
	
	td::String getIme() const {
		return _lime.getText();
	}
	td::String getPrez() const {
		return _lprez.getText();
	}
	td::INT4 getTipID() const {
		td::Variant v;
		_cpoz.getValue(v);
		return v.i4Val()+2;
	}
	td::String getFileName() const {
		return _fajl;
	}
	bool onClick(gui::Button* pBtn) override {
		if (pBtn == &_bpod) {
			gui::OpenFileDialog* s = new gui::OpenFileDialog(this, "Open file", {});
			s->openModal(DlgID::FileSelect, this);
		}
		return true;
	}
	bool onClick(gui::FileDialog* pDlg, td::UINT4 dlgID)
	{
		if ((DlgID)dlgID == DlgID::FileSelect)
		{
			if (pDlg->getStatus() == gui::FileDialog::Status::OK)
			{
				_fajl = pDlg->getFileName();
			}
		}
		return false;
	}

};