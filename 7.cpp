#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <chrono>
#include <thread>
#include <iomanip>

class VirtualThread {
private:
    std::queue<std::string> tasks;  
    std::string name;                 
    int completed_tasks;               
    bool is_busy;                      
    std::string current_task;          

public:
    VirtualThread(const std::string& thread_name, const std::vector<std::string>& initial_tasks) 
        : name(thread_name), completed_tasks(0), is_busy(false) {
        for (const auto& task : initial_tasks) {
            tasks.push(task);
        }
    }

    bool hasTasks() const {
        return !tasks.empty() || is_busy;
    }

    size_t remainingTasks() const {
        return tasks.size();
    }

    bool runNextTask() {
        if (tasks.empty()) {
            if (is_busy) {
                std::cout << name << " завершает " << current_task << std::endl;
                is_busy = false;
                completed_tasks++;
            }
            return false;
        }

        if (!is_busy) {
            current_task = tasks.front();
            tasks.pop();
            is_busy = true;
            std::cout << name << " начал " << current_task << std::endl;
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        } else {
            std::cout << name << " закончил " << current_task << std::endl;
            is_busy = false;
            completed_tasks++;
        }

        return true;
    }

    void forceComplete() {
        if (is_busy) {
            std::cout << name << " принудительно завершает " << current_task << std::endl;
            is_busy = false;
            completed_tasks++;
        }
    }

    void printStats() const {
        std::cout << name << ": выполнено " << completed_tasks 
                  << " заданий, осталось " << tasks.size() << std::endl;
    }

    bool isBusy() const {
        return is_busy;
    }

    std::string getName() const {
        return name;
    }
};

class HyperThreadingSimulator {
private:
    VirtualThread thread1;
    VirtualThread thread2;
    bool use_detailed_logging;
    int context_switches;

public:
    HyperThreadingSimulator(const std::vector<std::string>& tasks1, 
                           const std::vector<std::string>& tasks2,
                           bool detailed = true)
        : thread1("Виртуальный поток 1", tasks1)
        , thread2("Виртуальный поток 2", tasks2)
        , use_detailed_logging(detailed)
        , context_switches(0) {}

    void execute() {
        std::cout << "=== Запуск симуляции Hyper-Threading ===\n" << std::endl;
        
        while (thread1.hasTasks() || thread2.hasTasks()) {
            if (thread1.hasTasks()) {
                thread1.runNextTask();
            }
            
            if (thread2.hasTasks()) {
                thread2.runNextTask();
            }
            
            context_switches++;
            
            if (use_detailed_logging) {
                std::cout << "  --- переключение контекста #" << context_switches << " ---\n" << std::endl;
                std::this_thread::sleep_for(std::chrono::milliseconds(50)); // Пауза для наглядности
            }
        }
        
        if (thread1.isBusy()) thread1.forceComplete();
        if (thread2.isBusy()) thread2.forceComplete();
        
        printSummary();
    }

    void executeWithStrategy(const std::string& strategy) {
        std::cout << "\n=== Стратегия планирования: " << strategy << " ===\n" << std::endl;
        
        if (strategy == "Round Robin") {
            executeRoundRobin();
        } else if (strategy == "First Come First Served") {
            executeFCFS();
        } else if (strategy == "Shortest Task First") {
            executeShortestFirst();
        } else {
            execute(); // По умолчанию
        }
    }

    void printSummary() {
        std::cout << "\n=== Статистика выполнения ===" << std::endl;
        std::cout << "Всего переключений контекста: " << context_switches << std::endl;
        thread1.printStats();
        thread2.printStats();
    }

private:
    void executeRoundRobin() {
        int quantum = 3; 
        
        while (thread1.hasTasks() || thread2.hasTasks()) {
            for (int i = 0; i < quantum && thread1.hasTasks(); ++i) {
                thread1.runNextTask();
            }
            
            for (int i = 0; i < quantum && thread2.hasTasks(); ++i) {
                thread2.runNextTask();
            }
            
            context_switches++;
            std::cout << "  --- конец кванта времени ---\n" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }

    void executeFCFS() {
        while (thread1.hasTasks() || thread2.hasTasks()) {
            if (thread1.isBusy()) {
                thread1.runNextTask(); 
            } else if (thread1.hasTasks()) {
                thread1.runNextTask(); 
            }
            
            if (thread2.isBusy()) {
                thread2.runNextTask(); 
            } else if (thread2.hasTasks()) {
                thread2.runNextTask(); 
            }
            
            context_switches++;
        }
    }

    void executeShortestFirst() {
        
        while (thread1.hasTasks() || thread2.hasTasks()) {
            if (thread1.hasTasks()) {
                std::cout << thread1.getName() << " выполняет быстрое задание" << std::endl;
                thread1.runNextTask();
            }
            
            if (thread2.hasTasks()) {
                std::cout << thread2.getName() << " выполняет медленное задание" << std::endl;
                thread2.runNextTask();
            }
            
            context_switches++;
        }
    }
};

class AdvancedVirtualThread {
private:
    struct Task {
        std::string name;
        int priority;
        int duration;
    };
    
    std::queue<Task> tasks;
    std::string thread_name;
    int completed;

public:
    AdvancedVirtualThread(const std::string& name, const std::vector<std::string>& task_names) 
        : thread_name(name), completed(0) {
        for (const auto& task_name : task_names) {
            int priority = rand() % 3 + 1;
            int duration = (rand() % 200) + 50;
            tasks.push({task_name, priority, duration});
        }
    }

    bool runNextTask() {
        if (tasks.empty()) return false;
        
        Task current = tasks.front();
        tasks.pop();
        
        std::cout << thread_name << " начал " << current.name 
                  << " (приоритет: " << current.priority 
                  << ", длительность: " << current.duration << " мс)" << std::endl;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(current.duration));
        
        std::cout << thread_name << " закончил " << current.name << std::endl;
        completed++;
        
        return true;
    }

    bool hasTasks() const {
        return !tasks.empty();
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    
    std::cout << "=== Пример 1: Базовая реализация (как в задании) ===\n" << std::endl;
    
    std::vector<std::string> thread1_tasks = {"Задача A", "Задача C"};
    std::vector<std::string> thread2_tasks = {"Задача B", "Задача D"};
    
    VirtualThread vt1("Виртуальный поток 1", thread1_tasks);
    VirtualThread vt2("Виртуальный поток 2", thread2_tasks);
    
    vt1.runNextTask(); // Начал A
    vt2.runNextTask(); // Начал B
    vt1.runNextTask(); // Закончил A
    vt1.runNextTask(); // Начал C
    vt2.runNextTask(); // Закончил B
    vt2.runNextTask(); // Начал D
    
    std::cout << "\n" << std::string(50, '=') << "\n" << std::endl;
    
    std::cout << "=== Пример 2: Симулятор Hyper-Threading ===\n" << std::endl;
    
    HyperThreadingSimulator simulator(
        {"Задача A1", "Задача A2", "Задача A3"},
        {"Задача B1", "Задача B2"}
    );
    simulator.execute();
    
    std::cout << "\n" << std::string(50, '=') << "\n" << std::endl;
    
    std::cout << "=== Пример 3: Сравнение стратегий планирования ===\n" << std::endl;
    
    std::vector<std::string> long_tasks = {"Задача X1", "Задача X2", "Задача X3", "Задача X4"};
    std::vector<std::string> short_tasks = {"Задача Y1", "Задача Y2"};
    
    HyperThreadingSimulator strategy_sim(long_tasks, short_tasks, false);
    
    std::cout << "Стратегия 1: Round Robin" << std::endl;
    strategy_sim.executeWithStrategy("Round Robin");
    
    std::cout << "\nСтратегия 2: First Come First Served" << std::endl;
    strategy_sim.executeWithStrategy("First Come First Served");
    
    std::cout << "\n" << std::string(50, '=') << "\n" << std::endl;
    
    std::cout << "=== Пример 4: Задания с разными приоритетами ===\n" << std::endl;
    
    AdvancedVirtualThread adv_thread1("Поток 1 (высокий приоритет)", {"Срочная задача", "Обычная задача"});
    AdvancedVirtualThread adv_thread2("Поток 2 (низкий приоритет)", {"Фоновая задача", "Обслуживание"});
    
    while (adv_thread1.hasTasks() || adv_thread2.hasTasks()) {
        if (adv_thread1.hasTasks()) adv_thread1.runNextTask();
        if (adv_thread2.hasTasks()) adv_thread2.runNextTask();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    return 0;
}