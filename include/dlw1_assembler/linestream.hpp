#ifndef LINESTREAM_HPP
#define LINESTREAM_HPP

#include <string>

class LineStream {
 private:
  std::string_view line;
  std::size_t position;

 public:
  explicit LineStream(std::string_view line) : line(line), position(0) {}

  [[nodiscard]] bool EndOfStream() const;
  [[nodiscard]] char Get();
  [[nodiscard]] std::size_t GetPosition() const;
  [[nodiscard]] char Peek() const;
  void Skip();
};

#endif
