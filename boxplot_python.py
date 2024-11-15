import matplotlib.pyplot as plt
import numpy as np

# Function to plot boxplots without repeated outliers
def plot_boxplots(filenames, output_file, xlabel, ylabel, xtick_labels):
    data = []
    
    # Load data from each file (assuming each file has one column of data)
    for filename in filenames:
        data.append(np.loadtxt(filename))

    min_ttex_val = 0
    min_tsys_val = 0

    for filename1 in filenames:
        if filename1 == "ttex_6":
            data1 = []
            data1.append(np.loadtxt(filename1))
            for i, d in enumerate(data1, 1):
                for value in d:
                    if value > min_ttex_val:
                        min_ttex_val = value

    for filename2 in filenames: 
        if filename2 == "ttex_tsys2_6":
            data2 = []
            data2.append(np.loadtxt(filename2))
            for i, d in enumerate(data2, 1):  
                for value in d:
                    if value > min_tsys_val:
                        min_tsys_val = value
                        #print(filename)
                        #print(min_tsys_val)

    # Create figure and axis
    fig, ax = plt.subplots()

    # Create the boxplot
    boxplot = ax.boxplot(data, patch_artist=True, showfliers=False)

    # Plot outliers manually, ensuring no repeated outliers
    for i, d in enumerate(data, 1):  # i = 1-based index, d = data for each boxplot
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
    print(min_tsys_val)
    print(min_ttex_val)
    ax.axhline(y=min_tsys_val, color='gray', linestyle='--', linewidth=0.8)
    ax.axhline(y=min_ttex_val, color='gray', linestyle='--', linewidth=0.8)
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
#['ttex_6', 'ttex_10000', 'ttex_20000', 'ttex_40000', 'ttex_80000', 'ttex_tsys', "ttex_kernel_6", 
#filenames = ["ttex_traffic_kernel_1", "ttex_kernel_time_2"]#, 'ttex_200000']#, 'ttex_6', 'ttex_7'] #, 'ttex_8']#, 'ttex_9', 'ttex_10']
filenames = ['ttex_6', 'ttex_10000', 'ttex_20000', 'ttex_40000', 'ttex_500000', 'ttex_tsys2_6']
#filenames = ['ttex_2', 'ttex_3', 'ttex_4', 'ttex_5', 'ttex_6']#, 'ttex_tsys2_6']
# X-axis tick labels
xtick_labels = ["1", "2", "3", "4", "5", "6"]#, "8", "9"]#, "10"]

# Plot settings
#xlabel = "Number of T-Tex Iterations: Security Threshold: 60us"
xlabel = "1.T-Tex   2. 10us    3. 20us   4. 40us  5. 500us  6. T-SYS (No Attack)"
ylabel = "Execution time in ns (log10)"
#ylabel = "Execution time in ns"
output_file = "security_attack.png"
y_tick_interval = 100000

# Generate the plot
plot_boxplots(filenames, output_file, xlabel, ylabel, xtick_labels)
