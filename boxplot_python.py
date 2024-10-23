import matplotlib.pyplot as plt
import numpy as np

# Function to plot boxplots without repeated outliers
def plot_boxplots(filenames, output_file, xlabel, ylabel, xtick_labels):
    data = []
    
    # Load data from each file (assuming each file has one column of data)
    for filename in filenames:
        data.append(np.loadtxt(filename))

    # Create figure and axis
    fig, ax = plt.subplots()

    # Create the boxplot
    boxplot = ax.boxplot(data, patch_artist=True, showfliers=False)

    # Plot outliers manually, ensuring no repeated outliers
    for i, d in enumerate(data, 1):  # i = 1-based index, d = data for each boxplot
        # if filename == "ttex_8":
        #     for value in d:
        #         if value > 100000:
        #             print(value)
        q1 = np.percentile(d, 25)
        q3 = np.percentile(d, 75)
        iqr = q3 - q1
        lower_bound = q1 - 1.5 * iqr
        upper_bound = q3 + 1.5 * iqr

        # Get outliers
        outliers = d[(d < lower_bound) | (d > upper_bound)]

        # Ensure no duplicate outliers by converting to a set
        unique_outliers = set(outliers)

        # Plot each outlier at the correct x position (i) with y values as the unique outliers
        ax.plot([i] * len(unique_outliers), list(unique_outliers), 'r.', markersize=0.5)

    # Set labels and title
    ax.set_xticks(range(1, len(xtick_labels) + 1))
    ax.set_xticklabels(xtick_labels)
    
    # Set y-axis limits based on the data
    # all_data = np.concatenate(data)
    # ax.set_ylim(bottom=min(all_data), top=max(all_data))

    # # Set y-ticks manually with the specified interval
    # y_min, y_max = ax.get_ylim()
    # ax.set_yticks(np.arange(y_min, y_max, y_tick_interval))

    ax.set_xlabel(xlabel)
    ax.set_ylabel(ylabel)
    ax.set_yscale('log')

    # Save the plot to an output file
    plt.savefig(output_file)

# List of input files (each file can have different numbers of values)
filenames = ['ttex_2', 'ttex_3', 'ttex_4', 'ttex_5', 'ttex_6']

# X-axis tick labels
xtick_labels = ["1", "2", "3", "4", "5"]

# Plot settings
xlabel = "Number of T-Tex Iterations: Security Threshold: 60000 (10^4 <= 60000 <= 10^5)"
ylabel = "Execution time in ns (log10)"
#ylabel = "Execution time in ns"
output_file = "boxplot_output_shared_lib_secure_5.png"
y_tick_interval = 100000

# Generate the plot
plot_boxplots(filenames, output_file, xlabel, ylabel, xtick_labels)
