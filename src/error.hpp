#pragma once

#include <string>
#include <variant>

using namespace std;

class Error {

public:
  string message;
  Error(string _message) : message(_message){};
};

class Void {};

template <typename R> using Result = std::variant<R, Error>;

template <typename R> bool is_err(Result<R> r) {
  return std::holds_alternative<Error>(r);
}
template <typename R> bool is_ok(Result<R> r) {
  return std::holds_alternative<R>(r);
}

template <typename R> R Ok(Result<R> r) { return std::get<R>(r); }
/// enables rewrapping errors in a new Result type
template <typename R> Error Err(Result<R> r) { return std::get<Error>(r); }
template <typename R> string err_msg(Result<R> r) {
  return std::get<Error>(r).message;
}
