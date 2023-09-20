<img align="right" width="96px" src="./assets/1200px_cpp_logo.svg.png">

# result-cpp

[![Clang Format](https://github.com/Dup4/result-cpp/workflows/Clang%20Format/badge.svg)](https://github.com/Dup4/result-cpp/actions/workflows/clang_format.yml)
[![Test](https://github.com/Dup4/result-cpp/workflows/Test/badge.svg)](https://github.com/Dup4/result-cpp/actions/workflows/test.yml)
[![codecov](https://codecov.io/gh/Dup4/result-cpp/branch/main/graph/badge.svg)](https://codecov.io/gh/Dup4/result-cpp)
[![GitHub release](https://img.shields.io/github/release/Dup4/result-cpp.svg)](https://github.com/Dup4/result-cpp/releases/)

## Integration

### CMake

```cmake
FetchContent_Declare(
    result-cpp
    QUIET
    GIT_REPOSITORY https://github.com/Dup4/result-cpp.git
    GIT_TAG v{x.y.z} # Where `x.y.z` is the release version you want to use.
)
```

### Bazel

```bazel
git_repository(
    name = "result-cpp",
    remote = "https://github.com/Dup4/result-cpp.git",
    tag = "v{x.y.z}",  # Where `x.y.z` is the release version you want to use.
)
```

## Usage

See the usage in the [unit test](./test/result_test.cc)

## License

[MIT](./LICENSE) License © 2022 - PRESENT [Dup4][dup4]
