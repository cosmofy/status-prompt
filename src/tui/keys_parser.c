#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "keys_parser.h"

#define MAX_KEYS 10 // Adjust as needed
#define ARRAY_SIZE 100

void parseLine(char *line, KeyData *data) {
    // Extract 'name' field
    char *nameStart = strstr(line, "'name': '") + 9;
    char *nameEnd = strchr(nameStart, '\'');
    if (nameStart && nameEnd) {
        size_t nameLength = nameEnd - nameStart;
        char name[100]; // Temporary buffer for the full name
        strncpy(name, nameStart, nameLength);
        name[nameLength] = '\0';

        // Split 'name' into location and date
        char *dateStart = strrchr(name, ' ') - 9;
        if (dateStart) {
            strncpy(data->date, dateStart + 1, 18);
            data->date[17] = '\0';
            size_t locationLength = dateStart - name;
            strncpy(data->location, name, locationLength);
            data->location[locationLength] = '\0';
        }
    }

    // Extract 'keyId' field
    char *keyIdStart = strstr(line, "'keyId': '") + 10;
    // char *keyIdEnd = strchr(keyIdStart, ',');
    char *firstStar = strchr(keyIdStart, '*');
    if (keyIdStart && firstStar) {
        size_t keyIdLength = firstStar - keyIdStart;
        strncpy(data->keyId, keyIdStart, keyIdLength);
        data->keyId[keyIdLength] = '\0';
    }

    // Extract 'key' field
    char *keyStart = strstr(line, "'key': '") + 8;
    // char *keyEnd = strchr(keyStart, ',');
    char *firstStarNew = strchr(keyStart, '*'); 
    if (keyStart && firstStarNew) {
        size_t keyLength = firstStarNew - keyStart;
        strncpy(data->key, keyStart, keyLength);
        data->key[keyLength] = '\0';
    }
    
    // Extract 'revoked' field
    char *revokedStart = strstr(line, "'revoked': ");
    if (revokedStart) {
        revokedStart += 11; // Skip "'revoked': "
        if (strncmp(revokedStart, "True", 4) == 0) {
            data->revoked = 'T';
        } else if (strncmp(revokedStart, "False", 5) == 0) {
            data->revoked = 'F';
        }
    }

    // Extract 'costLimitInUsdOverTime' field
    char *costStart = strstr(line, "'costLimitInUsdOverTime': ");
    if (costStart) {
        costStart += strlen("'costLimitInUsdOverTime': ");
        data->costLimitInUsdOverTime = strtod(costStart, NULL);
    }
}

KeyData* parserFile(char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return NULL;
    }

    KeyData *dataArray = malloc(sizeof(KeyData)*MAX_KEYS);
    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), file) && count < MAX_KEYS) {
        parseLine(line, &dataArray[count]);
        count++;
    }
    fclose(file);
    
    // left out for now.
    // for (int i = 0; i < count; i++) {
    //     printf("Location: %s, Date: %s, KeyId: %s, Revoked: %c, Key: %s, Cost Limit: %f\n",
    //             dataArray[i].location, dataArray[i].date, dataArray[i].keyId, dataArray[i].revoked, dataArray[i].key, dataArray[i].costLimitInUsdOverTime);
    // }

    return dataArray;
}

int countLine(char* filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Failed to open file");
        return 0;
    }
    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), file) && count < MAX_KEYS) {
        count++;
    }
    return count;
}

// void main() {
//     parserFile("keys_status.txt");
// }