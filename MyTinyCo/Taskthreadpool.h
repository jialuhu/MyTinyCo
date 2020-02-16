//
// Created by Jialu  Hu on 2020-02-08.
//

#ifndef MYSNETCO_TASKTHREADPOOL_H
#define MYSNETCO_TASKTHREADPOOL_H

#include "Noncopyable.h"
#include <thread>
#include <functional>
#include <list>
#include <vector>
#include <mutex>
#include <condition_variable>

namespace SiNet {
    typedef std::function<void ()>Task;
    class TaskThreadPool : private Noncopyable {
    public:
        static const size_t  MAXTHREAD = std::numeric_limits<int>::max();
        TaskThreadPool(size_t ThreadNumber);
        TaskThreadPool();
        ~TaskThreadPool();
        void AddTask(Task task);
        void setStop(){
            stop = true;
        }
        void TaskWork();
        void Run();

    private:
        size_t ThreadNumber;
        std::list<Task> TaskList;
        std::vector<std::thread> ThreadSet;
        std::mutex Mutex;
        std::condition_variable CondVar;
        bool stop;
    };

}
#endif //MYSNETCO_TASKTHREADPOOL_H
