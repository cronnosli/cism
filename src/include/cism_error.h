#include <string>
#include <exception>

class CismError : public std::exception
{
public:
    explicit CismError(const std::string& message) : m_message(message) {}
    const char* what() const noexcept override { return m_message.c_str(); }

private:
    std::string m_message;
};