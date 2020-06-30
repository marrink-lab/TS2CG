

#include <stdio.h>
#include "ReadDTSFolder.h"
ReadDTSFolder::ReadDTSFolder(std::string foldername)
{


    /*if(foldername.at(foldername.size()-1)=='/')
    {
        foldername.pop_back();
    }*/
    std::string file1 = "./"+foldername+"/OuterBM.dat";
    std::string file2 = "./"+foldername+"/InnerBM.dat";
    std::string file3 = "./"+foldername+"/IncData.dat";
    bool health = true;
    bool monolayer =false;
    if(FileExist(file1) == false)
    {
        std::cout<<"--> error: file with file name "<<file1 <<" does not exist \n";
        health = false;
    }
    if(FileExist(file2) == false)
    {
        monolayer = true;
    }
    if(FileExist(file3) == false)
    {
        std::cout<<"--> no inclsuion file is provided, we will generate a random distribution of proteins if information is provided in STR file \n";
    }
    
    m_OuterPoint = ReadPointObjects(file1,1);
    if(monolayer==false)
    m_InnerPoint = ReadPointObjects(file2,-1);
    
    if(FileExist(file3) == true)
    {
        std::cout<<" inclusion file is provided, we will generate proteins according to this file \n";
        m_Inclusion =  ReadInclusionObjects(file3);
    }
    
    
   
     for (std::vector<point>::iterator it = m_OuterPoint.begin() ; it != m_OuterPoint.end(); ++it)
         m_pOuterPoint.push_back(&(*it));
    
    for (std::vector<point>::iterator it = m_InnerPoint.begin() ; it != m_InnerPoint.end(); ++it)
        m_pInnerPoint.push_back(&(*it));
    
    for (std::vector<inclusion>::iterator it = m_Inclusion.begin() ; it != m_Inclusion.end(); ++it)
        m_pInclusion.push_back(&(*it));
    
    
    for (std::vector<inclusion*>::iterator it = m_pInclusion.begin() ; it != m_pInclusion.end(); ++it)
    {
        int pid=(*it)->GetPointID();
        (m_pOuterPoint.at(pid))->UpdateInclusion(*it);
        
    }

}
ReadDTSFolder::~ReadDTSFolder()
{
    
}



std::vector<point> ReadDTSFolder::ReadPointObjects(std::string file, int lay)
{
    

   //  char str = new str[1000];
    char str1[256];
    char str2[256];

    FILE *fdtspoins;
    fdtspoins = fopen(file.c_str(), "r");
    char layer[256];
    float Lx,Ly,Lz;
    

    int NoPoints;

    if (fdtspoins == NULL){
        printf(" Error: Could not open file %s",file.c_str());
    }

    if(lay==1)
    int readafile =  fscanf(fdtspoins,"%s%f%f%f",str2,&Lx,&Ly,&Lz);

    int readafile = fscanf(fdtspoins,"%s%s%s%d%s",str2,str2,str2,&NoPoints,str2);

    m_Box(0) = Lx;
    m_Box(1) = Ly;
    m_Box(2) = Lz;
    
    
    m_pBox = (&m_Box);
    
    
    bool check = fgets(str1, sizeof(str1), fdtspoins);
    check = fgets(str1, sizeof(str1), fdtspoins);
    check = fgets(layer, sizeof(layer), fdtspoins);
    std::string LayerName=layer;
    LayerName.pop_back();
    LayerName.pop_back();
    LayerName.pop_back();
    LayerName.erase(LayerName.begin());
    LayerName.erase(LayerName.begin());

    std::vector<point>  AllPoint;


    float area,x,y,z,nx,ny,nz,p1x,p1y,p1z,p2x,p2y,p2z,c1,c2;
    int id;
    for (int i=0;i<NoPoints;i++)
    {
        check = fscanf(fdtspoins,"%d%f%f%f%f%f%f%f%f%f%f%f%f%f%f%f",&id,&area,&x,&y,&z,&nx,&ny,&nz,&p1x,&p1y,&p1z,&p2x,&p2y,&p2z,&c1,&c2);
        


        Vec3D X(x,y,z);
        Vec3D N(nx,ny,nz);
        Vec3D P1(p1x,p1y,p1z);
        Vec3D P2(p2x,p2y,p2z);
        std::vector <double> C;
        C.push_back(c1);
        C.push_back(c2);
        point p(id, area,X,N, P1, P2, C );
        if(lay==-1)
        p.UpdateUpperLayer(false);

        AllPoint.push_back(p);
    }
    
    
    return AllPoint;

}
std::vector<inclusion> ReadDTSFolder::ReadInclusionObjects(std::string file)
{
//    inclusion(int id, int typeID, int pointid,Vec3D D );

    
    //  char str = new str[1000];
    char str1[256];
    char str2[256];
    
    FILE *fdtspoins;
    fdtspoins = fopen(file.c_str(), "r");
    char layer[256];
    float Lx,Ly,Lz;
    int NoPoints;
    
    if (fdtspoins == NULL){
        printf(" Error: Could not open file %s",file.c_str());
    }
    
    int readafile = fscanf(fdtspoins,"%s%s%s%d%s",str2,str2,str2,&NoPoints,str2);
    
    
    bool  check = fgets(str1, sizeof(str1), fdtspoins);
    check = fgets(str1, sizeof(str1), fdtspoins);

    
    std::vector<inclusion>  AllInc;
    
    
    float x,y,z;

    int id,tid,pid;
    for (int i=0;i<NoPoints;i++)
    {
        readafile = fscanf(fdtspoins,"%d%d%d%f%f%f",&id,&tid,&pid,&x,&y,&z);
        
        Vec3D D(x,y,z);
        inclusion p(id, tid,pid,D);
       // std::cout<<id<<"  "<<tid<<"  "<<pid<<"  "<<x<<"  "<<y<<"  "<<z<<"\n";
        AllInc.push_back(p);
    }
    
    
    return AllInc;
    
}

bool ReadDTSFolder::FileExist (const std::string& name)
{
    std::ifstream f(name.c_str());
    return f.good();
}
