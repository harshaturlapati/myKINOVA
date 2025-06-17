#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// To manage the ethercat_gen0.json
#include <windows.h>
#include <string>
#include <filesystem>

// Includes for unix_epoch - start
using namespace std::chrono;
#include <time.h>
#include <ctime>
#include <chrono>
// Includes for unix_epoch - end

class myBOTA {

public:

    // --- Public members you want access to ---
    bota::BotaDriver* bota_ft_sensor_driver = nullptr;  // Use pointer to allow control over constructor timing
    uint16_t BOTA_DRIVER_VERSION_GENERATION = 0;
    uint16_t BOTA_DRIVER_VERSION_MAJOR = 0;
    uint16_t BOTA_DRIVER_VERSION_MINOR = 0;
    std::string BOTA_DRIVER_VERSION_STRING;

    //bota::BotaFrame* bota_frame;

    int data_count = 0;
    double* time_log;
    int64_t* unix_epoch;
    int DURATION;
    int arr_length;
    std::string configPath;

    std::array<float, 3> force;
    std::array<float, 3> torque;
    uint32_t timestamp_BOTA;
    float temperature;
    std::array<float, 3> acceleration;
    std::array<float, 3> angular_rate;

    int64_t timestamp;
    int64_t now = 0;   

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


    int BOTA_STARTUP() {
        configPath = getExeDir() + "\\..\\..\\..\\bota\\driver_config\\ethercat_gen0.json";

        // Initialize driver pointer
        bota_ft_sensor_driver = new bota::BotaDriver(configPath.c_str());

        // Retrieve version info
        BOTA_DRIVER_VERSION_GENERATION = bota_ft_sensor_driver->getDriverVersionGeneration();
        BOTA_DRIVER_VERSION_MAJOR = bota_ft_sensor_driver->getDriverVersionMajor();
        BOTA_DRIVER_VERSION_MINOR = bota_ft_sensor_driver->getDriverVersionMinor();
        BOTA_DRIVER_VERSION_STRING = bota_ft_sensor_driver->getDriverVersionString();

        std::cout << " >>>>>>>>>>> BotaDriver version: " << BOTA_DRIVER_VERSION_STRING << " <<<<<<<<<<<< " << std::endl;

        if (!bota_ft_sensor_driver->configure())
        {
            std::cerr << "Failed to configure driver" << std::endl;
            return 1;
        }
        if (!bota_ft_sensor_driver->activate())
        {
            std::cerr << "Failed to activate driver" << std::endl;
            return 1;
        }

        

        return 1;
    }

    void BOTA_READ() {
        bota::BotaFrame bota_frame = bota_ft_sensor_driver->readFrameAsync();
        bota::DataStatus status = bota_frame.status;
            force = bota_frame.force;
            torque = bota_frame.torque;
            timestamp_BOTA = bota_frame.timestamp;
            temperature = bota_frame.temperature;
            acceleration = bota_frame.acceleration;
            angular_rate = bota_frame.angular_rate;

            std::cout << "----------------------------" << std::endl;
                std::cout << "Status: [throttled=" << status.throttled << ", overrange=" << status.overrange << ", invalid=" << status.invalid << ", raw=" << status.raw << "]" << std::endl;
                std::cout << "Force: [" << force[0] << ", " << force[1] << ", " << force[2] << "] N" << std::endl;
                std::cout << "Torque: [" << torque[0] << ", " << torque[1] << ", " << torque[2] << "] Nm" << std::endl;
                std::cout << "Acceleration: [" << acceleration[0] << ", " << acceleration[1] << ", " << acceleration[2] << "] m/s²" << std::endl;
                std::cout << "Angular Rate: [" << angular_rate[0] << ", " << angular_rate[1] << ", " << angular_rate[2] << "] rad/s" << std::endl;
                std::cout << "Temperature: " << temperature << " °C" << std::endl;
                std::cout << "Timestamp: " << timestamp_BOTA << std::endl;
                std::cout << "----------------------------" << std::endl;

                time_log[data_count] = GetTickUs();
                unix_epoch[data_count] = timestamp;
                data_count++;
    }

    void CLOSE() {
        if (!bota_ft_sensor_driver->deactivate())
        {
            std::cerr << "Failed to deactivate driver" << std::endl;
        }
        if (!bota_ft_sensor_driver->shutdown())
        {
            std::cerr << "Failed to shutdown driver" << std::endl;
        }
        std::cout << "Completition WITHOUT errors." << std::endl;

    }

    std::string getExeDir() {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        std::string fullPath(buffer);
        size_t lastSlash = fullPath.find_last_of("\\/");
        std::cout << fullPath.substr(0, lastSlash) << std::endl;
        return fullPath.substr(0, lastSlash);
    }

	myBOTA() { // default constructor
	}

    myBOTA(int dur_in)
    {
        DURATION = dur_in;
        arr_length = DURATION * 1000;
        time_log = new double[arr_length];
        unix_epoch = new int64_t[arr_length];
        BOTA_STARTUP();
    }

};