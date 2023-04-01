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
};

#define LOCK lock_guard<mutex> _guard(chanMutex);

template<class T>
void GoChan<T>::Close() {
    LOCK
    opened = false;
    queue<T> empty;
    storage.swap(empty);
    readSubscriber.notify_all();
}

#define WRITE_BEGIN \
    if(!opened) \
    return false;
#define WRITE_END \
    readSubscriber.notify(); \
    return true;

template<class T>
bool GoChan<T>::Write(T &newElem) {
    LOCK
    WRITE_BEGIN
    storage.push(newElem);
    WRITE_END
}

template<class T>
bool GoChan<T>::Write(T &&newElem) {
    LOCK
    WRITE_BEGIN
    storage.push(std::move(newElem));
    WRITE_END
}

template<class T>
bool GoChan<T>::Read(T &readElem) {
    readSubscriber.wait();
    lock_guard<mutex> _guard(chanMutex);
    if(!opened)
        return false;
    readElem = std::move(storage.front());
    storage.pop();
    return true;
}


#endif //GOCHAN_GOCHAN_H
