#include <cstdint>
#include <cstdlib>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>

#include "cxxopts.hpp"
#include "dlw1/config.hpp"
#include "dlw1/emulator.hpp"
#include "logger/logger.hpp"
#include "spdlog/common.h"

[[nodiscard]] static spdlog::level::level_enum ParseLogLevel(
    const std::string& level_str, std::string_view option_name) {
  try {
    return Logger::StringToLevel(level_str);
  } catch (const std::exception& e) {
    throw std::runtime_error(std::string("Error reading ") +
                             std::string(option_name) +
                             " configuration: " + e.what());
  }
}

[[nodiscard]] static std::string GetFilePath(
    const cxxopts::ParseResult& parsed_options, std::string_view option_name) {
  try {
    return parsed_options[std::string(option_name)].as<std::string>();
  } catch (const cxxopts::exceptions::exception& e) {
    throw std::runtime_error(std::string("Error reading ") +
                             std::string(option_name) + ": " + e.what());
  }
}

// clang-tidy reports false positive
// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char* argv[]) {
  try {
    cxxopts::Options options("DLW-1", "DLW-1 CPU Microarchitecture Emulator");
    options.add_options()("f,file", "Path to the program file to execute",
                          cxxopts::value<std::string>())(
        "b,banks",
        "Number of memory banks (default: " +
            std::to_string(Config::DEFAULT_NUM_BANKS) +
            ", range: " + std::to_string(Config::MIN_BANKS) + "-" +
            std::to_string(Config::MAX_BANKS) + ")",
        cxxopts::value<uint8_t>()->default_value(
            std::to_string(Config::DEFAULT_NUM_BANKS)))(
        "c,console-level", "Console log level [debug, info, warn, error, off]",
        cxxopts::value<std::string>()->default_value("info"))(
        "l,file-level", "File log level [debug, info, warn, error, off]",
        cxxopts::value<std::string>()->default_value("debug"))(
        "version", "Print version information")("help",
                                                "Print usage information");

    cxxopts::ParseResult parsed_options;
    try {
      parsed_options = options.parse(argc, argv);
    } catch (const cxxopts::exceptions::exception& e) {
      throw std::runtime_error("Failed to parse command line arguments: " +
                               std::string(e.what()));
    }

    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    if (parsed_options.count("help")) {
      std::cout << options.help() << "\n";
      return EXIT_SUCCESS;
    }

    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    if (parsed_options.count("version")) {
      std::cout << PROJECT_VERSION << "\n";
      return EXIT_SUCCESS;
    }

    const spdlog::level::level_enum console_level = ParseLogLevel(
        parsed_options["console-level"].as<std::string>(), "console-level");
    const spdlog::level::level_enum file_level = ParseLogLevel(
        parsed_options["file-level"].as<std::string>(), "file-level");
    Logger::Init(console_level, file_level, APP_NAME);

    Config config{};

    // In DEBUG builds, use sample program if no file is specified
    // In RELEASE builds, require command-line argument
#ifdef NDEBUG
    // Release build: require command-line argument
    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    if (!parsed_options.count("file")) {
      throw std::runtime_error(
          "No program file specified. Use --file or -f to specify the program "
          "file.");
    }
    config.program_file_path = GetFilePath(parsed_options, "file");
#else
    // Debug build: use sample program if no file is specified
    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    if (!parsed_options.count("file")) {
      config.program_file_path = "sample_program.bin";
      LOG_INFO("No program file specified, using default sample program: {}",
               config.program_file_path);
    } else {
      config.program_file_path = GetFilePath(parsed_options, "file");
    }
#endif

    try {
      config.num_banks = parsed_options["banks"].as<uint8_t>();
    } catch (const cxxopts::exceptions::exception& e) {
      throw std::runtime_error(std::string("Error reading number of banks: ") +
                               e.what());
    }

    config.Validate();

    LOG_INFO("DLW-1 CPU Emulator Starting");
    LOG_INFO("Program file: {}", config.program_file_path);
    LOG_INFO("Memory banks: {}", config.num_banks);
    LOG_INFO("Console log level: {}",
             spdlog::level::to_string_view(console_level));
    LOG_INFO("File log level: {}", spdlog::level::to_string_view(file_level));

    Emulator emulator{config};

    LOG_INFO("Loading program...");
    emulator.LoadProgram();
    LOG_INFO("Program loaded successfully");

    LOG_INFO("Starting emulator execution...");
    emulator.Run();
    LOG_INFO("Emulator execution completed successfully");

    LOG_INFO("DLW-1 CPU Emulator Finished");
    return EXIT_SUCCESS;
  } catch (const std::exception& e) {
    const char* msg = "Error occurred";
    if (Logger::GetLogger()) {
      LOG_ERROR("{}: {}", msg, e.what());
    } else {
      std::cerr << "FATAL ERROR: " << msg << ": " << e.what() << '\n';
    }
    return EXIT_FAILURE;
  } catch (...) {
    const char* msg = "Unknown error occurred";
    if (Logger::GetLogger()) {
      LOG_ERROR("{}", msg);
    } else {
      std::cerr << "FATAL ERROR: " << msg << '\n';
    }
    return EXIT_FAILURE;
  }
}
