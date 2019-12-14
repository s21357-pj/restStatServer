#include "headers.h"
#include <fstream>
#include <vector>
#include "sys/sysinfo.h"
#include <chrono>



long prevIdle = 0; 
long prevTotal = 0;
std::deque<float> qMem(countProbes, 0);
std::deque<float> qCpu(countProbes, 0);
struct sysinfo sys_info;

void addToQMem(std::deque<float> &z) {
    try {
        sysinfo(&sys_info);
        const float used = (float)(sys_info.totalram - sys_info.freeram) / sys_info.totalram;
        const float percent = used * 100;
        z.push_back(percent);
        z.pop_front();
    } catch (const std::exception& e) {
        const float percent = 0;
    }
}

void addToQCpu(std::deque<float> &z, float percent) {
    z.push_back(percent);
    z.pop_front();
}


bool cpuTimes(long &idle, long &total) {
    try {
        std::ifstream proc_stat("/proc/stat");
        proc_stat.ignore(5, ' ');
        std::vector<long> t;
        long time;
        total = 0;
        while(proc_stat >> time) {
            t.push_back(time);
            total += time;
        }
        idle = t[3];
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

void cpuLoop() {
    long idle, total;
    cpuTimes(idle, total);
    const float totalDelta = total - prevTotal;
    const float idleDelta = idle - prevIdle;
    const float cpuPercent = 100.0 * (1.0 - idleDelta / totalDelta);
    if (cpuPercent <= 100) {
        addToQCpu(qCpu, cpuPercent);
    }    
    prevIdle = idle;
    prevTotal = total;
}


void infoLoop() {
    using namespace std::chrono_literals;
    cpuLoop();
    addToQMem(qMem);
    std::this_thread::sleep_for(500ms);
    infoLoop();
}