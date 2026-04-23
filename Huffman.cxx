#include "Huffman.h"

#include <algorithm>

// -------------------------------------------------------------------------
Huffman::
    Huffman(const std::uint8_t &d, const std::size_t &f)
    : m_D(d),
      m_F(f),
      m_L(nullptr),
      m_R(nullptr)
{
}

// -------------------------------------------------------------------------
Huffman::
    Huffman(Huffman *a, Huffman *b)
{
  this->merge(a, b);
}

// -------------------------------------------------------------------------
Huffman::
    Huffman(std::vector<Huffman *> &hf)
{
  this->_build(hf);
}

// -------------------------------------------------------------------------
Huffman::
    Huffman(const std::vector<std::uint8_t> &buffer)
{
  std::vector<Huffman *> hf(1 << (8 * sizeof(std::uint8_t)), nullptr);

  for (const std::uint8_t &v : buffer)
  {
    if (hf[v] == nullptr)
      hf[v] = new Huffman(v);
    ++(*(hf[v]));
  } // end for

  this->_build(hf);
}

// -------------------------------------------------------------------------
Huffman::
    ~Huffman()
{
  if (this->m_L != nullptr)
    delete this->m_L;
  if (this->m_R != nullptr)
    delete this->m_R;
}

// -------------------------------------------------------------------------
Huffman &Huffman::
operator++()
{
  this->m_F += 1;
  return (*this);
}

// -------------------------------------------------------------------------
bool Huffman::
operator<(const Huffman &o) const
{
  return (this->m_F < o.m_F);
}

// -------------------------------------------------------------------------
void Huffman::
    merge(Huffman *a, Huffman *b)
{
  this->m_D = 0;
  this->m_F = a->m_F + b->m_F;
  if (*a < *b)
  {
    this->m_L = a;
    this->m_R = b;
  }
  else
  {
    this->m_L = b;
    this->m_R = a;
  } // end if
}

// -------------------------------------------------------------------------
const std::size_t &Huffman::
    frequency() const
{
  return (this->m_F);
}

// -------------------------------------------------------------------------
std::size_t Huffman::
    estimate_compression_size(const std::size_t &d) const
{
  std::size_t s = 0;
  if (this->m_L == nullptr && this->m_R == nullptr)
    // frecuencia * profundidad
    // cuantos bits voy a necesitar para las hojas
    s = this->m_F * d;
  if (this->m_L != nullptr)
    s += this->m_L->estimate_compression_size(d + 1);
  if (this->m_R != nullptr)
    s += this->m_R->estimate_compression_size(d + 1);
  // s = cuantos bits voy a necesitar para codificar el mensaje
  return (s);
}

// -------------------------------------------------------------------------
std::size_t Huffman::
    estimate_compression_size() const
{
  // quiero representar s en bytes, entonces divido por 8
  std::size_t s = this->estimate_compression_size(0);
  // s >> 3 es lo mismo que s / 8. puede tener residuo
  // optimiza uso de procesador. s >> 3 usa 3 ticks de reloj, el resto usa uno
  // una division/modulo normal usa 25 ticks de reloj
  return ((s >> 3) + ((s | 7 != 0) ? 1 : 0));
  /* return ((s >> 3) + ((s & 7 != 0) ? 1 : 0)); */
}

// -------------------------------------------------------------------------
void Huffman::
    codes(
        std::vector<std::pair<std::string, std::size_t>> &C,
        const std::string &d)
{
  // si no esta vacio
  if (C.size() == 0)
  {
    C.resize(1 << (8 * sizeof(std::uint8_t)));
    std::fill(C.begin(), C.end(), std::make_pair("", 0));
  } // end if
  if (this->m_L != nullptr || this->m_R != nullptr)
  {
    // si arbol izquierdo
    if (this->m_L != nullptr)
      this->m_L->codes(C, d + "0");
    // si arbol derecho
    if (this->m_R != nullptr)
      this->m_R->codes(C, d + "1");
  }
  else
    C[this->m_D] = std::make_pair(d, this->m_F);
}

// -------------------------------------------------------------------------
std::size_t Huffman::
    count_leafs() const
{
  if (this->m_L == nullptr && this->m_R == nullptr)
    return (1);
  else
  {
    std::size_t l = 0;
    if (this->m_L != nullptr)
      l += this->m_L->count_leafs();
    if (this->m_R != nullptr)
      l += this->m_R->count_leafs();

    return (l);
  } // end if
}

// -------------------------------------------------------------------------
void Huffman::
    _build(std::vector<Huffman *> &hf)
{
  auto heap_cmp = [](const Huffman *a, const Huffman *b) -> bool
  {
    if (a != nullptr && b != nullptr)
      return (*b < *a);
    else
      return (a != nullptr && b == nullptr);
  };
  std::make_heap(hf.begin(), hf.end(), heap_cmp);

  while (hf.size() > 0)
  {
    std::pop_heap(hf.begin(), hf.end(), heap_cmp);
    Huffman *a = hf.back();
    hf.pop_back();

    std::pop_heap(hf.begin(), hf.end(), heap_cmp);
    Huffman *b = hf.back();
    hf.pop_back();

    if (a != nullptr && b != nullptr)
    {
      if (hf.size() > 0)
      {
        hf.push_back(new Huffman(a, b));
        std::push_heap(hf.begin(), hf.end(), heap_cmp);
      }
      else
        this->merge(a, b);
    }
    else
    {
      if (a != nullptr)
      {
        hf.push_back(a);
        std::push_heap(hf.begin(), hf.end(), heap_cmp);
      } // end if
      if (b != nullptr)
      {
        hf.push_back(b);
        std::push_heap(hf.begin(), hf.end(), heap_cmp);
      } // end if
    } // end if
  } // end while
}

// eof - Huffman.cxx
