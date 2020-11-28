#pragma once
#include <cstdint>
#include <mutex>

namespace LFramework::Threading {

class CriticalSection {
public:
    CriticalSection() {
        lock();
    }

    ~CriticalSection() {
        unlock(0);
    }

    static uint32_t lock() {
        _lock.lock();
        return 0;
    }

    static void unlock(int ) {
        _lock.unlock();
    }

private:
    static std::recursive_mutex _lock;
};


}
