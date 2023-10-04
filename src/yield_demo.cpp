//
// Created by sajit on 03/10/2023.
//

#include <thread>
#include <iostream>
#include <chrono>

void little_sleep(std::chrono::microseconds us)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + us;

    do
    {
        std::this_thread::yield();
        std::cout<<"Yield!\n";
    } while (std::chrono::high_resolution_clock::now() < end);
}

int main()
{
    auto start = std::chrono::high_resolution_clock::now();
    little_sleep(std::chrono::microseconds{2000});

    auto elapsed = std::chrono::high_resolution_clock::now() - start;
    std::cout << "wait for " << std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count()
              << "ms\n";
    return 0;
}

