set(LIBRARY_NAME "peer")
set(LIBRARY_SOURCES
    async_queue.cpp
    peer.cpp)

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_SOURCES})
target_link_libraries(${LIBRARY_NAME} PUBLIC asio::asio)
target_include_directories(${LIBRARY_NAME} PUBLIC "./")
if(WIN32)
    target_compile_definitions(${LIBRARY_NAME} PUBLIC _WIN32_WINNT=0x0A00)
endif()