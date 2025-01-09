#include <iostream>
#include <filesystem>
#include <sys/stat.h>

using namespace std::literals;
namespace fs = std::filesystem;

std::string getDirectoryName(const std::string fileName)
{
    int dirNameSize = 0;

    for (int i = 0; i < std::strlen(fileName.data()); ++i)
    {
        if (fileName[i] == '_')
        {
            dirNameSize = i;
            break;
        }
    }

    if (!dirNameSize)
    {
        std::string str = "unknown";

        return str;
    }

    std::string str;
    str.assign(fileName.data(), dirNameSize);

    return str;
}

int main(int argc, char* argv[])
{
    std::string path;
    int count = 0;

    if (argc < 3)
    {
        std::cout << "Please, set --path argument:" << std::endl;
        std::cout << "   PhotosSorter.exe --path [path]" << std::endl;

        return 0;
    }

    if (argv[1] == "--path"sv)
    {
        path = argv[2];
    }

    std::cout << path << std::endl;

    if (!fs::exists(path))
    {
        std::cout << "Directry doesn't exist" << std::endl;

        return 0;
    }

    if (!fs::is_directory(path))
    {
        std::cout << "Passed argument doesn't a directory" << std::endl;

        return 0;
    }

    for (const auto& entry : fs::directory_iterator(path))
    {
        if (entry.is_directory())
        {
            continue;
        }

        const std::string dirName = path + "\\" + getDirectoryName(entry.path().filename().string().data());

        if (!fs::exists(dirName))
        {
            fs::create_directory(dirName);
        }

        fs::rename(entry.path(), dirName + "\\" + entry.path().filename().string());
        count++;

        std::cout << '.';
    }

    std::cout << std::endl;
    std::cout << "Moved: " << count << " files" << std::endl;
    std::cout << "Done" << std::endl;
}
