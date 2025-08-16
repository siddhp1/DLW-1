#include "logger/logger.hpp"

#include <algorithm>
#include <cctype>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

#include "spdlog/common.h"
#include "spdlog/logger.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Logger::logger = nullptr;
bool Logger::initialized = false;

static std::string GetCurrentDateTime() {
  auto now = std::chrono::system_clock::now();
  auto time_t_now = std::chrono::system_clock::to_time_t(now);
  std::tm tm_now{};
#ifdef _WIN32
  // NOLINTNEXTLINE(cert-err33-c)
  localtime_s(&tm_now, &time_t_now);
#else
  localtime_r(&time_t_now, &tm_now);
#endif
  std::ostringstream oss;
  oss << std::put_time(&tm_now, "%Y%m%d-%H%M%S");
  return oss.str();
}

std::shared_ptr<spdlog::logger>& Logger::GetLogger() noexcept { return logger; }

void Logger::Init(spdlog::level::level_enum console_level,
                  spdlog::level::level_enum file_level,
                  const std::string& initializer) {
  if (initialized) {
    return;
  }

  try {
    const std::string log_file_path =
        std::string("logs/") + initializer + GetCurrentDateTime() + ".log";

    auto log_dir = std::filesystem::path(log_file_path).parent_path();
    if (!log_dir.empty() && !std::filesystem::exists(log_dir)) {
      std::filesystem::create_directories(log_dir);
    }

    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(console_level);

    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
        log_file_path, true);
    file_sink->set_level(file_level);

    std::vector<spdlog::sink_ptr> sinks{console_sink, file_sink};
    logger =
        std::make_shared<spdlog::logger>("main", sinks.begin(), sinks.end());
    logger->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
    logger->set_level(std::min(console_level, file_level));

    initialized = true;
    logger->info("Logger initialized");

  } catch (const spdlog::spdlog_ex& ex) {
    std::cerr << "Logger initialization failed: " << ex.what() << "\n";
  }
}

spdlog::level::level_enum Logger::StringToLevel(const std::string& level) {
  std::string level_lower = level;
  std::ranges::transform(level_lower, level_lower.begin(),
                         [](unsigned char c) { return std::tolower(c); });

  if (level_lower == "debug") {
    return spdlog::level::debug;
  }
  if (level_lower == "info") {
    return spdlog::level::info;
  }
  if (level_lower == "warn") {
    return spdlog::level::warn;
  }
  if (level_lower == "error") {
    return spdlog::level::err;
  }
  if (level_lower == "off") {
    return spdlog::level::off;
  }

  throw std::runtime_error("Invalid log level: " + level);
}
