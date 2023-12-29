import matplotlib.pyplot as plt
import pandas as pd
import argparse

def plot_polygon(ax, filename, label, color, close_polygon=True):
    df = pd.read_csv(filename, header=None, names=['x', 'y'])
    ax.plot(df['x'], df['y'], label=label, color=color)
    ax.scatter(df['x'], df['y'], color=color)
    
    if close_polygon:
        # Connect the last point to the first to close the polygon
        first_point = df.iloc[0]
        last_point = df.iloc[-1]
        ax.plot([last_point['x'], first_point['x']], [last_point['y'], first_point['y']], color=color)

def set_same_scale(axs):
    # Find the global min and max values for x and y
    x_min, x_max, y_min, y_max = float('inf'), float('-inf'), float('inf'), float('-inf')
    for ax in axs:
        xlim = ax.get_xlim()
        ylim = ax.get_ylim()
        x_min = min(x_min, xlim[0])
        x_max = max(x_max, xlim[1])
        y_min = min(y_min, ylim[0])
        y_max = max(y_max, ylim[1])
    
    # Set the same scale for all axes
    for ax in axs:
        ax.set_xlim(x_min, x_max)
        ax.set_ylim(y_min, y_max)

def main(polyA_filename, polyB_filename, output_filename, close_polygon):
    fig, axs = plt.subplots(1, 2, figsize=(10, 5))

    plot_polygon(axs[0], polyA_filename, 'Polygon A', 'blue', close_polygon)
    plot_polygon(axs[0], polyB_filename, 'Polygon B', 'red', close_polygon)
    axs[0].set_title('Input Polygons')
    axs[0].legend()

    plot_polygon(axs[1], output_filename, 'Result', 'green', close_polygon)
    axs[1].set_title('Result')

    set_same_scale(axs)
    
    plt.show()

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Visualize Polygons")
    parser.add_argument('--polyA', default='PolyA.csv', help='CSV file name for Polygon A')
    parser.add_argument('--polyB', default='PolyB.csv', help='CSV file name for Polygon B')
    parser.add_argument('--output', default='output.csv', help='CSV file name for the output polygon')
    parser.add_argument('--close', action='store_true', default=True, help='Close the polygon by connecting the last point to the first')

    args = parser.parse_args()
    main(args.polyA, args.polyB, args.output, args.close)
