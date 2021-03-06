#ifndef CCSH_CCSH_UTILS_HPP
#define CCSH_CCSH_UTILS_HPP

#include <cstddef>
#include <exception>
#include <stdexcept>
#include <string>

#include "ccsh_filesystem.hpp"

#include "CWrapper.hpp"

namespace ccsh {

namespace fs {
std::vector<path> expand(path const& p);
std::vector<path> expand(std::vector<path> const& paths);
} // namespace fs

// NEVER EVER USE boost::filesystem DIRECTLY, ALWAYS USE ccsh::fs
// boost::filesystem WILL BE CHANGED TO std::filesystem WITH C++17

fs::path get_home();
fs::path get_current_path();
fs::path get_current_directory();
fs::path get_current_path_abbreviated();

std::string get_hostname();
std::string get_short_hostname();

std::string get_shell_name();

std::string get_ttyname();

bool is_user_possibly_elevated();

class stdc_error : public std::runtime_error
{
    int error_number;
public:
    explicit stdc_error(int no = errno);
    stdc_error(int no, std::string const& msg);

    int no() const
    { return error_number; }
};


class shell_error : public std::runtime_error
{
    using std::runtime_error::runtime_error;
};


class env_var
{
    std::string name;
public:

    static const char* get(std::string const& name);
    static void set(std::string const& name, std::string const& value, bool override = true);
    static int try_set(std::string const& name, std::string const& value, bool override = true);

    explicit env_var(std::string name)
        : name(std::move(name))
    { }

    env_var(env_var&&) = default;
    env_var(env_var const&) = delete;
    env_var& operator=(env_var&&) = default;
    env_var& operator=(env_var const&) = delete;

    operator std::string() const; // getenv
    env_var& operator=(std::string const&); // setenv
};

namespace internal {

struct open_traits
{
    static constexpr int invalid_value = -1;
    using exception = stdc_error;

    static void dtor_func(int fd) noexcept;
};

using open_wrapper = CW::CWrapper<int, open_traits>;

enum class stdfd : uint8_t
{
    in = 0,
    out = 1,
    err = 2,
    count
};

} // namespace internal

} // namespace ccsh

#endif // CCSH_CCSH_UTILS_HPP
