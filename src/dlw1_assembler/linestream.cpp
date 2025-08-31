#include "dlw1_assembler/linestream.hpp"

#include <cstddef>

bool LineStream::EndOfStream() const { return position >= line.size(); }

char LineStream::Get() { return EndOfStream() ? '\0' : line[position++]; }

std::size_t LineStream::GetPosition() const { return position; }

char LineStream::Peek() const { return EndOfStream() ? '\0' : line[position]; }

void LineStream::Skip() { ++position; }
