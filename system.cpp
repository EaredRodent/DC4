#include "system.h"

DWORD processGetPid(const QString &aProcessName)
{
    HANDLE pHandle =        CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (pHandle == NULL)    {
        //        errorFatal("CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0) failed.");
    }
    PROCESSENTRY32          processEntry;
    processEntry.dwSize =   sizeof(PROCESSENTRY32);

    while (Process32Next(pHandle, &processEntry))
    {
        if (wcsstr(processEntry.szExeFile, aProcessName.toStdWString().c_str()))
        {
            qDebug() << "ProcessGetPid called! Return: " << processEntry.th32ProcessID;
            return processEntry.th32ProcessID;
        }
    }

//    qDebug() << "ProcessGetPid called! Return: 0";
    CloseHandle(pHandle);
    return 0;
}

unsigned int getFakeThreadStack(HANDLE processHandle, unsigned int processId) {

    THREAD_BASIC_INFORMATION        tbi;
    NT_TIB                          ntTib;
    THREADENTRY32                   threadEntry;
    HANDLE snapshot =               CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    threadEntry.dwSize =            sizeof(threadEntry);

    while (Thread32Next(snapshot, &threadEntry))
    {
        HANDLE threadHandle = OpenThread(THREAD_ALL_ACCESS, 0, threadEntry.th32ThreadID);
        NtQueryInformationThread(threadHandle, (THREADINFOCLASS) 0, &tbi, sizeof(tbi), NULL);
        ReadProcessMemory(processHandle, tbi.TebBaseAddress, &ntTib, sizeof(NT_TIB), 0);
        if (threadEntry.th32OwnerProcessID == processId)
        {
            return (unsigned int) ntTib.StackBase;
        }
    }
    return 0;
}

QString FM(DWORD errCode)
{
    DWORD cchBuffer;
    wchar_t lpszBuffer[256];
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, errCode, 0, lpszBuffer, cchBuffer, NULL);
    return QString::fromWCharArray(lpszBuffer);
}
