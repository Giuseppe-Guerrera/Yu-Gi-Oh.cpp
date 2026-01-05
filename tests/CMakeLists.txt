find_package(Catch2 REQUIRED)
include(Catch)

add_executable(async_queue_test async_queue_test.cpp)
target_link_libraries(async_queue_test PRIVATE Catch2::Catch2WithMain peer)

catch_discover_tests(async_queue_test)