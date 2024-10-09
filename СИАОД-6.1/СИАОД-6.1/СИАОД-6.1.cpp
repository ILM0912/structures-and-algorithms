#include <iostream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const float LOAD_FACTOR = 0.75;
const int INITIAL_SIZE = 10;

struct Account {
    int accountNumber; //key
    string name;
    string address;

    Account(int num, const string& name, const string& address) {
        this->accountNumber = num;
        this->name = name;
        this->address = address;
    }
};

class Hash {
private:
    vector<Account*> table;
    int size;
    int capacity;

public:

    Hash() {
        this->size = 0;
        this->capacity = INITIAL_SIZE;
        table.resize(capacity, nullptr);
    }
    int hashFunction(int accountNumber) {
        return accountNumber % capacity;
    }

    void rehash() {
        vector<Account*> oldTable = table;
        capacity *= 2;
        table.assign(capacity, nullptr);
        size = 0;
        for (Account* account : oldTable) {
            if (account) {
                insert(account->accountNumber, account->name, account->address);
            }
        }
    }

    void insert(int accountNumber, const string& fullName, const string& address) {
        if (size+1 >= capacity * LOAD_FACTOR) {
            rehash();
        }
        int index = hashFunction(accountNumber);
        while (table[index] != nullptr) {
            index = (index + 1) % capacity; // линейное пробирование
        }
        table[index] = new Account(accountNumber, fullName, address);
        size++;
    }

    Account* search(int accountNumber) {
        int index = hashFunction(accountNumber);
        while (table[index] != nullptr) {
            if (table[index]->accountNumber == accountNumber) {
                return table[index];
            }
            index = (index + 1) % capacity; // линейное пробирование
        }
        return nullptr;
    }

    void remove(int accountNumber) {
        int index = hashFunction(accountNumber);
        while (table[index] != nullptr) {
            if (table[index]->accountNumber == accountNumber) {
                delete table[index];
                table[index] = nullptr; // помечаем как удаленный
                size--;
                return;
            }
            index = (index + 1) % capacity; // линейное пробирование
        }
    }

    void print() {
        for (Account* account : table) {
            if (account != nullptr) {
                cout << "Account Number: " << account->accountNumber << ", Full Name: " << account->name << ", Address: " << account->address << endl;
            }
        }
        cout << "\nSize: " << size << "\nCapacity: " << capacity << endl << endl;
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    Hash hash;
    int com = 0;
    int number;
    Account* found;
    string name;   
    string address;
    hash.insert(1111111, "Ротчева Дарья Сергеевна", "Тверь, пер. Никитина, д. 8к2");
    hash.insert(2222222, "Красоткин Арсений Александрович", "Тверь, пер. Перекопский, д. 6");
    hash.insert(3333333, "Ротчева Мария Сергеевна", "Тверь, пер. Богданова, д. 22к2");
    hash.insert(4444444, "Хайруллин Данил Линардович", "Москва, ул. 11-я Парковая, д. 36");
    hash.insert(5555555, "Красоткина Арина Александровна", "Тверь, б-р Радищева, д. 19");/*
    hash.insert(6666666, "Лясковский Тимофей ", "Москва, ...");
    hash.insert(7777777, "Коротков Алексей Александрович", "Москва, ...");*/
    hash.print();
    while (true) {
        cout << "Список команд: " << endl << " - Вставка элемента (1)" << endl << " - Удаления элемента (2)" << endl << " - Поиска по ключу (3)" << endl << " - Вывод таблицы (4)" << endl << " - Для выхода нажмите 0" << endl << endl;
        cin >> com;
        if (com == 0) break;
        if (com == 1) {
            cout << "Укажите номер счета (7 цифр): ";
            cin >> number;
            cout << "ФИО: ";
            cin >> address;
            cout << "Адрес регистрации: ";
            cin >> name;
            hash.insert(number, name, address);
        }
        if (com == 2) {

            cout << "Укажите номер счета: ";
            cin >> number;
            hash.remove(number);
        }
        if (com == 3) {
            cout << "Укажите номер счета: ";
            cin >> number;
            found = hash.search(number);
            if (found) {
                cout << "Найден: \n - " << found->name << "\n - " << found->address << endl;
            }
            else {
                cout << "Запись не найдена!" << endl;
            }
        }
        if (com == 4) {
            hash.print();
        }
    }
}
