#ifndef FILEIOIMPL_H
#define FILEIOIMPL_H

#include "file-io.h"

class FileIoImpl : public FileIo
{
public:
    void Write(const std::string& filePath, const std::string& str) const override;
    std::string Read(const std::string& filePath) const override;

};

#endif
