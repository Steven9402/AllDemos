aux_source_directory(../myfUtils MyfUtils_SRC)
add_library( myfutils SHARED ${MyfUtils_SRC})
target_link_libraries( myfutils ${OpenCV_LIBS})