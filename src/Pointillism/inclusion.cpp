 #if !defined(AFX_inclusion_CPP_7F4A21C7_C13Q_8823_BF2E_124095086234__INCLUDED_)
#define AFX_inclusion_CPP_7F4A21C7_C13Q_8823_BF2E_124095086234__INCLUDED_

#include <stdio.h>
#include "inclusion.h"
#include "vertex.h"
#include "Nfunction.h"

inclusion::inclusion(int id)
{


m_ID=id;
    

    m_TypeID = 0;
    m_NSymmetry = 0;
    m_Type = "Pro";


}

inclusion::~inclusion()
{
    
}

void inclusion::UpdateC0(double c0,double c1,double c2)
{
    m_C0.clear();
    m_C0.push_back(c0);
    m_C0.push_back(c1);
    m_C0.push_back(c2);
}
void inclusion::UpdateKappa(double k1,double k2,double k3,double k4)
{
    m_kappa.clear();
    m_kappa.push_back(k1);
    m_kappa.push_back(k2);
    m_kappa.push_back(k3);
    m_kappa.push_back(k4);

}
void inclusion::UpdateType(std::string type, int inctypeid)
{
    m_Type=type;
    m_TypeID = inctypeid;
}
void inclusion::Updatevertex(vertex * v)
{
    m_pvertex = v;
}
void inclusion::UpdateNSymmetry(int n)
{
    m_NSymmetry = n;
}
void inclusion::UpdateLocalDirection(Vec3D v)
{
    m_LDirection = v;
}
/*void inclusion::UpdateGlobalDirection(Vec3D v)
{
    m_GDirection = v;
}*/
void inclusion::ReadInclusionFromFile(std::ifstream *inputfile,std::vector <vertex *> pv)
{

    int id,n;
    double x,y,z,k1,k2,k3,k4,c0,c1,c2;
    std::string word,name;
    bool readok=true;
   // getline((*inputfile),name);
  //  getline((*inputfile),name);
    (*inputfile)>>word>>id;
    if(id!=m_ID || word!="Inclusion")
    {
#if TEST_MODE == Enabled
        std::cout<<word<<" != Inclusion \n";
        std::cout<<word<<" this could be because of different complilers \n";
#endif
        readok=false;
        
    }
    

    (*inputfile)>>word>>name>>id;
    if( word!="Type")
        readok=false;
        m_Type = name;
        m_TypeID = id;
    
    (*inputfile)>>word>>n;
    if( word!="m_NSymmetry")
        readok=false;
    m_NSymmetry=n;
    
    
    (*inputfile)>>word>>x>>y>>z;
    if( word!="LocalDirection")
        readok=false;
    
    m_LDirection(0)=x;
    m_LDirection(1)=y;
    m_LDirection(2)=z;

    
    (*inputfile)>>word>>k1>>k2>>k3;
    if( word!="Kappa")
        readok=false;
  
    //==== to check if the trajectory belong to old version or new
    Nfunction f;
    std::getline((*inputfile),word);
    m_kappa.clear();

    if(word.size()==0)
    {
        m_kappa.push_back(k1);
        m_kappa.push_back(0);
        m_kappa.push_back(k2);
        m_kappa.push_back(k3);
    }
    else
    {
    m_kappa.push_back(k1);
    m_kappa.push_back(k2);
    m_kappa.push_back(k3);
    m_kappa.push_back(f.String_to_Double(word));
    }

    

    
    (*inputfile)>>word>>c0>>c1>>c2;
    if(word!="Curvature")
        readok=false;
    m_C0.clear();
    m_C0.push_back(c0);
    m_C0.push_back(c1);
    m_C0.push_back(c2);
    
    
    (*inputfile)>>word>>n;
    if( word!="Vertex")
        readok=false;
    
    m_pvertex = pv.at(n);

    if(readok!=true)
    {
        std::cout<<"Error: wrong Inclusion id in reading the file: perhpas the file is broken \n";
    }
}
void inclusion::WriteInclusionToFile(std::ofstream *output)
{
    (*output)<<std::fixed;
    (*output)<<std::setprecision( Precision );
   // (*output)<<"====================================================== \n";
    (*output)<<"Inclusion "<<m_ID<<"\n";
    (*output)<<"Type "<<m_Type<<" "<<m_TypeID<<"\n";
    (*output)<<"m_NSymmetry "<<m_NSymmetry<<"\n";
    (*output)<<"LocalDirection "<<m_LDirection(0)<<" "<<m_LDirection(1)<<" "<<m_LDirection(2)<<"\n";
    (*output)<<"Kappa "<<m_kappa.at(0)<<" "<<m_kappa.at(1)<<" "<<m_kappa.at(2)<<" "<<m_kappa.at(3)<<"\n";
    (*output)<<"Curvature "<<m_C0.at(0)<<" "<<m_C0.at(1)<<" "<<m_C0.at(2)<<"\n";
    (*output)<<"Vertex "<<m_pvertex->GetVID()<<"\n";
    
}
void inclusion::WriteInclusion()
{
    std::cout<<std::fixed;
    std::cout<<std::setprecision( Precision );
    std::cout<<"Inclusion "<<m_ID<<"\n";
    std::cout<<"Type "<<m_Type<<" "<<m_TypeID<<"\n";
    std::cout<<"m_NSymmetry "<<m_NSymmetry<<"\n";

    
}









#endif



