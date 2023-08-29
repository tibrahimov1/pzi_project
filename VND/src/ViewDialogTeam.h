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

class VND_LIB_API ViewDialogTeam : public gui::View
{
	//////////////////////////////////
	protected:
		gui::Label _name;
		gui::LineEdit _Nname;
		gui::Label _opis;
		gui::LineEdit _Oopis;

		gui::GridLayout _gl;
public:
	ViewDialogTeam() :
		_gl(2, 2) //zbog spacinga 
		, _name(tr("Naziv"))
		, _opis(tr("Opis"))
	{
		gui::GridComposer gc(_gl);
		gc.appendRow(_name);
		gc.appendCol(_Nname, -1);
		gc.appendRow(_opis);
		gc.appendCol(_Oopis, -1);
		gui::View::setLayout(&_gl);
	}

	bool spasi() {
		td::INT4 ID, ProjekatID=-1, BrClanova=0;
		td::String Opis, Ime;
		dp::IStatementPtr pSelect1 = dp::getMainDatabase()->createStatement("SELECT MAX(ID)+1 as ID FROM Tim");
		dp::Columns cols1 = pSelect1->allocBindColumns(1);

		cols1 << "ID" << ID;

		if (!pSelect1->execute())
			return false;
		pSelect1->moveNext();

		dp::IStatementPtr pInsertItem(dp::getMainDatabase()->createStatement("insert into Tim(ID, BrClanova, Ime, Opis, ProjekatID) values(?,?,?,?,?)"));
		dp::Params pParams2(pInsertItem->allocParams());
		
		td::Variant val;
		_Nname.getValue(val);
		Ime = val.strVal();

		_Oopis.getValue(val);
		Opis = val.strVal();
		
		pParams2 << ID << BrClanova << dp::toNCh(Ime,100) << dp::toNCh(Opis,100) << ProjekatID;

		if (!pInsertItem->execute())
		{
			return false;
		}


		/*dp::IDataSet* pDS = _te.getDataSet();
		auto rowCnt = pDS->getNumberOfRows();
		for (size_t iRow = 0; iRow < rowCnt; ++iRow)
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
};