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

class VND_LIB_API ViewDialogProj : public gui::View
{
protected:
	gui::Label _name;
	gui::GridLayout _gl;
	gui::LineEdit _Ename;
	gui::Label _spec;
	gui::Button _btnFile;
	gui::Label _date;
	gui::DateEdit _Ddate;
	gui::Label _man;
	gui::DBComboBox _Cman;

	td::String _filename;

public:
	ViewDialogProj() :
	_gl(4,2) //zbog spacinga 
	,_name(tr("NewProj"))
	,_spec(tr("Spec"))
	,_btnFile(tr("OpFile"))
	,_date(tr("date"))
	,_man(tr("ManName"))
	,_Cman(td::int4)
	{
		gui::GridComposer gc(_gl);
		gc.appendRow(_name);
		gc.appendCol(_Ename, -1);
		gc.appendRow(_spec);
		gc.appendCol(_btnFile,-1);
		gc.appendRow(_date);
		gc.appendCol(_Ddate,-1);
		gc.appendRow(_man);
		gc.appendCol(_Cman, -1);
		gc.appendEmptyCols(2);
		gui::View::setLayout(&_gl);
		populateCombo();
	}
	td::Date getDate() const {
		td::Variant v;
		_Ddate.getValue(v);
		return v.dateVal();
	}
	td::INT4 getID() const {
		td::Variant v;
		_Cman.getValue(v);
		return v.i4Val();
	}
	td::String getFileName() const {
		return _filename;
	}
	td::String getName() const{
		td::Variant v;
		_Ename.getValue(v);
		return v.strVal();
	}
	bool onClick(gui::Button* pBtn) override {
		//openfile handle
		if (pBtn == &_btnFile) {
			gui::OpenFileDialog* s = new gui::OpenFileDialog(this, "Open file",{});
			s->openModal(DlgID::FileSelect, this);
		}

		td::String fileName;
		return true;
	}
	bool onClick(gui::FileDialog* pDlg, td::UINT4 dlgID)
	{
		if ((DlgID)dlgID == DlgID::FileSelect)
		{
			if (pDlg->getStatus() == gui::FileDialog::Status::OK)
			{
				_filename = pDlg->getFileName();
			}
		}
		return false;
	}
	void populateCombo() {
		dp::IStatementPtr pStat(dp::getMainDatabase()->createStatement("SELECT ID, Ime, Prezime"
			" FROM Korisnik"
			" WHERE TipID=2"));
		dp::Columns cols(pStat->allocBindColumns(3));
		td::String name, lastname;
		td::INT4 ID;
		cols <<"ID"<<ID << "Ime" << name << "Prezime" << lastname;
		if (!pStat->execute()) return;
		while (pStat->moveNext()) {
			cnt::StringBuilderSmall sb();
			
			_Cman.addItem( name, ID); //NE ZNAM SABRAT STRINGOVE
		}
		_Cman.selectIndex(0);
	}
};