#pragma once

#include<thread>
#include<atomic>
#include<mutex>
#include<list>
#include "PeriodicTask.h"

namespace pwf {

class PeriodicHelper {
  public:
    explicit PeriodicHelper();
    virtual ~PeriodicHelper();

    static void AddTask(PeriodicTask *pTask);
    static void RemoveTask(PeriodicTask *pTask);

  private:
    static std::atomic<bool> m_terminateThread;
    static std::mutex m_taskListMutex;
    static std::thread *m_periodicThread;

    static std::list<PeriodicTask*> taskList;

    static void CallPeriodicTasks(void *pData);
};

}