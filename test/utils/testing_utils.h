/*
 *  PGMcpp : PRIMED Grid Modelling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file testing_utils.h
///
/// \brief Header file for various PGMcpp testing utilities.
///
/// This is a library of utility functions used throughout the various test suites.
///


#ifndef TESTING_UTILS_H
#define TESTING_UTILS_H


#include "../../header/std_includes.h"

#ifndef FLOAT_TOLERANCE
    ///
    /// \def FLOAT_TOLERANCE
    ///
    /// \brief A tolerance for application to floating point equality tests.
    ///
    
    #define FLOAT_TOLERANCE 1e-6
#endif  /* FLOAT_TOLERANCE */


#ifdef _WIN32
    void activateVirtualTerminal(void);
#endif  /* _WIN32 */


void printGreen(std::string);
void printGold(std::string);
void printRed(std::string);

void testFloatEquals(double, double, std::string, int);
void testGreaterThan(double, double, std::string, int);
void testGreaterThanOrEqualTo(double, double, std::string, int);
void testLessThan(double, double, std::string, int);
void testLessThanOrEqualTo(double, double, std::string, int);
void testTruth(bool, std::string, int);

void expectedErrorNotDetected(std::string, int);


#endif  /* TESTING_UTILS_H */
