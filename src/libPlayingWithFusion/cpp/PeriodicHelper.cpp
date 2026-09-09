#include "PeriodicHelper.h"

using namespace pwf;

std::atomic<bool> PeriodicHelper::m_terminateThread = {false};
std::thread *PeriodicHelper::m_periodicThread = NULL;
std::mutex PeriodicHelper::m_taskListMutex;
std::list<PeriodicTask*> PeriodicHelper::taskList;

PeriodicHelper::PeriodicHelper() {
    if (m_periodicThread == NULL) {
        m_periodicThread = new std::thread(PeriodicHelper::CallPeriodicTasks, this);
    }
}

PeriodicHelper::~PeriodicHelper() {
    // Dont kill the background thread, someone else might still be using it

    //m_terminateThread = true;
    //m_periodicThread->join();
    //m_periodicThread=NULL;
}

void PeriodicHelper::AddTask(PeriodicTask *pTask) {
    if (pTask != NULL)
    {
        std::lock_guard<std::mutex> lock(m_taskListMutex);
        taskList.push_back(pTask);
    }
}

void PeriodicHelper::RemoveTask(PeriodicTask *pTask) {
    std::lock_guard<std::mutex> lock(m_taskListMutex);
    taskList.remove(pTask);
}


void PeriodicHelper::CallPeriodicTasks(void *pData) {

    std::chrono::steady_clock::time_point tNextSample{ std::chrono::steady_clock::now() };

    while (m_terminateThread == false)
    {
        {
            std::lock_guard<std::mutex> lock(m_taskListMutex);
            for (auto itr = taskList.begin(); itr != taskList.end(); itr++) {
                (*itr)->ExecuteTask();
            }
        }

        tNextSample += std::chrono::milliseconds(10);
        std::this_thread::sleep_until(tNextSample);
    }
}