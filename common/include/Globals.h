#pragma once
#include <td/Types.h>
enum class DlgID : td::UINT4 { Login = 100, FileSelect, NewProj };


namespace Globals
{
	inline td::INT4 _currentUserID = -1;
	inline td::INT4 _currentPMID = -2;
}