# OpenGL & GLEW library
find_package(OpenGL)
ADD_DEFINITIONS(-DGLEW_STATIC)

# source files
file(GLOB SOURCES ./*.cpp ./rendering/*.cpp)
file(GLOB HEADERS ./*.h ./rendering/*.h)
file(GLOB HEADERS ./*.hpp)
file(GLOB SHADERS ./*.vert ./*.frag)

# Make sure the textures and shaders are available
set(TEXTURE_PATH ${CMAKE_SOURCE_DIR}/textures CACHE PATH "location of texture images")
set(SHADER_PATH  ${CMAKE_SOURCE_DIR}/src      CACHE PATH "location of shader programs")
add_definitions("-DTEXTURE_PATH=\"${TEXTURE_PATH}\"")
add_definitions("-DSHADER_PATH=\"${SHADER_PATH}\"")

# executable
add_executable(RandomTilemapGeneration ${HEADERS} ${SOURCES} utils.cpp voxels.cpp voxels.h)
target_include_directories(RandomTilemapGeneration SYSTEM PUBLIC ${GLEW_INCLUDE_DIRS})
# Note: target_link_libraries(glfw) should actually bring in the necessary header files.
target_link_libraries(RandomTilemapGeneration lodePNG glfw ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} pugiXML bitmap)
