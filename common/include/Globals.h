#pragma once
#include <td/Types.h>
enum class DlgID : td::UINT4 { Login = 100, FileSelect, NewProj, NewTick };


namespace Globals
{
	extern td::INT4 _currentUserID;
}