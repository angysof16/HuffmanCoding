#ifndef __Helpers__h__
#define __Helpers__h__

#include <cstdint>
#include <string>
#include <vector>

void read(
  std::vector< std::uint8_t >& buffer, const std::string& fname
  );

void save(
  const std::vector< std::uint8_t >& buffer, const std::string& fname
  );

#endif // __Helpers__h__

// eof - Helpers.h
