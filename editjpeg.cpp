#include "editjpeg.h"
#include <filesystem>
#include <fstream>
#include <sstream>

editJpeg::editJpeg() {}

bool editJpeg::verifyFile(std::string path) {
    std::filesystem::path filePath = path;
    if(filePath.extension() == ".jpg" || filePath.extension() == ".jpeg") {
        return true;
    }
    return false;
}

void editJpeg::findQuantTable() {
    for(size_t i = 0; i < imageHex.size(); ++i) {
        if(imageHex[i] == "ff" && imageHex[i + 1] == "db") {
            quantTablesIndex.push_back(i);
        }
    }
}

void editJpeg::importJpeg(std::string path) {
    std::ifstream file(path, std::ios::binary);
    char byte;
    while(file.get(byte)) {
        imageBytes.push_back(static_cast<uint8_t>(byte));
    }
    for(size_t i = 0; i < imageBytes.size(); ++i) {
        std::stringstream ss;
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(imageBytes[i]);
        std::string hex = ss.str();
        imageHex.push_back(hex);
    }
    findQuantTable();

    file.close();
}

std::string editJpeg::getImageHex() {
    std::string imageBytesString;
    for(size_t i = 0; i < imageHex.size(); ++i) {
        imageBytesString += imageHex[i] + " ";
    }
    return imageBytesString;
}

std::string editJpeg::getQuantTable(int index) {
    currTable = index;
    if(quantTablesIndex.size() > 0) {
        int quantIndex = quantTablesIndex[index] + 5;
        std::string quantTable;
        for(int i = quantIndex; i < quantIndex + 64; ++i) {
            quantTable += imageHex[i] + " ";
        }
        return quantTable;
    } else {
        return "ballsack";
    }
}

std::string editJpeg::getPrevTable() {
    if(currTable - 1 >= 0) {
        return getQuantTable(currTable - 1);
    }
    return "";
}

std::string editJpeg::getNextTable() {
    if(currTable + 1 < quantTablesIndex.size()) {
        return getQuantTable(currTable + 1);
    }
    return "";
}
