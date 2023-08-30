# ising2d
A C++ implementation of the 2D Ising model. 

## Input parameters:
Ngrid: number of grid cells per side,  
J: interaction energy (assuming units kB = 1),  
temp: temperature (assuming units kB = 1),  
Nsteps: number of time-steps.  

To call the program, call  
./ising2d \<Ngrid\> \<J\> \<temp\> \<Nsteps\>  
Example call: ./ising2d 256 1 1.22 1000000

![](https://github.com/tafcosta/ising2d/blob/master/animation.gif)
