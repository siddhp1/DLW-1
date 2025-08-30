#include "dlw1_emulator/config.hpp"

#include <cstddef>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <system_error>

void Config::ValidateProgramFile(const std::string& file_path) {
  std::error_code error_code;

  if (!std::filesystem::exists(file_path, error_code)) {
    if (error_code) {
      throw std::filesystem::filesystem_error(
          "Cannot check file existence for '" + file_path +
              "': " + error_code.message(),
          file_path, error_code);
    }
    throw std::filesystem::filesystem_error(
        "Program file does not exist: " + file_path, file_path,
        std::make_error_code(std::errc::no_such_file_or_directory));
  }

  auto file_status = std::filesystem::status(file_path, error_code);
  if (error_code) {
    throw std::filesystem::filesystem_error("Cannot get file status for '" +
                                                file_path +
                                                "': " + error_code.message(),
                                            file_path, error_code);
  }

  if (file_status.type() != std::filesystem::file_type::regular) {
    throw std::filesystem::filesystem_error(
        "Program file is not a regular file: " + file_path, file_path,
        std::make_error_code(std::errc::invalid_argument));
  }

  auto file_size = std::filesystem::file_size(file_path, error_code);
  if (error_code) {
    throw std::filesystem::filesystem_error("Cannot determine file size for '" +
                                                file_path +
                                                "': " + error_code.message(),
                                            file_path, error_code);
  }

  if (file_size == 0) {
    throw std::filesystem::filesystem_error(
        "Program file is empty: " + file_path, file_path,
        std::make_error_code(std::errc::invalid_argument));
  }

  constexpr std::size_t MAX_PROGRAM_SIZE = std::size_t{64} * 1024;  // 64KB
  if (file_size > MAX_PROGRAM_SIZE) {
    throw std::filesystem::filesystem_error(
        "Program file too large (" + std::to_string(file_size) +
            " bytes, max: " + std::to_string(MAX_PROGRAM_SIZE) +
            "): " + file_path,
        file_path, std::make_error_code(std::errc::file_too_large));
  }

  auto perms = file_status.permissions();
  if ((perms & std::filesystem::perms::owner_read) ==
          std::filesystem::perms::none &&
      (perms & std::filesystem::perms::group_read) ==
          std::filesystem::perms::none &&
      (perms & std::filesystem::perms::others_read) ==
          std::filesystem::perms::none) {
    throw std::filesystem::filesystem_error(
        "Program file is not readable: " + file_path, file_path,
        std::make_error_code(std::errc::permission_denied));
  }
}

void Config::Validate() const {
  if (program_file_path.empty()) {
    throw std::runtime_error(
        "Invalid configuration: Program file path is empty.");
  }
  ValidateProgramFile(program_file_path);

  if (num_banks < MIN_BANKS || num_banks > MAX_BANKS) {
    throw std::runtime_error(
        "Invalid configuration: Number of banks must be between " +
        std::to_string(MIN_BANKS) + " and " + std::to_string(MAX_BANKS));
  }
}
