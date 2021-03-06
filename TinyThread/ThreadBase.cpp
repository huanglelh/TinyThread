#include "ThreadBase.h"


class ThreadData
{
public:
    IThreadBase*     threadBase_;
};

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
    ThreadData* pData = reinterpret_cast<ThreadData*>(lpParameter);
    IThreadBase* pBase = pData->threadBase_;
    delete pData;

    pBase->threadID_ = GetCurrentThreadId();
    pBase->event_.Signal();

    DWORD dw = (DWORD)pBase->Run();
    return dw;
}


IThreadBase::IThreadBase( const wstring& name /*= _T("")*/ )
:threadName_(name), threadID_(-1), event_(FALSE, FALSE),
thread_(INVALID_HANDLE_VALUE)
{
}


bool IThreadBase::Start(Options op /*= Options()*/)
{
    ThreadData* data    = new ThreadData;
    data->threadBase_   = this;
    
    thread_ = CreateThread(NULL, op.stackSize_, 
        (LPTHREAD_START_ROUTINE)ThreadProc, data, 0, NULL);

    if (NULL == thread_)
    {
        delete data;
        return false;
    }
    event_.Wait();

    SetPriority(op.priority_);

    return true;
}

void IThreadBase::Join(int waitTimes)
{
    DWORD dw = WaitForSingleObject(thread_, waitTimes);
    if (dw == WAIT_TIMEOUT)
        TerminateThread(thread_, 1);
    thread_ = INVALID_HANDLE_VALUE;
}

void IThreadBase::SetPriority( int priority )
{
    SetThreadPriority(thread_, priority);
}
