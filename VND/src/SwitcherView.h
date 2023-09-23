#pragma once
#include <gui/ViewSwitcher.h>
#include "CEOViewEmpl.h"-
#include "CEOViewProj.h"
#include "CEOViewSrch.h"
#include "CEOViewReq.h"
#include "ViewTick.h"
#include "ViewTeam.h"
#include "CEOViewStat.h"

class SwitcherView : public gui::ViewSwitcher
{
private:
protected:
    CEOViewProj _viewProj;
    CEOViewEmpl _viewEmpl;
    CEOViewReq _viewReq;
    CEOViewSrch _viewSrch;
    CEOViewStat _viewStat;
    
public:
    SwitcherView()
        : gui::ViewSwitcher(7)

    {
        addView(&_viewProj, true);
        addView(&_viewEmpl);
        addView(&_viewReq);
        addView(&_viewSrch);
        addView(&_viewStat);
    }
};
