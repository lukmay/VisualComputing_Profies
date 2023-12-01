set(GLAD_HDR
	"${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h"
	)
	
set(GLAD_SRC
	"${CMAKE_CURRENT_SOURCE_DIR}/src/glad.cpp"
	)
	
source_group( TREE ${CMAKE_CURRENT_SOURCE_DIR}
    FILES ${GLAD_SRC} ${GLAD_HDR} )

add_library(glad STATIC ${GLAD_SRC} ${GLAD_HDR} )
target_include_directories( glad PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include/>
    )
    
target_compile_features( glad PUBLIC cxx_std_17 )
set_target_properties( glad PROPERTIES CXX_EXTENSIONS OFF )
set_property(TARGET glad APPEND_STRING PROPERTY COMPILE_FLAGS " -w")
