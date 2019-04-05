#pragma once
//-----------------------------------------------------------------------------
#include <string>
#include <vector>
//-----------------------------------------------------------------------------
class SNDownloader
{
public:
    SNDownloader(const std::string &url);
    ~SNDownloader();

    std::string GetErrorString() const;
    std::string GetFilePath() const;
    bool Download();

private:
    std::string ErrorString;
    std::string Url;
    std::string FilePath;
    std::vector<char> *Vector;
};
//-----------------------------------------------------------------------------
