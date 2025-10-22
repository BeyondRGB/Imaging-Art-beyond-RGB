#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "utils/color_conversions.hpp"
#include "reference_data/white_points.hpp"
#include "reference_data/standard_observer.hpp"
#include "reference_data/illuminants.hpp"

using namespace btrgb;
using Catch::Approx;

TEST_CASE("XYZ to Lab conversion with D65 white point", "[color][conversions]") {
    // Test with D65 standard illuminant and 2-degree observer (1931)
    WhitePoints wp(StandardObserver::SO_1931, Illuminants::D65);
    
    SECTION("Pure white should convert to L=100, a=0, b=0") {
        XYZ_t white;
        white.x = wp.get_white_point(WhitePoints::Xn);
        white.y = wp.get_white_point(WhitePoints::Yn);
        white.z = wp.get_white_point(WhitePoints::Zn);
        
        Lab_t lab = xyz_2_Lab(white, &wp);
        
        REQUIRE(lab.L == Approx(100.0).margin(1.0));
        REQUIRE(lab.a == Approx(0.0).margin(1.0));
        REQUIRE(lab.b == Approx(0.0).margin(1.0));
    }
    
    SECTION("Pure black should convert to L=0") {
        XYZ_t black;
        black.x = 0.0;
        black.y = 0.0;
        black.z = 0.0;
        
        Lab_t lab = xyz_2_Lab(black, &wp);
        
        REQUIRE(lab.L == Approx(0.0).margin(0.1));
    }
    
    SECTION("Mid-gray should have L between 40 and 80") {
        XYZ_t gray;
        gray.x = wp.get_white_point(WhitePoints::Xn) * 0.5;
        gray.y = wp.get_white_point(WhitePoints::Yn) * 0.5;
        gray.z = wp.get_white_point(WhitePoints::Zn) * 0.5;
        
        Lab_t lab = xyz_2_Lab(gray, &wp);
        
        // L* is not linear, but should be in this range
        REQUIRE(lab.L > 40.0);
        REQUIRE(lab.L < 80.0);
    }
}

TEST_CASE("XYZ to Lab conversion mathematical properties", "[color][conversions]") {
    WhitePoints wp(StandardObserver::SO_1931, Illuminants::D65);
    
    SECTION("L* should always be between 0 and 100") {
        // Test various XYZ values
        for (double factor = 0.0; factor <= 1.0; factor += 0.2) {
            XYZ_t xyz;
            xyz.x = wp.get_white_point(WhitePoints::Xn) * factor;
            xyz.y = wp.get_white_point(WhitePoints::Yn) * factor;
            xyz.z = wp.get_white_point(WhitePoints::Zn) * factor;
            
            Lab_t lab = xyz_2_Lab(xyz, &wp);
            
            // Allow small floating point errors
            REQUIRE(lab.L >= -0.001);
            REQUIRE(lab.L <= 100.001);
        }
    }
    
    SECTION("Increasing Y (luminance) should increase L*") {
        XYZ_t xyz1;
        xyz1.x = 50.0;
        xyz1.y = 25.0;
        xyz1.z = 50.0;
        
        XYZ_t xyz2;
        xyz2.x = 50.0;
        xyz2.y = 75.0;  // Higher luminance
        xyz2.z = 50.0;
        
        Lab_t lab1 = xyz_2_Lab(xyz1, &wp);
        Lab_t lab2 = xyz_2_Lab(xyz2, &wp);
        
        REQUIRE(lab2.L > lab1.L);
    }
}

TEST_CASE("WhitePoints class API", "[color][utils]") {
    SECTION("Can create WhitePoints with different illuminants") {
        WhitePoints d65(StandardObserver::SO_1931, Illuminants::D65);
        WhitePoints d50(StandardObserver::SO_1931, Illuminants::D50);
        WhitePoints a(StandardObserver::SO_1931, Illuminants::A);
        
        // D65 should have different values than D50
        REQUIRE(d65.get_white_point(WhitePoints::Xn) != d50.get_white_point(WhitePoints::Xn));
    }
    
    SECTION("White point values should be positive") {
        WhitePoints wp(StandardObserver::SO_1931, Illuminants::D65);
        
        REQUIRE(wp.get_white_point(WhitePoints::Xn) > 0.0);
        REQUIRE(wp.get_white_point(WhitePoints::Yn) > 0.0);
        REQUIRE(wp.get_white_point(WhitePoints::Zn) > 0.0);
    }
}
