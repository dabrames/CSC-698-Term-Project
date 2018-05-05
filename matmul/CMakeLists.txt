add_library (matmul-blocked dgemm-blocked.c)
add_library (matmul-simd dgemm-simd.c)
add_library (matmul-naive dgemm-naive.c)
add_library (matmul-naive-pthreads dgemm-naive-pthreads.c)
SET(GCC_COMPILER "-mavx")
add_definitions(${GCC_COMPILER})


target_include_directories (matmul-blocked PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (matmul-simd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (matmul-naive PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (matmul-naive-pthreads PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
