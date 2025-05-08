#CMake list file for compiling mesytec DAQ data processing software 
#library
include_guard(GLOBAL)

cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project(proj255)
set(CMAKE_CXX_FLAGS "-O3 -g -std=c++11")


get_filename_component(REPODirectory ${PROJECT_SOURCE_DIR}/../ ABSOLUTE)


set(HEADERS 
      ${PROJECT_SOURCE_DIR}/include/ke.h
      ${PROJECT_SOURCE_DIR}/include/em.h
      ${PROJECT_SOURCE_DIR}/include/baseClasses.h
      ${PROJECT_SOURCE_DIR}/include/jh.h
      )

set(SOURCES 
      ${PROJECT_SOURCE_DIR}/src/ke.cpp
      ${PROJECT_SOURCE_DIR}/src/em.cc
      ${PROJECT_SOURCE_DIR}/src/baseClasses.cpp
      ${PROJECT_SOURCE_DIR}/src/jh.cpp
    )

file(MAKE_DIRECTORY ${PROJECT_BINARY_DIR}/linkdef)
add_executable(main.exe 
               main.cc
               )
add_executable(shield.exe 
               shieldVal.cpp
               )
set(LINKDEF ${PROJECT_BINARY_DIR}/linkdef/LinkDef.h)
set(LD_HEADER
"#ifdef __CINT__

#pragma link off all globals\;
#pragma link off all classes\;
#pragma link off all functions\;
#pragma link C++ nestedclasses\;\n")
file(WRITE ${LINKDEF} ${LD_HEADER})
foreach(HEADER ${HEADERS})
  file(APPEND ${LINKDEF} "#pragma link C++ defined_in " ${HEADER}\;\n)
endforeach()
file(APPEND ${LINKDEF} "\n#endif\n")

find_package(ROOT REQUIRED)
include(${ROOT_USE_FILE})

root_generate_dictionary(G__proj255 ${HEADERS} LINKDEF ${LINKDEF})

add_library(proj255 SHARED ${SOURCES} G__proj255.cxx)
if(OpenMP_CXX_FOUND)
target_link_libraries(proj255
                      ${ROOT_LIBRARIES}
                      )
target_compile_definitions(proj255
                           PUBLIC
                           -DOMPFOUND
                           )
else()
target_link_libraries(proj255 ${ROOT_LIBRARIES})
endif()
target_include_directories(proj255 PUBLIC ${PROJECT_SOURCE_DIR}/include)
