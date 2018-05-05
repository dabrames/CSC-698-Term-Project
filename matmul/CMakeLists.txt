add_library (matmul-simd dgemm-simd.c)
add_library (matmul-naive dgemm-naive.c)
add_library (matmul-naive-pthreads dgemm-naive-pthreads.c)
add_library (matmul-naive-openmp dgemm-naive-openmp.c)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fopenmp")

SET(GCC_COMPILER "-mavx")
add_definitions(${GCC_COMPILER})


target_include_directories (matmul-simd PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (matmul-naive PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (matmul-naive-pthreads PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (matmul-naive-openmp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
