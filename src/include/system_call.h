#ifndef SYSTEM_CALL_H
#define SYSTEM_CALL_H

#include <call.h>

namespace execute
{
    class SystemCall : public Call
    {
    public:
        static SystemCall &getInstance()
        {
            static SystemCall instance;
            return instance;
        }
        bool run(std::string&);
    private:
        void lookupCommand(std::string &);
    };
}

#endif // SYSTEM_CALL_H