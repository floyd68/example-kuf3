#include "stdafx.h"
#include "JsonLoadMgr.h"

const int ReadBufferSize = 2048;
//-----------------------------------------------------------------------------------------------------------
rapidjson::Document CJsonLoadMgr::FileLoad(const char* szFileName)
{
	FILE* pFile;
	fopen_s(&pFile, szFileName, "rb");

	if (!pFile)
		return nullptr;

	rapidjson::Document document;
	char szReadBuffer[ReadBufferSize];
	rapidjson::FileReadStream fileRead(pFile, szReadBuffer, sizeof(szReadBuffer));
	document.ParseStream(fileRead);
	fclose(pFile);

	return document;
}