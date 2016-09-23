#ifndef CCSH_OPERATORS_HPP_INCLUDED
#define CCSH_OPERATORS_HPP_INCLUDED

#include "ccsh_command.hpp"

namespace ccsh
{

inline command_runnable shell(std::string const& str, std::vector<std::string> const& args = {})
{
    return {new command_native(str, args)};
}

inline command_runnable operator|(command_runnable const& a, command_runnable const& b)
{
    a.no_autorun();
    b.no_autorun();
    return {new command_pipe(a, b)};
}


/* ******************* file redirection operators ******************* */

inline command_runnable operator<(command_runnable const& c, fs::path const& p)
{
    c.no_autorun();
    return {new command_in_redirect(c, p)};
}

inline command_runnable operator>(command_runnable const& c, fs::path const& p)
{
    c.no_autorun();
    return {new command_out_redirect(c, p, false)};
}

inline command_runnable operator>>(command_runnable const& c, fs::path const& p)
{
    c.no_autorun();
    return {new command_out_redirect(c, p, true)};
}

inline command_runnable operator>=(command_runnable const& c, fs::path const& p)
{
    c.no_autorun();
    return {new command_err_redirect(c, p, false)};
}

inline command_runnable operator>>=(command_runnable const& c, fs::path const& p)
{
    c.no_autorun();
    return {new command_err_redirect(c, p, true)};
}

/* ******************* file redirection operators ******************* */



/* ******************* string redirection operators ******************* */

inline command_runnable operator<(command_runnable const& c, std::string& str)
{   // should be moved to a .cpp because of memcpy
    c.no_autorun();
    auto func = [&str](char* buf, std::size_t s) -> ssize_t
    {
        std::size_t len = str.length();
        len = len < s ? len : s;
        std::memcpy(buf, str.data(), len);
        if(len)
            str.erase(0, len);
        return len;
    };
    return {new command_in_mapping(c, func)};
}

inline command_runnable operator>>(command_runnable const& c, std::string& str)
{
    c.no_autorun();
    auto func = [&str](char* buf, std::size_t s) -> ssize_t
    {
        str += std::string(buf, s);
        return s;
    };
    return {new command_out_mapping(c, func)};
}

inline command_runnable operator>(command_runnable const& c, std::string& str)
{   // operator>> should be used here, but command_runnable has a private move ctor...
    str.clear(); // string should be cleaned only at "run" time!
    c.no_autorun();
    auto func = [&str](char* buf, std::size_t s) -> ssize_t
    {
        str += std::string(buf, s);
        return s;
    };
    return {new command_out_mapping(c, func)};
}

inline command_runnable operator>>=(command_runnable const& c, std::string& str)
{
    c.no_autorun();
    auto func = [&str](char* buf, std::size_t s) -> ssize_t
    {
        str += std::string(buf, s);
        return s;
    };
    return {new command_err_mapping(c, func)};
}

inline command_runnable operator>=(command_runnable const& c, std::string& str)
{   // operator>> should be used here, but command_runnable has a private move ctor...
    str.clear(); // string should be cleaned only at "run" time!
    c.no_autorun();
    auto func = [&str](char* buf, std::size_t s) -> ssize_t
    {
        str += std::string(buf, s);
        return s;
    };
    return {new command_err_mapping(c, func)};}

/* ******************* string redirection operators ******************* */



/* ******************* logical operators ******************* */

inline command_runnable operator&&(command_runnable const& a, command_runnable const& b)
{
    a.no_autorun();
    b.no_autorun();
    return {new command_and(a, b)};
}

inline command_runnable operator||(command_runnable const& a, command_runnable const& b)
{
    a.no_autorun();
    b.no_autorun();
    return {new command_or(a, b)};
}

inline command_runnable operator&&(command_runnable const& a, bool b)
{
    a.no_autorun();
    return {new command_and(a, command_runnable{new command_bool(b)})};
}

inline command_runnable operator||(command_runnable const& a, bool b)
{
    a.no_autorun();
    return {new command_or(a, command_runnable{new command_bool(b)})};
}

inline command_runnable operator&&(bool b, command_runnable const& a) // provided only for consistency, useless anyways
{
    a.no_autorun();
    return {new command_and(command_runnable{new command_bool(b)}, a)};
}

inline command_runnable operator||(bool b, command_runnable const& a) // provided only for consistency, useless anyways
{
    a.no_autorun();
    return {new command_or(command_runnable{new command_bool(b)}, a)};
}

/* ******************* logical operators ******************* */


namespace literals
{

    inline fs::path operator""_p(const char* str, std::size_t)
    {
        return fs::path{str};
    }

} // namespace literals


} // namespace ccsh


#endif // CCSH_OPERATORS_HPP_INCLUDED
