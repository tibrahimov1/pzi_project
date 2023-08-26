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

class VND_LIB_API ViewDialogTick : public gui::View
{
protected:
	gui::GridLayout _gl;
	gui::Label _tim;
	gui::DBComboBox _Ctim;
	gui::Label _dokum;
	gui::Button _btnFile;
	gui::Label _tezina;
	gui::NumericEdit _Dtezina;

	td::String _filename;

public:
	ViewDialogTick() :
		_gl(3, 2) //zbog spacinga 
		, _tim(tr("Team"))
		, _dokum(tr("Document"))
		, _btnFile(tr("Open File Dialog"))
		, _Ctim(td::int4)
		, _tezina(tr("Tezina"))
		, _Dtezina(td::int4)
	{
		gui::GridComposer gc(_gl);
		gc.appendRow(_tim);
		gc.appendCol(_Ctim, -1);
		gc.appendRow(_dokum);
		gc.appendCol(_btnFile, -1);
		gc.appendRow(_tezina);
		gc.appendCol(_Dtezina, -1);
		gc.appendEmptyCols(2);
		gui::View::setLayout(&_gl);
		populateCombo();
	}
	td::String getImeTima() const {
		td::Variant v;
		_Ctim.getValue(v);
		auto konj = _Ctim.getSelectedText();
		return konj;
	}
	td::INT4 getID() const {
		td::Variant v;
		_Ctim.getValue(v);
		return v.i4Val();
	}
	td::String getFileName() const {
		return _filename;
	}
	td::INT4 getTezina() const {
		td::Variant v;
		_Dtezina.getValue(v);
		return v.i4Val();
	}
	bool onClick(gui::Button* pBtn) override {
		//openfile handle
		if (pBtn == &_btnFile) {
			gui::OpenFileDialog* s = new gui::OpenFileDialog(this, "Open file", {});
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
		dp::IStatementPtr pStat(dp::getMainDatabase()->createStatement("SELECT ID, Ime, Opis"
			" FROM Tim"));
		dp::Columns cols(pStat->allocBindColumns(3));
		td::String name, opis;
		td::INT4 ID;
		cols << "ID" << ID << "Ime" << name<< "Opis"<<opis;
		if (!pStat->execute()) return;
		while (pStat->moveNext()) {
			cnt::StringBuilderSmall sb();

			_Ctim.addItem(name, ID); //NE ZNAM SABRAT STRINGOVE
		}
		_Ctim.selectIndex(0);
	};
};