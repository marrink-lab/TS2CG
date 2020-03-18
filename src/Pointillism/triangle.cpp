#include "triangle.h"

triangle::triangle(int id, vertex *v1, vertex *v2, vertex *v3)
{

m_V1=v1;
m_V2=v2;
m_V3=v3;
m_ID=id;

m_Representation=true;
#if BACKMAP == Enabled
    m_GotMashed = false;;
#endif
}
triangle::triangle(int id)
{
m_ID=id;
m_Representation=true;
#if BACKMAP == Enabled
    m_GotMashed = false;;
#endif

}

triangle::~triangle()
{
    
}
#if BACKMAP == Enabled
void triangle::UpdateGotMashed(bool z)
{
m_GotMashed=z;
}
#endif

void triangle::UpdateRepresentation(bool z)
{
    m_Representation = z;
}

/// normal vector update
void triangle::UpdateNormal_Area(Vec3D *pBox)
{

Vec3D Box=(*pBox);
    m_Area=0.0;
    
    
    double x1=m_V1->GetVXPos();
    double y1=m_V1->GetVYPos();
    double z1=m_V1->GetVZPos();
    double x2=m_V2->GetVXPos();
    double y2=m_V2->GetVYPos();
    double z2=m_V2->GetVZPos();
    double x3=m_V3->GetVXPos();
    double y3=m_V3->GetVYPos();
    double z3=m_V3->GetVZPos();
    

    
    double dx1=x2-x1;
    if(fabs(dx1)>Box(0)/2.0)
    {
        
        
        if(dx1<0)
        dx1=Box(0)+dx1;
        else if(dx1>0)
        dx1=dx1-Box(0);
        
        
        
    }
    double dy1=y2-y1;
    if(fabs(dy1)>Box(1)/2.0)
    {
        
        
        if(dy1<0)
        dy1=Box(1)+dy1;
        else if(dy1>0)
        dy1=dy1-Box(1);
        
        
    }


    
    double dz1=z2-z1;


    if(fabs(dz1)>Box(2)/2.0)
    {
        if(dz1<0)
        dz1=Box(2)+dz1;
        else if(dz1>0)
        dz1=dz1-Box(2);
    }
    double dx2=x3-x1;
    if(fabs(dx2)>Box(0)/2.0)
    {
        
        
        if(dx2<0)
        dx2=Box(0)+dx2;
        else if(dx2>0)
        dx2=dx2-Box(0);
        
       
    }
    double dy2=y3-y1;
    if(fabs(dy2)>Box(1)/2.0)
    {
        if(dy2<0)
        dy2=Box(1)+dy2;
        else if(dy2>0)
        dy2=dy2-Box(1);
        
    }
    double dz2=z3-z1;
    if(fabs(dz2)>Box(2)/2.0)
    {
        if(dz2<0)
        dz2=Box(2)+dz2;
        else if(dz2>0)
        dz2=dz2-Box(2);
    }
    
   
    
    Vec3D v1(dx1,dy1,dz1);
    Vec3D v2(dx2,dy2,dz2);
    m_Normal=v1*v2;
    m_AreaVector=m_Normal;
    m_Area=m_Normal.norm();    
    m_Normal=m_Normal*(1.0/m_Area);    
    m_Area=0.5*m_Area;



}

void triangle::UpdateVertex(vertex *v1,vertex *v2,vertex *v3)
{
m_V1=v1;
m_V2=v2;
m_V3=v3;


}

void triangle::ReadTriangleFromFile(std::ifstream *inputfile,std::vector <vertex *> pv)
{
    int id,n,v1,v2,v3;
    std::string word;
    bool readok=true;
    (*inputfile)>>word>>id;
    if(id!=m_ID || word!="Triangle")
        readok=false;
    
    
    (*inputfile)>>word>>n;
    if(word!="Visualization")
        readok=false;
    m_Representation=n;
    
    (*inputfile)>>word>>v1>>v2>>v3;
    if(word!="Nodes")
        readok=false;
    m_V1=pv.at(v1);
    m_V2=pv.at(v2);
    m_V3=pv.at(v3);

    
    if(readok!=true)
    {
        std::cout<<m_ID<<"   "<<id<<" Error: wrong triangle id in reading the file: perhpas the file is broken \n";
    }
}
void triangle::WriteTriangleToFile(std::ofstream *output)
{

    (*output)<<"Triangle "<<m_ID<<"\n";
    (*output)<<"Visualization "<<m_Representation<<"\n";

    
    (*output)<<"Nodes  "<<m_V1->GetVID()<<" "<<m_V2->GetVID()<<" "<<m_V3->GetVID()<<"\n";


}



