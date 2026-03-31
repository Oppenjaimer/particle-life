#include "raylib.h"

#include "utils.hpp"

#include <random>

/**
 * @brief Retrieve random number generator.
 * @returns Reference to the Mersenne Twister generator.
 */
static std::mt19937& get_generator() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    return gen;
}

float utils::get_random_float(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
    return dist(get_generator());
}

int utils::get_random_int(int min, int max) {
    std::uniform_int_distribution<int> dist(min, max);
    return dist(get_generator());
}

bool utils::set_resource_dir(const char* folder_name) {
    // Check current directory
    if (DirectoryExists(folder_name)) {
        ChangeDirectory(TextFormat("%s/%s", GetWorkingDirectory(), folder_name));
        return true;
    }

    const char* app_dir = GetApplicationDirectory();

    // Check app directory
    const char* dir = TextFormat("%s%s", app_dir, folder_name);
    if (DirectoryExists(dir)) {
        ChangeDirectory(dir);
        return true;
    }

    // Check one level up from app directory
    dir = TextFormat("%s../%s", app_dir, folder_name);
    if (DirectoryExists(dir)) {
        ChangeDirectory(dir);
        return true;
    }

    // Check two levels up from app directory
    dir = TextFormat("%s../../%s", app_dir, folder_name);
    if (DirectoryExists(dir)) {
        ChangeDirectory(dir);
        return true;
    }

    return false;
}
