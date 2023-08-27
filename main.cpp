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
int grid_bc(int i, int j);

string plot_filename = "plot.txt";
ofstream plotFile(plot_filename);

int nx {128};
int ny {128};
vector<vector<int>> grid = initialise_grid(nx, ny);

int main(){
  int nsteps {1000000};

  double J {0.1};
  double temp {0.3};
    
  do_timestepping(nsteps, grid, nx, ny, J, temp);
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
  
  random_device rd;
  mt19937 gen(rd());

  uniform_real_distribution<double> distribution(0., 1.);
  
  while(istep < nsteps){

    int i = distribution(gen) * ny;
    int	j = distribution(gen) * nx;

    double energy = -J * grid_bc(i,j) * (grid_bc(i-1,j) + grid_bc(i+1,j) + grid_bc(i,j-1) + grid_bc(i,j+1));
    double flip_energy_change = -2 * energy;

    if((flip_energy_change < 0) || ((flip_energy_change > 0) && (distribution(gen) < exp(-flip_energy_change/temp))))
      grid[i][j] *= -1;

    if(istep % 1000 == 0)
      saveGridToFile(grid, istep);
    istep++;
  }
}

int grid_bc(int i, int j)
{  
  if(i < 0)
    i += ny;
  else if(i >= ny)
    i -= ny;

  if(j < 0)
    j += nx;
  else if(j >= nx)
    j -= nx;
  
  return grid[i][j];
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
