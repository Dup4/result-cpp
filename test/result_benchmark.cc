#include "benchmark/benchmark.h"

#include "./custom_result/custom_another_result.h"
#include "./custom_result/custom_result.h"

using namespace custom_result;

static void BenchmarkResultToResult(benchmark::State& state) {
    auto f = []() -> CustomResult {
        auto res = CustomResult::Builder(CustomResult::ErrorCode::OK).Build();
        return res;
    };

    for (auto _ : state) {
        auto res = f();
    }
}

static void BenchmarkResultToResultOr(benchmark::State& state) {
    auto f = []() -> CustomResultOr<int> {
        auto res = CustomResult::Builder(CustomResult::ErrorCode::OK).Build();
        return res;
    };

    for (auto _ : state) {
        auto res = f();
    }
}

static void BenchmarkResultToAnotherResult(benchmark::State& state) {
    auto f = []() -> custom_result::CustomResult {
        auto res = custom_another_result::CustomAnotherResult::Builder(
                           custom_another_result::CustomAnotherResult::ErrorCode::OtherError)
                           .Build();
        return res;
    };

    for (auto _ : state) {
        auto res = f();
    }
}

static void BenchmarkResultOrToResult(benchmark::State& state) {
    auto f = []() -> CustomResult {
        CustomResultOr<int> res = CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build();
        return res;
    };

    for (auto _ : state) {
        auto res = f();
    }
}

static void BenchmarkResultOrToAnotherResult(benchmark::State& state) {
    auto f = []() -> CustomResult {
        custom_another_result::CustomAnotherResultOr<int> res =
                custom_another_result::CustomAnotherResult::Builder(
                        custom_another_result::CustomAnotherResult::ErrorCode::OtherError)
                        .Build();
        return res;
    };

    for (auto _ : state) {
        auto res = f();
    }
}

static void BenchmarkResultOrToResultOrWithOK(benchmark::State& state) {
    auto f = []() -> CustomResultOr<int> {
        CustomResultOr<int> res = 1;
        return res;
    };

    for (auto _ : state) {
        auto res = f();
    }
}

static void BenchmarkResultOrToResultOrWithError(benchmark::State& state) {
    auto f = []() -> CustomResultOr<int> {
        CustomResultOr<int> res = CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build();
        return res;
    };

    for (auto _ : state) {
        auto res = f();
    }
}

BENCHMARK(BenchmarkResultToResult);
BENCHMARK(BenchmarkResultToResultOr);
BENCHMARK(BenchmarkResultToAnotherResult);
BENCHMARK(BenchmarkResultOrToResult);
BENCHMARK(BenchmarkResultOrToAnotherResult);
BENCHMARK(BenchmarkResultOrToResultOrWithOK);
BENCHMARK(BenchmarkResultOrToResultOrWithError);
