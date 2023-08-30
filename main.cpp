#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

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

      if(istep % 1000 == 0)
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
  
  void saveGridToFile(const vector<vector<int>>& grid, int step) {
    string filename = "grid_step_" + to_string(step) + ".txt";
    ofstream outputFile(filename);
    
    if (!outputFile) {
      cerr << "Error: Could not open file for writing." << endl;
      return;
    }
    
    plotFile << "plot \"" << filename << "\" matrix w image\n";
    for (const auto& row : grid) {
      for (int value : row) {
	outputFile << value << " ";
      }
      outputFile << endl;
    }
    
    outputFile.close();
  }
};

int main(){
  int Nside = 256; 
  int nsteps {1000000};
  double J {0.1};
  double temp {0.1};

  Grid spins(Nside);
  spins.do_timestepping(nsteps, J, temp);
}
