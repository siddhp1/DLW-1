#include <dlw1/config.hpp>

bool Config::Validate() const {
  if (program_file_path.empty()) {
    // Add error handling here
    return false;
  }

  if (num_banks < MIN_BANKS || num_banks > MAX_BANKS) {
    // Add error handling here
    return false;
  }

  return true;
}
