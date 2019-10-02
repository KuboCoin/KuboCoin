// Copyright (c) 2014-2016 The Dash developers
// Copyright (c) 2015-2019 The PIVX developers
// Copyright (c) 2018-2019 The DogeCash developers
// Copyright (c) 2018-2019 The KuboCoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#ifndef kubocoin_CONCURRENTQUEUE_H
#define kubocoin_CONCURRENTQUEUE_H


#include <mutex>
#include <condition_variable>
#include <deque>

template <typename T>
class concurrentqueue
{
private:
    std::mutex              mutex;
    std::condition_variable condition;
    std::deque<T>           queue;

public:
    void push(T const& value) {
        {
            std::unique_lock<std::mutex> lock(this->mutex);
            queue.push_front(value);
        }
        this->condition.notify_one();
    }
    T pop() {
        std::unique_lock<std::mutex> lock(this->mutex);
        this->condition.wait(lock, [=]{ return !this->queue.empty(); });
        T rc(std::move(this->queue.back()));
        this->queue.pop_back();
        return rc;
    }

    T popNotWait(){
        std::unique_lock<std::mutex> lock(this->mutex);
        T rc(std::move(this->queue.back()));
        this->queue.pop_back();
        return rc;
    }

    bool hasElements(){
        std::unique_lock<std::mutex> lock(this->mutex);
        return !queue.empty();
    }
};

#endif //kubocoin_CONCURRENTQUEUE_H
