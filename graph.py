import matplotlib.pyplot as plt

digit_lengths = []
miller_times = []
fermat_times = []
solovay_times = []

# Load Miller-Rabin times
with open("./miller.csv") as f:
    next(f)  # skip header
    for line in f:
        digits, time = line.strip().split(",")
        digit_lengths.append(int(digits))
        miller_times.append(float(time))

# Load Fermat times
with open("./fermats.csv") as f:
    next(f)
    for line in f:
        _, time = line.strip().split(",")
        fermat_times.append(float(time))

# Load Solovay-Strassen times
with open("./solovay.csv") as f:
    next(f)
    for line in f:
        _, time = line.strip().split(",")
        solovay_times.append(float(time))

# Plotting
plt.figure(figsize=(12, 6))
plt.plot(digit_lengths, miller_times, label="Miller-Rabin (k=5)", marker="o")
plt.plot(digit_lengths, fermat_times, label="Fermat (k=10)", marker="s")
plt.plot(digit_lengths, solovay_times, label="Solovay-Strassen (k=10)", marker="^")

plt.xlabel("Number of Digits")
plt.ylabel("Time (seconds)")
plt.title("Primality Test Timing by Number of Digits (k values shown)")
plt.legend()
plt.grid(True)
plt.tight_layout()
plt.show()
