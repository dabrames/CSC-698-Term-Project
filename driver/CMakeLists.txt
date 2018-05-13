add_executable (benchmark benchmark.c)
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries (benchmark LINK_PUBLIC ${CMAKE_THREAD_LIBS_INIT} matmul-blocked matmul-simd matmul-naive
        matmul-naive-pthreads matmul-blocked-naive)
