set(VND_LIB_NAME VND)	

file(GLOB VND_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp ${CMAKE_CURRENT_LIST_DIR}/../Common/include/Globals.cpp)
file(GLOB VND_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
file(GLOB VND_GLOBAL_INCS  ${CMAKE_CURRENT_LIST_DIR}/../Common/include/VNDLib.h
${CMAKE_CURRENT_LIST_DIR}/../Common/include/Globals.h)


add_library(${VND_LIB_NAME} SHARED  ${VND_SOURCES} ${VND_INCS} ${VND_GLOBAL_INCS})

target_link_libraries(${VND_LIB_NAME} 		debug ${MU_LIB_DEBUG} 		optimized ${MU_LIB_RELEASE} 
												debug ${NATGUI_LIB_DEBUG}   optimized ${NATGUI_LIB_RELEASE}
												debug ${DP_LIB_DEBUG} 		optimized ${DP_LIB_RELEASE})


source_group("inc"            FILES ${VND_INCS}   )
source_group("inc_dll"        FILES ${VND_GLOBAL_INCS}   )
source_group("src"            FILES ${VND_SOURCES}	)

#MU_DEBUG, MU_RELEASE, MU_64BIT, MU_32BIT
addMUCompileDefinitions(${VND_LIB_NAME})
							
#CMake detektuje OS 
if (WIN32)
	target_compile_definitions(${VND_LIB_NAME} PUBLIC VND_LIB_EXPORTS MU_WINDOWS)
elseif(APPLE)                                             
	target_compile_definitions(${VND_LIB_NAME} PUBLIC VND_LIB_EXPORTS MU_MACOS)
else()                                                    
	target_compile_definitions(${VND_LIB_NAME} PUBLIC VND_LIB_EXPORTS MU_LINUX)
endif()

