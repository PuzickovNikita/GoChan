#include <iostream>
#include <chrono>
#include <thread>
#include "GoChan.h"

class movable{
public:
    movable() = default;
    movable(const movable&){
        std::cout << "copy" << '\n';
    }
    movable(movable&&) noexcept {
        std::cout << "move" << '\n';
    }
};

void f2(GoChan<int> *chan){
    int tmp;
    while((*chan).Read(tmp)){
        std::cout<<tmp<<'\n';
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void f3(GoChan<int> *chan){
    int tmp;
    while((*chan).Read(tmp)){
        std::cout<<tmp<<'\n';
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main() {

    GoChan<int> chan = GoChan<int>();
    for(int i = 0; i < 10; ++i){
        chan.Write(i);
    }

    std::thread t1(f2,&chan);
    std::thread t2(f3, &chan);
    std::this_thread::sleep_for(std::chrono::seconds(7));
    std::cout<<"Close\n";
    chan.Close();
    t1.join();
    t2.join();

    return 0;
}
