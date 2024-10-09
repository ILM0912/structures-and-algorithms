#include <iostream>
#include <Windows.h>
#include <bitset>
#include <fstream>
using namespace std;


void task1_1() {
    int x;
    cout << "Введите значение исходного числа: ";
    cin >> x;
    unsigned char maska = 1;
    x = x & (~(maska << 4));
    cout << "Ответ: " << x << endl;
}

void task1_2() {
    int x;
    cout << "Введите значение исходного числа: ";
    cin >> x;
    unsigned char maska = 1;
    x = x | (maska<<6);
    cout << "Ответ: " << x << endl;
}

void task2_1() {
    int n;
    int x;
    unsigned char list = 0;
    unsigned char maska = 128;
    cout << "Введите количество чисел от (1 до 8): ";
    cin >> n;
    cout << "Введите члены последовательности: ";
    for (int i = 0; i < n; i++) {
        cin >> x;
        list = (maska >> x) | list;
    }
    for (int i = 0; i < 8; i++) {
        if ((bitset<8>(list))[7-i])
            cout << i << " ";
    }
}

void task2_2() {
    int n;
    int x;
    unsigned long long list = 0;
    unsigned long long maska = pow(2, 63);
    cout << "Введите количество чисел от (1 до 64): ";
    cin >> n;
    cout << "Введите члены последовательности: ";
    for (int i = 0; i < n; i++) {
        cin >> x;
        list = (maska >> x) | list;
    }
    for (int i = 0; i < 64; i++) {
        if ((bitset<64>(list))[63 - i])
            cout << i << " ";
    }
}

void task2_3() {
    int n;
    int x;
    unsigned char list[64] = { 0 };
    unsigned long long maska = pow(2, 63);
    cout << "Введите количество чисел от (1 до 64): ";
    cin >> n;
    cout << "Введите члены последовательности: ";
    for (int i = 0; i < n; i++) {
        cin >> x;
        list[x] = 1;
    }
    for (int i = 0; i < 64; i++) {
        if (list[i])
            cout << i << " ";
    }
}

void task3() {
    ifstream input("input.txt");
    fstream output("output.txt");

    if (!(input.is_open() && output.is_open())) cout << "Ошибка открытия файла";


    bitset<1000000> bit(0);
    int temp;
    auto start = clock();
    while (input >> temp) {
        bit.set(temp);
    }
    for (int i = 0; i < bit.size(); i++) {
        if (bit[i] == 1) output << i << ' ';
    }
    auto finish = clock();
    auto duration = (double)(finish - start) / CLOCKS_PER_SEC;
    cout << "Время сортировки: " << duration << "c";
    cout << "\nРазмер массива: " << sizeof(bit) << " байт или " << sizeof(bit) / 1024 << " килобайт";
}

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    cout << "Задание 1в: ";
    unsigned int x = 25;
    const int n = sizeof(int) * 8;
    unsigned maska = (1 << n - 1);
    cout << "Начальный вид маски: " << bitset<n>(maska) << endl;
    cout << "Результат: ";
    for (int i = 1; i <= n; i++) {
        cout << ((x & maska) >> (n - i));
        maska = maska >> 1;
    }
    cout << endl;
    cout << "Задание 1а: ";
    task1_1();
    cout << "Задание 1б: ";
    task1_2();
    /*cout << "Задание 2а: ";
    task2_1();
    cout << "\nЗадание 2б: ";
    task2_2();
    cout << "\nЗадание 2в: ";
    task2_3();*/
    task3();
}


