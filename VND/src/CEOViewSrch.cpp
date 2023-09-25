#pragma once
#include "CEOViewSrch.h"
#include "gui/FileDialog.h"
#include <dp/IDatabase.h>
#include "DialogProj.h"
#include "../../common/include/Globals.h"
CEOViewSrch::CEOViewSrch() :
	_gl(8, 6)
	, _name(tr("ProjName"))
	, _manName(tr("ManName"))
	, _finish(tr("Finish"))
	, _date(tr("Date"))
	, _finDate(tr("FinDate"))
	, _num(tr("Pnum"))
	, _prog(tr("Prog"))
	, _spec(tr("Spec"))
	, _btnFile(tr("OpFile"))
	, _ucitaj(tr("Load"))
	, _db(dp::getMainDatabase())
	, _Nnum(td::int4, gui::LineEdit::Messages::DoNotSend)
	, _lpod(td::int4, gui::LineEdit::Messages::DoNotSend)
	, _lpdo(td::int4, gui::LineEdit::Messages::DoNotSend)
	, _lbod(td::int4, gui::LineEdit::Messages::DoNotSend)
	, _lbdo(td::int4, gui::LineEdit::Messages::DoNotSend)
	, _dod("Datum od: ")
	, _ddo("Datum do: ")
	, _pod("Progres od(%): ")
	, _pdo("Progres do(%): ")
	, _bod("Broj ljudi od: ")
	, _bdo("Broj ljudi do: ")
{
	// postavljanje layouta
	gui::GridComposer gc(_gl);

	gc.appendRow(_dod);
	gc.appendCol(_ldod);
	gc.appendCol(_ddo);
	gc.appendCol(_lddo);
	gc.appendRow(_pod);
	gc.appendCol(_lpod);
	gc.appendCol(_pdo);
	gc.appendCol(_lpdo);

	gc.appendRow(_bod);
	gc.appendCol(_lbod);
	gc.appendCol(_bdo);
	gc.appendCol(_lbdo);
	gc.appendCol(_manName);
	gc.appendCol(_Eman);

	gc.appendRow(_name);
	gc.appendCol(_Ename);
	gc.appendCol(_finish);
	gc.appendCol(_Cfinish);
	gc.appendCol(_spec);
	gc.appendCol(_btnFile);

	gc.appendRow(_date);
	gc.appendCol(_Ddate);
	gc.appendCol(_finDate);
	gc.appendCol(_DfinDate);
	gc.appendCol(_num);
	gc.appendCol(_Nnum);
	_Ddate.setAsReadOnly();
	_DfinDate.setAsReadOnly();
	_Nnum.setAsReadOnly();

	gc.appendRow(_prog);
	gc.appendCol(_Pprog);
	gc.appendEmptyCols(3);
	gc.appendCol(_ucitaj);
	_Pprog.setAsReadOnly();

	gc.appendRow(_te, 0);
	gui::View::setLayout(&_gl);

	_Cfinish.addItem("");
	_Cfinish.addItem("Da");
	_Cfinish.addItem("Ne");
	_Cfinish.selectIndex(0);

	populateData();
}

bool CEOViewSrch::onClick(gui::Button* pBtn) {
	if (pBtn == &_btnFile) {
		gui::OpenFileDialog* s = new gui::OpenFileDialog(this, "Open file", { ".xml" });
		s->openModal(DlgID::FileSelect, this);
	}
	else if (pBtn == &_ucitaj) {
		// prilikom svake nove pretrage rezultati stare se trebaju obrisati
		int vell = _pDS->getNumberOfRows();
		for (size_t i = 0; i < vell; ++i) {
			int iRow = _te.getFirstSelectedRow();
			if (iRow < 0) return false;
			dp::IDataSet* pDS = _te.getDataSet();
			auto& row = pDS->getRow(iRow);

			_te.beginUpdate();
			_te.removeRow(iRow);
			_te.endUpdate();
		}

		// gleda se koji podaci zadovoljavaju kriterije pa se spasava indeks onih redova koji se trebaju prikazati
		std::vector<int> zacit;
		for (size_t i = 0; i < _vel; ++i) {
			bool dodat = false;
			td::String da("Da"), ne("Ne"), nista("");
			td::Variant v, v1, v2, v3, v4, v5, v6, v7, v8;
			_Eman.getValue(v);
			_Cfinish.getValue(v1);
			_Ename.getValue(v2);
			_lbod.getValue(v3);
			_lbdo.getValue(v4);
			_lpod.getValue(v5);
			_lpdo.getValue(v6);
			_ldod.getValue(v7);
			_lddo.getValue(v8);
			td::Date d1 = v7.dateVal(), d2 = v8.dateVal();
			td::Date danas;
			danas.now();
; 
			// dozvoljava se da ne budu sva polja unesena, tj moze se pretrazivati po proizvoljnom broju kriterija
			if (v1.i4Val() == 0 || (_txt[i][2] == da && v1.i4Val() == 1) || (_txt[i][2] == ne && v1.i4Val() == 2))
				if (_txt[i][1] == v.strVal() || v.strVal().isNull())
					if (_txt[i][0] == v2.strVal() || v2.strVal().isNull())
						if (!v3.isNonZero() || _br[i][0] >= v3.i4Val())
							if (!v4.isNonZero() || _br[i][0] <= v4.i4Val())
								if (!v5.isNonZero() || _br[i][3] >= v5.i4Val())
									if (!v6.isNonZero() || _br[i][3] <= v6.i4Val())
										if (d1==danas || jelveci(_txt[i][3], d1))
											if (d2 == danas || !jelveci(_txt[i][4], d2))
												dodat = true;
			if (dodat==true) zacit.push_back(i);
		}

		// prikazivanje odgovarajucih redova
		for (size_t i = 0; i < _vel; ++i) {
			auto it = std::find(zacit.begin(), zacit.end(), i);
			if (it != zacit.end()) {
				dp::IDataSet* pDS = _te.getDataSet();
				auto vel2 = pDS->getNumberOfRows();
				auto& row = _te.getEmptyRow();

				_te.beginUpdate();

				row[0].setValue(_txt[i][0]);
				row[1].setValue(_txt[i][1]);
				row[2].setValue(_txt[i][2]);
				row[3].setValue(_txt[i][3]);
				row[4].setValue(_txt[i][4]);
				row[5].setValue(_br[i][0]);
				row[6].setValue(_br[i][1]);
				row[7].setValue(_br[i][2]);

				_te.insertRow(vel2);
				_te.endUpdate();
			}
		}
	}
	return true;
}

bool CEOViewSrch::jelveci(td::String s, td::Date d) {
	// pomocna funkcija koja poredi datume
	td::INT4 dan = 1, mjesec = 1, god = 2000;
	td::String a, a1;
	a1 = s.subStr(0, 1);
	dan = a1.toINT4();
	a1 = s.subStr(3, 4);
	mjesec = a1.toINT4();
	a1 = s.subStr(5, 8);
	god = a1.toINT4();
	td::INT4 d1 = god * 10000 + mjesec * 100 + dan;

	dan = d.getDay();
	mjesec = d.getMonth();
	god = d.getYear();
	td::INT4 d2 = god * 10000 + mjesec * 100 + dan;

	return d1>=d2;
}

void CEOViewSrch::populateData() {
	// kreiranje tabele
	_pDS = (_db->createDataSet("SELECT a.Ime as 'Naziv', "
		"CASE WHEN a.Zavrsen = 0 THEN 'Ne' WHEN a.Zavrsen = 1 THEN 'Da' END AS Zavrsen, "
		"CASE WHEN a.DatumPoc>0 THEN CAST(a.DatumPoc/1000000 AS TEXT) || '.' || CAST(a.DatumPoc%100000/10000 AS TEXT) || '.' || CAST(a.DatumPoc%10000 AS TEXT) || '.' "
		"ELSE '0' END AS 'Datum pocetka', "
		"CASE WHEN a.DatumKraj>0 THEN CAST(a.DatumKraj/1000000 AS TEXT) || '.' || CAST(a.DatumKraj%100000/10000 AS TEXT) || '.' || CAST(a.DatumKraj%10000 AS TEXT) || '.' "
		"ELSE '0' END AS 'Datum zavrsetka', "
		"b.Ime || ' ' || b.Prezime as 'Project manager', c.BrClanova as 'Broj ljudi', "
		" COALESCE( ( SELECT SUM(T.Tezina) FROM Tiketi T WHERE T.Stanje = 2 AND T.ProjekatID = a.ID ), 0 ) AS 'bzz2', "
		" COALESCE( ( SELECT SUM(T.Tezina) FROM Tiketi T WHERE T.ProjekatID = a.ID ), 0	) AS 'bzz1'"
		" FROM Projekti a INNER JOIN Korisnik b ON a.MenadzerID = b.ID INNER JOIN Tim c ON c.ProjekatID = a.ID WHERE a.ID > 0"));
	dp::DSColumns cols(_pDS->allocBindColumns(8));
	td::String Ime, Prezime, ProjName, DatumPoc, DatumKraj, Zavrsen;
	td::INT4 BrClanova, bzz1, bzz2;
	cols << "Naziv" << ProjName << "Project manager" << Ime << "Zavrsen" << Zavrsen << "Datum pocetka"
		<< DatumPoc << "Datum zavrsetka" << DatumKraj << "Broj ljudi" << BrClanova << "bzz1" << bzz1 << "bzz2" << bzz2;
	if (!_pDS->execute())
	{
		_pDS = nullptr;
		return;
	}
	_te.init(_pDS, { 0,1,2,3,4,5 });

	// radi lakse manipulacije s podacima velicina tabele kao i njeni sadrzaji se spasavaju u nove vektore, a zatim se brisu iz tabele
	_vel = _pDS->getNumberOfRows();
	for (size_t i = 0; i < _vel; ++i) {
		int iRow = _te.getFirstSelectedRow();
		if (iRow < 0) return;
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);

		std::vector < td::INT4 > brr;
		std::vector < td::String > txtt;

		auto p = row[7] * 100 / row[6];
		int m = p.i4Val();

		brr.push_back(row[5].i4Val());
		brr.push_back(row[6].i4Val());
		brr.push_back(row[7].i4Val());
		brr.push_back(m);
		_br.push_back(brr);

		txtt.push_back(row[0].strVal());
		txtt.push_back(row[1].strVal());
		txtt.push_back(row[2].strVal());
		txtt.push_back(row[3].strVal());
		txtt.push_back(row[4].strVal());
		_txt.push_back(txtt);

		_te.beginUpdate();
		_te.removeRow(iRow);
		_te.endUpdate();
	}
}

td::INT4 CEOViewSrch::getIDfromTable(int rowID)
{
	dp::IDataSet* pDS = _te.getDataSet();
	auto& row = pDS->getRow(rowID);
	return row[0].i4Val();
}

bool CEOViewSrch::onChangedSelection(gui::TableEdit* pTE)
{
	if (pTE == &_te) {
		int iRow = _te.getFirstSelectedRow();
		if (iRow < 0) {
			_ldod.toZero();
			_lddo.toZero();
			_lpdo.toZero();
			_lpod.toZero();
			_lbod.toZero();
			_lbdo.toZero();
			_Ename.toZero();
			_Eman.toZero();
			_Cfinish.toZero();
			_Ddate.toZero();
			_DfinDate.toZero();
			_Nnum.toZero();
			_Pprog.toZero();
			return true;
		}
		// ovim prikazujemo selektovani red
		dp::IDataSet* pDS = _te.getDataSet();
		auto& row = pDS->getRow(iRow);
		_Nnum.setValue(row[5]);

		auto p = row[7] * 100 / row[6];
		int mrs = p.i4Val();
		double w = mrs / 100.;
		_Pprog.setValue(w);

		td::INT4 dan, mjesec, god;
		td::String a, a1;
		a = row[3];
		a1 = a.subStr(0, 1);
		dan = a1.toINT4();
		a1 = a.subStr(3, 4);
		mjesec = a1.toINT4();
		a1 = a.subStr(5, 8);
		god = a1.toINT4();
		td::Date datum(god, mjesec, dan);
		_Ddate.setValue(datum);

		a = row[4];
		a1 = a.subStr(0, 1);
		dan = a1.toINT4();
		a1 = a.subStr(3, 4);
		mjesec = a1.toINT4();
		a1 = a.subStr(5, 8);
		god = a1.toINT4();
		td::Date datum1(god, mjesec, dan);
		_DfinDate.setValue(datum1);
		
		return true;
	}
	return false;
}