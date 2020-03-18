

#include <stdio.h>
#include "GroFile.h"
GroFile::GroFile(std::string gmxfilename)
{

    m_GroFileName = gmxfilename;
    ReadGroFile(m_GroFileName);

}


GroFile::~GroFile()
{
    
}

void GroFile::AddBead(bead b)
{

    m_AllBeads.push_back(b);
}
void GroFile::ReadGroFile(std::string file)
{
    char str[1000];
    if(file.size()<4)
    {
        file=file+".gro";
    }
    else if(file.at(file.size()-1)=='o' && file.at(file.size()-2)=='r' && file.at(file.size()-3)=='g')
    {
        
    }
    else
    {
        file=file+".gro";
    }
    
    FILE *fgro;
    fgro = fopen(file.c_str(), "r");

    if (fgro == NULL){
        printf(" Error: Could not open file %s",file.c_str());
    }
    bool check = fgets(str, 1000, fgro);
    m_Title = str;
    m_Title.pop_back();
     check = fgets(str, 1000, fgro);

    int NoBeads = atoi(str);
    
    // Reading the atoms from gro file
    
    float x,y,z,v1,v2,v3;
    char *A1;
    char *A2;
    
    char a[200];
    char b[200];
    int resid,beadid;
    std::string beadtype = "MDBeads";
    
    for (int i=0; i<NoBeads; i++) //NoBeads
    {


        int readafile = fscanf(fgro, "%d%s%s%d%f%f%f",&resid,a,b,&beadid,&x,&y,&z);
        check = fgets(str, 1000, fgro);
        
        std::string beadname = b;
        std::string resname = a;
        bead Be(i, beadname, beadtype, resname, resid, x, y, z);
        m_AllBeads.push_back(Be);

        //  std::cout<<resid<<"  "<<resname<<"  "<<beadname<<" "<<beadid<<" "<<x<<"  "<<y<<"  "<<z<<"  \n";


    }
    
    float Lx,Ly,Lz;
    int readafile = fscanf(fgro, "%f%f%f",&Lx,&Ly,&Lz);
   // std::cout<<Lx<<" "<<Ly<<" "<<Lz<<" \n";

    fclose(fgro);
    
    m_Box(0)=Lx; m_Box(1)=Ly; m_Box(2)=Lz;
    m_pBox = &m_Box;
    double xcm =0;
    double ycm =0;
    double zcm =0;

    for (std::vector<bead>::iterator it = m_AllBeads.begin() ; it != m_AllBeads.end(); ++it)
    {
        
        (*it).UpdateBox(m_pBox);
        double x=(*it).GetXPos();
        double y=(*it).GetYPos();
        double z=(*it).GetZPos();
        
        xcm+=x/double(m_AllBeads.size());
        ycm+=y/double(m_AllBeads.size());
        zcm+=z/double(m_AllBeads.size());


        
    }
    
    for (std::vector<bead>::iterator it = m_AllBeads.begin() ; it != m_AllBeads.end(); ++it)
    {
        
        (*it).UpdateBox(m_pBox);
        double x=(*it).GetXPos();
        double y=(*it).GetYPos();
        double z=(*it).GetZPos(); 
        (*it).UpdateXPos(x-xcm);
        (*it).UpdateYPos(y-ycm);
        (*it).UpdateZPos(z-zcm);
        
        
    }
    
    
    for (std::vector<bead>::iterator it = m_AllBeads.begin() ; it != m_AllBeads.end(); ++it)
    {
        
        m_pAllBeads.push_back(&(*it));
  
    }
    
}

void GroFile::WriteGroFile(std::string file)
{

    if(file.size()<4)
    {
        file=file+".gro";
    }
    else if(file.at(file.size()-1)=='o' && file.at(file.size()-2)=='r' && file.at(file.size()-3)=='g')
    {
        
    }
    else
    {
        file=file+".gro";
    }

    
    FILE *fgro;
    fgro = fopen(file.c_str(), "w");
    
    
    /// resid  res name   noatom   x   y   z
    const char* Title="dmc gmx file handler";
    int Size=m_AllBeads.size();
    
    fprintf(fgro,  "%s\n",Title);
    fprintf(fgro, "%5d\n",Size);
    int i=0;
    for (std::vector<bead>::iterator it = m_AllBeads.begin() ; it != m_AllBeads.end(); ++it)
    {
        
        i++;
        double x=(*it).GetXPos();
        double y=(*it).GetYPos();
        double z=(*it).GetZPos();
        
        
        const char* A1=((*it).GetResName()).c_str();
        const char* A2=((*it).GetBeadName()).c_str();
        int resid=(*it).GetResid();
        fprintf(fgro, "%5d%5s%5s%5d%8.3f%8.3f%8.3f\n",resid,A1,A2,i,x,y,z );

    }
    

    fprintf(fgro,  "%10.5f%10.5f%10.5f\n",m_Box(0),m_Box(1),m_Box(2) );
    fclose(fgro);
}


