#include <iostream>
#include <string>
#include <map>
#include <stdlib.h>

#include "allocwrap.hpp"

class MyDummyAllocator
{
public:
  void*
  malloc(std::size_t bytes)
  {
    return ::malloc(bytes);
  }

  void
  free(void *bulk)
  {
    ::free(bulk);
  }

  std::size_t
  max_size() const
  {
    return 0x80000000;
  }
};

MyDummyAllocator g_alloc;

DECLARE_STL_ALLOCATOR_WRAPPER(MyStlAllocator, &g_alloc);

// Now we can use MyStlAllocator to replace std::allocator



// Template alias to make the usage of MyMap just like std::map !
// This require compiler to support C++11.
// For MSVC, 2013 and above is required.
// For g++, 4.8 and above is required.
// Apple LLVM: tested ok with 8.0
// Clang++: 3.4 shoule be ok. (not yet tested)
// Also: '--std=c++11' is required for g++/apple llvm/clang++ to compile.
template < typename K, typename V, typename LESS = std::less<K> >
  using MyMap = std::map<K,V,LESS, MyStlAllocator< std::pair<const K, V> > >;

int main()
{
  typedef std::basic_string<char, std::char_traits<char>, MyStlAllocator<char> > mystring;
  mystring abc = "haha abc";

  MyMap<int, mystring> ccc = { { 5, "a123" },{ 7, "uuu" },{ 999, "t%%%" } };
  ccc.insert(std::make_pair(666, abc));
  
  for (auto it = ccc.begin(); it != ccc.end(); ++it)
    std::cout << "K: " << it->first << ", V: " << it->second << std::endl;

  return 0;
}

