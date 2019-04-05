#pragma once
//-----------------------------------------------------------------------------
#include <string>
//-----------------------------------------------------------------------------
class SNUrl
{
public:
    SNUrl(const std::string &url);
    ~SNUrl();

    bool GetIsValid() const;
    std::string GetUrl() const;
    std::string GetProtocol() const;
    std::string GetHostName() const;
    std::string GetHostAddress() const;
    std::string GetPath() const;
    std::string GetQuery() const;

private:
    bool IsValid;
    std::string Url;
    std::string Protocol;
    std::string HostName;
    std::string Path;
    std::string Query;
};
//-----------------------------------------------------------------------------
