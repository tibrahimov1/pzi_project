#pragma once
#include <gui/ViewSwitcher.h>
#include "ViewEmpl.h"
#include "ViewProj.h"
#include "ViewSrch.h"
#include "ViewTick.h"
#include "ViewTeam.h"
#include "ViewStat.h"

class SwitcherView : public gui::ViewSwitcher
{
private:
protected:
    ViewProj _viewProj;


public:
    SwitcherView()
        : gui::ViewSwitcher(7)

    {
        addView(&_viewProj, true);

    }
};
