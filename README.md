# ising2d
A C++ implementation of the 2D Ising model. Each spin i is associated to an energy  
$$ E_i \, = \, - J \Sum_j{\sigma_i \sigma_j} $$  
where J is the interaction energy scale and $$ \sigma_i \, = \, \pm 1$$ depending on whether a spin points "up" or "down". Thus, misaligned spins try to align, settling into a lower energy state.

## Input parameters:
+ Ngrid: number of grid cells per side,  
+ state: specifies how the spin grid is initialised, and admits values 1 (if all spins are "up"), -1 (if all spins point "down"), 0 (if spins are initialised randomly),  
+ J: interaction energy / kB,  
+ temp: temperature,  
+ Nsteps: number of Monte Carlo steps, i.e. number of spin flips,  
+ foutput: number of time-steps in between outputs.

To compile on a MAC, type
clang++ -std=c++2a -o ising2d main.cpp

To call the program, type
./ising2d \<Ngrid\>  \<state\> \<J\> \<temp\> \<Nsteps\> \<foutput\>

Example call: ./ising2d 256 0 1 1.23 1000000 1000
 

## Critical point:

A phase transition occurs at a temperature of T = 1.23, when the system spontaneously magnetises. At this critical point, spin fluctuations become correlated out to arbitrarily large-scales and the system becomes scale-invariant. 

![](https://github.com/tafcosta/ising2d/blob/master/animation.gif)
