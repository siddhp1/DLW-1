#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <memory>
#include <string>

#include "spdlog/spdlog.h"

class Logger {
 public:
  static void Init(
      spdlog::level::level_enum console_level = spdlog::level::info,
      spdlog::level::level_enum file_level = spdlog::level::debug,
      const std::string& initializer = "");

  [[nodiscard]] static std::shared_ptr<spdlog::logger>& GetLogger() noexcept;
  [[nodiscard]] static spdlog::level::level_enum StringToLevel(
      const std::string& level);

 private:
  static std::shared_ptr<spdlog::logger> logger;
  static bool initialized;
};

#define LOG_DEBUG(...) \
  if (Logger::GetLogger()) Logger::GetLogger()->debug(__VA_ARGS__)
#define LOG_INFO(...) \
  if (Logger::GetLogger()) Logger::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) \
  if (Logger::GetLogger()) Logger::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) \
  if (Logger::GetLogger()) Logger::GetLogger()->error(__VA_ARGS__)

#endif
