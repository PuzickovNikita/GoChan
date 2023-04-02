//
// Created by Ivan Ivanov on 02.04.2023.
//

#ifndef GOCHAN_BUFFER_H
#define GOCHAN_BUFFER_H

template<class T>
class Buffer{
public:
    //disable move and copy constructor
    Buffer(const Buffer&) = delete;
    Buffer(Buffer&&) = delete;
    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer &&) = delete;

    Buffer() = default;


    virtual void Write(T&) = 0;
    virtual void Write(T&&) = 0;
    virtual void Read(T&) =0;
    virtual void Clear() = 0;

    virtual ~Buffer() = default;
};


#endif //GOCHAN_BUFFER_H
