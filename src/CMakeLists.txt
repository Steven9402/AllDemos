aux_source_directory(parameterHandler parameterhandler_SRC)
add_library( parameter SHARED ${parameterhandler_SRC})

add_library(putchinesetext SHARED putChineseText/OpencvText.cpp)
target_link_libraries(putchinesetext freetype ${OpenCV_LIBS})

aux_source_directory(myfUtils MyfUtils_SRC)
add_library( myfutils SHARED ${MyfUtils_SRC})
target_link_libraries( myfutils ${OpenCV_LIBS})

