#include "PolygonIO.hpp"
#include "Point.hpp"
#include "Polygon.hpp"
#include "GeometryUtils.hpp"
#include "PolygonOperations.hpp"

std::string createAbsolutePath(const std::string& relativeFilePath) {
    std::filesystem::path currentDir = std::filesystem::current_path();
    std::filesystem::path filePath(relativeFilePath);
    std::filesystem::path fullPath = currentDir / filePath;
    return fullPath.string();
}

Polygon readPolygonFromCSV(const std::string& filename) {
    std::string absolutePath = createAbsolutePath(filename);
    std::cout << "Attempting to open file at: "
        << absolutePath << std::endl;
    std::ifstream file(absolutePath);
    if (!file) {
        std::cerr << "Error: Unable to open file '" << filename << std::endl;
        return Polygon(); // Return an empty Polygon or handle it as needed
    }
    std::cout<<"File opened successfully."<<std::endl;
    std::vector<Point> points;
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string xStr, yStr;
        if (std::getline(iss, xStr, ',') && std::getline(iss, yStr)) {
            try {
                double x = std::stod(xStr);
                double y = std::stod(yStr);
                points.emplace_back(x, y);
            }
            catch (const std::invalid_argument& e) {
                std::cerr << "Invalid data in file: " << e.what() << "\n";
                return Polygon(); // Return an empty Polygon or handle it as needed
            }
        }
    }
    file.close();
    if (!GeometryUtils::isPolygonConvex(points))
    {
        std::cout << "Polygon is not convex. Exiting." << std::endl;
        return Polygon();
    }
    return GeometryUtils::createPolygon(points);
}

Polygon readPolygonInput(const std::string& defaultFileName) {
    std::string input;
    std::cout << "Enter coordinates (x1 y1 x2 y2 ...) or CSV file name[" << defaultFileName << "]: ";
    std::getline(std::cin, input);

    // Use default file name if input is empty
    if (input.empty()) {
        input = defaultFileName;
    }

    if (input.find(".csv") != std::string::npos) {
        // Read from CSV file
        return readPolygonFromCSV(input);
    }
    else {
        // Read coordinates from input
        std::vector<Point> points;
        std::istringstream iss(input);
        double x, y;
        while (iss >> x) {
            if (!(iss >> y)) {
                std::cerr << "Invalid coordinate format. Please enter pairs of numbers." << std::endl;
                return Polygon(); // Return an empty Polygon or handle the error as needed
            }
            points.emplace_back(x, y);
        }
        return GeometryUtils::createPolygon(points);
    }
}

void savePolygonToFile(Polygon& polygon, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& point : polygon.getPoints()) {
        file << point.x << ", " << point.y << std::endl;
    }
    file.close();
}

void runPolygonOperations() {
    std::cout << "Polygon Operations"<<std::endl;

    std::cout << "Enter details for Polygon A:" << std::endl;
    Polygon A = readPolygonInput("PolyA.csv");


    std::cout << "Enter details for Polygon B:" << std::endl;
    Polygon B = readPolygonInput("PolyB.csv");


    std::cout << "Choose operation (union, intersection, subtraction) [intersection]: ";
    std::string operation;
    std::getline(std::cin, operation);

    // Set default operation to intersection if input is empty
    if (operation.empty()) {
        operation = "intersection";
    }

    Polygon result;
    if (operation == "union") {
        result = unionConvexPolygons(A, B);
    }
    else if (operation == "intersection") {
        result = intersectionConvexPolygons(A, B);
    }
    else if (operation == "subtraction") {
        result = differenceConvexPolygons(A, B);
    }
    else {
        std::cout << "Invalid operation. Performing default operation: intersection." << std::endl;
        result = intersectionConvexPolygons(A, B);
    }

    savePolygonToFile(result, "output.csv");
    std::cout << "Operation completed. Run 'python visualize.py' to see the result." << std::endl;
}
