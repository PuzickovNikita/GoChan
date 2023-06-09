//
// Created by Ivan Ivanov on 01.04.2023.
//

#ifndef GOCHAN_GOCHAN_H
#define GOCHAN_GOCHAN_H

#include <queue>
#include <mutex>
#include "semaphore.h"
#include <Buffer.h>
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
    std::unique_ptr<Buffer<T>> buffer;
    mutex chanMutex;
    semaphore readSubscriber;

public:
    explicit GoChan(Buffer<T>* _buffer):opened(true), buffer(_buffer) {}
    //disable default, move and copy constructors
    GoChan() = delete;
    GoChan(const GoChan&) = delete;
    GoChan(GoChan&&) = delete;
    GoChan& operator=(const GoChan&) = delete;
    GoChan& operator=(GoChan &&) = delete;

    ~GoChan(){
        if(opened)
            Close();
    }

    void Close();

    friend bool operator<<(GoChan<T> &chan, T &newElem){
        LOCK(chan)
        WRITE_BEGIN(chan)
        chan.buffer->Write(newElem);
        WRITE_END(chan)
    }

    friend bool operator<<(GoChan<T> &chan, T &&newElem){
        LOCK(chan)
        WRITE_BEGIN(chan)
        chan.buffer->Write(newElem);
        WRITE_END(chan)
    }

    friend bool operator>>(GoChan<T> &chan, T &readElem){
        chan.readSubscriber.wait();
        lock_guard<mutex> _guard(chan.chanMutex);
        if(!chan.opened)
            return false;
        chan.buffer->Read(readElem);
        return true;
    }
};


template<class T>
void GoChan<T>::Close() {
    LOCK((*this))
    opened = false;
    buffer->Clear();
    readSubscriber.notify_all();
}

#endif //GOCHAN_GOCHAN_H
