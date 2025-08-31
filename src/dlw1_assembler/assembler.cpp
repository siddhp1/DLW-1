#include "dlw1_assembler/assembler.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include "dlw1_assembler/lexer.hpp"
#include "dlw1_assembler/token.hpp"

void Assembler::Assemble(const std::string& program_file_path) {
  const std::ifstream program_file(program_file_path, std::ios::in);
  if (!program_file) {
    std::cout << "Failed to open program file";
    // TODO: Handle error here
  }

  std::stringstream program_contents;
  program_contents << program_file.rdbuf();

  const std::vector<Token> tokens =
      Lexer::Tokenize(std::move(program_contents));

  // Print all tokens for testing
  for (const Token& t : tokens) {
    std::cout << t.text << ' ' << t.type << ' ' << t.line_number << '\n';
  }
}
