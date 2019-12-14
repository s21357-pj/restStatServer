#ifndef HEADERS_H
#define HEADERS_H
#include <deque> 
#include <mutex>
#include <thread>
#include <string>
#include <fstream>
#include <streambuf>

const unsigned int countProbes = 100;

extern std::deque<float> qMem, qCpu;

void infoLoop();
void addToQMem(std::deque<float> &z);
void addToQCpu(std::deque<float> &z, float percent);
bool cpuTimes(long &idle, long &total);
void cpuLoop();

const std::string versionOutput = "GNU RestAPI Statistic Server v. 0.1.0\n"
"Use pistache C++ framework library\n"
"Open-source project for Pjatk Gdansk 2019.\n";

const std::string helpOutput = "GNU RestAPI Statistic Server v. 0.1.0\n"
"Usage: restStatServer [port] [threads] \n"
"Default: 80 port and 2 threads.\n\n"
"For execute server application on tcp port 80\n"
"you should have superuser permissions.\n"
"Otherwise you can execute application on underprivileged ports > 1024\n\n"
"Options:\n"
"      --help, -h     display this help and exit\n"
"      --version, -v  output version information and exit\n"
"Examples:\n"
"      sudo restStatServer\n"
"      restStatServer 8080 4\n";




#endif