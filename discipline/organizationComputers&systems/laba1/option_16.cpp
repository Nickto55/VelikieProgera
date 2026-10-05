#include <iostream>
#include <locale>

using namespace std;

union union_double
{
    double d;
    unsigned long long u;
};

void print_bin_for_int(unsigned int val)
{
    for (int i = 31; i >= 0; --i)
    {
        cout << ((val >> i) & 1);
    }
    cout << endl;
}

void print_bin_for_double(double val)
{
    union_double du;
    du.d = val;
    for (int i = 63; i >= 0; --i)
    {
        cout << ((du.u >> i) & 1);
    }
    cout << endl;
}

unsigned int transform_int(unsigned int val)
{
    int countOnes = 0;
    for (int i = 0; i < 32; ++i)
    {
        if ((val >> i) & 1)
        {
            countOnes++;
        }
    }

    if (countOnes == 0)
        return 0;
    if (countOnes == 32)
        return ~0U;
    return (~0U) << (32 - countOnes);
}

double transform_double(double val)
{
    union_double du;
    du.d = val;
    unsigned long long temp = du.u;

    int countOnes = 0;
    for (int i = 0; i < 64; ++i)
    {
        if ((temp >> i) & 1)
        {
            countOnes++;
        }
    }

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

    du.u = resultU;
    return du.d;
}

int main()
{
    setlocale(LC_ALL, "Russian");

    unsigned int uintVal;
    double doubleVal;

    // int
    cout << "введите значение int: ";
    cin >> uintVal;

    cout << "input int in bin:  ";
    print_bin_for_int(uintVal);

    unsigned int transformedUInt = transform_int(uintVal);
    cout << "transformed int: ";
    print_bin_for_int(transformedUInt);
    cout << "transformed int: " << transformedUInt << "\n\n";

    // double
    cout << "введите значение double: ";
    cin >> doubleVal;

    cout << "input double in bin:  ";
    print_bin_for_double(doubleVal);

    double transformedDouble = transform_double(doubleVal);
    cout << "transformed double bin: ";
    print_bin_for_double(transformedDouble);
    cout << "transformed double: " << transformedDouble << endl;
    

    return 0;
}