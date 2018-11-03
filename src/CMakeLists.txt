aux_source_directory(parameterHandler parameterhandler_SRC)
add_library( parameter SHARED ${parameterhandler_SRC})

add_library(putchinesetext SHARED putChineseText/OpencvText.cpp)
target_link_libraries(putchinesetext freetype ${OpenCV_LIBS})

add_subdirectory(myfUtils)

aux_source_directory(mouseUtils MOUSESRC)
add_library(mouseutils SHARED ${MOUSESRC})
target_link_libraries(mouseutils ${OpenCV_LIBS})
