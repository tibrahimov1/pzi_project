#pragma once
#include <td/Types.h>
enum class DlgID : td::UINT4 { Login = 100, FileSelect, NewProj };
//
//
//namespace Globals
//{
//	extern td::INT4 _currentUserID = -1;
//	inline td::INT4 _currentPMID = -2;
//	//inline td::INT4 _currentLevel = ;
//}
namespace Globals {
    extern td::INT4 _currentUserID;
}