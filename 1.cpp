#include <iostream>
#include <chrono>

// Функция для вычисления суммы чисел от 1 до N
long long calculateSum(int N) {
    long long sum = 0;
    for (int i = 1; i <= N; ++i) {
        sum += i;
    }
    return sum;
}

int main() {
    int N;
    std::cout << "Введите N: ";
    std::cin >> N;
    
    // Засекаем время перед выполнением функции
    auto start = std::chrono::high_resolution_clock::now();
    
    // Выполняем вычисление
    long long result = calculateSum(N);
    
    // Засекаем время после выполнения
    auto end = std::chrono::high_resolution_clock::now();
    
    // Вычисляем разницу и конвертируем в миллисекунды
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "Сумма чисел от 1 до " << N << " = " << result << std::endl;
    std::cout << "Время выполнения: " << duration.count() << " миллисекунды" << std::endl;
    
    return 0;
}