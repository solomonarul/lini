cmake_minimum_required(VERSION 3.10)

project(lini)

add_library(
    ${PROJECT_NAME}
        "src/file.cpp"   
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
        "inc/lini"
    PUBLIC
        "inc/"   
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        CXX_EXTENSIONS ON
)

target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<CXX_COMPILER_ID:GNU>:-Wall -Wextra -Werror -fno-exceptions>
    $<$<CXX_COMPILER_ID:Clang>:-Wall -Wextra -Werror -fno-exceptions>
)