add_library (matmul dgemm-blocked.c)
add_library (matmul-naive dgemm-naive.c)
SET(GCC_COMPILER "-mavx")
add_definitions(${GCC_COMPILER})


target_include_directories (matmul PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories (matmul-naive PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
