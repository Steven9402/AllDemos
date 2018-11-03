//
// Created by cuizhou on 18-2-4.
//

#ifndef CUIZHOUOBJDETECT_FILEOPERATOR_H
#define CUIZHOUOBJDETECT_FILEOPERATOR_H


#include <stdlib.h>
#include <dirent.h>
#include <string>
#include <vector>

#include <sys/stat.h>
#include <zconf.h>

namespace myf{

    /**
     * 递归读取一个文件夹下所有的文件
     * @param basePath
     * @return
     */
    std::vector<std::string> readFileList(const char *basePath);

    /**
     * 返回当前文件夹下的文件夹和文件
     * @param basePath
     * @param folders
     * @param files
     */
    void walk(const char *basePath,std::vector<std::string>& folders,std::vector<std::string>& files);


    //删除文件夹，成功返回0，否则-1
    int delete_dir(const char* dirname);
    //尝试创建文件夹子，如果存在，先删除
    bool updateDir(std::string dir);
    //尝试创建文件夹子，如果存在，不改变
    bool updateDirWithoutDelete(std::string dir);

    //demo，创建两级文件夹目录
    void createSubFoldersDemo(std::string parent_path,std::vector<std::string> child_paths);
}

#endif //CUIZHOUOBJDETECT_FILEOPERATOR_H
