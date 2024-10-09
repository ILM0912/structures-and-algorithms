#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
using namespace std;

struct Record {
    int key = 0;
    string surname;
    string phone;
};

void createBinaryFile(const string& filename, int numRecords) {
    ofstream file(filename, ios::binary);

    if (!file) {
        cout << "Error opening file for writing!" << endl;
        return;
    }

    vector<int> keys(numRecords);
    for (int i = 0; i < numRecords; i++) {
        keys[i] = i;
    }

    random_device rd;
    default_random_engine rng(rd());
    shuffle(keys.begin(), keys.end(), rng);

    for (int i = 0; i < numRecords; i++) {
        Record record;
        record.key= i;
        int cnt = 9 - to_string(keys[i]).length();
        record.phone = "+79" + string(cnt, '0') + to_string(keys[i]);
        record.surname = "Surname_" + to_string(keys[i]);
        //cout << record.key << "\t" << record.surname << "\t" << record.phone << endl;
        file.write(reinterpret_cast<char*>(&record), sizeof(record));
    }
    file.close();
}

void linearSearch(const string& filename, int key) {
    auto start = clock();
    ifstream file(filename, ios::binary);
    Record record;

    if (!file) {
        cout << "Error opening file for writing!" << endl;
        exit(-1);
    }
    int result = -1;
    while (file.read(reinterpret_cast<char*>(&record), sizeof(record))) {
        if (record.key == key) {
            result = 0;
            cout << "Found record with key: " << record.key << " --- " << record.phone << " --- " << record.surname << endl;
            auto finish = clock();
            auto duration = (double)(finish - start) / CLOCKS_PER_SEC;
            cout << "\nTime: " << duration << " seconds";
            file.close();
            exit(0);
        }
    }
    if (result == -1) {
        cout << "No such element";
    }
    auto finish = clock();
    auto duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "\nTime: " << duration << " seconds";
    file.close();
    exit(0);
}

struct IndexPosition {
    int key;
    streampos offset;
};

void interpolationSearch(const string& filename, int n, int x) {
    vector<IndexPosition> index(n);
    ifstream inFile(filename, ios::binary);
    if (!inFile) {
        cout << "Error opening file for reading!" << endl;
        exit(-1);
    }
    Record record;
    streampos position;
    streampos size = sizeof(Record);
    int i = 0;
    while (inFile.read(reinterpret_cast<char*>(&record), sizeof(Record))) {
        position = inFile.tellg();
        index[i] = { record.key, position - size };
        i += 1;
        if (i == n) {
            inFile.close();
        }
    }



    auto start = clock();
    int result = -1;
    int low = 0, high = n - 1;
    while (low <= high && x >= index[low].key && x <= index[high].key) {
        if (index[high].key == index[low].key) {
            if (index[low].key == x) result = low; break;
        }
        int pos = low + ((double)(high - low) * (x - index[low].key) / (index[high].key - index[low].key));
        if (index[pos].key == x) {
            result = pos;
        }
        if (index[pos].key < x) {
            low = pos + 1;
        }
        else {
            high = pos - 1;
        }
    }



    if (result != -1) {
        inFile.open(filename, ios::binary);
        if (!inFile) {
            cout << "Error opening file for reading!" << endl;
            exit(-1);
        }

        inFile.seekg(index[result].offset, ios::beg);
        inFile.read(reinterpret_cast<char*>(&record), size);
        cout << "Found record with key: " << record.key << " --- " << record.phone << " --- " << record.surname << endl;

        inFile.close();
    }
    else {
        cout << "No such element";
    }
    auto finish = clock();
    auto duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Time: " << duration << " seconds";
    exit(0);
}



int main()
{
    int n = 100000;
    createBinaryFile("file.bin", n);
    //linearSearch("file.bin", n-1);
    interpolationSearch("file.bin", n, 91222);
}