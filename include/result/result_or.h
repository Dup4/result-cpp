#ifndef RESULT_RESULT_OR_H
#define RESULT_RESULT_OR_H

#include <optional>
#include <type_traits>

#include "./result.h"

namespace result {

template <typename Result, typename T>
class ResultOr : public Result {
    template <typename F>
    using ROr = ResultOr<Result, F>;

public:
    ResultOr() = default;

    ResultOr(T&& value) : value_(std::move(value)) {}

    ResultOr(Result&& result) : Result(result) {}

    ResultOr(const ROr<T>& other) : Result(other), value_(other.value_) {}

    template <typename F, std::enable_if_t<!std::is_same_v<F, T>, bool> = true>
    ResultOr(const ROr<F>& other) : Result(other) {}

    ResultOr(ROr<T>&& other) : Result(other), value_(std::move(other.value_)) {}

    template <typename F, std::enable_if_t<!std::is_same_v<F, T>, bool> = true>
    ResultOr(ROr<F>&& other) : Result(other) {}

    ROr<T>& operator=(const ROr<T>& other) {
        Result::operator=(other);
        value_ = other.value_;
        return *this;
    }

    template <typename F, std::enable_if_t<!std::is_same_v<F, T>, bool> = true>
    ROr<T>& operator=(const ROr<F>& other) {
        Result::operator=(other);
        return *this;
    }

    ROr<T>& operator=(ROr<T>&& other) {
        Result::operator=(other);
        value_ = std::move(other.value_);
        return *this;
    }

    template <typename F, std::enable_if_t<!std::is_same_v<F, T>, bool> = true>
    ROr<T>& operator=(ROr<F>&& other) {
        Result::operator=(other);
        return *this;
    }

    const T& Value() const& {
        return *value_;
    }

    T& Value() & {
        return *value_;
    }

    const T&& Value() const&& {
        return std::move(*value_);
    }

    T&& Value() && {
        return std::move(*value_);
    }

    const T& operator*() const& {
        return *value_;
    }

    T& operator*() & {
        return *value_;
    }

    const T&& operator*() const&& {
        return std::move(*value_);
    }

    T&& operator*() && {
        return std::move(*value_);
    }

    template <typename U>
    T ValueOr(U&& default_value) const& {
        if (Result::IsOK()) {
            return *value_;
        }

        return std::forward<U>(default_value);
    }

    template <typename U>
    T ValueOr(U&& default_value) && {
        if (Result::IsOK()) {
            return std::move(*value_);
        }

        return std::forward<U>(default_value);
    }

private:
    std::optional<T> value_{std::nullopt};
};

}  // namespace result

#endif  // RESULT_RESULT_OR_H
