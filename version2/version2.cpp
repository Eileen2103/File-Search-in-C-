#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <filesystem>

namespace fs = std::filesystem;

std::string filePathToString(const std::string &filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open())
    {
        throw std::runtime_error("File can not be opened: " + filepath);
    }

    return std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

int countOccurrences(const std::string &content, const std::string &keyword)
{
    std::string contentLower = content;
    std::string keywordLower = keyword;

    std::transform(contentLower.begin(), contentLower.end(), contentLower.begin(), ::tolower);
    std::transform(keywordLower.begin(), keywordLower.end(), keywordLower.begin(), ::tolower);

    int count = 0;
    size_t pos = 0;
    while ((pos = contentLower.find(keywordLower, pos)) != std::string::npos)
    {
        count++;
        pos += keywordLower.length();
    }
    return count;
}

int main()
{
    const std::string folderPath = R"(/home/eileen21/Desktop/system_programming/word_search_project/tests/random_test_files)";
    const std::string keyword = "pokemon";
    int total = 0;

    try
    {
        for (const auto &entry : fs::directory_iterator(folderPath))
        {
            if (entry.path().extension() == ".txt")
            {
                std::string content = filePathToString(entry.path().string());
                total += countOccurrences(content, keyword);
            }
        }

        std::cout << "Total occurrences of" << keyword << "is: " << total << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Hata: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
