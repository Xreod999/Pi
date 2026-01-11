#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <numeric>
#include <string> 

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Użycie: " << argv[0] << " <liczba_podzialow> <liczba_watkow>" << std::endl;
        return 1;
    }

    
    long long num_steps = std::stoll(argv[1]);
    int num_threads = std::stoi(argv[2]);

    if (num_threads <= 0) num_threads = 1;

    double step = 1.0 / static_cast<double>(num_steps);
    std::vector<double> partial_sums(num_threads, 0.0);
    std::vector<std::thread> threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    
    auto compute_part = [step, num_steps, num_threads](int thread_id, double& result) {
        double sum = 0.0;
        // Strategia przeplatania (interleaving) dla równego obciążenia
        for (long long i = thread_id; i < num_steps; i += num_threads) {
            double x = (i + 0.5) * step;
            sum += 4.0 / (1.0 + x * x);
        }
        result = sum * step;
    };

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(compute_part, i, std::ref(partial_sums[i]));
    }

    for (auto& t : threads) {
        t.join();
    }
    
    double pi = std::accumulate(partial_sums.begin(), partial_sums.end(), 0.0);
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end_time - start_time;

    std::cout << std::fixed << std::setprecision(15);
    std::cout << "Wynik PI: " << pi << std::endl;
    std::cout << "Czas obliczen: " << elapsed.count() << " s" << std::endl;
    std::cout << "Watki: " << num_threads << " | Kroki: " << num_steps << std::endl;

    return 0;
}