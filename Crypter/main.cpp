#include <iostream>
#include <algorithm>
#include "SNImageCrypt.h"
//-----------------------------------------------------------------------------
void Usage();
//-----------------------------------------------------------------------------
int main(int argc, char *argv[])
{
    std::string Mode;
    std::string PathImage;
    std::string PathOutput;
    std::string Message;

    if (argc == 5)
    {
        Mode = argv[1];
        PathImage = argv[2];
        PathOutput = argv[3];
        Message = argv[4];
    }
    else if (argc == 3)
    {
        Mode = argv[1];
        PathImage = argv[2];
    }
    else
    {
        std::cout << "Error arguments." << std::endl;
        Usage();
        return EXIT_FAILURE;
    }

    std::transform(Mode.begin(), Mode.end(), Mode.begin(), ::tolower);
    bool Result = true;
    SNImageCrypt ImageCrypt;

    if (Mode == "crypt")
    {
        Result = ImageCrypt.Crypt(PathImage, PathOutput, Message);
        if (Result)
        {
            std::cout << "Message crypted in image " << PathOutput << std::endl;
        }
    }
    else if (Mode == "decrypt")
    {
        Result = ImageCrypt.Decrypt(PathImage, Message);
        if (Result)
        {
            std::cout << "Message decrypted." << std::endl;
            std::cout << Message << std::endl;
        }
    }

    if (!Result)
    {
        std::cout << "Error: " << ImageCrypt.GetErrorString() << std::endl;
    }

    return Result ? EXIT_SUCCESS : EXIT_FAILURE;
}
//-----------------------------------------------------------------------------
void Usage()
{
    std::cout << "Crypter is a utility for encrypting text information into PNG graphics." << std::endl;
    std::cout << std::endl;
    std::cout << "Usage:" << std::endl;
    std::cout << "  ./Crypter crypt /home/user/source.png /homeuser/output.png \"message\"" << std::endl;
    std::cout << "  ./Crypter decrypt /home/user/output.png" << std::endl;
}
//-----------------------------------------------------------------------------
