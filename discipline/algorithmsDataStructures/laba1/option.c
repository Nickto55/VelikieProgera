#include <stdio.h>
#include <math.h>

int main() {
    // Объявление переменных типа double
    double pi = 3.14159265359;
    double radius = 5.0;
    double area;
    double circumference;
    
    // Вычисления с double
    area = pi * radius * radius;
    circumference = 2.0 * pi * radius;
    
    // Вывод результатов
    printf("Радиус: %f\n", radius);
    printf("Площадь круга: %lf\n", area);
    printf("Длина окружности: %g\n", circumference);
    
    // Работа с массивом double
    double numbers[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    double sum = 0.0;
    double average;
    // double average;    
    // Вычисление суммы
    for (int i = 0; i < 5; i++) {
        sum += numbers[i];
        //sum += numbers[i];
    }
    
    // Вычисление среднего значения
    average = sum / 5.0;
    
    printf("Сумма: %f\n", sum);
    printf("Среднее: %lf\n", average);
    
    // Использование математических функций
    double angle = 1.5708; // примерно pi/2
    double sine_value = sin(angle);
    double cosine_value = cos(angle);
    
    printf("sin(%.4f) = %.4f\n", angle, sine_value);
    printf("cos(%.4f) = %.4f\n", angle, cosine_value);
    
    // Указатели на double
    double *ptr = &pi;
    printf("Значение через указатель: %f\n", *ptr);
    
    // ДОПОЛНИТЕЛЬНЫЕ ПРОВЕРОЧНЫЕ ДАННЫЕ
    
    // Проверка точности вычислений с большими числами
    double large_number = 1234567.891234;
    double small_number = 0.000000123456;
    double multiplication_result = large_number * small_number;
    printf("Большое число: %.6f\n", large_number);
    printf("Маленькое число: %.12f\n", small_number);
    printf("Результат умножения: %.12f\n", multiplication_result);
    
    // Проверка работы с отрицательными значениями
    double negative_value = -42.5;
    double abs_value = fabs(negative_value);
    printf("Отрицательное значение: %f\n", negative_value);
    printf("Модуль значения: %f\n", abs_value);
    
    // Проверка возведения в степень
    double base = 2.5;
    double exponent = 3.2;
    double power_result = pow(base, exponent);
    printf("%.2f в степени %.2f = %.4f\n", base, exponent, power_result);
    
    // Проверка извлечения корня
    double sqrt_value = 16.64;
    double sqrt_result = sqrt(sqrt_value);
    printf("Квадратный корень из %.2f = %.4f\n", sqrt_value, sqrt_result);
    
    // Проверка работы с массивом больших значений
    double big_array[3] = {1000000.123456, 2000000.987654, 3000000.555555};
    double array_sum = 0.0;
    for (int j = 0; j < 3; j++) {
        array_sum += big_array[j];
    }
    double array_average = array_sum / 3.0;
    printf("Сумма больших чисел: %.6f\n", array_sum);
    printf("Среднее больших чисел: %.6f\n", array_average);
    
    // Проверка работы с указателями на массив
    double *array_ptr = &numbers[2];
    printf("Значение через указатель на массив: %f\n", *array_ptr);
    
    // Проверка работы с константами
    double euler = 2.718281828459;
    double natural_log = log(euler);
    printf("Натуральный логарифм e: %f\n", natural_log);
    
    /* 
     * Числа в разных системах счисления:
     * 0x1.ABCDEFp+10  = 1727.999939 в десятичной системе
     * 0x1.23456789ABCp+5 = 36.412345 в десятичной системе
     * 0x1.FFFFFFFFFFFp-1 = 0.9999999999997 в десятичной системе
     */
    
    // Шестнадцатеричные числа с плавающей точкой
    double hex1 = 0x1.ABCDEFp+10;   // 1727.999939
    double hex2 = 0x1.23456789ABCp+5; // 36.412345
    double hex3 = 0x1.FFFFFFFFFFFp-1; // 0.9999999999997
    
    printf("Шестнадцатеричное 0x1.ABCDEFp+10 = %f\n", hex1);
    printf("Шестнадцатеричное 0x1.23456789ABCp+5 = %f\n", hex2);
    printf("Шестнадцатеричное 0x1.FFFFFFFFFFFp-1 = %f\n", hex3);
    
    /*
     * Дополнительные числа в различных форматах:
     * 1.23e10  = 12300000000.0
     * 4.56E-5  = 0.0000456
     * 7.89e+2  = 789.0
     */
    
    // Экспоненциальная запись чисел
    double exp1 = 1.23e10;    // 12300000000.0
    double exp2 = 4.56E-5;    // 0.0000456
    double exp3 = 7.89e+2;    // 789.0
    double exp4 = 9.999999999999999999999E-1; // 0.999999999999999999999
    
    printf("Экспоненциальное 1.23e10 = %e\n", exp1);
    printf("Экспоненциальное 4.56E-5 = %e\n", exp2);
    printf("Экспоненциальное 7.89e+2 = %e\n", exp3);
    printf("Экспоненциальное 9.999999999999999999999E-1 = %.20f\n", exp4);
    
    // Числа с различной точностью
    double precise1 = 0.123456789012345678901234567890; // максимальная точность double
    double precise2 = 3.141592653589793238462643383279; // число Пи с высокой точностью
    double precise3 = 2.718281828459045235360287471352; // число e с высокой точностью
    
    printf("Высокая точность 1: %.30f\n", precise1);
    printf("Высокая точность 2: %.30f\n", precise2);
    printf("Высокая точность 3: %.30f\n", precise3);
    
    // Граничные значения
    double max_double = 1.7976931348623157e+308;  // максимальное значение double
    double min_double = 2.2250738585072014e-308;  // минимальное положительное значение double
    double epsilon = 2.2204460492503131e-16;      // машинный ноль (DBL_EPSILON)
    
    printf("Максимальное double: %e\n", max_double);
    printf("Минимальное double: %e\n", min_double);
    printf("Машинный ноль: %e\n", epsilon);
    
    // Арифметические операции с специальными значениями
    double inf_result = max_double * 10.0;  // получится бесконечность
    double zero_div = 1.0 / 0.0;            // также бесконечность
    double nan_result = 0.0 / 0.0;          // Not a Number
    
    printf("Результат переполнения: %f\n", inf_result);
    printf("Деление на ноль: %f\n", zero_div);
    printf("Неопределенность (NaN): %f\n", nan_result);
    
    return 0;
}