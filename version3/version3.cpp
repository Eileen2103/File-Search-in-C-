#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <algorithm>
#include <thread>
#include <mutex>

namespace fs = std::filesystem;


int countOccurrences(const fs::path &filepath, const std::string &target)
{
    std::ifstream file(filepath);

    if (!file.is_open())
        return 0;

    std::string word;
    std::string targetLower = target;

    std::transform(targetLower.begin(), targetLower.end(), targetLower.begin(), ::tolower);
    int count = 0;
    while (file >> word)
    {
        std::string temp = word;
        std::transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
        if (temp == targetLower)
            count++;
    }
    return count;
}

// threads
void processThreads(const std::vector<fs::path> &files, const std::string &target, int &totalCount, std::mutex &count_mutex)
{
    int occurenceCount = 0;

    for (const auto &file : files)
    {
        occurenceCount += countOccurrences(file, target);
    }

    // critic, only 1 threas is allowed

    std::lock_guard<std::mutex> guard(count_mutex);

    totalCount += occurenceCount;
}

int main()
{
    std::string filePath = R"(/home/eileen21/Desktop/system_programming/word_search_project/tests/random_test_files)";
    ;
    std::string keyword = "pokemon";

    std::vector<fs::path> allFiles;
    for (const auto &entry : fs::directory_iterator(filePath))
    {

        if (entry.path().extension() == ".txt")
        {
            allFiles.push_back(entry.path());
        }

        if (allFiles.empty())
        {
            std::cerr << "Could not found a file in the directory, or inccorect file path.\n";
            return 1;
        }
    }
    int totalCount = 0;
    std::mutex count_mutex;
    const size_t numThreads = 4; // later in v4, thread number will be at least 8, because my computer has 16 cores
    std::vector<std::thread> threads;
    const size_t chunkSize = allFiles.size() / numThreads;

    for (size_t i = 0; i < numThreads; i++)
    {
        size_t start = i * chunkSize;
        size_t end = (i == numThreads - 1) ? allFiles.size() : start + chunkSize;
        std::vector<fs::path> chunk(allFiles.begin() + start, allFiles.begin() + end);
        threads.emplace_back(processThreads, chunk, std::ref(keyword), std::ref(totalCount), ref(count_mutex));
    }

    for (auto &t : threads)
        t.join();

    std::cout << "Total 'pokemon' count:" << totalCount << "\n";
    return 0;
}