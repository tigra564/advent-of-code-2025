#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <climits>

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/box.hpp>

namespace bg = boost::geometry;

using Point = bg::model::d2::point_xy<long long>;  // Use long long instead of int
using Polygon = bg::model::polygon<Point>;
using Box = bg::model::box<Point>;

struct Result {
    long long area;
    Point p1;
    Point p2;
    
    Result() : area(0), p1(0, 0), p2(0, 0) {}
    Result(long long a, Point a1, Point a2) : area(a), p1(a1), p2(a2) {}
};

std::vector<Point> readPoints(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    std::string line;

    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filename << '\n';
        return points;
    }

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        auto commaPos = line.find(',');
        if (commaPos == std::string::npos) continue;

        try {
            long long x = std::stoll(line.substr(0, commaPos));
            long long y = std::stoll(line.substr(commaPos + 1));
            
            // Overflow check
            if (x < LLONG_MIN/2 || x > LLONG_MAX/2 || y < LLONG_MIN/2 || y > LLONG_MAX/2) {
                std::cerr << "Warning: Skipping out-of-range coordinates: " << x << "," << y << '\n';
                continue;
            }
            
            points.emplace_back(x, y);
        } catch (const std::exception& e) {
            std::cerr << "Warning: Failed to parse line: " << line << " - " << e.what() << '\n';
        }
    }
    return points;
}

Polygon createPolygonFromPoints(const std::vector<Point>& points) {
    Polygon poly;
    for (const auto& p : points) {
        bg::append(poly.outer(), p);
    }
    if (!points.empty() && !bg::equals(points.front(), points.back())) {
        bg::append(poly.outer(), points.front());
    }
    bg::correct(poly);
    return poly;
}

Polygon createPolygonFromBox(const Box& box) {
    Polygon poly;
    bg::append(poly.outer(), box.min_corner());
    bg::append(poly.outer(), Point(box.max_corner().x(), box.min_corner().y()));
    bg::append(poly.outer(), box.max_corner());
    bg::append(poly.outer(), Point(box.min_corner().x(), box.max_corner().y()));
    bg::append(poly.outer(), box.min_corner());
    bg::correct(poly);
    return poly;
}

// Safe calculation of absolute value
long long safeAbs(long long x) {
    return x < 0 ? -x : x;
}

Result solveDay9Part2(const std::vector<Point>& points) {
    if (points.size() < 4) {
        return Result();
    }

    Polygon greenPolygon = createPolygonFromPoints(points);
    
    Result best;
    int n = points.size();

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            const Point& p1 = points[i];
            const Point& p2 = points[j];

            // Safe calculation of min/max
            long long minX = std::min(p1.x(), p2.x());
            long long maxX = std::max(p1.x(), p2.x());
            long long minY = std::min(p1.y(), p2.y());
            long long maxY = std::max(p1.y(), p2.y());

	    // Overflow check for area calculation
            long long length = safeAbs(p1.x() - p2.x()) + 1;
            long long height = safeAbs(p1.y() - p2.y()) + 1;
            
            if (length > LLONG_MAX / height) {
                continue;
            }
            
            long long area = length * height;

            Box rect(Point(minX, minY), Point(maxX, maxY));
            Polygon rectPolygon = createPolygonFromBox(rect);

            try {
                if (bg::covered_by(rectPolygon, greenPolygon)) {
                    if (area > best.area) {
                        best.area = area;
                        best.p1 = Point(minX, minY);
                        best.p2 = Point(maxX, maxY);
                    }
                }
            } catch (const std::exception& e) {
                std::cerr << "Warning: Geometry operation failed: " << e.what() << '\n';
            }
        }
    }

    return best;
}

void printUsage(const char* programName) {
    std::cerr << "Usage: " << programName << " <filename>" << '\n';
    std::cerr << "Example: " << programName << " input.txt" << '\n';
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printUsage(argv[0]);
        return 1;
    }

    std::string filename = argv[1];
    
    try {
        auto points = readPoints(filename);
        if (points.empty()) {
            std::cerr << "Error: No points loaded or file is empty." << '\n';
            return 1;
        }

        Result result = solveDay9Part2(points);
        
        std::cout << "=== Day 9 Part 2 Solution ===" << '\n';
        std::cout << "Maximum area: " << result.area << '\n';
        if (result.area > 0) {
            std::cout << "Rectangle coordinates:" << '\n';
            std::cout << "  Bottom-left:  (" << result.p1.x() << ", " << result.p1.y() << ")" << '\n';
            std::cout << "  Top-right:    (" << result.p2.x() << ", " << result.p2.y() << ")" << '\n';
            std::cout << "  Width:  " << (result.p2.x() - result.p1.x() + 1) << '\n';
            std::cout << "  Height: " << (result.p2.y() - result.p1.y() + 1) << '\n';
        } else {
            std::cout << "No valid rectangle found." << '\n';
        }
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << '\n';
        return 1;
    }

    return 0;
}
