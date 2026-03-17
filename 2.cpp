#include <iostream>
#include <thread>
#include <chrono>
#include <string>

std::string getSecondWord(int seconds) {
    if (seconds % 10 == 1 && seconds % 100 != 11) {
        return "секунда";
    } else if ((seconds % 10 >= 2 && seconds % 10 <= 4) && 
               (seconds % 100 < 10 || seconds % 100 >= 20)) {
        return "секунды";
    } else {
        return "секунд";
    }
}

int main() {
    int N;
    
    std::cout << "Введите количество секунд для обратного отсчёта: ";
    std::cin >> N;
    
    if (N <= 0) {
        std::cout << "Пожалуйста, введите положительное число!" << std::endl;
        return 1;
    }
    
    std::cout << "\nЗапуск таймера обратного отсчёта..." << std::endl;
    std::cout << "------------------------" << std::endl;
    
    for (int i = N; i > 0; --i) {
        std::cout << "Осталось: " << i << " " << getSecondWord(i) << std::endl;
        
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
    
    std::cout << "------------------------" << std::endl;
    std::cout << "ВРЕМЯ ВЫШЛО!" << std::endl;
    
    return 0;
}