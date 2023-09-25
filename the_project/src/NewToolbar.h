#pragma once
#include <gui/ToolBar.h>
#include <gui/Image.h>
#include <gui/Symbol.h>

class NewToolBar : public gui::ToolBar
{
protected:
    gui::Image _imgProj;
    gui::Image _imgEmpl;

public:
    NewToolBar()
        : gui::ToolBar("mainTB", 5)
        , _imgProj(":imgReq")
        , _imgEmpl(":imgStat")

    {
        initItem(0, "proj", tr("Requests"), &_imgProj, tr(""), 20, 0, 0, 30);//broj menija i broj akcije
        initItem(1, "empl", tr("Statistika"), &_imgEmpl, tr(""), 30, 0, 0, 20);
    }
};

