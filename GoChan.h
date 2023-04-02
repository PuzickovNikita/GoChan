//
// Created by Ivan Ivanov on 01.04.2023.
//

#ifndef GOCHAN_GOCHAN_H
#define GOCHAN_GOCHAN_H

#include <queue>
#include <mutex>
#include "semaphore.h"
using std::queue;
using std::mutex;
using std::lock_guard;

//TODO: заменить дефайны декоратором?
#define LOCK(chan) lock_guard<mutex> _guard(chan.chanMutex);

#define WRITE_BEGIN(chan) \
    if(!(chan.opened)) \
    return false;
#define WRITE_END(chan) \
    chan.readSubscriber.notify(); \
    return true;

template <class T>
class GoChan {
private:
    bool opened;
    queue<T> storage;
    mutex chanMutex;
    semaphore readSubscriber;

public:
    GoChan():opened(true) {}
    //disable move and copy constructors
    GoChan(const GoChan&) = delete;
    GoChan(GoChan&&) = delete;
    GoChan& operator=(const GoChan&) = delete;
    GoChan& operator=(GoChan &&) = delete;

    bool Write(T& newElem);
    bool Write(T &&newElem);
    bool Read(T& readElem);
    void Close();

    friend bool operator<<(GoChan<T> &chan, T &newElem){
        LOCK(chan)
        WRITE_BEGIN(chan)
        chan.storage.push(newElem);
        WRITE_END(chan)
    }

    friend bool operator<<(GoChan<T> &chan, T &&newElem){
        LOCK(chan)
        WRITE_BEGIN(chan)
        chan.storage.push(std::move(newElem));
        WRITE_END(chan)
    }

    friend bool operator>>(GoChan<T> &chan, T &readElem){
        chan.readSubscriber.wait();
        lock_guard<mutex> _guard(chan.chanMutex);
        if(!chan.opened)
            return false;
        readElem = std::move(chan.storage.front());
        chan.storage.pop();
        return true;
    }
};


template<class T>
void GoChan<T>::Close() {
    LOCK((*this))
    opened = false;
    queue<T> empty;
    storage.swap(empty);
    readSubscriber.notify_all();
}

#endif //GOCHAN_GOCHAN_H
