## @file pi.py
#  @brief Skrypt automatyzujący testy wydajnościowe programu obliczającego PI.
#
#  Skrypt uruchamia skompilowany program C++ dla różnych konfiguracji liczby kroków
#  i liczby wątków, zbiera wyniki czasowe, a następnie generuje wykres skalowalności.

import subprocess
import matplotlib.pyplot as plt
import time

## Ścieżka do pliku wykonywalnego programu obliczeniowego.
executable = "./pi_calc.exe"  

## Lista zawierająca liczbę kroków (precyzję) do przetestowania.
steps_list = [100_000_000, 1_000_000_000, 3_000_000_000]

## Zakres liczby wątków, dla których zostaną przeprowadzone testy (od 1 do 50).
threads_list = list(range(1, 51))

## Słownik przechowujący wyniki pomiarów czasu dla poszczególnych konfiguracji.
results = {steps: [] for steps in steps_list}

# Pętla główna przeprowadzająca testy
for steps in steps_list:
    print(f"Testowanie dla kroków: {steps}")
    for threads in threads_list:
        start = time.time()
        
        # Uruchomienie procesu zewnętrznego
        process = subprocess.run([executable, str(steps), str(threads)], 
                                 capture_output=True, text=True)
        
        output = process.stdout.splitlines()
        try:
            # Parsowanie wyjścia programu C++ w poszukiwaniu czasu obliczeń
            duration = float([line for line in output if "Czas" in line][0].split(":")[1].split()[0])
            results[steps].append(duration)
            print(f"  Wątki: {threads} -> {duration}s")
        except Exception as e:
            print(f"  Błąd dla {threads} wątków: {e}")

# Sekcja generowania wykresu
plt.figure(figsize=(12, 7))
for steps, times in results.items():
    plt.plot(threads_list, times, label=f'Kroki: {steps:,}')

plt.xlabel('Liczba wątków')
plt.ylabel('Czas wykonania (s)')
plt.title('Skalowanie wydajności obliczeń PI')
plt.legend()
plt.grid(True)

# Zapis wykresu do pliku graficznego
plt.savefig('wykres_wydajnosci.png')
plt.show()