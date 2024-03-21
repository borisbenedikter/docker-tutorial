#include <iostream>
#include <random>
#include <boost/random.hpp>
#include <Eigen/Dense>

int main() 
{
// Initialize a random device to generate seed
    std::random_device rd;
    
    // Setup a random number generator with a different seed at each call
    boost::random::mt19937 rng(rd());
    boost::random::uniform_real_distribution<> dist(-1.0, 1.0);

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
    std::cout << "Random matrix:\n" << matrix << std::endl;

    return 0;
}