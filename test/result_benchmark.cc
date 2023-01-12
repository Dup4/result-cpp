#include "benchmark/benchmark.h"

#include "result/macros.h"

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

static void BenchmarkResultHistoryInfoWithOK(benchmark::State& state) {
    auto deepCall = [deep = state.range(0)](auto&& d, int cur) {
        if (cur == deep) {
            auto res = CustomResult::OK();
            RESULT_DIRECT_RETURN(res);
        } else {
            RESULT_DIRECT_RETURN(d(d, cur + 1));
        }
    };

    for (auto _ : state) {
        auto res = deepCall(deepCall, 1);
        auto pretty_message = res.PrettyMessage();
    }
}

static void BenchmarkResultHistoryInfo(benchmark::State& state) {
    auto deepCall = [deep = state.range(0)](auto&& d, int cur) {
        if (cur == deep) {
            auto res = CustomResult::Builder(CustomResult::ErrorCode::OtherError).Build();
            RESULT_DIRECT_RETURN(res);
        } else {
            RESULT_DIRECT_RETURN(d(d, cur + 1));
        }
    };

    for (auto _ : state) {
        auto res = deepCall(deepCall, 1);
        auto pretty_message = res.PrettyMessage();
    }
}

BENCHMARK(BenchmarkResultToResult);
BENCHMARK(BenchmarkResultToResultOr);
BENCHMARK(BenchmarkResultToAnotherResult);
BENCHMARK(BenchmarkResultOrToResult);
BENCHMARK(BenchmarkResultOrToAnotherResult);
BENCHMARK(BenchmarkResultOrToResultOrWithOK);
BENCHMARK(BenchmarkResultOrToResultOrWithError);
BENCHMARK(BenchmarkResultHistoryInfoWithOK)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64)->Arg(128);
BENCHMARK(BenchmarkResultHistoryInfo)->Arg(4)->Arg(8)->Arg(16)->Arg(32)->Arg(64)->Arg(128);
