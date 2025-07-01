#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// Dosyanın tamamını string olarak okumak için
std::string fileToString(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        throw std::runtime_error("File can not be opened " + filepath);
    }

    std::string line, content;
    while (std::getline(file, line)) {
        content += line + '\n';
    }
    return content;
}

// Kelimenin dosyada kaç kere tekrar ettiğini bulmak için
int countOccurrences(const std::string& content, const std::string& keyword) {
    int count = 0;
    size_t pos = content.find(keyword);
    while (pos != std::string::npos) {
        ++count;
        pos = content.find(keyword, pos + keyword.length());
    }
    return count;
}

// Kelimenin hangi posizsyonlarda olduğunu bulur
std::vector<size_t> findPositions(const std::string& content, const std::string& keyword) {
    std::vector<size_t> positions;
    size_t pos = content.find(keyword);
    while (pos != std::string::npos) {
        positions.push_back(pos);
        pos = content.find(keyword, pos + keyword.length());
    }
    return positions;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./search <file_path> <word_to_search>\n"; 
        return 1;
    }

    std::string filepath = argv[1];
    std::string keyword = argv[2];

    try {
        std::string content = fileToString(filepath);

        int count = countOccurrences(content, keyword);
        std::vector<size_t> positions = findPositions(content, keyword);

        std::cout << "Word count " << count << "\n";
        std::cout << "Found Positions: ";
        for (size_t pos : positions) {
            std::cout << pos << " ";
        }
        std::cout << "\n";
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
