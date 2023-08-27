#include <iostream>
#include <vector>
#include <random>
#include <cmath>

using namespace std;

vector<vector<int>> initialise_grid(int nx, int ny);
void do_timestepping(int nsteps, vector<vector<int>>& grid, int nx, int ny, double J, double temp);
void print_grid_values(vector<vector<int>> grid, int nx, int ny);

int main(){
  
  int nx {16};
  int ny {16};
  int nsteps {1000};

  double J {0.1};
  double temp {0.0};
  
  vector<vector<int>> spin_grid = initialise_grid(nx, ny);

  do_timestepping(nsteps, spin_grid, nx, ny, J, temp);
  print_grid_values(spin_grid, nx, ny);  
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

    for(int i = 1; i < (ny - 1); i++)
      for(int j = 1; j < (nx - 1); j++){
	
	double energy = -J * grid[i][j] * (grid[i-1][j] + grid[i+1][j] + grid[i][j-1] + grid[i][j+1]);
	double flip_energy_change = -2. * energy;

	if(flip_energy_change < 0)
	  grid[i][j] *= -1;
	else
	  if(distribution(gen) < exp(-flip_energy_change/temp))
	    grid[i][j] *= -1;
      }
    
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
