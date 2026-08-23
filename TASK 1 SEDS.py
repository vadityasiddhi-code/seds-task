#ADITYA SIDDHI
#F2026B3PS0477H

import csv
import matplotlib.pyplot as plt

# --- Configuration & Parameters ---
MAX_DEVIATION_LIMIT = 50.0
FILE_NAME = "Depth Data.csv"

# --- Data Containers ---
time_series = []
processed_depths = []

# For visualizing distinct data categories
clean_t, clean_z = [], []
corrected_t, corrected_z = [], []

# --- Processing State ---
latest_stable_depth = None
anomalies_detected = 0

# --- File Reading & Data Processing ---
# Using the csv module provides a cleaner way to handle file parsing
try:
    with open(FILE_NAME, mode="r", encoding="utf-8") as dataset:
        csv_reader = csv.reader(dataset)
        
        for row in csv_reader:
            # Skip empty lines or improperly formatted rows
            if not row or len(row) < 2:
                continue
                
            # Heuristic to bypass header rows
            if 'time' in row[0].lower() or 'point' in row[0].lower():
                continue
                
            t_val = float(row[0])
            is_anomaly = False
            
            # 1. Check for corrupted text data
            try:
                z_val = float(row[1])
            except ValueError:
                is_anomaly = True
                
            # 2. Check for extreme jumps (only if we have a baseline and data is a number)
            if not is_anomaly and latest_stable_depth is not None:
                if abs(z_val - latest_stable_depth) > MAX_DEVIATION_LIMIT:
                    is_anomaly = True
                    
            # 3. Handle the data point based on our checks
            if is_anomaly:
                anomalies_detected += 1
                # If we have a previous good reading, use it to patch the bad data
                if latest_stable_depth is not None:
                    time_series.append(t_val)
                    processed_depths.append(latest_stable_depth)
                    
                    corrected_t.append(t_val)
                    corrected_z.append(latest_stable_depth)
            else:
                # Update our baseline with the new good reading
                latest_stable_depth = z_val
                
                time_series.append(t_val)
                processed_depths.append(z_val)
                
                clean_t.append(t_val)
                clean_z.append(z_val)

except FileNotFoundError:
    print(f"Error: Could not find '{FILE_NAME}'. Please ensure the file is in the same directory.")

# --- Visualization & Graph Styling ---
# Apply a built-in matplotlib style for a completely different base look
plt.style.use('seaborn-v0_8-darkgrid') 

# Setup the figure with a custom background color
fig, ax = plt.subplots(figsize=(11, 6))
fig.patch.set_facecolor('#e9ecef') # Light gray border
ax.set_facecolor('#ffffff')        # White plot background

# Plot 1: The underlying path connecting all points (Subtle blue-gray)
ax.plot(time_series, processed_depths, color='#34495e', linewidth=1.5, alpha=0.5, label='Interpolated Path')

# Plot 2: Clean/Valid Data Points (Emerald Green circles)
ax.scatter(clean_t, clean_z, color='#2ecc71', s=35, edgecolor='black', linewidth=0.5, 
           label='Verified Telemetry', zorder=3)

# Plot 3: Corrected/Replaced Data Points (Crimson Red 'X' markers)
if corrected_t:
    ax.scatter(corrected_t, corrected_z, color='#e74c3c', s=60, marker='X', edgecolor='black', 
               linewidth=0.5, label='Filtered Anomalies', zorder=4)

# Customizing Grid, Titles, and Labels
ax.grid(True, linestyle='-.', color='#bdc3c7', alpha=0.7)
ax.set_title("Avionics Telemetry: Subsystem Depth Analysis (Task 1)", fontsize=14, fontweight='900', pad=15, color='#2c3e50')
ax.set_xlabel("Elapsed Mission Time (s)", fontsize=11, fontweight='bold', color='#2c3e50')
ax.set_ylabel("Depth Measurement (m)", fontsize=11, fontweight='bold', color='#2c3e50')

# Adding a new feature: A statistics overlay box in the corner
stats_text = (
    f"Data Summary\n"
    f"--------------------\n"
    f"Total Points: {len(time_series)}\n"
    f"Anomalies Fixed: {anomalies_detected}"
)
props = dict(boxstyle='round', facecolor='#f8f9fa', alpha=0.9, edgecolor='#bdc3c7')
ax.text(0.02, 0.95, stats_text, transform=ax.transAxes, fontsize=10,
        verticalalignment='top', bbox=props, color='#2c3e50')

# Legend customization
ax.legend(loc='lower right', frameon=True, shadow=True, facecolor='white', edgecolor='gray')

# Render the graph
plt.tight_layout()
plt.show()
