#include <iostream>
#include <chrono>
#include <string>

std::string getEnding(int value, const std::string& variant1, 
                      const std::string& variant2, const std::string& variant3) {
    value = abs(value) % 100;
    if (value >= 11 && value <= 19) {
        return variant3;
    }
    
    value %= 10;
    if (value == 1) {
        return variant1;
    } else if (value >= 2 && value <= 4) {
        return variant2;
    } else {
        return variant3;
    }
}

int main() {
    int64_t total_seconds;
    
    std::cout << "Введите количество секунд: ";
    std::cin >> total_seconds;
    
    if (total_seconds < 0) {
        std::cout << "Количество секунд не может быть отрицательным!" << std::endl;
        return 1;
    }
    
    std::chrono::seconds seconds_chrono(total_seconds);
    
    auto hours = std::chrono::duration_cast<std::chrono::hours>(seconds_chrono);
    seconds_chrono -= hours;
    
    auto minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds_chrono);
    seconds_chrono -= minutes;
    
    auto seconds = seconds_chrono;
    
    std::cout << "\nРезультат (с использованием chrono):" << std::endl;
    std::cout << hours.count() << " " << getEnding(hours.count(), "час", "часа", "часов") << " "
              << minutes.count() << " " << getEnding(minutes.count(), "минута", "минуты", "минут") << " "
              << seconds.count() << " " << getEnding(seconds.count(), "секунда", "секунды", "секунд") 
              << std::endl;
    
    int64_t hours_math = total_seconds / 3600;
    int64_t minutes_math = (total_seconds % 3600) / 60;
    int64_t seconds_math = total_seconds % 60;
    
    std::cout << "\nРезультат (арифметический метод):" << std::endl;
    std::cout << hours_math << " " << getEnding(hours_math, "час", "часа", "часов") << " "
              << minutes_math << " " << getEnding(minutes_math, "минута", "минуты", "минут") << " "
              << seconds_math << " " << getEnding(seconds_math, "секунда", "секунды", "секунд") 
              << std::endl;
    
    return 0;
}