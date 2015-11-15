#define SUCCESS 0
#define FILE_NOT_FOUND 1
#define KEY_FILE_NOT_FOUND 2
#define INPUT_FILE_NOT_FOUND 3
#define DISTANCE_NOT_SATISFIED 4

char *encode(const char *inputMessageFile, const char *keyFile, int *status, int distance);
char *decode(const char *inputCodeFile, const char *keyFile, int *status);
char **hack(const char *inputCodeFile, int *status, int *size);