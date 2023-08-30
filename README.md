# ising2d
A C++ implementation of the 2D Ising model. 

## Input parameters:
Ngrid: number of grid cells per side,  
state: specifies how the spin grid is initialised, and admits values 1 (if all spins are `up'), -1 (if all spins point `down'), 0 (if spins are initialised randomly).
J: interaction energy (assuming units kB = 1),  
temp: temperature (assuming units kB = 1),  
Nsteps: number of time-steps.  

To call the program, do  
./ising2d \<Ngrid\>  \<state\> \<J\> \<temp\> \<Nsteps\>  
Example call: ./ising2d 256 1 1 1.22 1000000
 

## Critical point:
![](https://github.com/tafcosta/ising2d/blob/master/animation.gif)