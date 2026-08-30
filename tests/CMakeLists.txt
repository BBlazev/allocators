include(FetchContent)
find_package(Threads REQUIRED)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG        v1.15.2
    GIT_SHALLOW    ON
)
FetchContent_MakeAvailable(googletest)

target_compile_options(gtest PRIVATE -w)
target_compile_options(gtest_main PRIVATE -w)


