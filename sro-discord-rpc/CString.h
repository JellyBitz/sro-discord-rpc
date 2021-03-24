#pragma once
#include <string>

// florian0 stuff
class padding {
	char pad[4];
};

typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, std::allocator<wchar_t> > n_wstring_t;
typedef std::basic_string<char, std::char_traits<char>, std::allocator<char> > n_string_t;

class n_string : padding, public n_string_t {
	typedef n_string_t basic_string;
};

class n_wstring : padding, public n_wstring_t {
	typedef n_wstring_t basic_string;
};