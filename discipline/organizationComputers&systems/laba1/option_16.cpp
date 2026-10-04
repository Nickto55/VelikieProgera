#include <iostream>
#include <locale>

using namespace std;

// Структура union для работы с внутренним представлением double (64 бита)
union DoubleUnion
{
    double d;
    unsigned long long u;
};

// Функция вывода двоичного кода для unsigned int (32 бита)
void printBinaryUInt(unsigned int val)
{
    for (int i = 31; i >= 0; --i)
    {
        cout << ((val >> i) & 1);
    }
    cout << endl;
}

// Функция вывода двоичного кода для double через union (64 бита)
void printBinaryDouble(double val)
{
    DoubleUnion du;
    du.d = val;
    for (int i = 63; i >= 0; --i)
    {
        cout << ((du.u >> i) & 1);
    }
    cout << endl;
}

// Преобразование unsigned int: сдвиг 1 влево, 0 вправо
unsigned int transformUInt(unsigned int val)
{
    int countOnes = 0;
    // Подсчет количества единиц с помощью поразрядных операций
    for (int i = 0; i < 32; ++i)
    {
        if ((val >> i) & 1)
        {
            countOnes++;
        }
    }

    // Формирование результата: countOnes единиц слева, остальные нули
    if (countOnes == 0)
        return 0;
    if (countOnes == 32)
        return ~0U;
    return (~0U) << (32 - countOnes);
}

// Преобразование double: сдвиг 1 влево, 0 вправо
double transformDouble(double val)
{
    DoubleUnion du;
    du.d = val;
    unsigned long long temp = du.u;

    int countOnes = 0;
    // Подсчет количества единиц в 64-битном представлении
    for (int i = 0; i < 64; ++i)
    {
        if ((temp >> i) & 1)
        {
            countOnes++;
        }
    }

    // Формирование нового 64-битного кода
    unsigned long long resultU;
    if (countOnes == 0)
    {
        resultU = 0;
    }
    else if (countOnes == 64)
    {
        resultU = ~0ULL;
    }
    else
    {
        resultU = (~0ULL) << (64 - countOnes);
    }

    // Запись преобразованного кода обратно в union для получения double
    du.u = resultU;
    return du.d;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    unsigned int uintVal;
    double doubleVal;

    // --- Работа с unsigned int ---
    cout << "Введите значение unsigned int: ";
    cin >> uintVal;

    cout << "Исходный unsigned int (двоичный код):  ";
    printBinaryUInt(uintVal);

    unsigned int transformedUInt = transformUInt(uintVal);
    cout << "Преобразованный unsigned int (двоичный): ";
    printBinaryUInt(transformedUInt);
    cout << "Преобразованный unsigned int (значение): " << transformedUInt << "\n\n";

    // --- Работа с double ---
    cout << "Введите значение double: ";
    cin >> doubleVal;

    cout << "Исходный double (двоичный код):  ";
    printBinaryDouble(doubleVal);

    double transformedDouble = transformDouble(doubleVal);
    cout << "Преобразованный double (двоичный): ";
    printBinaryDouble(transformedDouble);
    cout << "Преобразованный double (значение): " << transformedDouble << endl;

    return 0;
}