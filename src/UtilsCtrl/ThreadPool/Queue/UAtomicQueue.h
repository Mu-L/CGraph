/***************************
@Author: Chunel
@Contact: chunel@foxmail.com
@File: UAtomicQueue.h
@Time: 2021/7/2 11:28 下午
@Desc: 设计了一个安全队列
***************************/

#ifndef CGRAPH_UATOMICQUEUE_H
#define CGRAPH_UATOMICQUEUE_H

#include <mutex>
#include <queue>
#include <utility>
#include <vector>
#include <condition_variable>

#include "../UThreadPoolDefine.h"
#include "UQueueObject.h"

CGRAPH_NAMESPACE_BEGIN

template<typename T>
class UAtomicQueue : public UQueueObject {
public:
    UAtomicQueue() = default;

    /**
     * 等待弹出
     * @param value
     */
    CVoid waitPop(T& value) {
        CGRAPH_UNIQUE_LOCK lk(mutex_);
        cv_.wait(lk, [this] { return !queue_.empty(); });
        value = std::move(queue_.front());
        queue_.pop();
    }


    /**
     * 尝试弹出
     * @param value
     * @return
     */
    CBool tryPop(T& value) {
        CBool result = false;
        if (!queue_.empty() && mutex_.try_lock()) {
            if (!queue_.empty()) {
                value = std::move(queue_.front());
                queue_.pop();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 尝试弹出多个任务
     * @param values
     * @param maxPoolBatchSize
     * @return
     */
    CBool tryPop(std::vector<T>& values, int maxPoolBatchSize) {
        CBool result = false;
        if (!queue_.empty() && mutex_.try_lock()) {
            while (!queue_.empty() && maxPoolBatchSize-- > 0) {
                values.emplace_back(std::move(queue_.front()));
                queue_.pop();
                result = true;
            }
            mutex_.unlock();
        }

        return result;
    }


    /**
     * 阻塞式等待弹出
     * @param value
     * @param ms
     * @return 是否成功弹出数据
     */
    CBool popWithTimeout(T& value, const CMSec ms) {
        CGRAPH_UNIQUE_LOCK lk(mutex_);
        if (!cv_.wait_for(lk, std::chrono::milliseconds(ms),
                          [this] { return (!queue_.empty()) || (!ready_flag_); })) {
            return false;
        }

        if (queue_.empty() || !ready_flag_) {
            return false;
        }

        value = std::move(queue_.front());
        queue_.pop();    // 如果等成功了，则弹出一个信息
        return true;
    }


    /**
     * 传入数据
     * @param value
     */
    CVoid push(T&& value) {
        while (true) {
            if (mutex_.try_lock()) {
                queue_.emplace(std::move(value));
                mutex_.unlock();
                break;
            }
            CGRAPH_YIELD();
        }
        cv_.notify_one();
    }


    /**
     * 判定队列是否为空
     * @return
     */
    CBool empty() {
        CGRAPH_LOCK_GUARD lk(mutex_);
        return queue_.empty();
    }


    /**
     * 功能是通知所有的辅助线程停止工作
     * @return
     */
    CVoid reset() {
        {
            CGRAPH_UNIQUE_LOCK lk(mutex_);
            ready_flag_ = false;
        }
        cv_.notify_all();
    }


    /**
     * 初始化状态
     * @return
     */
    CVoid setup() {
        {
            CGRAPH_UNIQUE_LOCK lk(mutex_);
            ready_flag_ = true;
        }
        queue_ = {};
    }

    CGRAPH_NO_ALLOWED_COPY(UAtomicQueue)

private:
    std::queue<T> queue_ {};     // 任务队列
    CBool ready_flag_ { true };  // 执行标记，主要用于快速释放 destroy 逻辑中，多个辅助线程等待的状态
};

CGRAPH_NAMESPACE_END

#endif //CGRAPH_UATOMICQUEUE_H
