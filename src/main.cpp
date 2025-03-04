#include <iostream>
#include <fstream>
#include <random>

#include <boost/random.hpp>

#include <Eigen/Dense>

int main(int argc, char** argv)
{
    double lower_bound = -1.0;
    double upper_bound = 1.0;
    std::string file_name = "";
    // Look for flags (-l, -u, -f) in the command line arguments.
    // If found, set the lower bound, upper bound, and file name respectively.
    for (int i = 1; i < argc; ++i)
    {
        if (std::string(argv[i]) == "-l")
        {
            lower_bound = std::stod(argv[i + 1]);
        }
        else if (std::string(argv[i]) == "-u")
        {
            upper_bound = std::stod(argv[i + 1]);
        }
        else if (std::string(argv[i]) == "-f")
        {
            file_name = argv[i + 1];
        }
    }

    // Initialize a random device to generate seed
    std::random_device rd;
    
    // Setup a random number generator with a different seed at each call
    boost::random::mt19937 rng(rd());
    boost::random::uniform_real_distribution<> dist(lower_bound, upper_bound);

    // Initialize Eigen matrix
    Eigen::MatrixXd matrix = Eigen::MatrixXd::Random(3, 3);

    // Fill Eigen matrix with random numbers
    for (int i = 0; i < matrix.rows(); ++i) 
    {
        for (int j = 0; j < matrix.cols(); ++j) 
        {
            matrix(i, j) = dist(rng);
        }
    }

    // Print the matrix
    // If a file name is provided, write the matrix to the file, else print it 
    // to the console.
    if (file_name != "")
    {
        std::cout << "Matrix written to file: " << file_name << std::endl;
        std::ofstream file(file_name);
        file << matrix;
        file.close();
    }
    else
    {
        std::cout << "Random matrix:\n" << matrix << std::endl;
    }

    return 0;
}