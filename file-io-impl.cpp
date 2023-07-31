#include "file-io-impl.h"
#include <fstream>

void FileIoImpl::Write(const std::string& filePath, const std::string& str) const
{
    std::ofstream ofs(filePath);
    ofs << str;
    ofs.close();
}

std::string FileIoImpl::Read(const std::string& filePath) const
{
    std::string value;
    std::ifstream ifs(filePath);
    ifs >> value;
    ifs.close();
    return value;
}
