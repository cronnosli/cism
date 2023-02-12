#ifndef CALL_H
#define CALL_H

#include <string>

namespace execute
{
    class Call
    {
    public:        
        virtual bool run(std::string&) = 0;
    };
}

#endif // CALL_H