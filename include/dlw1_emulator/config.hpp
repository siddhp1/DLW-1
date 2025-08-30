#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdint>
#include <string>

struct Config {
  static constexpr uint8_t DEFAULT_NUM_BANKS = 1;
  static constexpr uint8_t MIN_BANKS = 1;
  static constexpr uint8_t MAX_BANKS = 255;

  uint8_t num_banks;
  std::string program_file_path;

  void Validate() const;

 private:
  static void ValidateProgramFile(const std::string& file_path);
};

#endif
