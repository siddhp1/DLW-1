#include "dlw1_assembler/lexer.hpp"

#include <cctype>
#include <cstddef>
#include <sstream>
#include <string>
#include <vector>

#include "dlw1_assembler/linestream.hpp"
#include "dlw1_assembler/token.hpp"

void Lexer::CleanLine(std::string& line) {
  const std::string whitespace_chars = " \f\n\r\t\v";

  // Remove leading whitespace
  const std::size_t start = line.find_first_not_of(whitespace_chars);
  if (start == std::string::npos) {
    line.clear();
    return;
  }
  line.erase(0, start);

  // Remove comment
  const std::size_t semicolon_pos = line.find(';');
  if (semicolon_pos != std::string::npos) {
    line.erase(semicolon_pos);
  }

  // Remove trailing whitespace
  const std::size_t end = line.find_last_not_of(whitespace_chars);
  if (end == std::string::npos) {
    line.clear();
    return;
  }
  line.erase(end + 1);
}

Token Lexer::TokenizeNumber(LineStream& linestream,
                            const std::size_t current_line_number) {
  Token token{.type = TokenType::NUMBER, .line_number = current_line_number};

  std::string text;
  if (linestream.Peek() == '0') {
    text.push_back(linestream.Get());

    if (!linestream.EndOfStream()) {
      // Hexadecimal
      if (linestream.Peek() == 'x' || linestream.Peek() == 'X') {
        text.push_back(linestream.Get());
        // NOLINTNEXTLINE(readability-implicit-bool-conversion)
        while (std::isxdigit(linestream.Peek())) {
          text.push_back(linestream.Get());
        }
      }
      // Binary
      else if (linestream.Peek() == 'b' || linestream.Peek() == 'B') {
        text.push_back(linestream.Get());
        while (linestream.Peek() == '0' || linestream.Peek() == '1') {
          text.push_back(linestream.Get());
        }
      }
    }
  } else {
    // Decimal
    // NOLINTNEXTLINE(readability-implicit-bool-conversion)
    while (std::isdigit(linestream.Peek())) {
      text.push_back(linestream.Get());
    }
  }
  token.text = text;

  return token;
}

Token Lexer::TokenizeWord(LineStream& linestream,
                          const std::size_t current_line_number) {
  Token token{.line_number = current_line_number};

  std::string text;
  // NOLINTNEXTLINE(readability-implicit-bool-conversion)
  while (std::isalnum(linestream.Peek()) || linestream.Peek() == '.' ||
         linestream.Peek() == '_') {
    text.push_back(linestream.Get());
  }
  token.text = text;

  if (text.front() == '.') {
    token.type = TokenType::DIRECTIVE;
  } else if ((text.front() == 'r' || text.front() == 'R') &&
             text.size() == REGISTER_NAME_LENGTH &&
             (VALID_REGISTER_SUFFIXES.find(text[1]) !=
              std::string_view::npos)) {
    token.type = TokenType::REGISTER;
  } else {
    token.type = TokenType::IDENTIFIER;
  }

  return token;
}

Token Lexer::TokenizeSpecialCharacter(LineStream& linestream,
                                      const std::size_t current_line_number) {
  Token token{.line_number = current_line_number};

  const char current_character = linestream.Get();
  token.text = std::string(1, current_character);

  switch (current_character) {
    case ':':
      token.type = TokenType::COLON;
      break;
    case ',':
      token.type = TokenType::COMMA;
      break;
    case '#':
      token.type = TokenType::HASH;
      break;
    case '(':
      token.type = TokenType::LPARENTHESES;
      break;
    case '-':
      token.type = TokenType::MINUS;
      break;
    case '+':
      token.type = TokenType::PLUS;
      break;
    case ')':
      token.type = TokenType::RPARENTHESES;
      break;
    default:
      // TODO: Handle error here
      break;
  }

  return token;
}

std::vector<Token> Lexer::Tokenize(std::stringstream program_contents) {
  std::size_t current_line_number = 0;

  std::vector<Token> tokens;

  std::string line;
  while (std::getline(program_contents, line)) {
    CleanLine(line);

    // TODO: Reconsider this logic for error messages
    if (line.empty()) {
      ++current_line_number;
      continue;
    }

    LineStream linestream{line};
    while (!linestream.EndOfStream()) {
      const char current_character = linestream.Peek();

      // Skip whitespace between tokens
      // NOLINTNEXTLINE(readability-implicit-bool-conversion)
      if (std::isspace(current_character)) {
        linestream.Skip();
        continue;
      }

      Token token;
      // NOLINTNEXTLINE(readability-implicit-bool-conversion)
      if (std::isdigit(current_character)) {
        token = TokenizeNumber(linestream, current_line_number);
      }
      // NOLINTNEXTLINE(readability-implicit-bool-conversion)
      else if (std::isalpha(current_character) || current_character == '.' ||
               current_character == '_') {
        token = TokenizeWord(linestream, current_line_number);
      } else {
        token = TokenizeSpecialCharacter(linestream, current_line_number);
      }
      tokens.push_back(token);
    }

    ++current_line_number;
  }

  tokens.push_back({.text = "",
                    .type = TokenType::END_OF_FILE,
                    .line_number = current_line_number});

  return tokens;
}
