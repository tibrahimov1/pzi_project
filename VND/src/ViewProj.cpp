#pragma once
#include "ViewProj.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "DialogProj.h"
#include "../../common/include/Globals.h"
ViewProj::ViewProj():
	_gl(3,6)
	,_name(tr("ProjName"))
	//,_manName(tr("ManName"))
	//,_finish(tr("Finish"))
	,_date(tr("Date"))
	,_finDate(tr("FinDate"))
	,_num(tr("Pnum"))
	,_prog(tr("Prog"))
	,_spec(tr("Spec"))
	,_btnFile(tr("OpFile"))
	//,_btnProj(tr("NewProj"))
	,_db(dp::getMainDatabase())
	,_Nnum(td::int4, gui::LineEdit::Messages::DoNotSend)
{
	gui::GridComposer gc(_gl);
	gc.appendRow(_name);
	gc.appendCol(_Ename);
	_Ename.setAsReadOnly();
	//gc.appendCol(_manName);
	//gc.appendCol(_Eman);
	//gc.appendCol(_finish);
	//gc.appendCol(_Cfinish);

	gc.appendCol(_date);
	gc.appendCol(_Ddate);
	_Ddate.setAsReadOnly();
	gc.appendCol(_finDate);
	gc.appendCol(_DfinDate);
	_DfinDate.setAsReadOnly();

	gc.appendRow(_prog);
	gc.appendCol(_Pprog);
	_Pprog.setAsReadOnly();
	gc.appendCol(_spec);
	gc.appendCol(_btnFile);
	gc.appendCol(_num);
	gc.appendCol(_Nnum);
	
	
	
	//gc.appendEmptyCols(1);
	//gc.appendCol(_btnProj);

	gc.appendRow(_te, 0);
	gui::View::setLayout(&_gl);

	populateData();

	//setaasreadonly
}

bool ViewProj::onClick(gui::Button* pBtn) {
	if (pBtn == &_btnFile) {
		gui::OpenFileDialog* s = new gui::OpenFileDialog(this, "Open file", { ".xml" });
		s->openModal(DlgID::FileSelect,this);
	}
	/*else if (pBtn == &_btnProj) {
		DialogProj* p = new DialogProj(this);
		p->openModal(DlgID::NewProj, this);
		p->setTitle(tr("NewProj"));
	}*/
	return true;
}
bool ViewProj::onChangedValue(gui::DateEdit* dEdit) {
	return true;
}
void ViewProj::populateData() {
	_pDS = (_db->createDataSet("SELECT a.Ime as ProjName, a.Zavrsen, a.DatumPoc, a.DatumKraj, b.Ime, b.Prezime"
		" FROM Projekti a, Korisnik b WHERE a.MenadzerID=b.ID"));
	dp::DSColumns cols(_pDS->allocBindColumns(6));
	td::String Ime, Prezime, ProjName;
	td::INT4 Zavrsen, DatumPoc, DatumKraj;
	//ovdje ima sila popravki                                        
	cols << "ProjName" << ProjName << "Zavrsen" << Zavrsen << "DatumPoc" << DatumPoc << "DatumKraj" << DatumKraj << "Ime" << Ime << "Prezime" << Prezime;
	if (!_pDS->execute())
	{
		_pDS = nullptr;
		return;
	}
	_te.init(_pDS, { 0,1,2,3,4,5 });

}