#include "dlw1_assembler/token.hpp"

#include <iostream>

std::ostream& operator<<(std::ostream& os, const TokenType& type) {
  switch (type) {
    case TokenType::COLON:
      return os << "COLON";
    case TokenType::COMMA:
      return os << "COMMA";
    case TokenType::DIRECTIVE:
      return os << "DIRECTIVE";
    case TokenType::END_OF_FILE:
      return os << "END_OF_FILE";
    case TokenType::HASH:
      return os << "HASH";
    case TokenType::IDENTIFIER:
      return os << "IDENTIFIER";
    case TokenType::LPARENTHESES:
      return os << "LPARENTHESES";
    case TokenType::MINUS:
      return os << "MINUS";
    case TokenType::NUMBER:
      return os << "NUMBER";
    case TokenType::PLUS:
      return os << "PLUS";
    case TokenType::REGISTER:
      return os << "REGISTER";
    case TokenType::RPARENTHESES:
      return os << "RPARENTHESES";
    default:
      return os << "UNKNOWN";
  }
}
