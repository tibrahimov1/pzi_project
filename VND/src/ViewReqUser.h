#pragma once
#include <gui/View.h>
#include <gui/Label.h>
#include <gui/LineEdit.h>
#include <gui/TableEdit.h>
#include <dp/IDataSet.h>
#include <dp/IDatabase.h>
#include <dp/IStatement.h>
#include <gui/Button.h>
#include <gui/GridLayout.h>
#include <gui/GridComposer.h>
#include <gui/DateEdit.h>
#include "DialogNewReq.h"
#include "../../common/include/VNDLib.h"

class VND_LIB_API ViewReqUser : public gui::View 
{
protected:
	gui::Label _towards;
	gui::Label _req;
	gui::Label _date;
	gui::Label _status;
	
	gui::LineEdit _stowards;
	gui::LineEdit _sreq;
	gui::DateEdit _sdate;
	gui::LineEdit _sstatus;

	gui::Button _hidbtn;
	gui::Button _btn;
	gui::TableEdit _te;
	gui::GridLayout _gl;
	dp::IDatabase *_db;
	dp::IDataSet* _pDs;
public:
	ViewReqUser();
	bool onClick(gui::Button* pBtn) override;
	bool onChangedSelection(gui::TableEdit* pTe) override;
	void populateData();

};
