#pragma once
#include <Windows.h>


/**
*	\brief ��ȡӦ�ó���·��
*/
bool GetAppDir(TCHAR* path);
/**
*\brief  ��ȡƽ̨��ݱ�ʶ��
*/
bool epcGetSystemIdentityCode(ULONGLONG* pAuthorData);

/**
*	\brief  ��ȡGUID
*/
bool GetGuid(TCHAR* guid);
