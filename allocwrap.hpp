#pragma once

#include <cstddef>

/*======
 *  Provide a helper macro to wrap a custom allocator instance
 *  to be a std::allocator-compatible class. The output class 
 *  can then be used with all kind of STL containers.
 */

/*======
 *  Requirement:
 *  Your custom allocator should provide at least the 
 *  following 3 member functions:
 *
 *    void*   malloc(size_t);    // Allocate memory
 *    void    free(void*);       // Free memory
 *    size_t  max_size() const;  // The largest possible size
 *                                  in bytes a malloc() can take.
 */

/*======
 *  Usage:
 *     class MyAllocator { // Your custom allocator class
 *     public:
 *       void* malloc(size_t bytes) { ... implementation detail ...  }
 *       void  free(void* bulk)     { ... implementation detail ...  }
 *       size_t max_size() const    { ... implementation detail ...  }
 *     public:
 *       ... other memeber functions ....
 *
 *     } g_myallocator;
 *
 *     // Wrap your custom allocator instance to be a 
 *     // std::allocator-compatible class named 'MyStlAllocator'
 *     DECLARE_STL_ALLOCATOR_WRAPPER(MyStlAllocator, &g_myallocator);
 *
 *     // Use MyStlAllocator to replace std::allocator. Example:
 *
 *     typedef std::basic_string<char, std::char_traits<char>, MyStlAllocator<char> > mystring;
 *     mystring str1; // use mystring to replace std::string
 *
 *     typedef std::list<int, MyStlAllocator<int> > mylist;
 *     mylist lst1; // use mylist to replace std::list
 *
 *     ... and so does other STL container types ...
 *
 *     [C++11 only]
 *     Some container type already has a lot of template arguments and
 *     the allocator argument is unfortunatelly placed at the least.
 *     It may result in boilerplate code wherever you want to declare
 *     a container type using your allocator.
 *     For such case a template alias can give us a great help:
 *
 *     Without C++11 template alias:
 *         std::map<int, std::string, std::less<int>, MyStlAllocator< std::pair<const int, std::string> > > mymap1;
 *         std::map<int, float, std::less<int>, MyStlAllocator< std::pair<const int, float> > > mymap2;
 *
 *     With C++11 template alias:
 *         template < typename K, typename V >
 *           using MyMap = std::map<K,V,std::less<K>, MyStlAllocator< std::pair<const K, V> > >;
 *         MyMap<int, std::string> mymap1;
 *         MyMap<int, float> mymap2;       // Same usage as std::map !!
 */
#define DECLARE_STL_ALLOCATOR_WRAPPER(CLASS_TYPE, NATIVE_INSTANCE_PTR) \
	template < typename T > \
	class CLASS_TYPE \
	{ \
	public: \
		typedef T* pointer; \
		typedef const T* const_pointer; \
		typedef T& reference; \
		typedef const T& const_reference; \
		typedef T value_type; \
		typedef std::size_t size_type; \
		typedef std::ptrdiff_t difference_type; \
		template < typename U > struct rebind { typedef CLASS_TYPE<U> other; }; \
	public: \
		CLASS_TYPE() {} \
		CLASS_TYPE( const CLASS_TYPE& other ) {} \
		template < typename U > CLASS_TYPE ( const CLASS_TYPE<U>& other) {} \
		~CLASS_TYPE() {} \
		bool operator== ( const CLASS_TYPE& other ) const { return true; } \
		bool operator!= ( const CLASS_TYPE& other ) const { return false; } \
		CLASS_TYPE& operator= ( const CLASS_TYPE& other ) { return *this; } \
		template < typename U > CLASS_TYPE& operator= (const CLASS_TYPE<U>& other) { return *this; } \
		pointer address ( reference x ) const { return &x; } \
		const_pointer address ( const_reference x ) const { return &x; } \
		pointer allocate ( size_type n, void* hint = 0 ) {\
			pointer ptr = (pointer) (NATIVE_INSTANCE_PTR)->malloc(n * sizeof(value_type)); \
			if (NULL == ptr) throw std::bad_alloc(); \
			return ptr; \
		} \
		void deallocate ( pointer p, size_type n ) { \
			(NATIVE_INSTANCE_PTR)->free(p); n; \
		} \
		size_type max_size() const { \
			return ((NATIVE_INSTANCE_PTR)->max_size()/sizeof(value_type)); \
		} \
		void construct(pointer p, const_reference val) { \
			::new (p) value_type(val); \
		} \
		void destroy (pointer p) { \
			p->~value_type(); \
		} \
	};



