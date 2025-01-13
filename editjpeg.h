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
    void importJpeg(std::string path);
    std::string getImageHex();
    std::string getQuantTable(int index);

    std::string getPrevTable();
    std::string getNextTable();

private:
    void findQuantTable();

    int currTable;

    std::vector<uint8_t>imageBytes;
    std::vector<std::string>imageHex;

    std::vector<int>quantTablesIndex;

};

#endif // EDITJPEG_H
