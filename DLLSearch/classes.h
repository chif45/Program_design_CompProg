#ifndef EMPTY_EXCEPTION_H
#define EMPTY_EXCEPTION_H

#include <string>

class EmptyException
{
public:
    explicit EmptyException(const std::string& message);
private:
    std::string message;
};

#endif // EMPTY_EXCEPTION_H
