//
// Created by Jialu  Hu on 2020-02-08.
//
#include "Taskthreadpool.h"
using namespace SiNet;
TaskThreadPool::TaskThreadPool():
                ThreadNumber(10),
                stop(false){
        if(!stop){
            Run();
        }
}

TaskThreadPool::TaskThreadPool(size_t ThreadNum):
        ThreadNumber(ThreadNum),stop(false){
            if(!stop){
                Run();
        }
}

TaskThreadPool::~TaskThreadPool() {

}

void TaskThreadPool::Run() {
    for(int i=0; i<ThreadNumber; i++){
        std::thread thread_one([this]{this->TaskWork();});
        thread_one.detach();
        ThreadSet.push_back(std::move(thread_one));
    }
}

void TaskThreadPool::AddTask(SiNet::Task task) {
    {
        std::lock_guard<std::mutex> lock(Mutex);
        TaskList.push_back(task);
    }
}

void TaskThreadPool::TaskWork() {
    while(!stop){
        Task work;
        {
            std::unique_lock<std::mutex> lock(Mutex);
            CondVar.wait(lock);
            if(!TaskList.empty()){
                work = std::move(TaskList.front());
                TaskList.pop_front();
            }
        }
        if(work){
            work();
        }
    }
}