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

class Address2d{
public:
  Address2d() : index1(0), index2(0){}

  int get_index1() const{
    return index1;
  }

  int get_index2() const{
    return index2;
  }

  void set_index1(int i) {
    index1 = i;
  }
  
  void set_index2(int j) {
    index2 = j;
  }

private:
  int index1;
  int index2;
};

class Grid2d{
public:
  Grid2d(int Nside, int state) : Ncell {Nside}, grid_(Nside, vector<int>(Nside)){

    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(0, 1);
    
    for (int i = 0; i < Nside; i++)
      for (int j = 0; j < Nside; j++){
	
	switch(state){
	  
	case  1:
	  grid_[i][j] = 1;
	  break;
	  
	case -1:
	  grid_[i][j] = -1;
	  break;
	  
	default:
	  grid_[i][j] = distribution(gen) > 0.5 ? 1 : -1;
	}	
      }
  }
  
  void do_timestepping(long int nsteps, long int foutput, double J, double temp)
  {     
    cout << "Starting the computation...\n" << flush;

    Address2d address; 
    int i, j;
    
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(0., 1.);
    
    for(long int istep=0; istep!=nsteps; istep++){

      flip_one_spin(address);      
      double flip_delta_energy = evaluate_delta_energy(J, address);

      if(flip_delta_energy < 0 || (flip_delta_energy > 0 && distribution(gen) < exp(-flip_delta_energy/temp)))
	grid_[i][j] *= -1;

      if(istep % foutput == 0)
	saveGridToFile(grid_, istep);
    }
  }

  double compute_magnetisation()
  {
    double magnetisation = 0;
    for (int i = 0; i < Ncell; i++)
      for (int j = 0; j < Ncell; j++)
          magnetisation += grid_[i][j];

    return magnetisation / (Ncell * Ncell);
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

  void flip_one_spin(Address2d& address){
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> distribution(0., 1.);
    
    int i = static_cast<int>(distribution(gen) * Ncell);
    int j = static_cast<int>(distribution(gen) * Ncell);

    address.set_index1(i);
    address.set_index2(j);
  }
  
  double evaluate_delta_energy(double J, Address2d& address){
    int i = address.get_index1();
    int j = address.get_index2();

    return -2 * (-J * grid_bc(i,j) * (grid_bc(i-1,j) + grid_bc(i+1,j) + grid_bc(i,j-1) + grid_bc(i,j+1)));
  }

  void saveGridToFile(const vector<vector<int>>& grid, int step) {
    string outputFolder = "output/";

    if (!fs::exists(outputFolder)) {
      if (!fs::create_directory(outputFolder)) {
	cerr << "Error: Failed to create the output folder." << endl;
	return;
      }
    }

    string filename = outputFolder + "grid_step_" + to_string(step) + ".txt";
    ofstream outputFile(filename);
    
    if (!outputFile) {
      cerr << "Error: Could not open file for writing." << endl;
      return;
    }

    for (const auto& row : grid) {
      for (int value : row) {
	outputFile << value << " ";
      }
      outputFile << endl;
    }
    
    outputFile.close();
    
    plotFile << "plot \"" << filename << "\" matrix w image\n";
  }
  
};

int main(int argc, char* argv[])
{
  if (argc != 7)
    {
      cerr << "Usage: " << argv[0] << " Nside state J temp Nsteps foutput" << endl;
      return 1;
    }
  
  int Nside = stoi(argv[1]);
  int state = stoi(argv[2]);
  double J = stod(argv[3]);
  double temp = stod(argv[4]);
  long int nsteps = stol(argv[5]);
  long int foutput = stol(argv[6]);
  
  Grid2d spins(Nside, state);
  spins.do_timestepping(nsteps, foutput, J, temp);

  cout << "Magnetisation = " << spins.compute_magnetisation() << "\n";
}
