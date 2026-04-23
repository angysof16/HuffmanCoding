#include <iostream>
#include "Helpers.h"
#include "Huffman.h"

// creo clase HuffmanDecoder que hereda de Huffman, m_L m_R son protected
class HuffmanDecoder : public Huffman
{
public:
  HuffmanDecoder(std::vector<Huffman *> &hf) : Huffman(hf) {}
  Huffman *left() const { return this->m_L; }
  Huffman *right() const { return this->m_R; }
};


// intercambio output input
void decode(
    const std::vector<std::uint8_t> &encoded,
    std::vector<std::uint8_t> &decoded);

int main(int argc, char **argv)
{
}

void decode(
    const std::vector<std::uint8_t> &encoded, // leo el .bin
    std::vector<std::uint8_t> &decoded)       // reconstruir el texto original
{

  // en vez de escribir L al buffer, leer L desde el buffer
  const std::uint8_t *ptr = encoded.data();
  std::size_t L = *reinterpret_cast<const std::size_t *>(ptr);
  ptr += sizeof(std::size_t);

  // ------------------------------------------------------------------------

  // ya lei L, ahora tengo que leer los codigos y frecuencias
  std::vector<Huffman *> nodos;

  // en encoder se escribio L veces, ahora leo L veces
  for (std::size_t i = 0; i < L; i++)
  {
    // leo el simbolo
    std::uint8_t c = *ptr;
    // avanzo el apuntador
    ptr += sizeof(std::uint8_t);

    // leo la frecuencia
    std::size_t f = *reinterpret_cast<const std::size_t *>(ptr);
    // avanzo el apuntador
    ptr += sizeof(std::size_t);

    // creo nodo con esos datos y agrego e n vector
    Huffman *nuevoHuffman = new Huffman(c, f);
    nodos.push_back(nuevoHuffman);
  }

  // reconstruir arbol
  HuffmanDecoder h(nodos);
}

// eof - decode.cxx