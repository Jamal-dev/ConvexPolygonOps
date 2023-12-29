#pragma once

class Polygon;
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <filesystem> // C++17 feature
#include <stdexcept>

Polygon readPolygonFromCSV(const std::string& filename);
Polygon readPolygonInput(const std::string& defaultFileName);
void savePolygonToFile(const Polygon& polygon, const std::string& filename);
void runPolygonOperations();

