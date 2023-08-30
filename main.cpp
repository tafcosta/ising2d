#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>
#include <string>
#include <filesystem>  

namespace fs = std::filesystem;
using namespace std;

string plot_filename = "plot.txt";
ofstream plotFile(plot_filename);

class Grid
{
public:
  Grid(int Nside) : Ncell {Nside}, grid_(Nside, vector<int>(Nside)){

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(0, 1);
    
    for (int i = 0; i < Nside; i++)
	for (int j = 0; j < Nside; j++)
	  grid_[i][j] = distribution(gen) > 0.5 ? 1 : -1;
  }
  
  void do_timestepping(int nsteps, double J, double temp)
  {     
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(0., 1.);
    
    for(int istep=0; istep!=nsteps; istep++){
      
      int i = static_cast<int>(distribution(gen) * Ncell);
      int j = static_cast<int>(distribution(gen) * Ncell);

      double flip_delta_energy = evaluate_delta_energy(J,i,j);
      
      if(flip_delta_energy < 0 || (flip_delta_energy > 0 && distribution(gen) < exp(-flip_delta_energy/temp)))
	grid_[i][j] *= -1;

      if(istep % 1000000 == 0)
	saveGridToFile(grid_, istep);
    }
  }

private:
  vector<vector<int>> grid_;
  int Ncell;

  int grid_bc(int i, int j)
  {
    if(i < 0)
      i += Ncell;
    else if(i >= Ncell)
      i -= Ncell;
    
    if(j < 0)
      j += Ncell;
    else if(j >= Ncell)
      j -= Ncell;
    
    return grid_[i][j];
  }

  double evaluate_delta_energy(double J, int i, int j){
    return -2 * (-J * grid_bc(i,j) * (grid_bc(i-1,j) + grid_bc(i+1,j) + grid_bc(i,j-1) + grid_bc(i,j+1)));
  }

  void saveGridToFile(const std::vector<std::vector<int>>& grid, int step) {
    string outputFolder = "output/";

    if (!fs::exists(outputFolder)) {
      if (!fs::create_directory(outputFolder)) {
	std::cerr << "Error: Failed to create the output folder." << std::endl;
	return;
      }
    }

    string filename = outputFolder + "grid_step_" + std::to_string(step) + ".txt";
    ofstream outputFile(filename);
    
    if (!outputFile) {
      std::cerr << "Error: Could not open file for writing." << std::endl;
      return;
    }

    for (const auto& row : grid) {
      for (int value : row) {
	outputFile << value << " ";
      }
      outputFile << std::endl;
    }
    
    outputFile.close();
    
    plotFile << "plot \"" << filename << "\" matrix w image\n";
  }
  
};

int main(int argc, char* argv[])
{
  if (argc != 5)
    {
      cerr << "Usage: " << argv[0] << " Nside J temp Nsteps" << endl;
      return 1;
    }
  
  int Nside = stoi(argv[1]);
  double J = stod(argv[2]);
  double temp = stod(argv[3]);
  int nsteps = stod(argv[4]);

  Grid spins(Nside);
  spins.do_timestepping(nsteps, J, temp);
}
