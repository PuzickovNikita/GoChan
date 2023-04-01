//
// Created by Ivan Ivanov on 01.04.2023.
//

#ifndef GOCHAN_SEMAPHORE_H
#define GOCHAN_SEMAPHORE_H

#include <mutex>
#include <condition_variable>

class semaphore{
    std::mutex mutex;
    std::condition_variable cv;
    unsigned long count;
    unsigned long subscribers_count = 0;
public:
    semaphore():count(0){}
    inline void notify(){
        std::unique_lock lk(mutex);
        ++count;
        cv.notify_one();
    }
    inline void wait(){
        std::unique_lock lk(mutex);
        ++subscribers_count;
        while (count == 0){
            cv.wait(lk);
        }
        --subscribers_count;
        --count;
    }
    inline void notify_all(){
        std::unique_lock lk(mutex);
        count += subscribers_count;
        cv.notify_all();
    }
};


#endif //GOCHAN_SEMAPHORE_H
