file(GLOB_RECURSE SRCS ./*.cpp)
file(GLOB_RECURSE HDRS ./*.hpp)

if(UNIX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -Wall -pedantic -ansi")
endif()

add_library(pugiXML STATIC ${HDRS} ${SRCS})

