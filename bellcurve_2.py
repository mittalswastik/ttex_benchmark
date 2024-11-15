from sklearn import datasets
import pandas as pd
import seaborn as sns
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as patches
import matplotlib.ticker as ticker
from mpl_toolkits.axes_grid1.inset_locator import inset_axes, mark_inset

# Define paths to files for each column
file_paths = ['ttex_6', 'ttex_10000', 'ttex_40000', 'ttex_tsys2_6'] # Replace with actual paths
#file_paths = ['ttex_3', 'ttex_400000']
# Read each file as a separate column
column1_values = np.genfromtxt(file_paths[0], delimiter='\t')
column2_values = np.genfromtxt(file_paths[1], delimiter='\t')
column3_values = np.genfromtxt(file_paths[2], delimiter='\t')
column4_values = np.genfromtxt(file_paths[3], delimiter='\t')


# Step 1: Find the percentiles to determine an appropriate zoom-in region
combined_data = np.concatenate([column1_values, column4_values])
x_zoom_start, x_zoom_end = np.percentile(combined_data, [99.3, 100])  # Tail-end percentiles
y_zoom_end = 0.000001  # Adjust this based on initial plot inspection

# Step 2: Main KDE Plot
plt.figure(figsize=(10, 6))
sns.kdeplot(column1_values, color='red', shade=True, bw_adjust=0.5, label='T-Tex (No Attack)')
sns.kdeplot(column2_values, color='purple', shade=False, bw_adjust=0.5, label='10us Attack')
sns.kdeplot(column3_values, color='blue', shade=True, bw_adjust=0.5, label='40us Attack')
sns.kdeplot(column4_values, color='green', shade=False, bw_adjust=0.5, label='T-SYS (No Attack)')

# Customize the main plot
plt.xlabel('Code Region Execution Time (ns)')
plt.ylabel('Probability Density')
plt.legend(loc="lower right")
plt.tight_layout()

main_ax = plt.gca()  # Get current axis
rect = patches.Rectangle((x_zoom_start, 0), x_zoom_end - x_zoom_start, y_zoom_end,
                         linewidth=4, edgecolor='black', linestyle="--", facecolor='none')
main_ax.add_patch(rect)

# Step 3: Create the inset zoom region
ax_inset = inset_axes(plt.gca(), width="75%", height="75%", loc="upper right")
sns.kdeplot(column1_values, color='red', shade=True, bw_adjust=0.5, ax=ax_inset)
#sns.kdeplot(column2_values, color='purple', shade=False, bw_adjust=0.5, ax=ax_inset)
sns.kdeplot(column3_values, color='blue', shade=True, bw_adjust=0.5, ax=ax_inset)
sns.kdeplot(column4_values, color='green', shade=False, bw_adjust=0.5, ax=ax_inset)

# Set x and y limits for the inset
ax_inset.set_xlim(x_zoom_start, x_zoom_end)
ax_inset.set_ylim(0, y_zoom_end)  # Set a small range for low probability densities
ax_inset.set_xticks([])
ax_inset.set_yticks([])

main_ax.set_yscale('linear')  # Ensure it is on a linear scale
main_ax.xaxis.set_major_formatter(ticker.ScalarFormatter(useMathText=True))  # x-axis in scientific notation
main_ax.yaxis.set_major_formatter(ticker.ScalarFormatter(useMathText=True)) 

ax_inset.tick_params(axis='both', which='major', labelsize=8)  # Adjust tick label size if needed


# Add connecting lines to highlight zoomed area
mark_inset(plt.gca(), ax_inset, loc1=2, loc2=4, fc="none", ec="0.5")

plt.ticklabel_format(style='sci', axis='x', scilimits=(0, 0))  # For x-axis scientific notation
plt.ticklabel_format(style='sci', axis='y', scilimits=(0, 0))  # For y-axis scientific notation


# Save and show the plot
plt.savefig('bell_curves_with_zoom_new.png', format='png', dpi=300)
plt.show()
