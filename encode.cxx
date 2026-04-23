
#include <iostream>

#include "Helpers.h"
#include "Huffman.h"

// -------------------------------------------------------------------------
void encode(
    std::vector<std::uint8_t> &output,
    const std::vector<std::uint8_t> &input);

// -------------------------------------------------------------------------
int main(int argc, char **argv)
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " input output" << std::endl;
    return (EXIT_FAILURE);
  } // end if

  std::vector<std::uint8_t> buffer, encoded;

  read(buffer, argv[1]);
  encode(encoded, buffer);
  save(encoded, argv[2]);

  return (EXIT_SUCCESS);
}

// -------------------------------------------------------------------------
void encode(
    std::vector<std::uint8_t> &output,
    const std::vector<std::uint8_t> &input)
{
  Huffman h(input);

  // hojas L = cantidad de elementos que componen en mensaje
  std::size_t L = h.count_leafs();
  // sizeof( std::size_t ) cuanto ocupa en memoria una variable de tipo size_t
  std::size_t S = h.estimate_compression_size() + sizeof(std::size_t);
  S += L * (sizeof(std::size_t) * sizeof(std::uint8_t));
  // #L + #C (codigo) + #F (frecuencia) + #C + #F + ... + #C + #F
  //|-St-|---------------------------L---------------------------|   --> 0100...1

  output.resize(S, 0);

  // TODO: Generate the bitwise encoding

  // mensaje guardado en input
  // codigos de cada simbolo guardados en codigos

  std::vector<std::pair<std::string, std::size_t>> codigos;
  h.codes(codigos);

  for (std::size_t c = 0; c < codigos.size(); c++)
  {
    if (codigos[c].first != "")
    {
      std::cout << (char)c << " " << codigos[c].first << " " << codigos[c].second << std::endl;
    }
  }

  // apundar a la parte del buffer donde se va a guardar el mensaje codificado
  std::uint8_t *buffer = output.data();
  *reinterpret_cast<std::size_t *>(buffer) = L;
  buffer += sizeof(std::size_t);

  // primera posicion del buffer es L
  // segunda posicion es codigo
  // tercera posicion es frecuencia
  for (std::size_t c = 0; c < codigos.size(); c++)
  {
    if (codigos[c].first != "")
    {
      *buffer = std::uint8_t(c);
      buffer += sizeof(std::uint8_t);
      *reinterpret_cast<std::size_t *>(buffer) = codigos[c].second;
      buffer += sizeof(std::size_t);
    } // end if
  } // end for

  std::string encoded = "";

  for (const std::uint8_t &e : input)
  {
    encoded += codigos[e].first;
  }
  std::cout << encoded << std::endl;

  // volverlo en buffer de memoria donde viene el mensaje, codigo frecuencia codigo grecienam

  // falta recorrer e imprimir?
  // buffer ya apunta al mensaje encodificado
  //  en que bit vo
  std::size_t pos = 0;
  // recorro cada bit del mensaje codificado encoded
  for (char bit : encoded)
  {
    // el buffer ya venia lleno de ceros
    if (bit == '0')
      // fuerzo a 0 el bit en la posicion pos del buffer
      *buffer &= ~(1 << (7 - pos));
    else
      // por cada 1 escrbio un 1 en la posicion pos del buffer
      *buffer |= (1 << (7 - pos));
    // muevo el apuntador al siguiente bit del byte
    pos++;
    if (pos == 8)
    {
      pos = 0;
      buffer++;
    } // end if
  } // end for
}
// eof - encode.cxx
