#ifndef LEXER_HPP
#define LEXER_HPP

#include <sstream>
#include <string>
#include <vector>

#include "dlw1_assembler/linestream.hpp"
#include "dlw1_assembler/token.hpp"

class Lexer {
 private:
  static constexpr std::size_t REGISTER_NAME_LENGTH = 2;
  static constexpr std::string_view VALID_REGISTER_SUFFIXES = "abcdABCD";

  static void CleanLine(std::string& line);
  [[nodiscard]] static Token TokenizeNumber(
      LineStream& linestream, const std::size_t current_line_number);
  [[nodiscard]] static Token TokenizeWord(
      LineStream& linestream, const std::size_t current_line_number);
  [[nodiscard]] static Token TokenizeSpecialCharacter(
      LineStream& linestream, const std::size_t current_line_number);

 public:
  [[nodiscard]] static std::vector<Token> Tokenize(
      std::stringstream program_contents);
};

#endif
