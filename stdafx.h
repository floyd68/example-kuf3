// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.

#define _WINSOCK_DEPRECATED_NO_WARNINGS

// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <crtdbg.h>
#include <d2d1.h>
#include <dwrite.h>
#include <vector>
#include <map>
#include <deque>
#include <memory>
#include <winsock2.h>
#include <WS2tcpip.h>
#include "boost/any.hpp"
#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#pragma comment(lib, "d2d1.lib")
#pragma  comment(lib, "dwrite.lib")
#pragma comment(lib, "Ws2_32.lib")