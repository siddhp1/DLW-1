#include <cstdint>
#include <cxxopts.hpp>
#include <iostream>
#include <string>

#include "dlw1/config.hpp"
#include "dlw1/emulator.hpp"
#include "logger/logger.hpp"

int main(int argc, char* argv[]) {
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

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << "\n";
    return 0;
  }

  if (result.count("version")) {
    // Get the version from CMake
    return 0;
  }

  Config config;

  if (!result.count("file")) {
    // Add error handling here for no program file
    // return 1;
    config.program_file_path = "sample_program.bin";
  } else {
    // config.program_file_path = result["file"].as<std::string>();
  }
  config.num_banks = result["banks"].as<uint8_t>();

  if (!config.Validate()) {
    // Add error handling here
    return 1;
  }

  if (!std::filesystem::exists(config.program_file_path)) {
    // Add error handling here
    return 1;
  }

  Logger::Init(Logger::StringToLevel(result["console-level"].as<std::string>()),
               Logger::StringToLevel(result["file-level"].as<std::string>()));

  Emulator emulator{config};
  emulator.LoadProgram();
  emulator.Run();
  return 0;
}
