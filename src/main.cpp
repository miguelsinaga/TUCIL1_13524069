#include <iostream>
#include <fstream>
#include <cstring>
#include <ctime>
#include "QueensSolver.h"
#include "BoardValidator.h"

using namespace std;

#define MAX_ROWS 20
#define MAX_COLS 20
#define MAX_FILENAME 256

// Get current time in milliseconds
long long getCurrentTimeMs() {
    return (long long)(clock() * 1000.0 / CLOCKS_PER_SEC);
}

int main() {
    char filepath[MAX_FILENAME];
    
    cout << "-------QUEENS GAME SOLVER-------" << endl;
    cout << "Masukkan nama file (default: test/test1.txt): ";
    cin.getline(filepath, MAX_FILENAME);
    
    if(filepath[0] == '\0') {
        strcpy(filepath, "test/test1.txt");
    }
    
    cout << "Membaca file: " << filepath << endl << endl;
    

    ifstream file(filepath);
    if(!file.is_open()) {
        cerr << "Error: Tidak dapat membuka file " << filepath << endl;
        return 1;
    }
    
    char board[MAX_ROWS][MAX_COLS + 1];
    string line;
    int rows = 0;
    int cols = 0;
    

    while(getline(file, line) && rows < MAX_ROWS) {
        // Strip carriage return jika ada (Windows line endings)
        if(!line.empty() && line[line.length()-1] == '\r') {
            line = line.substr(0, line.length()-1);
        }
        
        if(line.length() > 0 && line.length() <= MAX_COLS) {
            for(size_t j = 0; j < line.length(); j++) {
                board[rows][j] = line[j];
            }
            board[rows][line.length()] = '\0';
            
            if(cols == 0) cols = line.length();
            rows++;
        }
    }
    file.close();
    
    BoardValidator validator;
    char validationMsg[256];
    if(!validator.isValid(board, rows, cols, validationMsg)) {
        cerr << "Error: Input tidak valid!" << endl;
        cerr << validationMsg << endl;
        return 1;
    }
    
    cout << "Parsing OK! Ukuran: " << rows << "x" << cols << endl;
    cout << "\n-------PAPAN AWAL (REGION)-------" << endl;
    for(int i = 0; i < rows; i++) {
        for(int j = 0; j < cols; j++) {
            cout << board[i][j] << " ";
        }
        cout << endl;
    }
    
    QueensSolver solver(board, rows, cols);
    
    cout << "\nMemulai pencarian solusi...\n" << endl;
    
    long long start = getCurrentTimeMs();
    
    int found = solver.solve();
    
    long long end = getCurrentTimeMs();
    long long durationMs = end - start;
    
    if(found) {
        cout << "\n-------SOLUSI DITEMUKAN------- " << endl;
        solver.printBoard();
        
        cout << "\nWaktu pencarian: " << durationMs << " ms" << endl;
        cout << "Banyak kasus yang ditinjau: " << solver.getIterationCount() << " kasus" << endl;
        
        cout << "\nApakah Anda ingin menyimpan solusi? (Ya/Tidak): ";
        string ans;
        cin >> ans;
        
        if(ans == "Ya" || ans == "ya" || ans == "Y" || ans == "y") {
            solver.saveSolution("output_solution.txt");
            cout << "Solusi disimpan ke output_solution.txt" << endl;
        }
    } else {
        cout << "\n=-------TIDAK ADA SOLUSI------" << endl;
        cout << "Waktu pencarian: " << durationMs << " ms" << endl;
        cout << "Banyak kasus yang ditinjau: " << solver.getIterationCount() << " kasus" << endl;
    }
    
    return 0;
}
