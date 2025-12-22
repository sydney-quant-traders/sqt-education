import numpy as np
import matplotlib
import matplotlib.pyplot as plt

# Configure matplotlib to use PGF backend for LaTeX compatibility
matplotlib.use("pgf")
matplotlib.rcParams.update({
    "pgf.texsystem": "pdflatex",
    "font.family": "serif",
    "font.size": 11,
    "text.usetex": True,
    "pgf.rcfonts": False,
})

def simulate_brownian_motion(n_steps=1000, dt=0.01, n_paths=5):
    """
    Simulate Brownian motion paths.
    
    Parameters:
    -----------
    n_steps : int
        Number of time steps
    dt : float
        Time step size
    n_paths : int
        Number of paths to simulate
        
    Returns:
    --------
    t : numpy array
        Time points
    paths : numpy array
        Brownian motion paths (shape: n_paths x n_steps+1)
    """
    # Time array
    t = np.linspace(0, n_steps * dt, n_steps + 1)
    
    # Generate random increments from normal distribution
    # dW = sqrt(dt) * N(0, 1)
    dW = np.sqrt(dt) * np.random.randn(n_paths, n_steps)
    
    # Initialize paths array
    paths = np.zeros((n_paths, n_steps + 1))
    
    # Compute cumulative sum to get Brownian motion paths
    paths[:, 1:] = np.cumsum(dW, axis=1)
    
    return t, paths

def plot_brownian_motion(t, paths, save_path='brownian_motion.pgf'):
    """
    Plot Brownian motion paths and save as PGF file.
    
    Parameters:
    -----------
    t : numpy array
        Time points
    paths : numpy array
        Brownian motion paths
    save_path : str
        Output file path
    """
    # Create figure with specific size (in inches)
    fig, ax = plt.subplots(figsize=(8, 5))
    
    # Plot each path
    for i, path in enumerate(paths):
        ax.plot(t, path, alpha=0.7, linewidth=1.5, label=f'Path {i+1}')
    
    # Add labels and title
    ax.set_xlabel('Time $t$')
    ax.set_ylabel('Position $W(t)$')
    ax.set_title('Brownian Motion Simulation')
    ax.grid(True, alpha=0.3)
    ax.legend(loc='best')
    
    # Add zero line for reference
    ax.axhline(y=0, color='k', linestyle='--', linewidth=0.5, alpha=0.5)
    
    # Save as PGF file
    plt.savefig(save_path, bbox_inches='tight')
    print(f"Plot saved to {save_path}")
    
    # Also save as PDF for preview
    pdf_path = save_path.replace('.pgf', '.pdf')
    plt.savefig(pdf_path, bbox_inches='tight')
    print(f"PDF preview saved to {pdf_path}")
    
    plt.close()

def main():
    """Main function to run the simulation and create plots."""
    # Set random seed for reproducibility
    np.random.seed(42)
    
    # Simulation parameters
    n_steps = 1000      # Number of time steps
    dt = 0.01           # Time step size
    n_paths = 5         # Number of paths to simulate
    
    print("Simulating Brownian motion...")
    print(f"Parameters: {n_steps} steps, dt={dt}, {n_paths} paths")
    
    # Simulate Brownian motion
    t, paths = simulate_brownian_motion(n_steps=n_steps, dt=dt, n_paths=n_paths)
    
    # Plot and save
    print("Creating plot...")
    plot_brownian_motion(t, paths, save_path='brownian_motion.pgf')
    
    # Print some statistics
    print("\nStatistics:")
    print(f"Final positions: {paths[:, -1]}")
    print(f"Maximum excursions: {np.max(np.abs(paths), axis=1)}")
    print(f"Mean final position: {np.mean(paths[:, -1]):.3f}")
    print(f"Std of final position: {np.std(paths[:, -1]):.3f}")
    print(f"Theoretical std at t={t[-1]:.1f}: {np.sqrt(t[-1]):.3f}")

if __name__ == "__main__":
    main()