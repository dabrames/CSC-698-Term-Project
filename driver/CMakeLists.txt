add_executable (benchmark benchmark.c)
find_package(Threads)
target_link_libraries (benchmark LINK_PUBLIC ${CMAKE_THREAD_LIBS_INIT} matmul matmul-naive matmul-naive-pthreads)
