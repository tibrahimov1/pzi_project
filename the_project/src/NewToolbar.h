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
        , _imgProj(":imgProj")
        , _imgEmpl(":imgEmpl")

    {
        initItem(0, "proj", tr("Projects"), &_imgProj, tr(""), 20, 0, 0, 10);//broj menija i broj akcije
        initItem(1, "empl", tr("Employees"), &_imgEmpl, tr(""), 20, 0, 0, 20);
    }
};

