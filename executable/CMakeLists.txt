add_executable(file_operator_ex file_operator_ex.cpp)
target_link_libraries(file_operator_ex myfutils)

add_executable(parameter_ex parameter_ex.cpp)
target_link_libraries(parameter_ex parameter)

add_executable(put_chinese_txt_ex put_chinese_txt_ex.cpp)
target_link_libraries(put_chinese_txt_ex putchinesetext freetype ${OpenCV_LIBS})

add_executable(mouse_click_get_color_ex mouse_click_get_color_ex.cpp)
target_link_libraries(mouse_click_get_color_ex mouseutils ${OpenCV_LIBS})

add_executable(mouse_select_polygon_ex mouse_select_polygon_ex.cpp)
target_link_libraries(mouse_select_polygon_ex mouseutils ${OpenCV_LIBS})

add_executable(copy_file_ex copy_file_ex.cpp)
target_link_libraries(copy_file_ex ${Boost_LIBRARIES})

add_executable(create_subfolder_directories_ex create_subfolder_directories_ex.cpp)
target_link_libraries(create_subfolder_directories_ex myfutils)

add_executable(linear_lighten_ex linear_lighten_ex.cpp)
target_link_libraries(linear_lighten_ex myfutils)

add_executable(thresh_otsu_ex thresh_otsu_ex.cpp)
target_link_libraries(thresh_otsu_ex myfutils ${OpenCV_LIBS})


add_executable(background_subtractor_mog_ex background_subtractor_mog_ex.cpp)
target_link_libraries(background_subtractor_mog_ex myfutils ${OpenCV_LIBS})

add_executable(lbp_ex lbp_ex.cpp)
target_link_libraries(lbp_ex myfutils ${OpenCV_LIBS})

add_executable(split_train_val PrepareDataTrainCaffeClassification/split_train_val_ex.cpp)
target_link_libraries(split_train_val myfutils ${Boost_LIBRARIES})

#---------------------- 直线检测
add_executable(detect_line_ex detect_line_ex.cpp)
target_link_libraries(detect_line_ex myfutils ${OpenCV_LIBS})

#---------------------- 矩形检测
add_executable(detect_rectangle_ex detect_rectangle_ex.cpp)
target_link_libraries(detect_rectangle_ex myfutils ${OpenCV_LIBS} glog)

#---------------------- 程序运行时间
add_executable(calc_exe_runtime calc_exe_runtime.cpp)