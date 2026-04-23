#ifndef __Huffman__h__
#define __Huffman__h__

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

class Huffman
{
public:
  Huffman( const std::uint8_t& d, const std::size_t& f = 0 );
  Huffman( Huffman* a, Huffman* b );
  Huffman( std::vector< Huffman* >& hf );
  Huffman( const std::vector< std::uint8_t >& buffer );
  virtual ~Huffman( );

  Huffman& operator++( );
  bool operator<( const Huffman& o ) const;

  void merge( Huffman* a, Huffman* b );

  const std::size_t& frequency( ) const;

  std::size_t estimate_compression_size( const std::size_t& d ) const;
  std::size_t estimate_compression_size( ) const;

  void codes(
    std::vector< std::pair< std::string, std::size_t > >& C,
    const std::string& d = ""
    );

  std::size_t count_leafs( ) const;

private:
  void _build( std::vector< Huffman* >& hf );

protected:
  std::uint8_t m_D { 0 };
  std::size_t  m_F { 0 };
  Huffman*     m_L { nullptr };
  Huffman*     m_R { nullptr };
};

#endif // __Huffman__h__

// eof - Huffman.h
