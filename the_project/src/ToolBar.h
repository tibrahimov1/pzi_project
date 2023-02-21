#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include <gui/Symbol.h>

class ToolBar : public gui::ToolBar
{
protected:
    gui::Image _imgProj;
    gui::Image _imgEmpl;
    gui::Image _imgReq;
    gui::Image _imgSrch;
    gui::Image _imgStat;
public:
    ToolBar()
    : gui::ToolBar("mainTB", 5)
    , _imgProj(":imgProj")
    , _imgEmpl(":imgEmpl")
    , _imgReq(":imgReq")
    , _imgSrch(":imgSrch")
    , _imgStat(":imgStat")
    {
        initItem(0, "proj", tr("Projects"), &_imgProj, tr(""), 20, 0, 0, 10);//broj menija i broj akcije
        initItem(1, "empl", tr("Employees"), &_imgEmpl, tr(""), 20, 0, 0, 20);
        initItem(2, "req", tr("Requests"), &_imgReq, tr(""), 20, 0, 0, 30);
        initItem(3, "srch", tr("Search"), &_imgSrch, tr(""), 30, 0, 0, 10);
        initItem(4, "stat", tr("Statistics"), &_imgStat, tr(""), 30, 0, 0, 20);
    }
};

