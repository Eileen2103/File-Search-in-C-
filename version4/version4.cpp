#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <chrono>

namespace fs = std::filesystem;

// küçük harfe çevirme
inline std::string toLower(const std::string &str)
{
    std::string lowered = str;
    for (char &c : lowered)
    {
        c = std::tolower(c);
    }
    return lowered;
}

// Dosyada kelime sayma
inline int countOccurrencesInFile(const fs::path &filepath, const std::string &targetLower)
{
    std::ifstream file(filepath);
    if (!file.is_open())
        return 0;

    std::string content((std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>());

    std::vector<std::string> words;
    std::string word;
    for (char c : content)
    {
        if (std::isspace(c))
        {
            if (!word.empty())
            {
                words.push_back(word);
                word.clear();
            }
        }
        else
        {
            word += c;
        }
    }
    if (!word.empty())
        words.push_back(word);

    int count = 0;
    for (size_t i = 0; i < words.size(); ++i)
    {
        if (toLower(words[i]) == targetLower)
            count++;
    }

    return count;
}

// Threads
void childWorker(std::queue<fs::path> &fileQueue,
                 std::mutex &queueMutex,
                 std::atomic<int> &totalCount,
                 const std::string &targetLower)
{
    while (true)
    {
        fs::path file;
        {
            std::lock_guard<std::mutex> lock(queueMutex);
            if (fileQueue.empty())
                return;
            file = fileQueue.front();
            fileQueue.pop();
        }

        int count = countOccurrencesInFile(file, targetLower);
        totalCount += count;
    }
}

int main()
{
    const std::string directory = R"(/home/eileen21/Desktop/system programming/word_search_project/tests/random_test_files)";
    const std::string target = "pokemon";
    const std::string targetLower = toLower(target);

    auto startTime = std::chrono::high_resolution_clock::now();

    std::queue<fs::path> fileQueue;
    std::mutex queueMutex;
    std::atomic<int> totalCount(0);

    // Dosyaları sıraya koy ve say
    size_t fileCount = 0;
    for (const auto &entry : fs::directory_iterator(directory))
    {
        if (entry.path().extension() == ".txt")
        {

            // not necessary actually,but as a habit I didn't remove the mutex
            std::lock_guard<std::mutex> lock(queueMutex);
            fileQueue.push(entry.path());
            ++fileCount;
        }
    }

    if (fileQueue.empty())
    {
        std::cerr << "No .txt files found.\n";
        return 1;
    }

    // maksimum thread sayısı
    size_t maxThreads = std::thread::hardware_concurrency();
    if (maxThreads == 0)
        maxThreads = 8;

    // Dosya sayısı kadar veya daha az thread aç
    size_t numThreads = std::min(fileCount, maxThreads);

    // Thread'leri başlat
    std::vector<std::thread> threads;
    for (size_t i = 0; i < numThreads; ++i)
    {
        threads.emplace_back(childWorker,
                             std::ref(fileQueue),
                             std::ref(queueMutex),
                             std::ref(totalCount),
                             std::cref(targetLower));
    }

    // Thread'leri bekle
    for (auto &t : threads)
        t.join();

    auto endTime = std::chrono::high_resolution_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

    std::cout << "Total '" << target << "' count: " << totalCount.load() << "\n";
    std::cout << "Execution time: " << elapsed << " ms\n";

    return 0;
}
