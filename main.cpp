#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// Improved hash function using a polynomial rolling hash
int HashString(const string& value) {
    const int p = 31;  // Prime number
    const int m = 1e9 + 9;  // Large prime for modulo
    long long hashCode = 0;
    long long p_pow = 1;

    for (char character : value) {
        hashCode = (hashCode + (character - 'a' + 1) * p_pow) % m;
        p_pow = (p_pow * p) % m;
    }
    return (int)(hashCode & 0x7fffffff);
}

int main() {
    unordered_map<int, vector<string>> passwordTable; // Handles collisions using a vector
    ifstream passwordFile("10-million-password-list-top-10000.txt"); // Replace with actual file
    
    if (!passwordFile) {
        cerr << "Error: Password file not found!" << endl;
        return 1;
    }

    string password;
    int collisionCount = 0;
    unordered_map<int, int> hashFrequency;

    cout << "Reading passwords and hashing...\n";
    while (getline(passwordFile, password)) {
        int hashValue = HashString(password);
        
        if (!passwordTable[hashValue].empty()) {
            collisionCount++; // Increment if a collision occurs
        }
        passwordTable[hashValue].push_back(password);
        hashFrequency[hashValue]++;
    }
    passwordFile.close();

    cout << "Hashing complete. Total collisions: " << collisionCount << "\n";
    
    cout << "Enter a hashed password to find the original: ";
    
    int userHash;
    while (true) {
        cout << "\nEnter hashed value (or -1 to exit): ";
        cin >> userHash;

        if (userHash == -1) break;

        if (passwordTable.find(userHash) != passwordTable.end()) {
            cout << "Possible original passwords:\n";
            for (const string& pass : passwordTable[userHash]) {
                cout << " - " << pass << endl;
            }
        } else {
            cout << "Password not found in database." << endl;
        }
    }

    cout << "Goodbye!" << endl;
    return 0;
}
