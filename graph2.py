import matplotlib.pyplot as plt
import pandas as pd

# Load CSV
df = pd.read_csv("all.cpp")

# Plot
plt.figure(figsize=(10, 6))
plt.plot(df["Digits"], df["AvgFermat(us)"], label="Fermat", marker="o")
plt.plot(df["Digits"], df["AvgMillerRabin(us)"], label="Miller-Rabin", marker="s")

# Optional: Log scale Y axis (uncomment to use)
# plt.yscale('log')

# Labels and title
plt.xlabel("Number of Digits")
plt.ylabel("Average Time (microseconds)")
plt.title("Fermat vs Miller-Rabin Primality Test (Time vs Digits)")
plt.legend()
plt.grid(True)

# Show plot
plt.tight_layout()
plt.show()
