#ifndef RESULT_RESULT_OR_H
#define RESULT_RESULT_OR_H

#include <iostream>
#include <optional>
#include <type_traits>

#include "./types_check/common.h"
#include "./types_check/is_result.h"
#include "./types_check/is_result_or.h"

#include "./result.h"

namespace result {

template <typename Result, typename T>
class ResultOr : public Result {
public:
    template <typename F>
    using ROr = ResultOr<Result, F>;
    using E = typename Result::ErrorCode;
    using ValueType = T;

public:
    ResultOr() = default;
    virtual ~ResultOr() override = default;

    ResultOr(const T& value) : value_(value) {}
    ResultOr(T&& value) : value_(std::move(value)) {}

    ResultOr(const Result& res) : Result(res) {}
    ResultOr(Result&& result) : Result(std::move(result)) {}

    ResultOr(const ROr<T>& other) : Result(other), value_(other.Value()) {}

    ResultOr(ROr<T>&& other) : Result(other), value_(std::move(other.Value())) {}

    template <typename A, std::enable_if_t<internal::is_result_v<A>, bool> = true>
    ResultOr(A&& a) : Result(std::forward<A>(a)) {
        if constexpr (internal::is_result_or_v<A>) {
            using _ValueType = decltype(a.CloneValue());
            if constexpr (std::is_same_v<_ValueType, ValueType>) {
                if (a.HasValue()) {
                    value_ = std::move(a.Value());
                }
            }
        }
    }

    template <typename A, std::enable_if_t<internal::is_result_v<A>, bool> = true, typename B>
    ResultOr(A&& a, B&& b) : Result(std::forward<A>(a), std::forward<B>(b)) {
        if constexpr (internal::is_result_or_v<A>) {
            using _ValueType = decltype(a.CloneValue());
            if constexpr (std::is_same_v<_ValueType, ValueType>) {
                if (a.HasValue()) {
                    value_ = std::move(a.Value());
                }
            }
        }
    }

    ROr<T>& operator=(const ROr<T>& other) {
        Result::operator=(other);
        value_ = other.Value();
        return *this;
    }

    ROr<T>& operator=(ROr<T>&& other) {
        Result::operator=(other);
        value_ = std::move(other.Value());
        return *this;
    }

    template <typename F, std::enable_if_t<!std::is_same_v<F, T>, bool> = true>
    ResultOr(const ROr<F>& other) : Result(other) {}

    template <typename F, std::enable_if_t<!std::is_same_v<F, T>, bool> = true>
    ResultOr(ROr<F>&& other) : Result(other) {}

    template <typename F, std::enable_if_t<!std::is_same_v<F, T>, bool> = true>
    ROr<T>& operator=(const ROr<F>& other) {
        Result::operator=(other);
        return *this;
    }

    template <typename F, std::enable_if_t<!std::is_same_v<F, T>, bool> = true>
    ROr<T>& operator=(ROr<F>&& other) {
        Result::operator=(other);
        return *this;
    }

    bool HasValue() const {
        return value_.has_value();
    }

    T CloneValue() const {
        return *value_;
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
