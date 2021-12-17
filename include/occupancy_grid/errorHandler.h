/**
 * @file errorHandler.h
 * Error handler for Occupancy Grid
 */
#pragma once
#include <exception> 
#include <string>

/**
 * @brief 
 * Error handler for incorrect size for the occupancy grid
 */
class IncorrectSize : public std::exception {

    private:
        std::string _msg;
    public:
        explicit IncorrectSize(const char* message)
        : _msg(message) {}

        virtual ~IncorrectSize() noexcept {}

        virtual const char* what() const noexcept override {
       return _msg.c_str();
    }
};

/**
 * @brief 
 * Error handler for incorrect resolution of the grid cell
 */
class IncorrectResolution : public std::exception {

    private:
        std::string _msg;
    public:
        explicit IncorrectResolution(const char* message)
        : _msg(message) {}

        virtual ~IncorrectResolution() noexcept {}

        virtual const char* what() const noexcept override {
       return _msg.c_str();
    }
};