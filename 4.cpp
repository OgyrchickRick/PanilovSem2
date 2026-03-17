#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

class TaskTimer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
    bool is_running;

public:
    TaskTimer() : is_running(false) {}

    void start() {
        start_time = std::chrono::high_resolution_clock::now();
        is_running = true;
    }

    void stop() {
        if (is_running) {
            end_time = std::chrono::high_resolution_clock::now();
            is_running = false;
        }
    }

    long long getDuration() const {
        if (!is_running && end_time > start_time) {
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
            return duration.count();
        }
        return 0;
    }

    long long measureSorting(std::vector<int>& vec) {
        start();
        std::sort(vec.begin(), vec.end());
        stop();
        return getDuration();
    }

    long long measureSorting(const std::vector<int>& vec) {
        std::vector<int> copy = vec;  // Создаём копию для сортировки
        start();
        std::sort(copy.begin(), copy.end());
        stop();
        return getDuration();
    }

    bool isTimerRunning() const {
        return is_running;
    }
};

std::vector<int> generateRandomVector(size_t size, int min_value = 1, int max_value = 100000) {
    std::vector<int> vec(size);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min_value, max_value);
    
    for (auto& num : vec) {
        num = dist(gen);
    }
    
    return vec;
}

void printFirstElements(const std::vector<int>& vec, int count = 10) {
    std::cout << "Первые " << count << " элементов: ";
    for (int i = 0; i < std::min(count, (int)vec.size()); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

int main() {
    TaskTimer timer;
    const int VECTOR_SIZE = 100000;
    
    std::cout << "=== Тестирование класса TaskTimer ===" << std::endl;
    std::cout << "Размер вектора: " << VECTOR_SIZE << " элементов\n" << std::endl;
    
    std::cout << "Запуск 1:" << std::endl;
    std::vector<int> data1 = generateRandomVector(VECTOR_SIZE);
    std::cout << "До сортировки: ";
    printFirstElements(data1, 5);
    
    long long duration1 = timer.measureSorting(data1);
    std::cout << "После сортировки: ";
    printFirstElements(data1, 5);
    std::cout << "Время выполнения сортировки: " << duration1 << " миллисекунд" << std::endl;
    
    std::cout << std::endl;
    
    std::cout << "Запуск 2:" << std::endl;
    std::vector<int> data2 = generateRandomVector(VECTOR_SIZE);
    std::cout << "До сортировки: ";
    printFirstElements(data2, 5);
    
    long long duration2 = timer.measureSorting(data2);
    std::cout << "После сортировки: ";
    printFirstElements(data2, 5);
    std::cout << "Время выполнения сортировки: " << duration2 << " миллисекунд" << std::endl;
    
    std::cout << std::endl;
    
    std::cout << "Запуск 3 (сортировка отсортированного вектора):" << std::endl;
    long long duration3 = timer.measureSorting(data2);  // data2 уже отсортирован
    std::cout << "Время выполнения сортировки: " << duration3 << " миллисекунд" << std::endl;
    
    std::cout << std::endl;
    
    std::cout << "Демонстрация ручного управления таймером:" << std::endl;
    std::vector<int> data3 = generateRandomVector(VECTOR_SIZE / 10);  // Меньший размер
    
    timer.start();
    std::sort(data3.begin(), data3.end());
    timer.stop();
    
    std::cout << "Время сортировки (ручной замер): " << timer.getDuration() << " миллисекунд" << std::endl;
    
    std::cout << "\n=== Сводка результатов ===" << std::endl;
    std::cout << "Запуск 1 (случайные данные): " << duration1 << " мс" << std::endl;
    std::cout << "Запуск 2 (случайные данные): " << duration2 << " мс" << std::endl;
    std::cout << "Запуск 3 (отсортированные данные): " << duration3 << " мс" << std::endl;
    
    return 0;
}