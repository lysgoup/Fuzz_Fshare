cmake_minimum_required(VERSION 3.18)  # 자동 생성구문

project(google-test)

option(BUILD_GTEST "Builds the googletest subproject" ON)
option(BUILD_GMOCK "Builds the googlemock subproject" ON)

if(BUILD_GMOCK)
  add_subdirectory( googletest/googlemock )
elseif(BUILD_GTEST)
  add_subdirectory( googletest/googletest )
endif()

#SET(CMAKE_C_COMPILER ~/AFLplusplus/afl-gcc)
#SET(CMAKE_CXX_COMPILER ~/AFLplusplus/afl-g++)

#SET(CMAKE_CXX_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage")
#SET(CMAKE_C_FLAGS "-g -O0 --coverage -fprofile-arcs -ftest-coverage")



# 더하기(sum) 기능을 지원하는 내 라이브러리 빌드 (C 코드)
add_library(target STATIC target_program.c target_program.h)


# gtest 빌드 (C++ 코드)
#set(gtest_SRC_DIR googletest)
#add_subdirectory(${gtest_SRC_DIR})
include_directories(googletest/googlemock/include)

# 더하기 라이브러리를 이용하는 실행 프로그램 빌드 (C 코드)
# 테스트만 수행 시에는 사용되지 않으므로, 빌드하지 않아도 된다.
#include_directories(libFshared)
add_executable(runTest test_code.cc)
target_link_libraries(runTest target)
include_directories(googletest/googletest/include)
target_link_libraries(runTest gtest gtest_main gmock)




# 사용되지 않음 (중간단계에서 테스트용으로 작성되었음)
##set(CMAKE_CXX_STANDARD 11)
##set(CMAKE_C_COMPILER g++)
##set(CMAKE_CXX_LINK_EXECUTABLE cxx)
#add_subdirectory(googletest-release-1.8.1)
#set(gtest_SRC_DIR googletest-release-1.8.1)
#include_directories(${gtest_SRC_DIR}/googletest/include ${gtest_SRC_DIR}/googletest)
#add_executable(runSumTests main.cc sum_test.cc)
##set_target_properties(runSumTests PROPERTIES LINKER_LANGUAGE CXX)
#target_link_libraries(runSumTests gtest gtest_main)
#target_link_libraries(runSumTests libsum)