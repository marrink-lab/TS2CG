

#include <stdio.h>
#include "GroFile.h"
#include "Nfunction.h"

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
    Nfunction f;
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
    
    
    std::ifstream FGRO;
    FGRO.open(file.c_str());
    std::string str1;
    getline (FGRO,str1);
    getline (FGRO,str1);
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

        getline (FGRO,str1);
        std::vector <std::string> l=f.split(str1);
        
        int readafile = fscanf(fgro, "%d%s%s%d%f%f%f",&resid,a,b,&beadid,&x,&y,&z);
        check = fgets(str, 1000, fgro);
        
        if(l.size()==7 || l.size()==10)
        {
            x=atof((l.at(4)).c_str());
            y=atof((l.at(5)).c_str());
            z=atof((l.at(6)).c_str());
        }
        else if(l.size()==6 || l.size()==9)
        {
            x=atof((l.at(3)).c_str());
            y=atof((l.at(4)).c_str());
            z=atof((l.at(5)).c_str());
        }
        else if(l.size()==5 || l.size()==8)
        {
            x=atof((l.at(2)).c_str());
            y=atof((l.at(3)).c_str());
            z=atof((l.at(4)).c_str());
        }
        else
        {
            std::cout<<"Warning: Perhaps error, something wrong with"<<file <<"file \n";
        }
        std::string bt = b;
        std::string beadname;
        if(bt.size()>0)
        beadname.push_back(bt.at(0));
        if(bt.size()>1)
        beadname.push_back(bt.at(1));

        std::string resname = a;
        bead Be(i, beadname, beadtype, resname, resid, x, y, z);
        m_AllBeads.push_back(Be);

        //  std::cout<<resid<<"  "<<resname<<"  "<<beadname<<" "<<beadid<<" "<<x<<"  "<<y<<"  "<<z<<"  \n";


    }
    FGRO.close();
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


