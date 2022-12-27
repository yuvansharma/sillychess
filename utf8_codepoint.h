#ifndef _UTF8_CODE_POINT_
#define _UTF8_CODE_POINT_

#include <iostream>
#include <string>
#include <cstdint>  // char32_t

using std::ostream;
using std::istream;

class UTF8CodePoint {
	char32_t code_point;
public:
	// Since we created other constructors, we have to use " = default" to get the
	// default constructor (or define our own default constructor).
	UTF8CodePoint() = default;
	UTF8CodePoint(char32_t code_point);

	operator char32_t() const;

	friend ostream& operator<<(ostream& os, const UTF8CodePoint cp);
	friend istream& operator>>(istream& is, UTF8CodePoint& cp);
};

#endif  // _UTF8_CODE_POINT_#pragma once
