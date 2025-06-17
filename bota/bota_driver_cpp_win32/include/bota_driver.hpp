/**
 * @file bota_driver.hpp
 * @brief Public header file for the BotaDriver class.
 */

#ifndef BOTA_DRIVER_HPP
#define BOTA_DRIVER_HPP

#include <array>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string>

// Platform-specific DLL export/import macros
#ifdef _WIN32
    #ifdef BOTA_DRIVER_EXPORTS
        #define BOTA_DRIVER_API __declspec(dllexport)
    #else
        #define BOTA_DRIVER_API __declspec(dllimport)
    #endif
#else
    #define BOTA_DRIVER_API
#endif

// Platform-specific packed structure macros
#ifdef _MSC_VER
    #define PACKED_BEGIN __pragma(pack(push, 1))
    #define PACKED_END __pragma(pack(pop))
    #define PACKED_STRUCT
#elif defined(__GNUC__)
    #define PACKED_BEGIN
    #define PACKED_END
    #define PACKED_STRUCT __attribute__((packed))
#else
    #define PACKED_BEGIN
    #define PACKED_END
    #define PACKED_STRUCT
#endif
namespace bota {

/**
 * @enum DriverState
 * @brief Represents the core lifecycle states and transitions of the driver.
 */
enum class DriverState {
  INITIAL,
  UNCONFIGURED,
  INACTIVE,
  ACTIVE,
  FINALIZED,
  TERMINAL,
  CONFIGURING,
  CLEANING_UP,
  SHUTTING_DOWN,
  ACTIVATING,
  DEACTIVATING,
  ERROR_PROCESSING
};

PACKED_BEGIN
union DataStatus {
  struct PACKED_STRUCT {
    uint16_t throttled : 1;  // 1 bit
    uint16_t overrange : 1;  // 1 bit
    uint16_t invalid : 1;    // 1 bit
    uint16_t raw : 1;        // 1 bit
    uint16_t : 12;           // 12 reserved bits
  };
  uint16_t val;
  uint8_t bytes[1];
};
PACKED_END

struct BotaFrame {
  DataStatus status;
  std::array<float, 3> force;
  std::array<float, 3> torque;
  uint32_t timestamp;
  float temperature;
  std::array<float, 3> acceleration;
  std::array<float, 3> angular_rate;

  // Constructor
  BotaFrame(DataStatus status_data, std::array<float, 3> force_data,
            std::array<float, 3> torque_data, uint32_t timestamp_data, float temperature_data,
            std::array<float, 3> acceleration_data, std::array<float, 3> angular_rate_data)
      : status(status_data),
        force(force_data),
        torque(torque_data),
        timestamp(timestamp_data),
        temperature(temperature_data),
        acceleration(acceleration_data),
        angular_rate(angular_rate_data) {}
};

class BotaDriverExposed;  // to allow friendship

/**
 * @class BotaDriver
 * @brief Public API for managing the lifecycle of a sensor driver.
 */
class BOTA_DRIVER_API BotaDriver {
 public:
  BotaDriver();  // Default constructor
  explicit BotaDriver(const std::string &config_path);
  ~BotaDriver();

  [[nodiscard]] uint16_t getDriverVersionGeneration() const;
  [[nodiscard]] uint16_t getDriverVersionMajor() const;
  [[nodiscard]] uint16_t getDriverVersionMinor() const;
  [[nodiscard]] std::string getDriverVersionString() const;

  [[nodiscard]] bool configure();
  [[nodiscard]] bool activate();
  [[nodiscard]] bool deactivate();
  [[nodiscard]] bool cleanup();
  [[nodiscard]] bool shutdown();

  [[nodiscard]] DriverState getDriverState() const;
  [[nodiscard]] std::chrono::microseconds getExpectedTimestep() const;

  [[nodiscard]] bool tare() const;                 // It can only be called in INACTIVE state
  [[nodiscard]] BotaFrame readFrameSync() const;   // It can only be called in ACTIVE state
  [[nodiscard]] BotaFrame readFrameAsync() const;  // It can only be called in ACTIVE state
 private:
  // Implementation details - forward declaration
  class Impl;
  std::unique_ptr<Impl> impl_;
  friend class BotaDriverExposed;
};

}  // namespace bota

#endif  // BOTA_DRIVER_HPP
