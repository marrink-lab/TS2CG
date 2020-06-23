

#include <stdio.h>
#include "PDBFile.h"
#include "Nfunction.h"

PDBFile::PDBFile()
{


}


PDBFile::~PDBFile()
{
    
}

void PDBFile::ReadPDBFile(std::string file)
{
    Nfunction f;
    char str[1000];
    if(file.size()<4)
    {
        file=file+".pdb";
    }
    else if(file.at(file.size()-1)=='o' && file.at(file.size()-2)=='r' && file.at(file.size()-3)=='g')
    {
        
    }
    else
    {
        file=file+".pdb";
    }
    
    
    std::cout<<"Error: this function has not been implemented yet. No use for it :D \n";
    
}

void PDBFile::WritePDBFile(std::string file,std::vector<point*> p1)
{

    if(file.size()<4)
    {
        file=file+".pdb";
    }
    else if(file.at(file.size()-1)=='b' && file.at(file.size()-2)=='d' && file.at(file.size()-3)=='p')
    {
        
    }
    else
    {
        file=file+".pdb";
    }

    
    FILE *fpdb;
    fpdb  = fopen(file.c_str(), "w");
    
    

    int i=0;
    for (std::vector<point*>::iterator it = p1.begin() ; it != p1.end(); ++it)
    {
        
        i++;
        Vec3D X=(*it)->GetPos();
        X=X*10;
        std::vector <double> C= (*it)->GetCurvature();

        const char* A0="ATOM";
        const char* aname="W";
        const char* A1="A";
        const char* resname="WL";
        const char* chain="A";
        const char* A2=" ";
        double meanC=(C.at(0)+C.at(1))/2.0;
        double KG=(C.at(0)*C.at(1));

        fprintf(fpdb, "%4s%7d%5s%1s%3s%2s%4d%3s%8.3f%8.3f%8.3f%6.2f%6.2f\n",A0,i,aname,A1,resname,chain,1,A2,X(0),X(1),X(2),meanC,KG );

    }
    

}


