# CMake entry point
cmake_minimum_required (VERSION 2.6)
project (Tutorials)

find_package(OpenGL REQUIRED)

if( CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR )
    message( FATAL_ERROR "Please select another Build Directory ! (and give it a clever name, like bin_Visual2012_64bits/)" )
endif()
if( CMAKE_SOURCE_DIR MATCHES " " )
	message( "Your Source Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()
if( CMAKE_BINARY_DIR MATCHES " " )
	message( "Your Build Directory contains spaces. If you experience problems when compiling, this can be the cause." )
endif()

# Compile external dependencies 
add_subdirectory (external)

# On Visual 2005 and above, this module can set the debug working directory
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-1c73e35")
#include(CreateLaunchers)
#include(MSVCMultipleProcessCompile) # /MP

include_directories(
	external/AntTweakBar-1.16/include/
	external/glfw-3.0.3/include/GLFW/
	external/glm-0.9.4.0/
	external/glew-1.9.0/include/
	external/assimp-3.0.1270/include/
	external/bullet-2.81-rev2613/src/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLFW_303
	GLEW_190
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

# rendering to texture
add_executable(rendering_to_texture
	src/render_to_texture.cpp
        src/drawableToTexture.hpp
	src/drawableToTexture.cpp
	src/drawableToScreen.hpp
	src/drawableToScreen.cpp
	src/uniforms.hpp
	src/uniforms.cpp
        src/glvbo.hpp
        src/glvbo.cpp
	src/shader.hpp
	src/shader.cpp
#	common/shader.cpp
#	common/shader.hpp
	common/controls.cpp
	common/controls.hpp
	common/texture.cpp
	common/texture.hpp
	common/objloader.cpp
	common/objloader.hpp
	common/vboindexer.cpp
	common/vboindexer.hpp
#	common/text2D.hpp
#	common/text2D.cpp
	
	src/StandardShadingRTT.vertexshader
	src/StandardShadingRTT.fragmentshader
	src/Passthrough.vertexshader
	src/WobblyTexture.fragmentshader
)

target_link_libraries(rendering_to_texture
	${ALL_LIBS}
)

SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )

add_custom_command(
   TARGET rendering_to_texture POST_BUILD
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/Simple.vertexshader" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/" 
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/Simple.fragmentshader" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/" 
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/Passthrough.vertexshader" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/" 
   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/src/WobblyTexture.fragmentshader" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/" 
)

