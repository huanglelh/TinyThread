#include <Windows.h>
#include <winioctl.h>
#include <Iphlpapi.h>
#include <cassert>
#include "utils.h"
#include "log.h"


//  ��ȡ��֤оƬ DS2460 ��SN��, �����û�Ӧ�ó������
#define IOCTL_GET_IDENTIFY_CHIP_SN			CTL_CODE(FILE_DEVICE_HAL, 2055, METHOD_BUFFERED, FILE_ANY_ACCESS)


#pragma warning(disable:4996)

/**
*\brief  ��ȡƽ̨��ݱ�ʶ��
*/
#ifdef WINCE
bool epcGetSystemIdentityCode(ULONGLONG* pAuthorData)
{
    static ULONGLONG authorUID = 0;
    static BOOL firstFlg = TRUE;

    if ( firstFlg )
    {		
        /* WinCE6�����KIC1�ļ�����ȡƽ̨��ݱ�ʶ�� */
        HANDLE hKICFile = CreateFile(
            TEXT("KIC1:"), 
            GENERIC_READ | GENERIC_WRITE, 
            0, 
            NULL, 
            OPEN_EXISTING, 
            0, 
            0);                 
        /* �� IIC0 ����                */
        if (hKICFile == INVALID_HANDLE_VALUE) 
        {
            LOG(LOG_ERROR, _T("KIC:open failed!\r\n"));
            return FALSE;
        } 

        DWORD dwActLen	= 0;
        BOOL  bRet = DeviceIoControl(hKICFile, 
            IOCTL_GET_IDENTIFY_CHIP_SN, 
            NULL, 
            0, 
            &authorUID, 
            sizeof(ULONGLONG), 
            &dwActLen, 
            NULL);
        if ((bRet != TRUE) || (dwActLen != sizeof(ULONGLONG))) 
        {
            LOG(LOG_ERROR, _T("KIC:ioctl failed! %x, dwActLen %d\r\n"),
                pAuthorData, dwActLen);
            bRet = FALSE;
        }
        CloseHandle(hKICFile);
        if ( !bRet )
        {
            return FALSE;
        }

        firstFlg = FALSE;
    }		

    *pAuthorData = authorUID;

    return TRUE;
}
#else
bool epcGetSystemIdentityCode(ULONGLONG* pAuthorData)
{
#ifdef _UNICODE
    wcscpy((wchar_t*)pAuthorData, _T("1234"));
#else
    strcpy((char*)pAuthorData, "1234");
#endif
    return true;
}

#endif


bool GetAppDir(TCHAR* path)
{
    TCHAR filePath[MAX_PATH] = {0};
    GetModuleFileName(NULL, filePath, MAX_PATH-1);
    int len = 0;
    len = _tcslen(filePath);
    if (filePath[len] == _T('/') || filePath[len] == _T('\\'))
    {
        wcscpy(path, filePath);
        path[len] = _T('\0');
        return true;
    }
    wchar_t* subStr = wcsrchr(filePath, _T('\\'));
    wcsncpy(path, filePath, int(subStr-filePath+1));
    path[len+1] = _T('\0');

    return true;
}
