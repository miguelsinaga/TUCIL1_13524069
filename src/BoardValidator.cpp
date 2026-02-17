#include "BoardValidator.h"

void strcpy_validator(char* dest, const char* src) {
    int i = 0;
    while(src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int strlen_validator(const char* str) {
    int len = 0;
    while(str[len] != '\0') {
        len++;
    }
    return len;
}

int is_upper_alpha(char c) {
    return (c >= 'A' && c <= 'Z');
}

int BoardValidator::hasConsistentDimensions(char board[MAX_ROWS][MAX_COLS + 1], 
                                             int rows, int cols, 
                                             char* errorMsg) {
    if(rows == 0) {
        strcpy_validator(errorMsg, "Board kosong!");
        return 0;
    }
    
    for(int i = 0; i < rows; i++) {
        int len = strlen_validator(board[i]);
        if(len != cols) {
            strcpy_validator(errorMsg, "Semua baris harus memiliki panjang yang sama!");
            return 0;
        }
    }
    
    return 1;
}

int BoardValidator::hasValidSize(int rows, int cols, char* errorMsg) {

    if(rows < 4 || cols < 4) {
        strcpy_validator(errorMsg, "Ukuran board minimal 4x4!");
        return 0;
    }

    if(rows > 20 || cols > 20) {
        strcpy_validator(errorMsg, "Ukuran board maksimal 20x20!");
        return 0;
    }
    
    return 1;
}

int BoardValidator::hasValidCharacters(char board[MAX_ROWS][MAX_COLS + 1], 
                                       int rows, int cols, 
                                       char* errorMsg) {
    for(int i = 0; i < rows; i++) {
        int len = strlen_validator(board[i]);
        for(int j = 0; j < len; j++) {
            char c = board[i][j];
            if(!is_upper_alpha(c)) {
                if(c != '#' && c != '-') {
                    strcpy_validator(errorMsg, "Board hanya boleh berisi huruf besar (A-Z), #, atau -!");
                    return 0;
                }
            }
        }
    }
    
    return 1;
}

int BoardValidator::isValid(char board[MAX_ROWS][MAX_COLS + 1], int rows, int cols, 
                             char* errorMsg) {
    if(!hasConsistentDimensions(board, rows, cols, errorMsg)) {
        return 0;
    }
    
    if(!hasValidSize(rows, cols, errorMsg)) {
        return 0;
    }
    
    if(!hasValidCharacters(board, rows, cols, errorMsg)) {
        return 0;
    }
    
    return 1;
}
