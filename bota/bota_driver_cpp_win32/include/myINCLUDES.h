#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// To manage the ethercat_gen0.json
#include <windows.h>
#include <string>
#include <filesystem>

std::string getExeDir() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string fullPath(buffer);
    size_t lastSlash = fullPath.find_last_of("\\/");
    return fullPath.substr(0, lastSlash);
}

// Usage
std::string configPath = getExeDir() + "\\..\\..\\..\\driver_config\\ethercat_gen0.json";

// Includes for unix_epoch - start
using namespace std::chrono;
#include <time.h>
#include <ctime>
#include <chrono>
// Includes for unix_epoch - end

int64_t                         timestamp;
int64_t now = 0;
int64_t last = 0;

int64_t GetTickUs()
{
#if defined(_MSC_VER)
    LARGE_INTEGER start, frequency;

    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    timestamp = duration_cast<nanoseconds>(system_clock::now().time_since_epoch()).count();
    now = (start.QuadPart * 1000000) / frequency.QuadPart;
    return now;
#else
    struct timespec start;
    clock_gettime(CLOCK_MONOTONIC, &start);

    return (start.tv_sec * 1000000LLU) + (start.tv_nsec / 1000);
#endif
}
