#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <random>
#include <algorithm>

namespace fs = std::filesystem;

// Rastgele kelimeler listesi
std::vector<std::string> word_pool = {
    "apple", "banana", "chair", "computer", "dream", "elephant", "forest", "galaxy", "harmony", "island",
    "jungle", "kettle", "lemon", "mountain", "notebook", "ocean", "piano", "queen", "rabbit", "sunset",
    "tiger", "umbrella", "violin", "window", "xylophone", "yellow", "zebra", "artist", "balloon", "candle",
    "dragon", "engine", "flower", "garden", "hero", "ice", "jacket", "king", "ladder", "mirror",
    "night", "orange", "planet", "quiet", "river", "shadow", "table", "unicorn", "valley", "wind",
    "xenon", "yacht", "zone", "ambition", "breeze", "courage", "destiny", "echo", "flame", "glory",
    "hope", "idea", "joy", "kindness", "laugh", "magic", "nature", "opinion", "peace", "quest",
    "respect", "silence", "trust", "unity", "victory", "wisdom", "youth", "zeal", "afford", "behave",
    "climb", "deliver", "explore", "forgive", "gather", "help", "imagine", "jump", "keep", "learn",
    "move", "notice", "obey", "play", "question", "run", "sing", "travel", "understand", "visit", "wait",
    "x-ray", "yell", "zoom", "bubbles", "pokemon"};

std::vector<std::string> word_variants = {"pokemon","Pokemon", "POKEMON"};

std::string generate_random_line(std::mt19937 &rng, const std::string &targetWord, double keywordProbability)
{
    std::uniform_real_distribution<> probDist(0.0, 1.0);
    std::uniform_int_distribution<> wordDist(0, word_pool.size() - 1);

    std::vector<std::string> keyword_variants = {targetWord,
                                                 std::string(1, std::toupper(targetWord[0])) + targetWord.substr(1),
                                                 std::string(targetWord.length(), ' ')};
    std::transform(targetWord.begin(), targetWord.end(), keyword_variants[2].begin(), ::toupper);

    std::uniform_int_distribution<> variantDist(0, keyword_variants.size() - 1);

    std::string line;
    for (int i = 0; i < 10; ++i)
    {
        if (probDist(rng) < keywordProbability)
        {
            line += keyword_variants[variantDist(rng)] + " ";
        }
        else
        {
            line += word_pool[wordDist(rng)] + " ";
        }
    }
    return line + "\n";
}

int main()
{
    std::string outputDir = "random_test_files";
    int numFiles = 50000;
    int linesPerFile = 1000;
    std::string keyword = "pokemon";
    double keywordProbability = 0.15; // %15 ihtimalle anahtar kelime gelir

    if (!fs::exists(outputDir))
    {
        fs::create_directory(outputDir);
    }

    std::random_device rd;
    std::mt19937 rng(rd());

    for (int i = 0; i < numFiles; ++i)
    {
        std::string filename = outputDir + "/file_" + std::to_string(i) + ".txt";
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "File failed to open: " << filename << "\n";
            continue;
        }

        for (int line = 0; line < linesPerFile; ++line)
        {
            file << generate_random_line(rng, keyword, keywordProbability);
        }

        file.close();
    }

    std::cout << numFiles << " File with random context created in: " << outputDir << "\n";
    return 0;
}
