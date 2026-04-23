#include <iostream>
#include "Helpers.h"
#include "Huffman.h"

/*
// creo clase HuffmanDecoder que hereda de Huffman, m_L m_R son protected
class HuffmanDecoder : public Huffman
{
public:
  HuffmanDecoder(std::vector<Huffman *> &hf) : Huffman(hf) {}
  Huffman *left() const { return this->m_L; }
  Huffman *right() const { return this->m_R; }
  std::uint8_t symbol() const { return this->m_D; }
};
*/


// intercambio output input
void decode(
    const std::vector<std::uint8_t> &encoded,
    std::vector<std::uint8_t> &decoded);



int main(int argc, char **argv)
{
  if (argc < 3)
  {
    std::cerr << "Usage: " << argv[0] << " input output" << std::endl;
    return (EXIT_FAILURE);
  }

  std::vector<std::uint8_t> buffer, decoded;

  read(buffer, argv[1]);
  decode(buffer, decoded);
  save(decoded, argv[2]);

  return (EXIT_SUCCESS);
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
    /* Huffman *nuevoHuffman = new Huffman(c, f); */
    nodos.push_back(new Huffman(c, f));
  }
  
  // reconstruir con el mismo vector de 256 que usa el encode 
  std::vector<Huffman *> hf(1 << (8 * sizeof(std::uint8_t)), nullptr);
  for (Huffman *n: nodos)
    hf[n->symbol()] = n;

  // reconstruir arbol identico al de encode
  Huffman h(hf);

  // NAvegar el arbol con los bits del mensaje codificado
  Huffman *nodo = &h; //empiezo en la raiz
  std::size_t pos = 0;

  std::size_t total = h.frequency(); // cantidad de simbolos que tengo que decodificar
  
  for (std::size_t i = 0; i < total; i++){
    // navegar bit a bit hasta llegar a una hoja

    while(nodo->left() != nullptr || nodo->right() != nullptr){
      // leo un bit del mensaje codificado
      std::uint8_t bit = (*ptr >> (7 - pos)) & 1;

      //cuando pos llegue a 8 avanzo ptr y reseteo pos
      pos++;
      if (pos == 8){
        pos = 0;
        ptr++;
      }

      // si el bit es 0 voy a la izquierda
      if(bit == 0){
        nodo = nodo->left();
      }
      // si el bit es 1 voy a la derecha
      else{
        nodo = nodo->right();
      }
    }

    decoded.push_back( nodo->symbol() );
    nodo = &h; // vuelvo a la raiz

  }
}

// eof - decode.cxx