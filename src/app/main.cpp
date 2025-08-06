#include <cstdint>
#include <cxxopts.hpp>
#include <iostream>

#include "dlw1/emulator.hpp"

int main(int argc, char* argv[]) {
  // Note: logging verbosity option is not yet implemented
  cxxopts::Options options("DLW-1", "DLW-1 CPU Microarchitecture Emulator");
  options.add_options()("f,file", "Path to the program file to execute",
                        cxxopts::value<std::string>())(
      "b,banks",
      "Number of memory banks (default: " +
          std::to_string(Config::DEFAULT_NUM_BANKS) +
          ", range: " + std::to_string(Config::MIN_BANKS) + "-" +
          std::to_string(Config::MAX_BANKS) + ")",
      cxxopts::value<uint8_t>()->default_value(std::to_string(
          Config::DEFAULT_NUM_BANKS)))("version", "Print version information")(
      "h,help", "Print usage information");

  auto result = options.parse(argc, argv);

  if (result.count("help")) {
    std::cout << options.help() << "\n";
    return 0;
  }

  if (result.count("version")) {
    // Get the version from CMake and output it here
    return 0;
  }

  Config config;

  if (!result.count("file")) {
    // Add error handling here for no program file
    // return 1;
    config.program_file_path = "sample_program.bin";
  }

  // config.program_file_path = result["file"].as<std::string>();
  config.num_banks = result["banks"].as<uint8_t>();

  if (!config.Validate()) {
    // Add error handling here
    return 1;
  }

  if (!std::filesystem::exists(config.program_file_path)) {
    // Add error handling here
    return 1;
  }

  Emulator emulator{config};
  emulator.LoadProgram();
  emulator.Run();
  return 0;
}
