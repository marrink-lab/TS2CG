#include <iostream>
#include "help.h"
#include "Def.h"

help::help(std::string exe)
{
    int size= exe.size();
 {
     

    std::cout<<"\n";
     std::cout<<"=========================================================================================================="<<"\n";
     std::cout<<"-- CG Membrane builder:   "<<"\n";
     std::cout<<"-- Version:  "<<SoftWareVersion<<"\n";
     std::cout<<"-- Groningen Biomolecular Sciences and Biotechnology Institute and Zernike Institute for Advanced Materials,\n-- University of Groningen, Groningen, Netherlands"<<"\n";
     std::cout<<"-- For more information contact Weria Pezeshkian: w.pezeshkian@rug.nl and weria.pezeshkian@gmail.com"<<"\n";
     std::cout<<"-- citation: Pezeshkian, W., König, M., Wassenaar, T.A. et al. Backmapping triangulated surfaces to coarse-grained membrane models. Nat Commun 11, 2296 (2020)."<<"\n";
     std::cout<<"=========================================================================================================="<<"\n";
     std::cout<<"-- With option -Bondlength, you can chnage the initial bond guess. Large Bondlength may generate an unstable structure ";
     std::cout<<"-- With  option  -renorm   the molar ratio of the lipid will be renormalized  "<<"\n";
    std::cout<<"while small one may require a longer energy minimization  "<<"\n";
     std::cout<<"-WallDen: Note: is a sort of Wall bead density. However, it should not be larger than 1. Larger than 1 acts as one.  "<<"\n";
     std::cout<<"To get higher denisty, you may increase -Mashno value or reduce -ap value in PLM command.   "<<"\n";
     std::cout<<"=========================================================================================================="<<"\n";
    std::cout<<"------------ This script convert Pointillism outputs to a CG model -------------------"<<"\n";
    std::cout<<"-------------------------------------------------------------------------------"<<"\n";
    std::cout<<"  option            type        default            description "<<"\n";
    std::cout<<"-------------------------------------------------------------------------------"<<"\n";
    std::cout<<"  -dts              string       point                dts folder address "<<"\n";
     std::cout<<" -str              string       input."<<STRExt<<"            file "<<"\n";
     std::cout<<" -defout           string       output               output files prefix "<<"\n";
     std::cout<<" -Bondlength       double       0.1                  initial bond guess;  "<<"\n";
     std::cout<<" -LLIB             string       no                   a CG lipid library file name;  "<<"\n";
     std::cout<<" -renorm           ------       no                   renormalized the lipid molar ratio  "<<"\n";
     std::cout<<" -iter             double       4                    the number of point selection is iter*number of the point  "<<"\n";
     std::cout<<" -incdirtype       string       Global               the type of protein direction data (Local/Global)  "<<"\n";
     std::cout<<" -Wall             ------       off                  a flag to create a wall around the membrane  "<<"\n";
     std::cout<<" -WallDen          double       1                    Density of the wall beads  (for more info, see above)"<<"\n";
     std::cout<<" -WallBName        string       WL                   Name of the Wall beads  "<<"\n";
     std::cout<<" -WallH            double       0                    Distance of the wall from the lipid head groups  "<<"\n";

    

     std::cout<< "example: PCG -dts point -str input.str -seed 39234  -Bondlength 0.15 "<<"\n";




   }

    

}

help::~help()
{
    
}
