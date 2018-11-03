//
// Created by cuizhou on 18-2-4.
//

#include <cstring>
#include <iostream>

#include "FileOperator.h"
namespace  myf{
    std::vector<std::string> readFileList(const char *basePath)
    {
        std::vector<std::string> result;
        DIR *dir;
        struct dirent *ptr;
        char base[1000];

        if ((dir=opendir(basePath)) == NULL)
        {
            perror("Open dir error...");
            exit(1);
        }

        while ((ptr=readdir(dir)) != NULL)
        {
            if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
                continue;
            else if(ptr->d_type == 8)    ///file
            {printf("d_name:%s/%s\n",basePath,ptr->d_name);
                result.push_back(std::string(ptr->d_name));}
            else if(ptr->d_type == 10)    ///link file
            {printf("d_name:%s/%s\n",basePath,ptr->d_name);
                result.push_back(std::string(ptr->d_name));}
            else if(ptr->d_type == 4)    ///dir
            {
                memset(base,'\0',sizeof(base));
                strcpy(base,basePath);
                strcat(base,"/");
                strcat(base,ptr->d_name);
                result.push_back(std::string(ptr->d_name));
                readFileList(base);
            }
        }
        closedir(dir);
        return result;
    }

    void walk(const char *basePath, std::vector<std::string>& folders, std::vector<std::string>& files)
    {
        DIR *dir;
        struct dirent *ptr;

        if ((dir=opendir(basePath)) == NULL)
        {
            perror("Open dir error...");
            exit(1);
        }

        while ((ptr=readdir(dir)) != NULL)
        {
            if(strcmp(ptr->d_name,".")==0 || strcmp(ptr->d_name,"..")==0)
                continue;
            else if(ptr->d_type == 8)    ///file
            {printf("d_name:%s/%s\n",basePath,ptr->d_name);
                files.push_back(std::string(ptr->d_name));}
            else if(ptr->d_type == 10)    ///link file
            {printf("d_name:%s/%s\n",basePath,ptr->d_name);
                files.push_back(std::string(ptr->d_name));}
            else if(ptr->d_type == 4)    ///dir
            {
                folders.push_back(std::string(ptr->d_name));
            }
        }
        closedir(dir);
    }


    int is_dir(const char* filename)
    {
        struct stat buf;
        int ret = stat(filename,&buf);
        if(0 == ret) {
            if(buf.st_mode & S_IFDIR) {
                return 0;
            }
            else {
                return 1;
            }
        }
        return -1;
    }

    int delete_dir(const char* dirname)
    {
        char chBuf[256];
        DIR * dir = NULL;
        struct dirent *ptr;
        int ret = 0;
        dir = opendir(dirname);
        if(NULL == dir) {
            return -1;
        }
        while((ptr = readdir(dir)) != NULL)
        {
            ret = strcmp(ptr->d_name, ".");
            if(0 == ret) {
                continue;
            }
            ret = strcmp(ptr->d_name, "..");
            if(0 == ret) {
                continue;
            }
            snprintf(chBuf, 256, "%s/%s", dirname, ptr->d_name);
            ret = is_dir(chBuf);
            if(0 == ret) {
                ret = delete_dir(chBuf);
                if(0 != ret) {
                    return -1;
                }
            }
            else if(1 == ret) {
                ret = remove(chBuf);
                if(0 != ret) {
                    return -1;
                }
            }
        }
        (void)closedir(dir);

        ret = remove(dirname);
        if(0 != ret) {
            return -1;
        }
        return 0;
    }

    bool updateDir(std::string dir)
    {
        delete_dir( dir.c_str() );
        const int dir_err = mkdir( dir.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if(dir_err == -1)
        {
            std::cerr << "[ERROR] Error creating " << dir << " directory." << std::endl;
            return false;
        }
        return true;
    }

    bool updateDirWithoutDelete(std::string dir)
    {
        const int dir_err = mkdir( dir.c_str() , S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        if(dir_err == -1)
        {
            std::cerr << "[ERROR] Error creating " << dir << " directory." << std::endl;
            return false;
        }
        return true;
    }

    int trave_dir(const char* path)
    {
        DIR *d = NULL;
        struct dirent *file;
        struct stat buf;
        char chBuf[256];

        if(!(d = opendir(path)))
        {
            printf("error opendir %s!!!\\n",path);
            return -1;
        }

        long latest_file_time = 0;
        std::string latest_file_name = "";
        while((file = readdir(d)) != NULL)
        {
            if(strcmp(file->d_name, ".") == 0)
                continue;
            if(strcmp(file->d_name, "..") == 0)
                continue;
            snprintf(chBuf, 256, "%s/%s", path, file->d_name);
            if(stat(chBuf, &buf) >= 0 ) {
                long tmp_file_time = buf.st_mtime;
                if(tmp_file_time > latest_file_time) {
                    latest_file_time = tmp_file_time;
                    latest_file_name = file->d_name;
                }
            }
        }
        closedir(d);
        return atoi((latest_file_name.substr(
                latest_file_name.find("-") + 1, 5)).c_str());
    }

    void createSubFoldersDemo(std::string parent_path,std::vector<std::string> child_paths){
        if(access(parent_path.c_str(), 0) == -1) {
            if (!updateDirWithoutDelete(parent_path))
                std::cerr << "ERROR when creating output dirs";
        }
        for(std::string child_path:child_paths){
            std::string child_w_path=parent_path+"/"+child_path;
            if(access(child_w_path.c_str(), 0) == -1) {
                if (!updateDirWithoutDelete(child_w_path))
                    std::cerr << "ERROR when creating output dirs";
            }
        }
    }



}
