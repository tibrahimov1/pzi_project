set(PROJECT_NAME PROJECT)				#Naziv prvog projekta u solution-u

file(GLOB PROJECT_SOURCES  ${CMAKE_CURRENT_LIST_DIR}/src/*.cpp)
file(GLOB PROJECT_INCS  ${CMAKE_CURRENT_LIST_DIR}/src/*.h)
set(PROJECT_PLIST  ${CMAKE_CURRENT_LIST_DIR}/src/Info.plist)

# add executable
add_executable(${PROJECT_NAME} ${PROJECT_INCS} ${PROJECT_SOURCES})

source_group("inc"            FILES ${PROJECT_INCS})
source_group("src"            FILES ${PROJECT_SOURCES})


target_link_libraries(${PROJECT_NAME}	debug ${MU_LIB_DEBUG}   	optimized ${MU_LIB_RELEASE}
									debug ${NATGUI_LIB_DEBUG}   optimized ${NATGUI_LIB_RELEASE}
									debug ${DP_LIB_DEBUG} 		optimized ${DP_LIB_RELEASE}
									debug ${VND_LIB_NAME} optimized ${VND_LIB_NAME}
									)


setTargetPropertiesForGUIApp(${PROJECT_NAME} ${PROJECT_PLIST})

setIDEPropertiesForExecutable(${PROJECT_NAME})

setPlatformDLLPath(${PROJECT_NAME})


#MU_DEBUG, MU_RELEASE, MU_64BIT, MU_32BIT
addMUCompileDefinitions(${PROJECT_NAME})


