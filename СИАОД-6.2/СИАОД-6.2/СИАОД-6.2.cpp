#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <chrono>

using namespace std;

string generateRandomString(int length) {
    const char chars[] = { 'a', 'b', 'c', 'd' };
    string result;
    srand(time(0));
    for (int i = 0; i < length; ++i) {
        int index = rand() % (sizeof(chars) / sizeof(chars[0]));
        result += chars[index];
    }
    return result;
}

void bubbleSort(vector<string>& words) {
    size_t n = words.size();
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (words[j] > words[j + 1]) {
                swap(words[j], words[j + 1]);
            }
        }
    }
}

array<int, 256> createShiftTable(string image) {
    array<int, 256> shiftTable;
    shiftTable.fill(image.length());
    for (int i = 2; i <= image.length(); i++) {
        unsigned char letter = image[image.length() - i];
        if (shiftTable[letter] == image.length()) {
            shiftTable[letter] = i - 1;
        }
    }
    cout << endl;
    for (int i = 0; i < image.length(); i++) {
        unsigned char letter = image[i];
    }
    return shiftTable;
}

int search(array<int, 256> shiftTable, string text, string image) {
    int count = 0;
    int imageLength = image.length();
    int textLength = text.length();
    int index = imageLength - 1;
    if (textLength < imageLength) return count;
    while (index < textLength) {
        int j = imageLength - 1;
        while (j >= 0 && text[index - (imageLength - 1 - j)] == image[j]) {
            j--;
        }
        if (j < 0) {
            count++;
            index += (index < textLength - 1) ? shiftTable[text[index]] : 1;
        }
        else {
            index += shiftTable[text[index]];
        }
    }
    return count;
}

void task_1() {
    vector<string> keywords = {
        "int", "float", "if", "else", "while", "for", "return",
        "void", "class", "public", "private", "protected"
    };
    bubbleSort(keywords);
    cout << "Sorted keywords: " << endl;
    for (string word : keywords) {
        cout << word << endl;
    }
}

void task_2() {
    vector<string> images = { "ab" };
    cout << "Тестирование при тексте длиной 100";
    string text = generateRandomString(100);
    auto start = chrono::high_resolution_clock::now();
    for (string image : images) {
        array<int, 256> shiftTable = createShiftTable(image);
        cout << image << ": " << search(shiftTable, text, image);
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> duration = end - start;
    cout << "\nВремя сортировки: " << duration.count() << " мc";

    cout << "\n\nТестирование при тексте длиной 1000";
    text = generateRandomString(1000);
    start = chrono::high_resolution_clock::now();
    for (string image : images) {
        array<int, 256> shiftTable = createShiftTable(image);
        cout << image << ": " << search(shiftTable, text, image);
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "\nВремя сортировки: " << duration.count() << " мc";

    cout << "\n\nТестирование при тексте длиной 10000";
    text = generateRandomString(10000);
    start = chrono::high_resolution_clock::now();
    for (string image : images) {
        array<int, 256> shiftTable = createShiftTable(image);
        cout << image << ": " << search(shiftTable, text, image);
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "\nВремя сортировки: " << duration.count() << " мc";

    cout << "\n\nТестирование при тексте длиной 100000";
    text = generateRandomString(100000);
    start = chrono::high_resolution_clock::now();
    for (string image : images) {
        array<int, 256> shiftTable = createShiftTable(image);
        cout << image << ": " << search(shiftTable, text, image);
    }
    end = chrono::high_resolution_clock::now();
    duration = end - start;
    cout << "\nВремя сортировки: " << duration.count() << " мc";
}

int main() {
    setlocale(LC_ALL, "ru");
    task_2();
}