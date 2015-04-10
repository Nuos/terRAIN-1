#ifndef EXCEPTION_BASE
#define EXCEPTION_BASE

#include <stdexcept>

class exception_base: public std::exception
{
public:
	exception_base(const char * lpszWhat);
};

#endif