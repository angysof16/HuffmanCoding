#include <iostream>
#include "Helpers.h"
#include "Huffman.h"

// header
// intercambio output input
void decode(
  const std::vector<std::uint8_t> &encoded,
  std::vector<std::uint8_t> &decoded);

int main( int argc, char **argv){
  
}

void decode(
  const std::vector<std::uint8_t> &encoded, //leo el .bin
  std::vector<std::uint8_t> &decoded) // reconstruir el texto original
{


  // en vez de escribir L al buffer, leer L desde el buffer
  const std::uint8_t *ptr = encoded.data();
  std::size_t L = *reinterpret_cast<const std::size_t *>(ptr);
  ptr += sizeof(std::size_t);

  // ------------------------------------------------------------------------

  //
}

//eof - decode.cxx