#pragma once
#include <queue>
#include <thread>
#include <mutex>  // pthread_mutex_t
#include <condition_variable>  // pthread_condition_t

// 异步写日志的日志队列
// 模板代码的具体实现不能在.cc中实现 只能放在.h中
template<typename T>
class LockQueue{
public:
    // 线程安全的入队和出队 使用同一把锁lock
    void Push(const T &data){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_queue.push(data);
        m_condvariable.notify_one();   // 写进去之后提醒写日志线程 queue不再empty
    }  // 出}锁自动释放

    T Pop(){
        std::unique_lock<std::mutex> lock(m_mutex);
        while(m_queue.empty()){
            // 日志队列为空，线程进入wait状态 不去抢锁
            m_condvariable.wait(lock);
        }
        T data = m_queue.front();
        m_queue.pop();
        return data;
    }
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condvariable;
};