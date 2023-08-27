#include <iostream>
#include <vector>
#include <random>
#include <cmath>
#include <fstream>

using namespace std;

vector<vector<int>> initialise_grid(int nx, int ny);
void do_timestepping(int nsteps, vector<vector<int>>& grid, int nx, int ny, double J, double temp);
void print_grid_values(vector<vector<int>> grid, int nx, int ny);
void saveGridToFile(const vector<vector<int>>& grid, int step);

string plot_filename = "plot.txt";
ofstream plotFile(plot_filename);

int main(){
  
  int nx {64};
  int ny {64};
  int nsteps {1000};

  double J {0.1};
  double temp {0.6};
  
  vector<vector<int>> spin_grid = initialise_grid(nx, ny);
  
  do_timestepping(nsteps, spin_grid, nx, ny, J, temp);
}

void print_grid_values(vector<vector<int>> grid, int nx, int ny)
{
    for(int i = 0; i < ny; i++)
      for(int j = 0; j < nx; j++)
	cout << grid[i][j] << "\n";

    cout << " \n";
}

void do_timestepping(int nsteps, vector<vector<int>>& grid, int nx, int ny, double J, double temp)
{
  int istep {0};
  vector<vector<int>> grid_tmp(ny, vector<int>(nx, 0));
  
  random_device rd;
  mt19937 gen(rd());

  uniform_real_distribution<double> distribution(0., 1.);
  
  while(istep < nsteps){

    for(int i = 0; i < ny; i++)
      for(int j = 0; j < nx; j++)
	grid_tmp[i][j] = grid[i][j];
    
    for(int i = 1; i < (ny - 1); i++)
      for(int j = 1; j < (nx - 1); j++){
	
	double energy = -J * grid_tmp[i][j] * (grid_tmp[i-1][j] + grid_tmp[i+1][j] + grid_tmp[i][j-1] + grid_tmp[i][j+1]);
	double flip_energy_change = -2. * energy;

	if((flip_energy_change < 0) || ((flip_energy_change > 0) && (distribution(gen) < exp(-flip_energy_change/temp))))
	  grid[i][j] *= -1;
      }

    saveGridToFile(grid, istep);
    istep++;
  }
}

vector<vector<int>> initialise_grid(int nx, int ny)
{
  vector<vector<int>> grid(ny, vector<int>(nx));

  random_device rd;
  mt19937 gen(rd());
  
  uniform_real_distribution<double> distribution(0., 1.);
  
  for(int i = 0; i < ny; i++)
    for(int j = 0; j < nx; j++)
      {
      double random_number = distribution(gen);
      
      if(random_number > 0.5)
	grid[i][j] =  1;
      else
	grid[i][j] = -1;
      }
  
  return grid;
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
