#ifndef SYSTEM_H
#define SYSTEM_H

#include <QDebug>
#include <QString>

//getFakeThreadStack
#include <windows.h>
#include <tlhelp32.h>
#include <Winternl.h>

// processGetPid
#include <string.h>

typedef struct _THREAD_BASIC_INFORMATION
{
    NTSTATUS  ExitStatus;
    PVOID     TebBaseAddress;
    CLIENT_ID ClientId;
    ULONG     AffinityMask;
    LONG      Priority;
    LONG      BasePriority;
} THREAD_BASIC_INFORMATION;

DWORD processGetPid(const QString &aProcessName);
unsigned int getFakeThreadStack(HANDLE processHandle, unsigned int processId);
QString FM(DWORD errCode);

#endif // SYSTEM_H
