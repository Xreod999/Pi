/**
 * @file pi.cpp
 * @brief Program obliczający przybliżoną wartość liczby PI metodą całki numerycznej.
 * * Program wykorzystuje wielowątkowość (std::thread) do równoległego wykonywania obliczeń.
 * Zastosowano metodę prostokątów oraz strategię przeplatania (interleaving), 
 * aby zapewnić równe obciążenie procesora dla każdego wątku.
 */

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <string> 

/**
 * @brief Główna funkcja programu realizująca obliczenia PI.
 * * Funkcja przyjmuje parametry z linii komend, dzieli zakres obliczeniowy na wątki,
 * sumuje wyniki cząstkowe i mierzy czas wykonania.
 * * @param argc Liczba argumentów wejściowych.
 * @param argv Tablica argumentów (argv[1]: liczba kroków, argv[2]: liczba wątków).
 * @return int Kod zakończenia programu (0 - sukces, 1 - błąd parametrów).
 */
int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Użycie: " << argv[0] << " <liczba_podzialow> <liczba_watkow>" << std::endl;
        return 1;
    }

    // Konwersja argumentów wejściowych
    long long num_steps = std::stoll(argv[1]);
    int num_threads = std::stoi(argv[2]);

    if (num_threads <= 0) num_threads = 1;

    double step = 1.0 / static_cast<double>(num_steps);
    std::vector<double> partial_sums(num_threads, 0.0);
    std::vector<std::thread> threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    /**
     * @brief Lambda wykonująca obliczenia dla konkretnego wątku.
     * * @param thread_id Indeks bieżącego wątku.
     * @param result Referencja do zmiennej przechowującej wynik cząstkowy.
     */
    auto compute_part = [step, num_steps, num_threads](int thread_id, double& result) {
        double sum = 0.0;
        // Strategia przeplatania (interleaving) dla równego obciążenia
        for (long long i = thread_id; i < num_steps; i += num_threads) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        result = sum * step;
    };

    // Tworzenie i uruchamianie wątków
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(compute_part, i, std::ref(partial_sums[i]));
    }

    // Oczekiwanie na zakończenie wszystkich wątków
    for (auto& t : threads) {
        t.join();
    }
    
    // Sumowanie wyników cząstkowych (redukcja)
    double pi = std::accumulate(partial_sums.begin(), partial_sums.end(), 0.0);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    // Wyświetlanie wyników
    std::cout << std::fixed << std::setprecision(15);
    std::cout << "Wynik PI: " << pi << std::endl;
    std::cout << "Czas obliczen: " << elapsed.count() << " s" << std::endl;
    std::cout << "Watki: " << num_threads << " | Kroki: " << num_steps << std::endl;

    return 0;
}