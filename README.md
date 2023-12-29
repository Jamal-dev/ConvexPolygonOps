# Polygon Operations Project

## Overview
This C++ project performs geometric operations on convex polygons, supporting operations like union, intersection, and subtraction. It includes a Python script for visualizing results and is built using C++17 standards. The project also incorporates unit tests using Google Test (gtest).

## File Structure

PolygonOpsProject
├───dlls
│   └───gtestx64_debug
├───include
├───src
├───tests
└───x64
    ├───Debug
    │   └───PolygonOpts.tlog
    └───Release
        └───PolygonOpts.tlog


## Building the Project
1. **Prerequisites**:
   - Visual Studio with C++17 support.
   - Google Test (gtest) installed in Visual Studio.

2. **Clone the Repository**:
   ```
   git clone https://github.com/Jamal-dev/ConvexPolygonOps
   ```

3. **Open the Solution**:
   - Open the solution file (`PolygonOpsProject.sln`) in Visual Studio.

4. **Build the Solution**:
   - Select the desired build configuration (Debug or Release).
   - Build the solution by going to `Build -> Build Solution`.

## Running the Project
- After building the project, run the executable from the `Debug` or `Release` directory.
- The application runs in input mode by default. Follow the on-screen prompts to input polygon data either by entering coordinates or providing CSV file paths. CSV files should have the first column for x-coordinates and the second column for y-coordinates.
- To run in test mode, use the command-line argument `test` to execute all unit tests.

## Visualizing Results
- The project includes a Python script `visualize.py` for visualizing the polygon operations.
- Ensure Python is installed and set up on your system.

1. **Install Python Dependencies**:
   - Navigate to the project directory.
   - Install required packages using:
     ```
     pip install -r requirements.txt
     ```

2. **Run the Visualization Script**:
   - After performing polygon operations, run:
     ```
     python visualize.py
     ```
   - The script reads input data and the output from CSV files and displays the results.

## Installing Google Test in Visual Studio
1. **Open Visual Studio Installer**:
   - Modify your Visual Studio installation.

2. **Modify Workloads**:
   - Ensure the "Desktop development with C++" workload is installed.

3. **Install Google Test**:
   - Go to the Individual components tab.
   - Scroll down to the "Testing tools" section.
   - Check "Google Test Adapter" and "Test Adapter for Google Test".
   - Click Modify to install.

4. **Add Google Test to Your Project**:
   - In your project, right-click on the solution.
   - Choose "Manage NuGet Packages for Solution".
   - Search for "GoogleTest" and install it for your project.

## Contributing
- Contributions to the project are welcome. Please follow the standard Git workflow for contributions.

## License
- MIT License

## Contact
- jamalahmed68@gmail.com
```

