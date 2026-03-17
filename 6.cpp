#include <iostream>
#include <vector>
#include <string>

class FactorialCalculator {
public:
    static long long calculate(int n) {
        if (n < 0) return 0;
        long long result = 1;
        for (int i = 2; i <= n; ++i) {
            result *= i;
        }
        return result;
    }
};

class VirtualThread {
private:
    std::vector<int> numbers;        
    size_t current_index;             
    
public:
    VirtualThread(const std::vector<int>& nums) : numbers(nums), current_index(0) {}
    
    bool hasNext() const {
        return current_index < numbers.size();
    }
    
    bool run(int thread_id, long long& result, int& number) {
        if (!hasNext()) {
            return false;
        }
        
        number = numbers[current_index];
        result = FactorialCalculator::calculate(number);
        current_index++;
        
        return true;
    }
    
    size_t remainingTasks() const {
        return numbers.size() - current_index;
    }
};

class HyperThreadingSimulator {
private:
    VirtualThread thread1;
    VirtualThread thread2;
    
public:
    HyperThreadingSimulator(const std::vector<int>& nums1, const std::vector<int>& nums2)
        : thread1(nums1), thread2(nums2) {}
    
    void execute() {
        std::cout << "=== Имитация Hyper-Threading ===\n" << std::endl;
        
        bool thread1_active = true;
        bool thread2_active = true;
        
        while (thread1_active || thread2_active) {
            if (thread1_active) {
                long long result;
                int number;
                
                if (thread1.run(1, result, number)) {
                    std::cout << "Виртуальный поток 1 вычисляет " << number << "! = " << result << std::endl;
                } else {
                    thread1_active = false;
                }
            }
            
            if (thread2_active) {
                long long result;
                int number;
                
                if (thread2.run(2, result, number)) {
                    std::cout << "Виртуальный поток 2 вычисляет " << number << "! = " << result << std::endl;
                } else {
                    thread2_active = false;
                }
            }
        }
        
        std::cout << "\n=== Все задания выполнены ===" << std::endl;
    }
    
    void executeDetailed() {
        std::cout << "=== Детальная симуляция Hyper-Threading ===\n" << std::endl;
        
        int iteration = 1;
        
        while (thread1.hasNext() || thread2.hasNext()) {
            std::cout << "Итерация " << iteration << ":" << std::endl;
            
            if (thread1.hasNext()) {
                long long result;
                int number;
                thread1.run(1, result, number);
                std::cout << "  Виртуальный поток 1 вычисляет " << number << "! = " << result << std::endl;
            } else {
                std::cout << "  Виртуальный поток 1: нет заданий" << std::endl;
            }
            
            if (thread2.hasNext()) {
                long long result;
                int number;
                thread2.run(2, result, number);
                std::cout << "  Виртуальный поток 2 вычисляет " << number << "! = " << result << std::endl;
            } else {
                std::cout << "  Виртуальный поток 2: нет заданий" << std::endl;
            }
            
            std::cout << std::endl;
            iteration++;
        }
        
        std::cout << "Все виртуальные потоки завершили работу." << std::endl;
    }
};

std::string formatNumber(long long num) {
    std::string result = std::to_string(num);
    int insertPosition = result.length() - 3;
    
    while (insertPosition > 0) {
        result.insert(insertPosition, ",");
        insertPosition -= 3;
    }
    
    return result;
}

class AdvancedHyperThreadingSimulator {
private:
    VirtualThread thread1;
    VirtualThread thread2;
    
    struct ThreadStats {
        int tasks_completed = 0;
        long long total_computation_time = 0;
    } stats1, stats2;
    
public:
    AdvancedHyperThreadingSimulator(const std::vector<int>& nums1, const std::vector<int>& nums2)
        : thread1(nums1), thread2(nums2) {}
    
    void executeWithStats() {
        std::cout << "=== Hyper-Threading симуляция со статистикой ===\n" << std::endl;
        
        std::cout << "Поток 1 задания: ";
        printVector(thread1);
        std::cout << "Поток 2 задания: ";
        printVector(thread2);
        std::cout << std::endl;
        
        int total_iterations = 0;
        
        while (thread1.hasNext() || thread2.hasNext()) {
            total_iterations++;
            
            if (thread1.hasNext()) {
                long long result;
                int number;
                thread1.run(1, result, number);
                std::cout << "Виртуальный поток 1: " << number << "! = " 
                          << formatNumber(result) << " (" << result << ")" << std::endl;
                stats1.tasks_completed++;
            }
            
            if (thread2.hasNext()) {
                long long result;
                int number;
                thread2.run(2, result, number);
                std::cout << "Виртуальный поток 2: " << number << "! = " 
                          << formatNumber(result) << " (" << result << ")" << std::endl;
                stats2.tasks_completed++;
            }
            
            if (thread1.hasNext() || thread2.hasNext()) {
                std::cout << "  --- переключение контекста ---" << std::endl;
            }
        }
        
        std::cout << "\n=== Статистика выполнения ===" << std::endl;
        std::cout << "Всего итераций планировщика: " << total_iterations << std::endl;
        std::cout << "Поток 1 выполнил заданий: " << stats1.tasks_completed << std::endl;
        std::cout << "Поток 2 выполнил заданий: " << stats2.tasks_completed << std::endl;
        std::cout << "Всего заданий: " << (stats1.tasks_completed + stats2.tasks_completed) << std::endl;
    }
    
private:
    void printVector(const VirtualThread& thread) {
        std::cout << "[...]" << std::endl;
    }
};

int main() {
    std::vector<int> thread1_numbers = {5, 10, 15};  // Добавим ещё число для демонстрации
    std::vector<int> thread2_numbers = {7, 12, 8, 6};
    
    std::cout << "Исходные данные:" << std::endl;
    std::cout << "Виртуальный поток 1: ";
    for (int num : thread1_numbers) std::cout << num << " ";
    std::cout << "(факториалы: " << num << "!)" << std::endl;
    
    std::cout << "Виртуальный поток 2: ";
    for (int num : thread2_numbers) std::cout << num << " ";
    std::cout << "(факториалы: " << num << "!)" << std::endl;
    
    std::cout << "\n" << std::string(50, '=') << "\n" << std::endl;
    
    HyperThreadingSimulator simulator(thread1_numbers, thread2_numbers);
    simulator.execute();
    
    std::cout << "\n" << std::string(50, '=') << "\n" << std::endl;
    
    HyperThreadingSimulator detailed_sim(thread1_numbers, thread2_numbers);
    detailed_sim.executeDetailed();
    
    std::cout << "\n" << std::string(50, '=') << "\n" << std::endl;
    
    AdvancedHyperThreadingSimulator advanced_sim(thread1_numbers, thread2_numbers);
    advanced_sim.executeWithStats();
    
    return 0;
}