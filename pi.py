 import subprocess
import matplotlib.pyplot as plt
import time

# Konfiguracja
executable = "./pi_calc.exe"  # lub ./pi_calc na Linux
steps_list = [100_000_000, 1_000_000_000, 3_000_000_000]
threads_list = list(range(1, 51))

results = {steps: [] for steps in steps_list}

for steps in steps_list:
    print(f"Testowanie dla kroków: {steps}")
    for threads in threads_list:
        start = time.time()
        process = subprocess.run([executable, str(steps), str(threads)], 
                                 capture_output=True, text=True)
        
        # Parsowanie czasu z outputu programu C++
        output = process.stdout.splitlines()
        try:
            # Szukamy linii z czasem
            duration = float([line for line in output if "Czas" in line][0].split(":")[1].split()[0])
            results[steps].append(duration)
            print(f"  Wątki: {threads} -> {duration}s")
        except Exception as e:
            print(f"  Błąd dla {threads} wątków: {e}")

# Rysowanie wykresu
plt.figure(figsize=(12, 7))
for steps, times in results.items():
    plt.plot(threads_list, times, label=f'Kroki: {steps:,}')

plt.xlabel('Liczba wątków')
plt.ylabel('Czas wykonania (s)')
plt.title('Skalowanie wydajności obliczeń PI')
plt.legend()
plt.grid(True)
plt.savefig('wykres_wydajnosci.png')
plt.show()