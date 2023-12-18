#include "location.h"


void saveLocation(const Location *location) {
    FILE *configFile = fopen("config.json", "a");
    if (configFile == NULL) {
        perror("Error opening config file");
        exit(EXIT_FAILURE);
    }

    // Move to the end of "locations": [
    fseek(configFile, 0, SEEK_SET);
    while (1) {
        if (fgetc(configFile) == ']') {
            // Move to the position just before the closing square bracket ']'
            fseek(configFile, -1, SEEK_CUR);
            break;
        }
        if (feof(configFile)) {
            perror("Error reading config file");
            fclose(configFile);
            exit(EXIT_FAILURE);
        }
    }

    fprintf(configFile, ",\n    {\"name\": \"%s\"}", location->name);
    // fprintf(configFile, "\n]}");

    fclose(configFile);
}
const char* createLocation(char* name) {
    Location newLocation;

    newLocation.name = name;

    saveLocation(&newLocation);

    char* response = (char*)malloc(strlen(name) + 25);  // Allocate enough space
    sprintf(response, "Location '%s' created.", name);

    return response;
}



