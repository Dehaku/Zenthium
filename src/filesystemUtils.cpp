#include "filesystemUtils.hpp"

#ifdef GALAXY_LINUX
#include <sys/stat.h>
#else
#include <windows.h>
#include <direct.h>
#endif

#include <dirent.h>

#include <iostream>

void galaxy_mkdir(std::string const &name)
{
#ifdef GALAXY_LINUX
    mkdir(name.c_str(), 0755);
#else
    _mkdir(name.c_str());
#endif
}

int getdir(std::string dir, std::vector<std::string> &files)
{
    using namespace std;
    DIR *dp;
    struct dirent *dirp;
    if ((dp = opendir(dir.c_str())) == nullptr)
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
    while ((dirp = readdir(dp)) != nullptr)
    {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}
