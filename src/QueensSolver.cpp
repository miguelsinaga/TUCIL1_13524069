#include "QueensSolver.h"
#include <iostream>
#include <fstream>
#include <cstring>

using namespace std;

void strcpy_simple(const char* src, char* dest) {
    int i = 0;
    while(src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

QueensSolver::QueensSolver(char initialBoard[MAX_ROWS][MAX_COLS + 1], int r, int c) 
    : rows(r), cols(c), iterationCount(0), regionCount(0) {
    

    for(int i = 0; i < MAX_REGIONS; i++) {
        regionCellCount[i] = 0;
        regionLetters[i] = '\0';
    }
    
    for(int i = 0; i < rows; i++) {
        strcpy_simple(initialBoard[i], originalBoard[i]);
        strcpy_simple(initialBoard[i], board[i]);
        
        for(int j = 0; j < cols; j++) {
            solution[i][j] = initialBoard[i][j];
        }
        solution[i][cols] = '\0';
    }
    
    getRegions();
}

// Identifikasi semua region dan cells-nya
void QueensSolver::getRegions() {
    regionCount = 0;
    
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            char c = originalBoard[i][j];
            
            // Skip blocked cells
            if(c == '-' || c == '#') continue;
            
            // Cari apakah region sudah ada
            int regionIdx = -1;
            for(int r = 0; r < regionCount; r++) {
                if(regionLetters[r] == c) {
                    regionIdx = r;
                    break;
                }
            }
            
            // Jika region baru, tambahkan
            if(regionIdx == -1) {
                regionIdx = regionCount;
                regionLetters[regionCount] = c;
                regionCellCount[regionCount] = 0;
                regionCount++;
            }
            
            // Tambahkan cell ke region
            int cellIdx = regionCellCount[regionIdx];
            regionCells[regionIdx][cellIdx].row = i;
            regionCells[regionIdx][cellIdx].col = j;
            regionCellCount[regionIdx]++;
        }
    }
}

// Check apakah dua posisi saling attack (same row, col, atau diagonal bersentuhan)
int QueensSolver::isAttacking(int r1, int c1, int r2, int c2) {
    // Same row
    if(r1 == r2) return 1;
    
    // Same column
    if(c1 == c2) return 1;
    
    // Diagonal bersentuhan (adjacent diagonal) - untuk Queens Game
    // Dua queen saling attack jika bersentuhan diagonal (selisih row dan col = 1)
    int rowDiff = r1 - r2;
    if(rowDiff < 0) rowDiff = -rowDiff;
    int colDiff = c1 - c2;
    if(colDiff < 0) colDiff = -colDiff;
    
    // Diagonal bersentuhan: selisih row = 1 DAN selisih col = 1
    if(rowDiff == 1 && colDiff == 1) {
        return 1;
    }
    
    return 0;
}

// PURE BRUTE FORCE: Check validitas SETELAH semua queen ditempatkan
int QueensSolver::isValidSolution(char currentBoard[MAX_ROWS][MAX_COLS + 1]) {
    // Cari semua queens di board
    int queenPositions[MAX_POSITIONS][2];
    int queenCount = 0;
    
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            if(currentBoard[i][j] == '#') {
                queenPositions[queenCount][0] = i;
                queenPositions[queenCount][1] = j;
                queenCount++;
            }
        }
    }
    
    // Jumlah queen harus sama dengan jumlah region
    if(queenCount != regionCount) {
        return 0;
    }
    
    // Check per ROW - setiap row maksimal 1 queen
    int queensPerRow[MAX_ROWS] = {0};
    for(int i = 0; i < queenCount; i++) {
        int row = queenPositions[i][0];
        queensPerRow[row]++;
        if(queensPerRow[row] > 1) {
            return 0;
        }
    }
    
    // Check per COLUMN - setiap column maksimal 1 queen
    int queensPerCol[MAX_COLS] = {0};
    for(int i = 0; i < queenCount; i++) {
        int col = queenPositions[i][1];
        queensPerCol[col]++;
        if(queensPerCol[col] > 1) {
            return 0;
        }
    }
    
    // Check diagonal bersentuhan - tidak boleh ada 2 queens adjacent diagonal
    for(int i = 0; i < queenCount; i++) {
        for(int j = i + 1; j < queenCount; j++) {
            int r1 = queenPositions[i][0];
            int c1 = queenPositions[i][1];
            int r2 = queenPositions[j][0];
            int c2 = queenPositions[j][1];
            
            if(isAttacking(r1, c1, r2, c2)) {
                return 0;
            }
        }
    }
    
    return 1;
}

void QueensSolver::copyBoard(char dest[MAX_ROWS][MAX_COLS + 1], 
                             char src[MAX_ROWS][MAX_COLS + 1]) {
    for(int i = 0; i < rows; i++) {
        strcpy_simple(src[i], dest[i]);
    }
}

// PURE BRUTE FORCE - Iterate semua kombinasi placement (1 queen per region)
// Total kombinasi = product dari cells per region
int QueensSolver::solve() {
    iterationCount = 0;
    
    cout << "Jumlah region: " << regionCount << endl;
    
    // Hitung total kombinasi = product(regionCellCount[i]) untuk semua region
    long long totalCombinations = 1;
    for(int r = 0; r < regionCount; r++) {
        cout << "Region " << regionLetters[r] << ": " << regionCellCount[r] << " cells" << endl;
        totalCombinations *= regionCellCount[r];
    }
    
    cout << "\nTotal kombinasi yang akan dicek: " << totalCombinations << endl;
    cout << "\nMencari solusi..." << endl;
    
    // Array untuk track index cell yang dipilih per region
    int chosenIdx[MAX_REGIONS];
    for(int r = 0; r < regionCount; r++) {
        chosenIdx[r] = 0;
    }
    
    // PURE BRUTE FORCE: Iterate semua kombinasi
    // Mirip counting dari 0 sampai totalCombinations-1 tapi dalam "mixed radix"
    int done = 0;
    while(!done) {
        // Reset board ke original
        copyBoard(board, originalBoard);
        
        // Place 1 queen per region berdasarkan chosenIdx
        for(int r = 0; r < regionCount; r++) {
            int cellIdx = chosenIdx[r];
            int row = regionCells[r][cellIdx].row;
            int col = regionCells[r][cellIdx].col;
            board[row][col] = '#';
        }
        
        iterationCount++;
        
        // Display progress setiap 10000 iterasi
        if(iterationCount % 10000 == 0) {
            double progress = (100.0 * iterationCount) / totalCombinations;
            cout << "\r[Progress " << (int)progress << "%] Iterasi: " << iterationCount 
                 << " / " << totalCombinations << "    " << flush;
        }
        
        // Check validitas SETELAH placement lengkap (pure brute force)
        if(isValidSolution(board)) {
            copyBoard(solution, board);
            cout << "\n\n✓ SOLUSI DITEMUKAN!" << endl;
            cout << "Iterasi akhir: " << iterationCount << " / " << totalCombinations << endl;
            return 1;
        }
        
        // Increment chosenIdx (seperti counting)
        int carry = 1;
        for(int r = regionCount - 1; r >= 0 && carry; r--) {
            chosenIdx[r] += carry;
            if(chosenIdx[r] >= regionCellCount[r]) {
                chosenIdx[r] = 0;
                carry = 1;
            } else {
                carry = 0;
            }
        }
        
        // Jika semua region sudah wrap around, kita selesai
        if(carry) {
            done = 1;
        }
    }
    
    
    cout << "Iterasi akhir: " << iterationCount << " / " << totalCombinations << endl;
    return 0;
}

void QueensSolver::printBoard() {
    cout << "\n--- SOLUSI (# = QUEEN) ---" << endl;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            cout << solution[i][j] << " ";
        }
        cout << endl;
    }
}

long long QueensSolver::getIterationCount() const {
    return iterationCount;
}

void QueensSolver::saveSolution(const char* filename) {
    ofstream file(filename);
    if(!file.is_open()) {
        cerr << "Error: Tidak dapat membuat file output!" << endl;
        return;
    }
    
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            file << solution[i][j] << " ";
        }
        file << endl;
    }
    file.close();
}
