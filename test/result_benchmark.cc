#include <benchmark/benchmark.h>

#include "./custom_result.h"

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

static void BenchmarkResultOrToResult(benchmark::State& state) {
    auto f = []() -> CustomResult {
        CustomResultOr<int> a = 1;
        return a;
    };

    for (auto _ : state) {
        auto res = f();
    }
}

static void BenchmarkResultOrToResultOr(benchmark::State& state) {
    auto f = []() -> CustomResultOr<int> {
        CustomResultOr<int> a = 1;
        return a;
    };

    for (auto _ : state) {
        auto res = f();
    }
}

BENCHMARK(BenchmarkResultToResult);
BENCHMARK(BenchmarkResultToResultOr);
BENCHMARK(BenchmarkResultOrToResult);
BENCHMARK(BenchmarkResultOrToResultOr);