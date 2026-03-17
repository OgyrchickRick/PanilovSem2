#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <iomanip>
#include <string>

class Timer {
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
    std::chrono::time_point<std::chrono::high_resolution_clock> end_time;
    bool running;

public:
    Timer() : running(false) {}

    void start() {
        start_time = std::chrono::high_resolution_clock::now();
        running = true;
    }

    void stop() {
        if (running) {
            end_time = std::chrono::high_resolution_clock::now();
            running = false;
        }
    }

    long long elapsedMilliseconds() const {
        if (!running && end_time > start_time) {
            return std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count();
        }
        return 0;
    }

    long long elapsedMicroseconds() const {
        if (!running && end_time > start_time) {
            return std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();
        }
        return 0;
    }
};

void bubbleSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 0; i < n - 1; ++i) {
        bool swapped = false;
        for (int j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

void insertionSort(std::vector<int>& arr) {
    int n = arr.size();
    for (int i = 1; i < n; ++i) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            --j;
        }
        arr[j + 1] = key;
    }
}

void merge(std::vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    
    std::vector<int> L(n1), R(n2);
    
    for (int i = 0; i < n1; ++i)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; ++j)
        R[j] = arr[mid + 1 + j];
    
    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            ++i;
        } else {
            arr[k] = R[j];
            ++j;
        }
        ++k;
    }
    
    while (i < n1) {
        arr[k] = L[i];
        ++i;
        ++k;
    }
    
    while (j < n2) {
        arr[k] = R[j];
        ++j;
        ++k;
    }
}

void mergeSort(std::vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        
        merge(arr, left, mid, right);
    }
}

void mergeSortWrapper(std::vector<int>& arr) {
    mergeSort(arr, 0, arr.size() - 1);
}

int partition(std::vector<int>& arr, int low, int high) {
    int pivot = arr[high]; // Опорный элемент
    int i = low - 1; // Индекс меньшего элемента
    
    for (int j = low; j <= high - 1; ++j) {
        if (arr[j] <= pivot) {
            ++i;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(std::vector<int>& arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void quickSortWrapper(std::vector<int>& arr) {
    quickSort(arr, 0, arr.size() - 1);
}

void stdSortWrapper(std::vector<int>& arr) {
    std::sort(arr.begin(), arr.end());
}

std::vector<int> generateRandomVector(int size, int min_val = 1, int max_val = 1000000) {
    std::vector<int> vec(size);
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min_val, max_val);
    
    for (int& num : vec) {
        num = dist(gen);
    }
    
    return vec;
}

std::vector<int> copyVector(const std::vector<int>& source) {
    return std::vector<int>(source.begin(), source.end());
}

bool isSorted(const std::vector<int>& vec) {
    for (size_t i = 1; i < vec.size(); ++i) {
        if (vec[i] < vec[i - 1]) return false;
    }
    return true;
}

struct SortResult {
    std::string name;
    long long time_ms;
    long long time_us;
    bool success;
};

SortResult testSortAlgorithm(const std::string& name, 
                             void (*sortFunc)(std::vector<int>&), 
                             std::vector<int> data) {
    Timer timer;
    
    timer.start();
    sortFunc(data);
    timer.stop();
    
    SortResult result;
    result.name = name;
    result.time_ms = timer.elapsedMilliseconds();
    result.time_us = timer.elapsedMicroseconds();
    result.success = isSorted(data);
    
    return result;
}

int main() {
    const int ARRAY_SIZE = 100000;
    
    std::cout << "================================================" << std::endl;
    std::cout << "СРАВНЕНИЕ АЛГОРИТМОВ СОРТИРОВКИ" << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << "Размер массива: " << ARRAY_SIZE << " элементов" << std::endl;
    std::cout << "------------------------------------------------\n" << std::endl;
    
    std::vector<int> original = generateRandomVector(ARRAY_SIZE);
    
    std::vector<SortResult> results;
    
    std::cout << "Внимание: Пузырьковая сортировка будет протестирована на массиве меньшего размера!" << std::endl;
    std::cout << "Тестирование на полном размере может занять несколько минут...\n" << std::endl;
    
    const int SMALL_SIZE = 10000;  // 10 тысяч элементов для демонстрации
    
    std::cout << "Тест 1: Пузырьковая сортировка (Bubble Sort) - O(n²)" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    
    std::vector<int> bubbleData = generateRandomVector(SMALL_SIZE);
    SortResult bubbleResult = testSortAlgorithm("Bubble Sort", bubbleSort, bubbleData);
    bubbleResult.time_ms = bubbleResult.time_ms * (ARRAY_SIZE / SMALL_SIZE) * (ARRAY_SIZE / SMALL_SIZE); // Приблизительная экстраполяция
    std::cout << "  Время (фактическое на " << SMALL_SIZE << " элементов): " 
              << bubbleResult.time_us / 1000.0 << " мс" << std::endl;
    std::cout << "  Время (экстраполированное на " << ARRAY_SIZE << " элементов): ~" 
              << bubbleResult.time_ms << " мс (" << bubbleResult.time_ms / 1000.0 << " сек)" << std::endl;
    std::cout << "  Корректность: " << (bubbleResult.success ? "✓" : "✗") << std::endl;
    std::cout << std::endl;
    
    std::cout << "Тестирование на полном размере (" << ARRAY_SIZE << " элементов):" << std::endl;
    std::cout << "================================================\n" << std::endl;
    
    std::cout << "Тест 2: Сортировка вставками (Insertion Sort) - O(n²)" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::vector<int> insertionData = copyVector(original);
    SortResult insertionResult = testSortAlgorithm("Insertion Sort", insertionSort, insertionData);
    std::cout << "  Время: " << insertionResult.time_ms << " мс (" 
              << insertionResult.time_us << " мкс)" << std::endl;
    std::cout << "  Корректность: " << (insertionResult.success ? "✓" : "✗") << std::endl;
    std::cout << std::endl;
    
    std::cout << "Тест 3: Сортировка слиянием (Merge Sort) - O(n log n)" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::vector<int> mergeData = copyVector(original);
    SortResult mergeResult = testSortAlgorithm("Merge Sort", mergeSortWrapper, mergeData);
    std::cout << "  Время: " << mergeResult.time_ms << " мс (" 
              << mergeResult.time_us << " мкс)" << std::endl;
    std::cout << "  Корректность: " << (mergeResult.success ? "✓" : "✗") << std::endl;
    std::cout << std::endl;
    
    std::cout << "Тест 4: Быстрая сортировка (Quick Sort) - O(n log n)" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::vector<int> quickData = copyVector(original);
    SortResult quickResult = testSortAlgorithm("Quick Sort", quickSortWrapper, quickData);
    std::cout << "  Время: " << quickResult.time_ms << " мс (" 
              << quickResult.time_us << " мкс)" << std::endl;
    std::cout << "  Корректность: " << (quickResult.success ? "✓" : "✗") << std::endl;
    std::cout << std::endl;
    
    std::cout << "Тест 5: Стандартная сортировка (std::sort) - Introsort" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    std::vector<int> stdData = copyVector(original);
    SortResult stdResult = testSortAlgorithm("std::sort", stdSortWrapper, stdData);
    std::cout << "  Время: " << stdResult.time_ms << " мс (" 
              << stdResult.time_us << " мкс)" << std::endl;
    std::cout << "  Корректность: " << (stdResult.success ? "✓" : "✗") << std::endl;
    std::cout << std::endl;
    
    std::cout << "================================================" << std::endl;
    std::cout << "СВОДНАЯ ТАБЛИЦА РЕЗУЛЬТАТОВ" << std::endl;
    std::cout << "================================================" << std::endl;
    std::cout << std::left << std::setw(25) << "Алгоритм" 
              << std::right << std::setw(15) << "Время (мс)" 
              << std::setw(15) << "Относ. std::sort" << std::endl;
    std::cout << "------------------------------------------------" << std::endl;
    
    std::cout << std::left << std::setw(25) << "Bubble Sort (экстрап.)" 
              << std::right << std::setw(15) << bubbleResult.time_ms 
              << std::setw(15) << std::fixed << std::setprecision(1) 
              << (double)bubbleResult.time_ms / stdResult.time_ms << "x" << std::endl;
    
    std::cout << std::left << std::setw(25) << "Insertion Sort" 
              << std::right << std::setw(15) << insertionResult.time_ms 
              << std::setw(15) << std::fixed << std::setprecision(1) 
              << (double)insertionResult.time_ms / stdResult.time_ms << "x" << std::endl;
    
    std::cout << std::left << std::setw(25) << "Merge Sort" 
              << std::right << std::setw(15) << mergeResult.time_ms 
              << std::setw(15) << std::fixed << std::setprecision(1) 
              << (double)mergeResult.time_ms / stdResult.time_ms << "x" << std::endl;
    
    std::cout << std::left << std::setw(25) << "Quick Sort" 
              << std::right << std::setw(15) << quickResult.time_ms 
              << std::setw(15) << std::fixed << std::setprecision(1) 
              << (double)quickResult.time_ms / stdResult.time_ms << "x" << std::endl;
    
    std::cout << std::left << std::setw(25) << "std::sort" 
              << std::right << std::setw(15) << stdResult.time_ms 
              << std::setw(15) << "1.0x" << std::endl;
    
    std::cout << "================================================" << std::endl;
    
    return 0;
}