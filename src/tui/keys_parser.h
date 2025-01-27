#ifndef KEYS_HEADER__
#define KEYS_HEADER__
typedef struct {
    char location[50];
    char date[18]; // Date in YYYYMMDD format
    char keyId[50];
    char revoked;
    char key[50];
    double costLimitInUsdOverTime;
} KeyData;

void parseLine(char *line, KeyData *data);
KeyData* parserFile(char* filename);
int countLine(char* filename);

#endif