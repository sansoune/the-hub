#define MAX_CAPABILITIES 1000
#define MAX_LOCATION 1000

typedef void (*ModuleFunction)(void);

struct Module {
    int id;
    char capabilities[MAX_CAPABILITIES];
    char location[MAX_LOCATION];
    ModuleFunction functionality;
    // Add other necessary fields
};

void createModule();