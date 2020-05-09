cmake_minimum_required(VERSION 3.2)
if($ENV{TRAVIS_BUILD_NUMBER})
    project(bulkmt VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(bulkmt VERSION 0.0.3)
endif()

find_package (Threads REQUIRED)
configure_file(version.h.in "${CMAKE_CURRENT_SOURCE_DIR}/version.h")

add_subdirectory(
        "${CMAKE_CURRENT_SOURCE_DIR}/gtest"
        gtest
)

add_executable(bulkmt main.cpp subscriber.h bulk.h bulkmanager.h threadqueue.h)
add_executable(test_google testg.cpp)

set_target_properties(bulkmt PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED ON
        )
target_link_libraries (bulkmt ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(test_google
        gtest
        ${CMAKE_THREAD_LIBS_INIT}
        )

install(TARGETS bulkmt RUNTIME DESTINATION bin)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT lismut@gmail.com)

include(CPack)
enable_testing()

#add_test(test_valid_version print_ip)
add_test(GOOGLE_TEST1 bulkmt)
add_test(GOOGLE_TEST2 bulkmt)
add_test(GOOGLE_TEST3 bulkmt)
add_test(GOOGLE_TEST4 bulkmt)
add_test(GOOGLE_TEST5 bulkmt)
add_test(GOOGLE_TEST6 bulkmt)