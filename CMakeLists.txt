cmake_minimum_required(VERSION 3.2)

project(allocator VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
#project(allocator VERSION 0.0.1)

configure_file(version.h.in version.h)

add_executable(allocator main.cpp)
add_library(allocator_lib customallocator.h)
add_library(container_lib customcontainer.h customiterator.h)
add_library(math_func_lib mathfunctions.h)
add_executable(test_allocator test_allocator.cpp)

set_target_properties(allocator allocator_lib container_lib math_func_lib test_allocator
    PROPERTIES
    CXX_STANDARD 14
    CXX_STANDARD_REQUIRED ON
    LINKER_LANGUAGE CXX
)
target_include_directories(allocator_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)
target_include_directories(container_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)
target_include_directories(math_func_lib
    PRIVATE "${CMAKE_BINARY_DIR}"
)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    add_subdirectory(googletest)
endif()

if(CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include"
                        "${gmock_SOURCE_DIR}/include")
endif()

target_link_libraries(allocator PRIVATE
    allocator_lib container_lib math_func_lib 
)
target_link_libraries(test_allocator PRIVATE
     allocator_lib gtest gmock gtest_main
)

if (MSVC)
    target_compile_options(allocator PRIVATE
        /W4
    )
    target_compile_options(allocator_lib PRIVATE
        /W4
    )
    target_compile_options(container_lib PRIVATE
        /W4
    )
    target_compile_options(math_func_lib PRIVATE
        /W4
    )
    target_compile_options(test_allocator PRIVATE
        /W4
    )
else ()
    target_compile_options(allocator PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(allocator_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(container_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
    target_compile_options(math_func_lib PRIVATE
        -Wall -Wextra -pedantic -Werror
    )	
    target_compile_options(test_allocator PRIVATE
        -Wall -Wextra -pedantic -Werror
   )
endif()


install(TARGETS allocator RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT example@example.com)

add_test(test_allocator test_allocator)