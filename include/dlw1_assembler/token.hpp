#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <cstdint>
#include <iostream>
#include <string>

enum class TokenType : uint8_t {
  COLON,
  COMMA,
  DIRECTIVE,
  END_OF_FILE,
  HASH,
  IDENTIFIER,
  LPARENTHESES,
  MINUS,
  NUMBER,
  PLUS,
  REGISTER,
  RPARENTHESES,
};

std::ostream& operator<<(std::ostream& os, const TokenType& type);

struct Token {
  std::string text;
  TokenType type;
  std::size_t line_number;
};

inline bool operator==(const Token& lhs, const Token& rhs) {
  return lhs.text == rhs.text && lhs.type == rhs.type &&
         lhs.line_number == rhs.line_number;
}

#endif
