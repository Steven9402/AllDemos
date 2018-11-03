//
// Created by cuizhou on 18-5-17.
//
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"

using namespace std;
namespace bfs = boost::filesystem;

int main(int argc, char* argv[])
{

    string rawFile= "../../data/timg2.jpg";

    string copyFile="../../data/timg2_new.jpg";

    bfs::copy_file(rawFile,copyFile,bfs::copy_option::overwrite_if_exists);

    return 0;

}
