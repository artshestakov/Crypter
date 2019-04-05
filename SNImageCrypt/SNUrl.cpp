#include "SNUrl.h"
#include <algorithm>
#include <cctype>
#include <functional>
#include "netdb.h"
#include <arpa/inet.h>
//-----------------------------------------------------------------------------
SNUrl::SNUrl(const std::string &url) :
    IsValid(false),
    Url(url)
{
    const std::string URL = Url;
    const std::string ProtocolEnd("://");
    std::string::const_iterator ProtocolIterator = std::search(URL.begin(), URL.end(), ProtocolEnd.begin(), ProtocolEnd.end());
    Protocol.reserve(std::distance(URL.begin(), ProtocolIterator));
    std::transform(URL.begin(), ProtocolIterator, std::back_inserter(Protocol), std::ptr_fun<int,int>(tolower)); // protocol is icase

    if (ProtocolIterator == URL.end())
    {
        return;
    }

    std::advance(ProtocolIterator, ProtocolEnd.length());
    std::string::const_iterator PathIterator = std::find(ProtocolIterator, URL.end(), '/');
    HostName.reserve(std::distance(ProtocolIterator, PathIterator));
    std::transform(ProtocolIterator, PathIterator, back_inserter(HostName), std::ptr_fun<int,int>(tolower)); // host is icase
    std::string::const_iterator QueryIterator = std::find(PathIterator, URL.end(), '?');
    Path.assign(PathIterator, QueryIterator);

    if (QueryIterator != URL.end())
    {
        ++QueryIterator;
    }

    Query.assign(QueryIterator, URL.end());
    IsValid = true;
}
//-----------------------------------------------------------------------------
SNUrl::~SNUrl()
{

}
//-----------------------------------------------------------------------------
bool SNUrl::GetIsValid() const
{
    return IsValid;
}
//-----------------------------------------------------------------------------
std::string SNUrl::GetUrl() const
{
    return Url;
}
//-----------------------------------------------------------------------------
std::string SNUrl::GetProtocol() const
{
    return Protocol;
}
//-----------------------------------------------------------------------------
std::string SNUrl::GetHostName() const
{
    return HostName;
}
//-----------------------------------------------------------------------------
std::string SNUrl::GetHostAddress() const
{
    char *IPAddress = nullptr;
    struct hostent *Host = gethostbyname(HostName.c_str());
    if (Host == nullptr)
    {
        switch (h_errno)
        {
        case HOST_NOT_FOUND:
            fputs("The host was not found.\n", stderr);
            break;
        case NO_ADDRESS:
            fputs("The name is valid but it has no address.\n", stderr);
            break;
        case NO_RECOVERY:
            fputs("A non-recoverable name server error occurred.\n", stderr);
            break;
        case TRY_AGAIN:
            fputs("The name server is temporarily unavailable.", stderr);
            break;
        }
    }
    else
    {
        IPAddress = inet_ntoa(*((struct in_addr*)Host->h_addr_list[0]));
    }

    return IPAddress;
}
//-----------------------------------------------------------------------------
std::string SNUrl::GetPath() const
{
    return Path;
}
//-----------------------------------------------------------------------------
std::string SNUrl::GetQuery() const
{
    return Query;
}
//-----------------------------------------------------------------------------
