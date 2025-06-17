#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <ctime>

/**
 * @brief Структура, описывающая сотрудника.
 */
struct Employee {
    std::string fullName; ///< Полное имя сотрудника.
    std::string department; ///< Название отдела.
    std::string position; ///< Должность.
    std::string hireDate; ///< Дата найма.

    /**
     * @brief Оператор сравнения для сортировки.
     */
    bool operator<(const Employee &other) const {
        if (department < other.department) return true;
        if (department > other.department) return false;

        if (fullName < other.fullName) return true;
        if (fullName > other.fullName) return false;

        if (hireDate < other.hireDate) return true;
        if (hireDate > other.hireDate) return false;

        return position < other.position;
    }

    bool operator>(const Employee &other) const {
        return other < *this;
    }

    bool operator<=(const Employee &other) const {
        return !(other < *this);
    }

    bool operator>=(const Employee &other) const {
        return !(*this < other);
    }
};

/**
 * @brief Считывает CSV-файл с сотрудниками.
 * @param filename Имя CSV-файла.
 * @return Вектор сотрудников.
 */
std::vector<Employee> read_csv(const std::string &filename) {
    std::vector<Employee> employees;
    std::ifstream file(filename);
    std::string line;
    getline(file, line); // skip header
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string fullName, department, position, hireDate;
        getline(ss, fullName, ',');
        getline(ss, department, ',');
        getline(ss, position, ',');
        getline(ss, hireDate, ',');
        employees.push_back({fullName, department, position, hireDate});
    }
    return employees;
}

/**
 * @brief Записывает список сотрудников в CSV-файл.
 * @param filename Имя выходного файла.
 * @param employees Вектор сотрудников.
 */
void write_csv(const std::string &filename, const std::vector<Employee> &employees) {
    std::ofstream file(filename);
    file << "FullName,Department,Position,HireDate\n";
    for (const auto &e: employees) {
        file << e.fullName << "," << e.department << "," << e.position << "," << e.hireDate << "\n";
    }
}

/**
 * @brief Сортировка пузырьком.
 * @param arr Вектор сотрудников.
 */
void bubbleSort(std::vector<Employee> &arr) {
    size_t n = arr.size();
    for (size_t i = 0; i < n - 1; ++i) {
        for (size_t j = 0; j < n - i - 1; ++j) {
            if (arr[j] > arr[j + 1]) {
                std::swap(arr[j], arr[j + 1]);
            }
        }
    }
}

/**
 * @brief Построение кучи (heapify).
 * @param arr Вектор сотрудников.
 * @param n Размер кучи.
 * @param i Индекс корня поддерева.
 */
void heapify(std::vector<Employee> &arr, int n, int i) {
    // i - индекс корня поддерева, n - размер пирамиды
    int largest = i;
    int l = 2 * i + 1, r = 2 * i + 2; // индексы левого и правого узлов
    // поиск наибольшего узла
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) {
        std::swap(arr[i], arr[largest]);
        heapify(arr, n, largest);
    }
}

/**
 * @brief Сортировка кучей.
 * @param arr Вектор сотрудников.
 */
void heapSort(std::vector<Employee> &arr) {
    int n = arr.size();
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) {
        std::swap(arr[0], arr[i]);
        heapify(arr, i, 0);
    }
}

/**
 * @brief Слияние двух подмассивов.
 * @param arr Вектор сотрудников.
 * @param left Левый индекс.
 * @param mid Средний индекс.
 * @param right Правый индекс.
 */
void merge(std::vector<Employee> &arr, int left, int mid, int right) {
    std::vector<Employee> leftArr(arr.begin() + left, arr.begin() + mid + 1);
    std::vector<Employee> rightArr(arr.begin() + mid + 1, arr.begin() + right + 1);
    int i = 0, j = 0, k = left;
    while (i < leftArr.size() && j < rightArr.size()) {
        if (leftArr[i] <= rightArr[j]) arr[k++] = leftArr[i++];
        else arr[k++] = rightArr[j++];
    }
    while (i < leftArr.size()) arr[k++] = leftArr[i++];
    while (j < rightArr.size()) arr[k++] = rightArr[j++];
}

/**
 * @brief Сортировка слиянием.
 * @param arr Вектор сотрудников.
 * @param left Левый индекс.
 * @param right Правый индекс.
 */
void mergeSort(std::vector<Employee> &arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    merge(arr, left, mid, right);
}

/**
 * @brief Главная функция программы. Выполняет чтение, сортировку и запись CSV-файлов.
 * @return Код завершения.
 */
int main() {
    std::vector<std::string> filenames = {
        "data\\employees_100.csv",
        "data\\employees_500.csv",
        "data\\employees_1000.csv",
        "data\\employees_2000.csv",
        "data\\employees_5000.csv",
        "data\\employees_7000.csv",
        "data\\employees_10000.csv",
        "data\\employees_15000.csv",
        "data\\employees_20000.csv",
        "data\\employees_30000.csv",
        "data\\employees_40000.csv",
        "data\\employees_50000.csv",
        "data\\employees_70000.csv",
        "data\\employees_100000.csv",
    };

    std::string sortChoice = "standart";

    std::cout << "Method: " << sortChoice << std::endl;
    for (const auto &filename: filenames) {
        std::vector<Employee> employees = read_csv(filename);

        std::clock_t start = std::clock();

        if (sortChoice == "bubble") {
            bubbleSort(employees);
        } else if (sortChoice == "heap") {
            heapSort(employees);
        } else if (sortChoice == "merge") {
            mergeSort(employees, 0, employees.size() - 1);
        } else if (sortChoice == "standart") {
            std::sort(employees.begin(), employees.end());
        }

        std::clock_t end = std::clock();
        double duration_ms = 1000.0 * (end - start) / CLOCKS_PER_SEC;

        std::cout << filename << " : " << duration_ms << " ms\n";

        write_csv(filename + "_sorted.csv", employees);
    }

    std::cout << "All sorts completed.\n";
    return 0;
}
