#ifndef EDITJPEG_H
#define EDITJPEG_H

#include <string>
#include <vector>
#include <stdint.h>

class editJpeg
{
public:
    editJpeg();
    bool verifyFile(std::string path);
    bool isStringHex(std::string str);
    void importJpeg(std::string path);

    std::string getImageHex();
    std::string getImageBytesStr();
    std::string getQuantTableStr(int index);

    std::string getColourSpaceText();

    std::string getPrevTable();
    std::string getNextTable();

    bool tableChanged(std::string table);

    std::string calculateQuantizationTable(int qFactor);

    void writeJpeg();
    void writeJpeg(std::string path);

private:
    void findQuantTable();
    std::vector<std::string>convertTable(std::string table);

    bool isTableValid(std::vector<std::string>table);
    void insertQuantTable(std::vector<std::string>table);

    void convertImageHex();

    std::string uint8ToHex(uint8_t);
    uint8_t hexToUint8(const std::string &hex);
    uint8_t hexCharToValue(char c);

    void resetData();

    int currTable;

    //image in binary
    std::vector<uint8_t>imageBytes;
    //image in hexadecimal
    std::vector<std::string>imageHex;

    std::vector<int>quantTablesIndex;

};

#endif // EDITJPEG_H
