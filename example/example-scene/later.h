#ifndef LATER_H
#define LATER_H

//timer
#include <functional>
#include <chrono>
#include <future>
#include <cstdio>
#include <atomic>

class Later
{
public:
    template <class callable, class... arguments>
    Later(int after, bool async, unsigned repeat, callable&& f, arguments&&... args)
    {
        m_runningflag.store(false);
        m_producingflag.store(true);

        std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

        if (async)
        {
            m_runningflag.store(true);
            std::thread([this, after, repeat, task]() {
                unsigned rep = repeat;
                while(rep--)
                {
                    std::this_thread::sleep_for(std::chrono::milliseconds(after));

                    if (!this->m_producingflag) break;
                    task();
                }
                this->m_runningflag.store(false);
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
        stop();
    }

    void stop()
    {
        while (m_runningflag)
        {
            m_producingflag.store(false);
        }
    }

    bool waitingStop()
    {
        return m_producingflag;
    }

//private:
    std::atomic_bool m_runningflag;
    std::atomic_bool m_producingflag;
};

#endif