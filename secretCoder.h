#define SUCCESS 0
#define FILE_NOT_FOUND 1
#define KEY_FILE_NOT_FOUND 2
#define INPUT_FILE_NOT_FOUND 3
#define DISTANCE_NOT_SATISFIED 4

/**
 * inputMessageFile is the name of the file with the message the user wants to encode.
 * keyFile is the name of the file with the key the user wishes to encrypt his encoded message with.
 * status is a pointer to a status variable. Use accordingly with values in #define.
 * distance is the distance between adjacent codes.
 *
 * The return value should be the encoded message.
 * Return NULL if message can not be encoded.
 */
char *encode(const char *inputMessageFile, const char *keyFile, int *status, int distance);

/**
 * inputCodeFile is the name of a file containing a encoded message the user wishes to decode.
 * keyFile is the name of a file used to encode the hidden message.
 * status is a pointer to a status variable. Use accordingly with values in #define.
 *
 * The return value should be the the decoded message.
 * Return NULL if message cannot be decoded
 */
char *decode(const char *inputCodeFile, const char *keyFile, int *status);

/**
 * inputCodeFile is the name of a file containing a encoded message the user wishes to hack.
 * status is a pointer to a status variable. Use accordingly with values in #define.
 * size is the amount of items in the return value.
 *
 * Return a double array containing the decoded messages.
 * Return NULL if there's no decoded messages.
 */
char **hack(const char *inputCodeFile, int *status, int *size);