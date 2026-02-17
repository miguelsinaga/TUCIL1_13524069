#ifndef QUEENS_SOLVER_H
#define QUEENS_SOLVER_H

#define MAX_ROWS 20
#define MAX_COLS 20
#define MAX_POSITIONS 400
#define MAX_REGIONS 26

class QueensSolver {
private:
    char board[MAX_ROWS][MAX_COLS + 1];
    char originalBoard[MAX_ROWS][MAX_COLS + 1];
    char solution[MAX_ROWS][MAX_COLS + 1];
    int rows;
    int cols;
    long long iterationCount;
    
    struct Position {
        int row;
        int col;
    };
    
    // Untuk setiap region, simpan cells yang termasuk region itu
    Position regionCells[MAX_REGIONS][MAX_POSITIONS];
    int regionCellCount[MAX_REGIONS];   // jumlah cells per region
    char regionLetters[MAX_REGIONS];    // huruf region (A, B, C, ...)
    int regionCount;                    // jumlah unique regions
    
    // Helper methods
    void getRegions();
    
    // Pure brute force: check valid solution SETELAH placement lengkap
    int isValidSolution(char currentBoard[MAX_ROWS][MAX_COLS + 1]);
    
    // Check apakah ada 2 queens saling attack
    int isAttacking(int r1, int c1, int r2, int c2);
    
    // Copy board
    void copyBoard(char dest[MAX_ROWS][MAX_COLS + 1], 
                   char src[MAX_ROWS][MAX_COLS + 1]);
    
public:
    QueensSolver(char initialBoard[MAX_ROWS][MAX_COLS + 1], int r, int c);
    
    // Pure brute force - iterate semua kombinasi (1 queen per region)
    int solve();
    
    void printBoard();
    long long getIterationCount() const;
    void saveSolution(const char* filename);
};

#endif
