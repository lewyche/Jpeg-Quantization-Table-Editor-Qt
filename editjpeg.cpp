#include "editjpeg.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cctype>

editJpeg::editJpeg() {}

//check if file has jpeg file extension
bool editJpeg::verifyFile(std::string path) {
    std::filesystem::path filePath = path;
    if(filePath.extension() == ".jpg" || filePath.extension() == ".jpeg") {
        return true;
    }
    return false;
}

//find index of quant tables in imageHex
void editJpeg::findQuantTable() {
    for(size_t i = 0; i < imageHex.size(); ++i) {
        //ffdb means the start of a quantization table
        //https://en.wikipedia.org/wiki/JPEG#Syntax_and_structure
        if(imageHex[i] == "ff" && imageHex[i + 1] == "db") {
            quantTablesIndex.push_back(i);
        }
    }
}

void editJpeg::resetData() {
    imageHex.clear();
    imageBytes.clear();
    quantTablesIndex.clear();
}

void editJpeg::importJpeg(std::string path) {
    resetData();

    std::ifstream file(path, std::ios::binary);
    char byte;
    //get file binary
    while(file.get(byte)) {
        imageBytes.push_back(static_cast<uint8_t>(byte));
    }
    //convert to hexadecimal
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
    std::string imageHexString;
    for(size_t i = 0; i < imageHex.size(); ++i) {
        imageHexString += imageHex[i] + " ";
    }
    return imageHexString;
}

std::string editJpeg::getImageBytesStr() {
    std::string imageBytesString;
    for(size_t i = 0; i < imageBytes.size(); ++i) {
        imageBytesString += std::to_string(imageBytes[i]) + " ";
    }
    return imageBytesString;
}

std::string editJpeg::getQuantTableStr(int index) {
    currTable = index;
    if(quantTablesIndex.size() > 0) {
        //skip quant table markers
        //https://en.wikipedia.org/wiki/JPEG#Syntax_and_structure
        int quantIndex = quantTablesIndex[index] + 5;
        std::string quantTable;
        for(int i = quantIndex; i < quantIndex + 64; ++i) {
            quantTable += imageHex[i] + " ";
        }
        return quantTable;
    } else {
        return "";
    }
}

std::string editJpeg::getPrevTable() {
    if(currTable - 1 >= 0) {
        return getQuantTableStr(currTable - 1);
    }
    return "";
}

std::string editJpeg::getNextTable() {
    if(currTable + 1 < quantTablesIndex.size()) {
        return getQuantTableStr(currTable + 1);
    }
    return "";
}

bool editJpeg::isTableValid(std::vector<std::string> table) {
    std::string validChars = "0123456789abcdefABCDEF";

    if(table.size() != 64) {
        return false;
    }

    for(size_t i = 0; i < table.size(); ++i) {
        if(table[i].size() != 2 || table[i].find_first_not_of(validChars) != std::string::npos) {
            return false;
        }
    }
    return true;
}

//tokenize table into vector
std::vector<std::string> editJpeg::convertTable(std::string table) {
    std::string tmp;
    std::stringstream ss(table);
    std::vector<std::string> newTable;
    while(std::getline(ss, tmp, ' ')) {
        newTable.push_back(tmp);
    }
    return newTable;
}

//insert table into imageHex
void editJpeg::insertQuantTable(std::vector<std::string> table) {
    int index = quantTablesIndex[currTable] + 5;
    for(int i = index; i < index + 64; ++i) {
        imageHex[i] = table[i - index];
    }
}

uint8_t editJpeg::hexCharToValue(char c) {
    c = std::toupper(c);
    if (c >= '0' && c <= '9') {
        return c - '0';
    } else if (c >= 'A' && c <= 'F') {
        return 10 + (c - 'A');
    }
}

uint8_t editJpeg::hexToUint8(const std::string &hex) {
    uint8_t high = hexCharToValue(hex[0]);
    uint8_t low = hexCharToValue(hex[1]);
    return (high << 4) | low;
}

//convert imageHex to binary and copy it onto imageBytes
void editJpeg::convertImageHex() {
    for(int i = 0; i < imageHex.size(); ++i) {

        imageBytes[i] = hexToUint8(imageHex[i]);

    }
}

//return whether table was successfully changed
bool editJpeg::tableChanged(std::string table) {
    std::vector<std::string>newTable = convertTable(table);
    if(isTableValid(newTable)) {
        insertQuantTable(newTable);
        convertImageHex();
        return true;
    } else {
        return false;
    }
}

void editJpeg::writeJpeg() {
    //C-Style write in binary
    FILE* fil = fopen("test.jpg", "wb");
    fwrite(imageBytes.data(), sizeof(uint8_t),imageBytes.size(), fil);
    fclose(fil);
}
