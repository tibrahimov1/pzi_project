#pragma once
#include <gui/View.h>
#include "SwitcherView.h"
#include <gui/Controls.h>
#include <gui/Navigator.h>
#include "../../common/include/VNDLib.h"
class VND_LIB_API NavigatorView : public gui::View
{

protected:
    gui::HorizontalLayout _hl;
    gui::Image _imgProj;
    gui::Image _imgEmpl;
    gui::Image _imgReq;
    gui::Image _imgSrch;
    gui::Image _imgStat;
    gui::Navigator _navigator;
    SwitcherView _switcherView;
public:
    NavigatorView():
    _hl(2)
    , _navigator(5,true)
    , _imgProj(":imgProj")
    , _imgEmpl(":imgEmpl")
    , _imgReq(":imgReq")
    , _imgSrch(":imgSrch")
    , _imgStat(":imgStat")
    {
        setMargins(0, 0, 0, 0);
        _navigator.setItem(0, &_imgProj, tr("Project"));
        _navigator.setItem(1, &_imgEmpl, tr("Employees"));
        _navigator.setItem(2, &_imgReq, tr("Requests"));
        _navigator.setItem(3, &_imgSrch, tr("Search"));
        _navigator.setItem(4, &_imgStat, tr("Statistics"));
        _hl.append(_navigator);
        _hl.append(_switcherView);
        setLayout(&_hl);
    }
    void setNavigatorSelection(td::UINT2 sel)
    {
        _navigator.setCurrentSelection(sel);
        _switcherView.showView((int)sel);
    }

    virtual bool onChangedSelection(gui::Navigator* pNav)
    {
        td::UINT2 currSelection = pNav->getCurrentSelection();
        _switcherView.showView((int)currSelection);
        return true;
    }
};