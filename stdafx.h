// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#define _WINSOCK_DEPRECATED_NO_WARNINGS

// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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