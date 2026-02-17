#ifndef BOARD_VALIDATOR_H
#define BOARD_VALIDATOR_H

#define MAX_ROWS 20
#define MAX_COLS 20

class BoardValidator {
public:

    int isValid(char board[MAX_ROWS][MAX_COLS + 1], int rows, int cols, 
                char* errorMsg);
    
private:

    int hasConsistentDimensions(char board[MAX_ROWS][MAX_COLS + 1], 
                                 int rows, int cols, char* errorMsg);
    

    int hasValidSize(int rows, int cols, char* errorMsg);
    

    int hasValidCharacters(char board[MAX_ROWS][MAX_COLS + 1], 
                          int rows, int cols, char* errorMsg);
};

#endif
