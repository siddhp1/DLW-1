#include "dlw1_assembler/lexer.hpp"

#include <sstream>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

#include "dlw1_assembler/token.hpp"
#include "gtest/gtest.h"

class LexerTokenizeTest : public ::testing::TestWithParam<
                              std::tuple<std::string, std::vector<Token>>> {};

TEST_P(LexerTokenizeTest, Tokenize) {
  const auto& [assembly_string, expected_tokens] = GetParam();
  std::stringstream program_contents(assembly_string);
  const std::vector<Token> actual_tokens =
      Lexer::Tokenize(std::move(program_contents));
  EXPECT_EQ(actual_tokens, expected_tokens);
}

INSTANTIATE_TEST_SUITE_P(
    Individual, LexerTokenizeTest,
    ::testing::Values(
        std::make_tuple(":", std::vector<Token>{{.text = ":",
                                                 .type = TokenType::COLON,
                                                 .line_number = 0},
                                                {.text = "",
                                                 .type = TokenType::END_OF_FILE,
                                                 .line_number = 1}}),

        std::make_tuple(",", std::vector<Token>{{.text = ",",
                                                 .type = TokenType::COMMA,
                                                 .line_number = 0},
                                                {.text = "",
                                                 .type = TokenType::END_OF_FILE,
                                                 .line_number = 1}}),

        std::make_tuple(".directive",
                        std::vector<Token>{{.text = ".directive",
                                            .type = TokenType::DIRECTIVE,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("#", std::vector<Token>{{.text = "#",
                                                 .type = TokenType::HASH,
                                                 .line_number = 0},
                                                {.text = "",
                                                 .type = TokenType::END_OF_FILE,
                                                 .line_number = 1}}),

        std::make_tuple("identifier",
                        std::vector<Token>{{.text = "identifier",
                                            .type = TokenType::IDENTIFIER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("identifier_with_underscores",
                        std::vector<Token>{
                            {.text = "identifier_with_underscores",
                             .type = TokenType::IDENTIFIER,
                             .line_number = 0},
                            {.text = "",
                             .type = TokenType::END_OF_FILE,
                             .line_number = 1}}),

        std::make_tuple("(",
                        std::vector<Token>{{.text = "(",
                                            .type = TokenType::LPARENTHESES,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple(")",
                        std::vector<Token>{{.text = ")",
                                            .type = TokenType::RPARENTHESES,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("-", std::vector<Token>{{.text = "-",
                                                 .type = TokenType::MINUS,
                                                 .line_number = 0},
                                                {.text = "",
                                                 .type = TokenType::END_OF_FILE,
                                                 .line_number = 1}}),

        std::make_tuple("+", std::vector<Token>{{.text = "+",
                                                 .type = TokenType::PLUS,
                                                 .line_number = 0},
                                                {.text = "",
                                                 .type = TokenType::END_OF_FILE,
                                                 .line_number = 1}}),

        std::make_tuple("0", std::vector<Token>{{.text = "0",
                                                 .type = TokenType::NUMBER,
                                                 .line_number = 0},
                                                {.text = "",
                                                 .type = TokenType::END_OF_FILE,
                                                 .line_number = 1}}),

        std::make_tuple("123",
                        std::vector<Token>{{.text = "123",
                                            .type = TokenType::NUMBER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("9876",
                        std::vector<Token>{{.text = "9876",
                                            .type = TokenType::NUMBER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("0x10",
                        std::vector<Token>{{.text = "0x10",
                                            .type = TokenType::NUMBER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("0XFF",
                        std::vector<Token>{{.text = "0XFF",
                                            .type = TokenType::NUMBER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("0xABCDEF",
                        std::vector<Token>{{.text = "0xABCDEF",
                                            .type = TokenType::NUMBER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("0b101",
                        std::vector<Token>{{.text = "0b101",
                                            .type = TokenType::NUMBER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("0B1110",
                        std::vector<Token>{{.text = "0B1110",
                                            .type = TokenType::NUMBER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("0b0",
                        std::vector<Token>{{.text = "0b0",
                                            .type = TokenType::NUMBER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("ra",
                        std::vector<Token>{{.text = "ra",
                                            .type = TokenType::REGISTER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("rb",
                        std::vector<Token>{{.text = "rb",
                                            .type = TokenType::REGISTER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("Rc",
                        std::vector<Token>{{.text = "Rc",
                                            .type = TokenType::REGISTER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple("Rsp",
                        std::vector<Token>{{.text = "Rsp",
                                            .type = TokenType::IDENTIFIER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}})));

INSTANTIATE_TEST_SUITE_P(
    Multiple, LexerTokenizeTest,
    ::testing::Values(
        std::make_tuple(
            "load ra, #123",
            std::vector<Token>{
                {.text = "load",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 0},
                {.text = "ra", .type = TokenType::REGISTER, .line_number = 0},
                {.text = ",", .type = TokenType::COMMA, .line_number = 0},
                {.text = "#", .type = TokenType::HASH, .line_number = 0},
                {.text = "123", .type = TokenType::NUMBER, .line_number = 0},
                {.text = "",
                 .type = TokenType::END_OF_FILE,
                 .line_number = 1}}),

        std::make_tuple("start:",
                        std::vector<Token>{{.text = "start",
                                            .type = TokenType::IDENTIFIER,
                                            .line_number = 0},
                                           {.text = ":",
                                            .type = TokenType::COLON,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple(".org 0x100",
                        std::vector<Token>{{.text = ".org",
                                            .type = TokenType::DIRECTIVE,
                                            .line_number = 0},
                                           {.text = "0x100",
                                            .type = TokenType::NUMBER,
                                            .line_number = 0},
                                           {.text = "",
                                            .type = TokenType::END_OF_FILE,
                                            .line_number = 1}}),

        std::make_tuple(
            "(ra + rb - 10)",
            std::vector<Token>{
                {.text = "(",
                 .type = TokenType::LPARENTHESES,
                 .line_number = 0},
                {.text = "ra", .type = TokenType::REGISTER, .line_number = 0},
                {.text = "+", .type = TokenType::PLUS, .line_number = 0},
                {.text = "rb", .type = TokenType::REGISTER, .line_number = 0},
                {.text = "-", .type = TokenType::MINUS, .line_number = 0},
                {.text = "10", .type = TokenType::NUMBER, .line_number = 0},
                {.text = ")",
                 .type = TokenType::RPARENTHESES,
                 .line_number = 0},
                {.text = "",
                 .type = TokenType::END_OF_FILE,
                 .line_number = 1}}),

        std::make_tuple(
            "start:\tload ra, #0x10\t; Load initial value",
            std::vector<Token>{
                {.text = "start",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 0},
                {.text = ":", .type = TokenType::COLON, .line_number = 0},
                {.text = "load",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 0},
                {.text = "ra", .type = TokenType::REGISTER, .line_number = 0},
                {.text = ",", .type = TokenType::COMMA, .line_number = 0},
                {.text = "#", .type = TokenType::HASH, .line_number = 0},
                {.text = "0x10", .type = TokenType::NUMBER, .line_number = 0},
                {.text = "",
                 .type = TokenType::END_OF_FILE,
                 .line_number = 1}}),

        std::make_tuple(
            "data_start: .byte 0xFF ; Initial data",
            std::vector<Token>{
                {.text = "data_start",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 0},
                {.text = ":", .type = TokenType::COLON, .line_number = 0},
                {.text = ".byte",
                 .type = TokenType::DIRECTIVE,
                 .line_number = 0},
                {.text = "0xFF", .type = TokenType::NUMBER, .line_number = 0},
                {.text = "",
                 .type = TokenType::END_OF_FILE,
                 .line_number = 1}}),

        std::make_tuple(
            "loop: sub ra, rb, (ra + #5)",
            std::vector<Token>{
                {.text = "loop",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 0},
                {.text = ":", .type = TokenType::COLON, .line_number = 0},
                {.text = "sub",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 0},
                {.text = "ra", .type = TokenType::REGISTER, .line_number = 0},
                {.text = ",", .type = TokenType::COMMA, .line_number = 0},
                {.text = "rb", .type = TokenType::REGISTER, .line_number = 0},
                {.text = ",", .type = TokenType::COMMA, .line_number = 0},
                {.text = "(",
                 .type = TokenType::LPARENTHESES,
                 .line_number = 0},
                {.text = "ra", .type = TokenType::REGISTER, .line_number = 0},
                {.text = "+", .type = TokenType::PLUS, .line_number = 0},
                {.text = "#", .type = TokenType::HASH, .line_number = 0},
                {.text = "5", .type = TokenType::NUMBER, .line_number = 0},
                {.text = ")",
                 .type = TokenType::RPARENTHESES,
                 .line_number = 0},
                {.text = "",
                 .type = TokenType::END_OF_FILE,
                 .line_number = 1}}),

        std::make_tuple(
            "start:\n\tload ra, #10\n\thalt",
            std::vector<Token>{
                {.text = "start",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 0},
                {.text = ":", .type = TokenType::COLON, .line_number = 0},
                {.text = "load",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 1},
                {.text = "ra", .type = TokenType::REGISTER, .line_number = 1},
                {.text = ",", .type = TokenType::COMMA, .line_number = 1},
                {.text = "#", .type = TokenType::HASH, .line_number = 1},
                {.text = "10", .type = TokenType::NUMBER, .line_number = 1},
                {.text = "halt",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 2},
                {.text = "",
                 .type = TokenType::END_OF_FILE,
                 .line_number = 3}}),

        std::make_tuple(
            "  \n\n\tload ra, #1\n  ",
            std::vector<Token>{
                {.text = "load",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 2},
                {.text = "ra", .type = TokenType::REGISTER, .line_number = 2},
                {.text = ",", .type = TokenType::COMMA, .line_number = 2},
                {.text = "#", .type = TokenType::HASH, .line_number = 2},
                {.text = "1", .type = TokenType::NUMBER, .line_number = 2},
                {.text = "",
                 .type = TokenType::END_OF_FILE,
                 .line_number = 4}}),

        std::make_tuple(
            "; This is a comment\nload ra, #1",
            std::vector<Token>{
                {.text = "load",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 1},
                {.text = "ra", .type = TokenType::REGISTER, .line_number = 1},
                {.text = ",", .type = TokenType::COMMA, .line_number = 1},
                {.text = "#", .type = TokenType::HASH, .line_number = 1},
                {.text = "1", .type = TokenType::NUMBER, .line_number = 1},
                {.text = "",
                 .type = TokenType::END_OF_FILE,
                 .line_number = 2}}),

        std::make_tuple(
            "load ra, 0b1101",
            std::vector<Token>{
                {.text = "load",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 0},
                {.text = "ra", .type = TokenType::REGISTER, .line_number = 0},
                {.text = ",", .type = TokenType::COMMA, .line_number = 0},
                {.text = "0b1101", .type = TokenType::NUMBER, .line_number = 0},
                {.text = "",
                 .type = TokenType::END_OF_FILE,
                 .line_number = 1}}),

        std::make_tuple(
            ".data var1, var2, #123",
            std::vector<Token>{
                {.text = ".data",
                 .type = TokenType::DIRECTIVE,
                 .line_number = 0},
                {.text = "var1",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 0},
                {.text = ",", .type = TokenType::COMMA, .line_number = 0},
                {.text = "var2",
                 .type = TokenType::IDENTIFIER,
                 .line_number = 0},
                {.text = ",", .type = TokenType::COMMA, .line_number = 0},
                {.text = "#", .type = TokenType::HASH, .line_number = 0},
                {.text = "123", .type = TokenType::NUMBER, .line_number = 0},
                {.text = "",
                 .type = TokenType::END_OF_FILE,
                 .line_number = 1}})));
