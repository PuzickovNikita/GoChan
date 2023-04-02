//
// Created by Ivan Ivanov on 02.04.2023.
//

#ifndef GOCHAN_QUEUEBUFFER_H
#define GOCHAN_QUEUEBUFFER_H

#include <queue>
#include "Buffer.h"

template<class T>
class QueueBuffer: public Buffer<T>{
private:
    std::queue<T> storage;
public:
    void Write(T &t) override {
        storage.push(t);
    }

    void Write(T &&t) override {
        storage.push(std::move(t));
    }

    void Read(T &t) override {
        t = std::move(storage.front());
        storage.pop();
    }

    void Clear() override {
        std::queue<T> empty;
        storage.swap(empty);
    }

};


#endif //GOCHAN_QUEUEBUFFER_H
