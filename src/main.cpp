#include <iostream>
#include <cstdlib>
#include <string>
#include <set>
#include <time.h>
#include <chrono>
#include <fstream>
using namespace std;

//Memudahkan operasi perhitungan sekaligus pengganti eval dalam python
double operation(double num1, double num2,char oprt) {
    if (oprt == '+') {
        return num1 + num2;
    }
    if (oprt == '-') {
        return num1 - num2;
    }
    if (oprt == '*') {
        return num1*num2;
    }
    if (oprt == '/') {
        return num1/num2;
    }
    else return 0;
}

//Memvalidasi input user sesuai dengan kartu yang ada
bool validateInput(string a, string b, string c, string d) {
    string card[4] = {a, b, c, d};
    string possibleCard[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    bool inpvalid;
    
    for(int i = 0; i < 4; i++) {
        inpvalid = false;
        for(int j = 0; j < 13; j++) {
            if (card[i] == possibleCard[j]) {
                inpvalid = true;
                break;
            }
        }
        if (!inpvalid) {
            break;
        }
    }

    return inpvalid;
}

//Mengubah kartu menjadi angka, diperlukan untuk operasi perhitungan
int convertCardToInt(string card) {
    if (card == "A") {
        return 1;
    }
    if (card == "J") {
        return 11;
    }
    if (card == "Q") {
        return 12;
    }
    if (card == "K") {
        return 13;
    }
    else {
        return stoi(card);
    }
}

//Mengubah angka menjadi kartu, digunakan dalam menunjukkan kartu yang dihasilkan secara random
string convertIntToCard(int number) {
    if (number == 1) {
        return "A";
    }
    if (number == 11) {
        return "J";
    }
    if (number == 12) {
        return "Q";
    }
    if (number == 13) {
        return "K";
    }
    else {
        return to_string(number);
    }
}

//Handle rounding error, khususnya dalam operasi pembagian
bool compareResult(double result) {
    return (result >= (24 - 0.0001) && result <= (24 + 0.0001));
}

int main() {
    string rndm, card1, card2, card3, card4;
    int intCard[4];
    double doubleCard[4];

    //Memastikan jenis input
    cout << "\n\n\nSelamat Datang di 24 Game Solver\n\n\n";
    cout << "Apakah kartu dirandom atau tidak?(y/n): ";
    cin >> rndm;
    while (rndm != "y" && rndm != "n") { //Error handling untuk input di luar opsi 
        cout << "\nApakah kartu dirandom atau tidak?(y/n): ";
        cin >> rndm;
    }
    if (rndm == "y") {  //Generate kartu secara random
        srand(time(0));
        for (int rn = 0; rn < 4; rn++) {
            intCard[rn] = (rand() % 13) + 1;
        }
        card1 = convertIntToCard(intCard[0]);
        card2 = convertIntToCard(intCard[1]);
        card3 = convertIntToCard(intCard[2]);
        card4 = convertIntToCard(intCard[3]);
    }
    else {              //Generate kartu dari input user
        bool found;
        cout << "Input 4 kartu: ";
        cin >> card1 >> card2 >> card3 >> card4;
        while (validateInput(card1, card2, card3, card4) != 1) {    //Error handling bila kartu
            cout << "\nInput tidak sesuai. Ulangi masukan kembali: ";
            cin >> card1 >> card2 >> card3 >> card4;
        }
        intCard[0] = convertCardToInt(card1);
        intCard[1] = convertCardToInt(card2);
        intCard[2] = convertCardToInt(card3);
        intCard[3] = convertCardToInt(card4);
    }

    cout << "Kartu anda adalah " << card1 << " " << card2 << " " << card3 << " " << card4 << "\n\n";

    auto begin = chrono::high_resolution_clock::now();

    doubleCard[0] = intCard[0];
    doubleCard[1] = intCard[1];
    doubleCard[2] = intCard[2];
    doubleCard[3] = intCard[3];

    set<string> solution;   //Menggunakan set agar tidak ada solusi duplikat


    char oprt[4] = {'+', '-', '*', '/'};
    for (int i = 0; i < 4; i++) {   //Loop untuk mencari semua permutasi yang mungkin
        for (int j = 0; j < 4; j++) {
            if (i != j) {
                for (int k = 0; k < 4; k++) {
                    if (i != k && j != k) {
                        for (int l = 0; l < 4; l++) {
                            if (i != l && j != l && k != l) {

                                string a = to_string(intCard[i]);
                                string b = to_string(intCard[j]);
                                string c = to_string(intCard[k]);
                                string d = to_string(intCard[l]);

                                double v = doubleCard[i];
                                double w = doubleCard[j];
                                double x = doubleCard[k];
                                double y = doubleCard[l];

                                for (int p = 0; p < 4; p++) {   //Loop untuk menentukan operator yang digunakan
                                    for (int q = 0; q < 4; q++) {
                                        for (int r = 0; r < 4; r++) {
                                            
                                            //(a b) (c d)
                                            if (compareResult(operation(operation(v, w, oprt[p]),operation(x, y, oprt[r]), oprt[q]))) {
                                                solution.insert("("+ a + oprt[p] + b + ")" + oprt[q] + "(" + c + oprt[r] + d + ")");
                                            }
                                            //((a b) c) d
                                            if (compareResult(operation(operation(operation(v, w, oprt[p]), x, oprt[q]), y, oprt[r]))) {
                                                solution.insert("(("+ a + oprt[p] + b + ")" + oprt[q] + c + ")" + oprt[r] + d);
                                            }
                                            //(a (b c)) d
                                            if (compareResult(operation(operation(v, operation(w, x, oprt[q]), oprt[p]), y, oprt[r]))) {
                                                solution.insert("("+ a + oprt[p] + "(" + b + oprt[q] + c + "))" + oprt[r] + d);
                                            }
                                            //a ((b c) d)
                                            if (compareResult(operation(v, operation(operation(w, x, oprt[q]), y, oprt[r]), oprt[p]))) {
                                                solution.insert(a + oprt[p] + "((" + b + oprt[q] + c + ")" + oprt[r] + d + ")");
                                            }
                                            //a (b (c d))
                                            if (compareResult(operation(v, operation(w, operation(x, y, oprt[r]), oprt[q]), oprt[p]))) {
                                                solution.insert(a + oprt[p] + "(" + b + oprt[q] + "(" + c + oprt[r] + d + "))");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end-begin);

    //Menampilkan banyak solusi dan solusi yang mungkin
    int nSolution = solution.size();
    if (nSolution == 0) {
        cout << "Tidak ada solusi yang mungkin";
    }
    else {
        cout << "Terdapat " << nSolution << " solusi";
    }
    for (auto it = solution.begin(); it != solution.end(); ++it)
        cout << '\n' << *it;

    //Membuat file baru untuk menyimpan solusi
    string save;
    cout << "\n\nApakah ingin menyimpan solusi pada file baru?(y/n): ";
    cin >> save;
    while (save != "y" && save != "n") {
        cout << "\nApakah ingin menyimpan solusi pada file baru?(y/n): ";
        cin >> save;
    }

    if (save == "y") {
        string filename;
        cout << "Masukkan nama file yang ingin dibuat(tanpa .txt): ";
        cin >> filename;

        fstream newfile;
        newfile.open(filename + ".txt", ios::out);
        if (!newfile) {
            cout << "File baru gagal dibuat";
        }
        else {
            cout << "File baru berhasil dibuat";
            if (nSolution == 0) {
                newfile << "Tidak ada solusi yang mungkin";
            }
            else {
                newfile << "Terdapat " << nSolution << " solusi";
            } 
            for (auto it = solution.begin(); it != solution.end(); ++it)
                newfile << '\n' << *it;
        }
        newfile.close();
    }
    
    cout << "\n\nWaktu eksekusi: " << duration.count() << "ms\n\n";

    cout << "\nTerima kasih telah memakai program ini. Sampai jumpa\n\n\n";
    
    return 0;
}