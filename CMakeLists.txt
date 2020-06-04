cmake_minimum_required(VERSION 3.2)
if($ENV{TRAVIS_BUILD_NUMBER})
    project(bulk_srv VERSION 0.0.$ENV{TRAVIS_BUILD_NUMBER})
else()
    project(bulk_srv VERSION 0.0.3)
endif()

configure_file(version.h.in "${CMAKE_CURRENT_SOURCE_DIR}/version.h")
include_directories(${Boost_INCLUDE_DIRS})
add_subdirectory(
        "${CMAKE_CURRENT_SOURCE_DIR}/gtest"
        gtest
)

add_executable(bulk_server main.cpp async.h async.cpp bulk.h subscriber.h bulkmanager.h threadqueue.h)
#add_executable(client client.cpp client.h)
add_executable(tests testg.cpp async.h async.cpp bulk.h subscriber.h bulkmanager.h threadqueue.h)

set(CXX_STANDARD 17)
set(CXX_STANDARD_REQUIRED ON)
find_package (Threads REQUIRED)
find_package(Boost COMPONENTS system REQUIRED)

target_link_libraries (bulk_server ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
#target_link_libraries (client ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})
target_link_libraries(tests
    gtest
    ${CMAKE_THREAD_LIBS_INIT}
    ${Boost_LIBRARIES}
)

set(CPACK_GENERATOR DEB)

set(CPACK_PACKAGE_VERSION_MAJOR "${PROJECT_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${PROJECT_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${PROJECT_VERSION_PATCH}")

set(CPACK_PACKAGE_CONTACT lismut@gmail.com)

include(CPack)
enable_testing()

add_test(GOOGLE_TEST1 tests)
add_test(GOOGLE_TEST2 tests)
add_test(GOOGLE_TEST3 tests)
add_test(GOOGLE_TEST4 tests)
add_test(GOOGLE_TEST5 tests)
add_test(GOOGLE_TEST6 tests)
add_test(GOOGLE_TEST7 tests)