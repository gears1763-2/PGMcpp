/*
 *  PGMcpp : PRIMED Grid Interpolatorling (in C++)
 *
 *  Anthony Truelove MASc, P.Eng.
 *  email:  gears1763@tutanota.com
 *  github: gears1763-2
 *
 *  See license terms
 *
 */


///
/// \file Interpolator.cpp
///
/// \brief Implementation file for the Interpolator class.
///
/// A class which contains interpolation data and functionality. Intended to serve as a
//      component of the Production and Storage hierarchies.
///


#include "../header/Interpolator.h"



// ======== PRIVATE ================================================================= //

// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: __checkDataKey1D(int data_key)
///
/// \brief Helper method to check if given data key (1D) is already in use.
///
/// \param data_key The key associated with the given 1D interpolation data.
///

void Interpolator :: __checkDataKey1D(int data_key)
{
    if (this->interp_map_1D.count(data_key) > 0) {
        std::string error_str = "ERROR:  Interpolator::addData1D()  ";
        error_str += "data key (1D) ";
        error_str += std::to_string(data_key);
        error_str += " is already in use";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkDataKey1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: __checkDataKey2D(int data_key)
///
/// \brief Helper method to check if given data key (2D) is already in use.
///
/// \param data_key The key associated with the given 2D interpolation data.
///

void Interpolator :: __checkDataKey2D(int data_key)
{
    if (this->interp_map_2D.count(data_key) > 0) {
        std::string error_str = "ERROR:  Interpolator::addData2D()  ";
        error_str += "data key (2D) ";
        error_str += std::to_string(data_key);
        error_str += " is already in use";
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    return;
}   /* __checkDataKey2D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn
///

void Interpolator :: __throwReadError(std::string path_2_data, int dimensions)
{
    std::string error_str = "ERROR:  Interpolator::__readData";
    error_str += std::to_string(dimensions);
    error_str += "D()  ";
    error_str += " failed to read ";
    error_str += path_2_data;
    error_str += " (this is probably a std::stod() error; is there non-numeric ";
    error_str += "data where only numeric data should be?)";

    #ifdef _WIN32
        std::cout << error_str << std::endl;
    #endif

    throw std::runtime_error(error_str);
    
    return;
}   /* __throwReadError() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn
///

bool Interpolator :: __isNonNumeric(std::string str)
{
    for (size_t i = 0; i < str.size(); i++) {;
        if (isalpha(str[i])) {
            return true;
        }
    }
    
    return false;
}   /* __isAlpha() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn
///

std::vector<std::string> Interpolator :: __splitCommaSeparatedString(
    std::string str,
    std::string break_str
)
{
    std::vector<std::string> str_split_vec;
    
    size_t idx = 0;
    std::string substr;
    
    while ((idx = str.find(',')) != std::string::npos) {
        substr = str.substr(0, idx);
        
        if (substr == break_str) {
            break;
        }
        
        str_split_vec.push_back(substr);
        
        str.erase(0, idx + 1);
    }
    
    return str_split_vec;
}   /* __splitCommaSeparatedString() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn
///

std::vector<std::vector<std::string>> Interpolator :: __getDataStringMatrix(
    std::string path_2_data
)
{
    //  1. create input file stream
    std::ifstream ifs;
    ifs.open(path_2_data);
    
    //  2. check that open() worked
    if (not ifs.is_open()) {
        std::string error_str = "ERROR:  Interpolator::__getDataStringMatrix()  ";
        error_str += " failed to open ";
        error_str += path_2_data;
        
        #ifdef _WIN32
            std::cout << error_str << std::endl;
        #endif

        throw std::invalid_argument(error_str);
    }
    
    //  3. read file line by line
    bool is_header = true;
    std::string line;
    std::vector<std::string> line_split_vec;
    std::vector<std::vector<std::string>> string_matrix;
    
    while (not ifs.eof()) {
        std::getline(ifs, line);
        
        if (is_header) {
            is_header = false;
            continue;
        }
        
        line_split_vec = this->__splitCommaSeparatedString(line);
        
        if (not line_split_vec.empty()) {
            string_matrix.push_back(line_split_vec);
        }
    }
    
    ifs.close();
    return string_matrix;
}   /* __getDataStringMatrix() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn
///

void Interpolator :: __readData1D(int data_key, std::string path_2_data)
{
    //  1. get string matrix
    std::vector<std::vector<std::string>> string_matrix =
        this->__getDataStringMatrix(path_2_data);

    //  2. read string matrix contents into 1D interpolation struct
    InterpolatorStruct1D interp_struct_1D;
    
    interp_struct_1D.n_points = string_matrix.size();
    interp_struct_1D.x_vec.resize(interp_struct_1D.n_points, 0);
    interp_struct_1D.y_vec.resize(interp_struct_1D.n_points, 0);
    
    for (int i = 0; i < interp_struct_1D.n_points; i++) {
        try {
            interp_struct_1D.x_vec[i] = std::stod(string_matrix[i][0]);
            interp_struct_1D.y_vec[i] = std::stod(string_matrix[i][1]);
        }
        
        catch (...) {
            this->__throwReadError(path_2_data, 1);
        }
    }
    
    //  3. write struct to map
    this->interp_map_1D.insert(
        std::pair<int, InterpolatorStruct1D>(data_key, interp_struct_1D)
    );
    
    /*
    // ==== TEST PRINT ==== //
    std::cout << std::endl;
    std::cout << path_2_data << std::endl;
    std::cout << "--------" << std::endl;
    
    std::cout << "n_points: " << this->interp_map_1D[data_key].n_points << std::endl;
    
    std::cout << "x_vec: [";
    for (
        int i = 0;
        i < this->interp_map_1D[data_key].n_points;
        i++
    ) {
        std::cout << this->interp_map_1D[data_key].x_vec[i] << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << "y_vec: [";
    for (
        int i = 0;
        i < this->interp_map_1D[data_key].n_points;
        i++
    ) {
        std::cout << this->interp_map_1D[data_key].y_vec[i] << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << std::endl;
    // ==== END TEST PRINT ==== //
    //*/
    
    return;
}   /* __readData1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn
///

void Interpolator :: __readData2D(int data_key, std::string path_2_data)
{
    //  1. get string matrix
    std::vector<std::vector<std::string>> string_matrix =
        this->__getDataStringMatrix(path_2_data);

    //  2. read string matrix contents into 2D interpolation map
    InterpolatorStruct2D interp_struct_2D;
    
    interp_struct_2D.n_rows = string_matrix.size() - 1;
    interp_struct_2D.n_cols = string_matrix[0].size() - 1;
    
    interp_struct_2D.x_vec.resize(interp_struct_2D.n_cols, 0);
    interp_struct_2D.y_vec.resize(interp_struct_2D.n_rows, 0);
    
    interp_struct_2D.z_matrix.resize(interp_struct_2D.n_rows, {});
    
    for (int i = 0; i < interp_struct_2D.n_rows; i++) {
        interp_struct_2D.z_matrix[i].resize(interp_struct_2D.n_cols, 0);
    }
    
    for (size_t i = 1; i < string_matrix[0].size(); i++) {
        try {
            interp_struct_2D.x_vec[i - 1] = std::stod(string_matrix[0][i]);
        }
        
        catch (...) {
            this->__throwReadError(path_2_data, 2);
        }
    }
    
    for (size_t i = 1; i < string_matrix.size(); i++) {
        try {
            interp_struct_2D.y_vec[i - 1] = std::stod(string_matrix[i][0]);
        }
        
        catch (...) {
            this->__throwReadError(path_2_data, 2);
        }
    }
    
    for (size_t i = 1; i < string_matrix.size(); i++) {
        for (size_t j = 1; j < string_matrix[0].size(); j++) {
            try {
                interp_struct_2D.z_matrix[i - 1][j - 1] = std::stod(string_matrix[i][j]);
            }
            
            catch (...) {
                this->__throwReadError(path_2_data, 2);
            }
        }
    }
    
    //  3. write struct to map
    this->interp_map_2D.insert(
        std::pair<int, InterpolatorStruct2D>(data_key, interp_struct_2D)
    );
    
    /*
    // ==== TEST PRINT ==== //
    std::cout << std::endl;
    std::cout << path_2_data << std::endl;
    std::cout << "--------" << std::endl;
    
    std::cout << "n_rows: " << this->interp_map_2D[data_key].n_rows << std::endl;
    std::cout << "n_cols: " << this->interp_map_2D[data_key].n_cols << std::endl;
    
    std::cout << "x_vec: [";
    for (
        int i = 0;
        i < this->interp_map_2D[data_key].n_cols;
        i++
    ) {
        std::cout << this->interp_map_2D[data_key].x_vec[i] << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << "y_vec: [";
    for (
        int i = 0;
        i < this->interp_map_2D[data_key].n_rows;
        i++
    ) {
        std::cout << this->interp_map_2D[data_key].y_vec[i] << ", ";
    }
    std::cout << "]" << std::endl;
    
    std::cout << "z_matrix:" << std::endl;
    for (
        int i = 0;
        i < this->interp_map_2D[data_key].n_rows;
        i++
    ) {
        std::cout << "\t[";
        
        for (
            int j = 0;
            j < this->interp_map_2D[data_key].n_cols;
            j++
        ) {
            std::cout << this->interp_map_2D[data_key].z_matrix[i][j] << ", ";
        }
        
        std::cout << "]" << std::endl;
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
    // ==== END TEST PRINT ==== //
    //*/
    
    return;
}   /* __readData2D() */

// ---------------------------------------------------------------------------------- //

// ======== END PRIVATE ============================================================= //



// ======== PUBLIC ================================================================== //

// ---------------------------------------------------------------------------------- //

///
/// \fn Interpolator :: Interpolator(void)
///
/// \brief Constructor for the Interpolator class.
///

Interpolator :: Interpolator(void)
{
    //...
    
    return;
}   /* Interpolator() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: addData1D(int data_key, std::string path_2_data)
///
/// \brief Method to add 1D interpolation data to the Interpolator.
///
/// \param data_key A key used to index into the Interpolator.
///
/// \param path_2_data A path (either relative or absolute) to the given 1D
///     interpolation data.
///

void Interpolator :: addData1D(int data_key, std::string path_2_data)
{
    //  1. check key
    this->__checkDataKey1D(data_key);
    
    //  2. read data into map
    this->__readData1D(data_key, path_2_data);
    
    //  3. record path
    this->path_map_1D.insert(std::pair<int, std::string>(data_key, path_2_data));
    
    return;
}   /* addData1D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn void Interpolator :: addData2D(int data_key, std::string path_2_data)
///
/// \brief Method to add 2D interpolation data to the Interpolator.
///
/// \param data_key A key used to index into the Interpolator.
///
/// \param path_2_data A path (either relative or absolute) to the given 2D
///     interpolation data.
///

void Interpolator :: addData2D(int data_key, std::string path_2_data)
{
    //  1. check key
    this->__checkDataKey2D(data_key);
    
    //  2. read data into map
    this->__readData2D(data_key, path_2_data);
    
    //  3. record path
    this->path_map_2D.insert(std::pair<int, std::string>(data_key, path_2_data));
    
    return;
}   /* addData2D() */

// ---------------------------------------------------------------------------------- //



// ---------------------------------------------------------------------------------- //

///
/// \fn Interpolator :: ~Interpolator(void)
///
/// \brief Destructor for the Interpolator class.
///

Interpolator :: ~Interpolator(void)
{
    //...
    
    return;
}   /* ~Interpolator() */

// ---------------------------------------------------------------------------------- //

// ======== END PUBLIC ============================================================== //
