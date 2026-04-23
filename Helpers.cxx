#include "Helpers.h"

#include <filesystem>
#include <fstream>
#include <system_error>

// -------------------------------------------------------------------------
void read(
  std::vector< std::uint8_t >& buffer, const std::string& fname
  )
{
  std::filesystem::path path
    =
    std::filesystem::canonical( std::filesystem::path( fname ) );

  std::ifstream file( path, std::ios::binary | std::ios::ate );
  if( !file.is_open( ) )
    throw std::runtime_error( "Could not open: \"" + path.string( ) + "\"" );

  std::streamsize size = file.tellg( );
  buffer.resize( static_cast< std::size_t >( size ) );
  file.seekg( 0, std::ios::beg );
  file.read( reinterpret_cast< char* >( buffer.data( ) ), size );
}

// -------------------------------------------------------------------------
void save(
  const std::vector< std::uint8_t >& buffer, const std::string& fname
  )
{
  std::ofstream file( fname, std::ios::binary );

  if( !file.is_open ( ) )
    throw std::runtime_error( "Could not open: \"" + fname + "\"" );

  file.write(
    reinterpret_cast< const char* >( buffer.data( ) ), buffer.size( )
    );

  if( !file.good( ) )
    throw std::runtime_error( "Error occurred while writing to file." );

  file.close( );
}

// eof - Helpers.cxx
