#ifndef LATER_H
#define LATER_H

//timer
#include <functional>
#include <chrono>
#include <future>
#include <cstdio>

class Later
{
public:
    template <class callable, class... arguments>
    Later(int after, bool async, unsigned repeat, callable&& f, arguments&&... args)
    {
        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        if (async)
        {
            std::thread([after, repeat, task]() {
                unsigned rep = repeat;
                while(rep--)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(after));
                    task();
                }
            }).detach();
        }
        else
        {
            while(repeat--)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(after));
                task();
            }
        }
    }

    ~Later()
    {
        //wait thread to stop
    }

};

#endif