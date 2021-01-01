#pragma once
#include "CString.h"
#include <sstream>
#include <codecvt>

/// Making a bit easier handling strings on Silkroad Online

// Converts a stringstream to wstring
inline std::wstring ss_to_wstr(std::stringstream &ss) {
	typedef std::codecvt_utf8_utf16<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.from_bytes(ss.str());
}
// Converts a stringstream to n_wstring
inline n_wstring ss_to_n_wstr(std::stringstream &ss) {
	return n_wstring(ss_to_wstr(ss).c_str());
}
// Converts a stringstream to string
inline std::string ss_to_str(std::stringstream &ss) {
	return std::string(ss.str());
}
// Converts a wstring to string
inline std::string wstr_to_str(std::wstring &ws) {
	typedef std::codecvt_utf8_utf16<wchar_t> convert_type;
	std::wstring_convert<convert_type, wchar_t> converter;
	return converter.to_bytes(ws);
}
// Converts a n_wstring to string
inline std::string n_wstr_to_str(n_wstring &nws) {
	return wstr_to_str(std::wstring(nws));
}

// Check if the string starts equal to the given string
inline bool str_starts_with(std::string const &value, std::string const &preffix) {
	return value.rfind(preffix, 0) == 0;
}
// Check if the string ends equal to the given string
inline bool str_ends_with(std::string const &value, std::string const &suffix) {
	if (value.length() >= suffix.length())
		return value.compare(value.length() - suffix.length(), suffix.length(), suffix) == 0;
	return false;
}

/// Printing stuffs for debugging only

// Prints char array as raw of bytes
inline void PrintByteArray(unsigned char* ByteArray, unsigned int Length) {
	int max = Length - 1;
	for (int i = 0; i < max; i++)
		std::printf("%02X ", ByteArray[i]);
	std::printf("%02X\n", ByteArray[max]);
}