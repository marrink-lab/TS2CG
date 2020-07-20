

#include <time.h>
#include <stdio.h>
#include <cstdlib>
#include <cstdlib>
#include "Curvature.h"
#include "Edit_configuration.h"
#include "WriteFiles.h"
#include "Nfunction.h"
#include "Vec3D.h"
#include "VMDOutput.h"
#include "Trajectory.h"
#include "Topology.h"
#include "Surface_Mosaicing.h"
#include "VertexMove.h"
#include "MakePBCTS.h"
#include "Traj_XXX.h"


/*
 this class is used to edit output configuration from a DTS simulations. Mosaicing increases this number of the verteices to be used for backmapping,
then generate two surfaces that can be used to create a bilayer.
 
 first rescale the trjectory file,
 Mosaic the surface few times.
 Then generate two surfaces .
 
 */
Edit_configuration::Edit_configuration( Argument *pArgument)
{


///=======
//== Read  variables
//===========
Nfunction f;
    m_Shape ="";
bool ShapeFlag = false;
//f.CleanFiles();	
std::vector <std::string> Arguments = pArgument->GetArgumentString();
    std::string edittype = "PLM";
    m_Folder = "point";
    m_smooth = false;
    m_monolayer = 0;
    Vec3D Box(3,3,3);
    Box(0)=f.String_to_Double(pArgument->GetLx());
    Box(1)=f.String_to_Double(pArgument->GetLy());
    Box(2)=f.String_to_Double(pArgument->GetLz());
    m_FindnewBox = false;
    Vec3D *pBox=&Box;
bool health = true;
m_pBox=pBox;
    m_MosAlType = "Type1";
double bilyaerthickness = 1;
m_Zoom = 1;
  m_Iteration = -1;
    std::string file = "TS.q";
    m_AP = 0.62;
    for (int i=1;i<Arguments.size();i=i+2)
    {
        if(Arguments.at(i)=="-TSfile")
        {
            file=Arguments.at(i+1);
        }
        if(Arguments.at(i)=="-bilayerThickness")
        {
            bilyaerthickness=f.String_to_Double(Arguments.at(i+1));
        }
        if(Arguments.at(i)=="-AlgType")
        {
            m_MosAlType=(Arguments.at(i+1));
        }
        if(Arguments.at(i)=="-rescalefactor")
        {
            m_Zoom=f.String_to_Double(Arguments.at(i+1));
        }
        if(Arguments.at(i)=="-ap")
        {
            m_AP=f.String_to_Double(Arguments.at(i+1));
        }
        if(Arguments.at(i)=="-Mashno")
        {
            m_Iteration=f.String_to_Int(Arguments.at(i+1));
        }
        if(Arguments.at(i)=="-r")
        {
            edittype=Arguments.at(i+1);
        }
        if(Arguments.at(i)=="-o")
        {
            m_Folder=Arguments.at(i+1);
        }
        if(Arguments.at(i)=="-smooth")
        {
            m_smooth=true;
            i=i-1;
        }
        if(Arguments.at(i)=="-resizebox")
        {
            m_FindnewBox=true;
            i=i-1;
        }
        if(Arguments.at(i)=="-monolayer")
        {
            m_monolayer=f.String_to_Int(Arguments.at(i+1));
            bilyaerthickness = 0;
        }
        if(Arguments.at(i)=="-shape")
        {
            ShapeFlag = true;
            m_Shape = Arguments.at(i+1);
        }

    }
    


    
  if(health == true && ShapeFlag == false)
  {
      if(FileExist(file) == false)
      {
          std::cout<<"error: The TS file does not exist \n";
          health = false;
      }
      if(edittype=="check" && health == true)
      {
        check(file);

      }
      else if(edittype=="minimize" && health == true)
      {
          Minimize(file);
          
      }
      else if(edittype=="add_pbc" && health == true)
      {
          MakePBCTS mpbc(file);
          
      }
      else if (edittype=="PLM" && health == true )
      {
  

       // WriteFiles vtu(m_pBox);
        
        
        double H=bilyaerthickness/2.0;
        

        
        const int dir_err = system(("mkdir -p "+m_Folder).c_str());
        if (-1 == dir_err)
        {
            std::cout<<"Error: creating directory  "<<m_Folder<<"\n";
            exit(1);
        }
        
        const int dir_err2 = system(("mkdir -p "+m_Folder+"visualization_data").c_str());
        if (-1 == dir_err2)
        {
            std::cout<<"Error: creating directory  visualization_data"<<"\n";
            exit(1);
        }
        
          
        BackMapOneLayer(1 , file, H);
        if(m_monolayer==0)
        BackMapOneLayer(-1 , file, H);
        
        
        

        }
    }
    else if (edittype=="PLM" && health == true && ShapeFlag == true)
    {
        
        m_pAllV.clear();
        m_AllV.clear();
        double bin=sqrt(1.0/(3.0*m_AP));
        double z=(*m_pBox)(2)/2;
        int s=0;
        int N= (*m_pBox)(0)/bin;
        int M= (*m_pBox)(1)/bin;
        double ap=(*m_pBox)(0)*(*m_pBox)(1)/(N*M);
        for (int n=0;n<N;n++)
        {
            for (int m=0;m<M;m++)
            {
                double x=n*bin+bin/2.0;
                double y=m*bin+bin/2.0;
                vertex v(s,x,y,z);
                Vec3D Normal(0,0,1);
                v.UpdateNormal_Area(Normal,ap);
                v.UpdateBox(m_pBox);

                m_AllV.push_back(v);
                s++;
            }
            
        }

        for (std::vector<vertex>::iterator it = m_AllV.begin() ; it != m_AllV.end(); ++it)
        {
            m_pAllV.push_back(&(*it));
        }

        double H=bilyaerthickness/2.0;
        
        
        
        const int dir_err = system(("mkdir -p "+m_Folder).c_str());
        if (-1 == dir_err)
        {
            std::cout<<"Error: creating directory  "<<m_Folder<<"\n";
            exit(1);
        }


        
        if(m_Shape=="flat")
        {
            MakeFlatMonolayer(1 , file, H);
            if(m_monolayer==0)
            MakeFlatMonolayer(-1 , file, H);
        }
        
        

        
    }
    else
    {
        std::cout<<" unrecognized edit type \n";
    }




}

Edit_configuration::~Edit_configuration()
{
    
}
void Edit_configuration::Rescaling(double zoom )
{
    
    Vec3D L((*m_pBox)(0),(*m_pBox)(1),(*m_pBox)(2));
    
    (*m_pBox)(0) = zoom*L(0);
    (*m_pBox)(1) = zoom*L(1);
    (*m_pBox)(2) = zoom*L(2);
    
    for (std::vector<vertex *>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
    {
        double x = (*it)->GetVXPos();
        double y = (*it)->GetVYPos();
        double z = (*it)->GetVZPos();
        (*it)->UpdateVXPos(zoom*x);
        (*it)->UpdateVYPos(zoom*y);
        (*it)->UpdateVZPos(zoom*z);
        
    }
    
    
    
}

double  Edit_configuration::PPBCM_Cluster(double Lx, std::vector <double> X)
{
    double cm=0;
    
    
    std::vector <double> X1,X2;
    for (std::vector<double>::iterator it = X.begin() ; it != X.end(); ++it)
    {
        if((*it)>Lx/2)
        {
            X2.push_back((*it));
        }
        else
        {
            X1.push_back((*it));
        }
        
    }
    double cm1=0;
    double cm2=0;
    for (std::vector<double>::iterator it = X1.begin() ; it != X1.end(); ++it)
    {
        cm1+=(*it)/X1.size();
        
    }
    for (std::vector<double>::iterator it = X2.begin() ; it != X2.end(); ++it)
    {
        cm2+=(*it)/X2.size();
        
    }
    
        bool boxout=false;
    
    if(fabs(cm2-cm1)>Lx/2)
        boxout=true;

    
    

    

    
    if(boxout==false)
    {
        cm=(cm2*X2.size()+cm1*X1.size())/X.size();
    }
    else
    {
        cm=(cm2*X2.size()+(cm1+Lx)*X1.size())/X.size();
    }
    
    
    
    
    return cm;
    
}
void  Edit_configuration::UpdateGeometry()
{
    
    
    for (std::vector<triangle *>::iterator it = m_pAllT.begin() ; it != m_pAllT.end(); ++it)
        (*it)->UpdateNormal_Area(m_pBox);
    for (std::vector<links *>::iterator it = m_pHalfLinks1.begin() ; it != m_pHalfLinks1.end(); ++it)
    {
        (*it)->UpdateNormal();
        (*it)->UpdateShapeOperator(m_pBox);
    }
    for (std::vector<vertex *>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
        Curvature P(*it);
   
    
}
bool Edit_configuration::FileExist (const std::string& name)
{
    std::ifstream f(name.c_str());
    return f.good();
}
void Edit_configuration::BackMapOneLayer(int layer , std::string file, double H)
{
    Trajectory  TrajClass(m_pBox);
    double lm = 0.1;
    double lmax =1000;
    double mina = 0;
    Topology S(m_pBox, &mina, &lm, &lmax);
    Traj_XXX TSI(m_pBox);

if(file.at(file.size()-1)=='t' && file.at(file.size()-2)=='a' && file.at(file.size()-3)=='d')
{
    m_pAllV.clear();
    m_pInc.clear();
    m_pAllT.clear();
    m_pAllLinks.clear();
    m_pHalfLinks1.clear();
    m_pHalfLinks2.clear();

    TrajClass.Read(file,m_FindnewBox);
    m_pAllV=TrajClass.GetVertex();
    m_pAllT=TrajClass.GetTriangle();
    m_pAllLinks=TrajClass.GetLinks();
    m_pHalfLinks1=TrajClass.GetHalfLinks();
    m_pHalfLinks2=TrajClass.GetMHalfLinks();
    m_pInc=TrajClass.GetInclusion();
}
else if(file.at(file.size()-1)=='q' && file.at(file.size()-2)=='.')
{

    S.FastReadQFile(file);
    bool topohealth= S.GetTopologyHealth();
    if(topohealth==false)
    {
        std::cout<<" error: Provided TS file is bad \n";
    }
    m_pAllV.clear();
    m_pInc.clear();
    m_pAllT.clear();
    m_pAllLinks.clear();
    m_pHalfLinks1.clear();
    m_pHalfLinks2.clear();
    m_pAllV=S.GetVertex();
    m_pAllT=S.GetTriangle();
    m_pAllLinks=S.GetLinks();
    m_pHalfLinks1=S.GetHalfLinks();
    m_pHalfLinks2=S.GetMHalfLinks();
    
}
else if(file.at(file.size()-1)=='i' && file.at(file.size()-2)=='s' && file.at(file.size()-3)=='t')
{
        m_pAllV.clear();
        m_pInc.clear();
        m_pAllT.clear();
        m_pAllLinks.clear();
        m_pHalfLinks1.clear();
        m_pHalfLinks2.clear();
        
        TSI.ReadTSI(file);
        m_pAllV=TSI.GetVertex();
        m_pAllT=TSI.GetTriangle();
        m_pAllLinks=TSI.GetLinks();
        m_pHalfLinks1=TSI.GetHalfLinks();
        m_pHalfLinks2=TSI.GetMHalfLinks();
        m_pInc=TSI.GetInclusion();
}
else
{
std::cout<<" error: Unknown TS File Format "<<file<<"\n";
}
    
    /*std::cout<<m_pAllV.size()<<" vertex size \n";
    VMDOutput GRO((*m_pBox), m_pAllV , m_pHalfLinks1, "test");
    GRO.WriteGro();
    GRO.WriteGro2();
    
    while (2>1)
    {
        
        
    }*/
    
    
    UpdateGeometry();
    if(m_Iteration==-1 )
    {       double Tarea  = 0;
            for (std::vector<vertex *>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
            Tarea+= (*it)->GetArea();
            double requiredNo = Tarea*m_Zoom*m_Zoom/(m_AP*double (m_pAllV.size()));
            m_Iteration = int (log(requiredNo)/log(4))+1;
            if(m_Iteration<5)
            std::cout<<"We will increase the number of the available points by 4^"<<m_Iteration<<"\n";
            else
            std::cout<<"The requested rescaling requires  4^"<<m_Iteration<<" points, this may take some time to finish \n";
    }
    
Rescaling(m_Zoom);
UpdateGeometry();
    
    for (std::vector<vertex *>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
    {
        double x = (*it)->GetVXPos();
        double y = (*it)->GetVYPos();
        double z = (*it)->GetVZPos();
        Vec3D normal = (*it)->GetNormalVector();
        x=x+layer*H*(normal(0));
        y=y+layer*H*(normal(1));
        z=z+layer*H*(normal(2));
        
        (*it)->UpdateVXPos(x);
        (*it)->UpdateVYPos(y);
        (*it)->UpdateVZPos(z);
        
    }
    
    UpdateGeometry();
#if BACKMAP == Enabled

    //
    std::vector <Surface_Mosaicing> Vmos;
    for (int j=0;j<m_Iteration;j++)
    {
        Surface_Mosaicing  MOS(m_MosAlType,m_smooth);
        Vmos.push_back(MOS);
    }
    for (int j=0;j<m_Iteration;j++)
    {
        (Vmos.at(j)).PerformMosaicing(m_pBox,  m_pAllV ,  m_pAllT , m_pHalfLinks1, m_pInc,m_Iteration-j);
        m_pAllV.clear();
        m_pAllT.clear();
        m_pAllLinks.clear();
        m_pHalfLinks1.clear();
        
        m_pAllLinks = (Vmos.at(j)).GetLinks();
        m_pAllV = (Vmos.at(j)).GetVertexs();
        m_pAllT = (Vmos.at(j)).GetTriangles();
        m_pHalfLinks1 = (Vmos.at(j)).GetHLinks();
        
    }
#elif
    std::cout<<"turn on Backmap Flag in the Def "<<std::endl;
#endif
  
    double Lx=(*m_pBox)(0);
    double Ly=(*m_pBox)(1);
    double Lz=(*m_pBox)(2);
    
  
    {
        Vec3D BoxSides=(*m_pBox);
        std::string filename;
        
        if(layer==1)
        filename=m_Folder+"visualization_data/Upper";
        if(layer==-1)
        filename=m_Folder+"visualization_data/Lower";
        VMDOutput GRO(BoxSides, m_pAllV , m_pHalfLinks1, filename);
        GRO.WriteGro();
        GRO.WriteGro2();
        WriteFiles vtu(m_pBox);
        std::string fi;
        if(layer==1)
        fi=m_Folder+"visualization_data/Upper.vtu";
        if(layer==-1)
        fi=m_Folder+"visualization_data/Lower.vtu";
        vtu.Writevtu(m_pAllV,m_pAllT,m_pAllLinks,fi);
    }
    
    //=============
    
    std::string     UFUpper = m_Folder+"/OuterBM.dat";
    std::string     UFInner = m_Folder+"/InnerBM.dat";
    
    
    
    FILE *BMFile1;
    if(layer==1)
    BMFile1 = fopen(UFUpper.c_str(), "w");
    FILE *BMFile2;
    if(layer==-1)
    BMFile2 = fopen(UFInner.c_str(), "w");
    
    const char* Cbox="Box";
    
    if(layer==1)
    fprintf(BMFile1,  "%s%8.3f%8.3f%8.3f\n",Cbox,Lx,Ly,Lz);
    if(layer==2)
    fprintf(BMFile2,  "%s%8.3f%8.3f%8.3f\n",Cbox,Lx,Ly,Lz);
    
    const char* STR1="< Point NoPoints";
    const char* STR2=">";
    
    int NoPoints=m_pAllV.size();
    if(layer==1)
    fprintf(BMFile1,  "%s%8d%s\n",STR1,NoPoints,STR2);
    if(layer==-1)
    fprintf(BMFile2,  "%s%8d%s\n",STR1,NoPoints,STR2);
    
    const char* Cont="< id area X Y Z Nx Ny Nz P1x P1y P1z P2x P2y P2z C1 C2  >";
    if(layer==1)
    fprintf(BMFile1,  "%s\n",Cont);
    if(layer==-1)
    fprintf(BMFile2,  "%s\n",Cont);
    
    {
        const char* lay="< Outer >";
        if(layer==1)
        fprintf(BMFile1,  "%s\n",lay);
    }
    {
        const char* lay="< Inner >";
        if(layer==-1)
        fprintf(BMFile2,  "%s\n",lay);
    }
    
    
    
    int i=0;
    double dr=1;
    if (m_monolayer==-1)
    dr=-1;
    for (std::vector<vertex *>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
    {
        
        double area=(*it)->GetArea();
        Vec3D normal=(*it)->GetNormalVector();
        normal = normal*(layer)*(dr);
        std::vector <double> curvature = (*it)->GetCurvature();
        //    inclusion* inc = (*it)->GetInclusion();
        Tensor2  L2G = (*it)->GetL2GTransferMatrix();
        Vec3D LD1(1,0,0);
        Vec3D GD1 = L2G*LD1;
        Vec3D LD2(0,1,0);
        Vec3D GD2 = L2G*LD2;
        double x=(*it)->GetVXPos();
        double y=(*it)->GetVYPos();
        double z=(*it)->GetVZPos();
        
        
        
        if(layer==1) fprintf(BMFile1,"%5d%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f\n",i,area,x,y,z,normal(0),normal(1),normal(2),GD1(0),GD1(1),GD1(2),GD2(0),GD2(1),GD2(2),curvature.at(0),curvature.at(1));
        
        if(layer==-1) fprintf(BMFile2,"%5d%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f\n",i,area,x,y,z,normal(0),normal(1),normal(2),GD1(0),GD1(1),GD1(2),GD2(0),GD2(1),GD2(2),curvature.at(0),curvature.at(1));
        
        i++;
        
    }
    
if(layer==1)
{
    
    FILE *IncFile;
    IncFile = fopen((m_Folder+"/IncData.dat").c_str(), "w");
    
    
    
    const char* CHAR1 ="< Inclusion NoInc   ";
    NoPoints=m_pInc.size();
    const char* CHAR2 ="   >";
    
    fprintf(IncFile,  "%s%5d%s\n",CHAR1,NoPoints,CHAR2);
    
    const char* CHAR3 ="< id typeid pointid lx ly lz  >";
    fprintf(IncFile,  "%s\n",CHAR3);
    
    
    
    i=0;
    
    for (std::vector<inclusion *>::iterator it = m_pInc.begin() ; it != m_pInc.end(); ++it)
    {
        
        int intypeid = (*it)->GetTypeID();
        vertex* ver = (*it)->Getvertex();
        Tensor2  L2G = ver->GetL2GTransferMatrix();
       // int Rot = (*it)->GetNSymmetry();
        Vec3D LD = (*it)->GetLDirection();
        Vec3D GD = L2G*LD;
        int verid=ver->GetVID();
        fprintf(IncFile,  "%5d%5d%5d%8.3f%8.3f%8.3f\n",i,intypeid,verid,GD(0),GD(1),GD(2));
        i++;
    }
}
    
    
    
    
}
bool Edit_configuration::check(std::string file){
    
    bool OK=true;
    Trajectory  TrajClass(m_pBox);
    double lm = 0.1;
    double lmax =1000;
    double mina = 0;
    Topology S(m_pBox, &mina, &lm, &lmax);
    Traj_XXX TSI(m_pBox);
    if(file.at(file.size()-1)=='t' && file.at(file.size()-2)=='a' && file.at(file.size()-3)=='d')
    {
        m_pAllV.clear();
        m_pInc.clear();
        m_pAllT.clear();
        m_pAllLinks.clear();
        m_pHalfLinks1.clear();
        m_pHalfLinks2.clear();
        
        TrajClass.Read(file,m_FindnewBox);
        m_pAllV=TrajClass.GetVertex();
        m_pAllT=TrajClass.GetTriangle();
        m_pAllLinks=TrajClass.GetLinks();
        m_pHalfLinks1=TrajClass.GetHalfLinks();
        m_pHalfLinks2=TrajClass.GetMHalfLinks();
        m_pInc=TrajClass.GetInclusion();
    }
    else if(file.at(file.size()-1)=='q' && file.at(file.size()-2)=='.' )
    {

        S.FastReadQFile(file);
        bool topohealth= S.GetTopologyHealth();
        if(topohealth==false)
        {
            std::cout<<" error: Provided TS file is bad \n";
        }
        m_pAllV.clear();
        m_pInc.clear();
        m_pAllT.clear();
        m_pAllLinks.clear();
        m_pHalfLinks1.clear();
        m_pHalfLinks2.clear();
        m_pAllV=S.GetVertex();
        m_pAllT=S.GetTriangle();
        m_pAllLinks=S.GetLinks();
        m_pHalfLinks1=S.GetHalfLinks();
        m_pHalfLinks2=S.GetMHalfLinks();
        
    }
    else if(file.at(file.size()-1)=='i' && file.at(file.size()-2)=='s' && file.at(file.size()-3)=='t')
    {
        m_pAllV.clear();
        m_pInc.clear();
        m_pAllT.clear();
        m_pAllLinks.clear();
        m_pHalfLinks1.clear();
        m_pHalfLinks2.clear();
        
        TSI.ReadTSI(file);
        m_pAllV=TSI.GetVertex();
        m_pAllT=TSI.GetTriangle();
        m_pAllLinks=TSI.GetLinks();
        m_pHalfLinks1=TSI.GetHalfLinks();
        m_pHalfLinks2=TSI.GetMHalfLinks();
        m_pInc=TSI.GetInclusion();
    }
    else
    {
        std::cout<<" error: TS Unknown File Format "<<file<<"\n";
        OK=false;
    }

    if(OK==true)
    {
        UpdateGeometry();
        double Tarea  = 0;
        double totalgaussianC = 0;
        double pi = acos(-1);
        for (std::vector<vertex *>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
        {
            
            Vec3D X((*it)->GetVXPos(),(*it)->GetVYPos(),(*it)->GetVZPos());
            Vec3D normal = (*it)->GetNormalVector();
            Tarea+= (*it)->GetArea();
            std::vector <double> C = (*it)->GetCurvature();
            totalgaussianC+= (C.at(0))*(C.at(1))*((*it)->GetArea());
        }
        
        std::cout<<" ************************************************************************************** \n";
        std::cout<<" Total area of the surface is "<<Tarea<<" nm^2 you can use -rescalefactor to increase it \n";
        std::cout<<" total gaussian curvature /4PI is: "<<totalgaussianC/(4*pi)<<" \n";

    }
        

    
    
    return true;
    
}

void Edit_configuration::MakeFlatMonolayer(int layer , std::string file, double H)
{



    
    for (std::vector<vertex *>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
    {
        double x = (*it)->GetVXPos();
        double y = (*it)->GetVYPos();
        double z = (*it)->GetVZPos();
        
        Vec3D normal = (*it)->GetNormalVector();

        x=x+layer*H*(normal(0));
        y=y+layer*H*(normal(1));
        z=z+layer*H*(normal(2));
        (*it)->UpdateVXPos(x);
        (*it)->UpdateVYPos(y);
        (*it)->UpdateVZPos(z);
        
    }


    
    double Lx=(*m_pBox)(0);
    double Ly=(*m_pBox)(1);
    double Lz=(*m_pBox)(2);
    
    

    
    //=============
    
    std::string     UFUpper = m_Folder+"/OuterBM.dat";
    std::string     UFInner = m_Folder+"/InnerBM.dat";
    

    
    FILE *BMFile1;
    if(layer==1)
        BMFile1 = fopen(UFUpper.c_str(), "w");
    FILE *BMFile2;
    if(layer==-1)
        BMFile2 = fopen(UFInner.c_str(), "w");
    
    const char* Cbox="Box";
    
    if(layer==1)
        fprintf(BMFile1,  "%s%8.3f%8.3f%8.3f\n",Cbox,Lx,Ly,Lz);
    if(layer==2)
        fprintf(BMFile2,  "%s%8.3f%8.3f%8.3f\n",Cbox,Lx,Ly,Lz);
    
    const char* STR1="< Point NoPoints";
    const char* STR2=">";
    
    int NoPoints=m_pAllV.size();
    if(layer==1)
        fprintf(BMFile1,  "%s%8d%s\n",STR1,NoPoints,STR2);
    if(layer==-1)
        fprintf(BMFile2,  "%s%8d%s\n",STR1,NoPoints,STR2);
    
    const char* Cont="< id area X Y Z Nx Ny Nz P1x P1y P1z P2x P2y P2z C1 C2  >";
    if(layer==1)
        fprintf(BMFile1,  "%s\n",Cont);
    if(layer==-1)
        fprintf(BMFile2,  "%s\n",Cont);
    
    {
        const char* lay="< Outer >";
        if(layer==1)
            fprintf(BMFile1,  "%s\n",lay);
    }
    {
        const char* lay="< Inner >";
        if(layer==-1)
            fprintf(BMFile2,  "%s\n",lay);
    }
    
    
    
    int i=0;
    srand(372);
    for (std::vector<vertex *>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
    {
        
        double area=(*it)->GetArea();
        Vec3D normal=(*it)->GetNormalVector();
        normal = normal*(layer);
        std::vector <double> curvature ;
        curvature.push_back(0);curvature.push_back(0);
        double tx=double(rand()%100)/100.0;
        double ty=double(rand()%100)/100.0;
        tx=tx/sqrt(tx*tx+ty*ty);
        ty=ty/sqrt(tx*tx+ty*ty);

        Vec3D LD1(tx,ty,0);
        Vec3D GD1(tx,ty,0);
        Vec3D LD2(ty,-tx,0);
        Vec3D GD2(ty,-tx,0);
        double x=(*it)->GetVXPos();
        double y=(*it)->GetVYPos();
        double z=(*it)->GetVZPos();
        
        
        if(layer==1) fprintf(BMFile1,"%5d%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f\n",i,area,x,y,z,normal(0),normal(1),normal(2),GD1(0),GD1(1),GD1(2),GD2(0),GD2(1),GD2(2),curvature.at(0),curvature.at(1));
        
        if(layer==-1) fprintf(BMFile2,"%5d%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f%8.3f\n",i,area,x,y,z,normal(0),normal(1),normal(2),GD1(0),GD1(1),GD1(2),GD2(0),GD2(1),GD2(2),curvature.at(0),curvature.at(1));
        
        i++;
        
    }
    

    
    
    
    
}
void Edit_configuration::Minimize(std::string file){
    
    bool OK=true;
    Trajectory  TrajClass(m_pBox);
    double lm = 0.1;
    double lmax =1000;
    double mina = 0;
    Topology S(m_pBox, &mina, &lm, &lmax);
    Traj_XXX TSI(m_pBox);
    if(file.at(file.size()-1)=='t' && file.at(file.size()-2)=='a' && file.at(file.size()-3)=='d')
    {
        m_pAllV.clear();
        m_pInc.clear();
        m_pAllT.clear();
        m_pAllLinks.clear();
        m_pHalfLinks1.clear();
        m_pHalfLinks2.clear();
        TrajClass.Read(file,m_FindnewBox);
        m_pAllV=TrajClass.GetVertex();
        m_pAllT=TrajClass.GetTriangle();
        m_pAllLinks=TrajClass.GetLinks();
        m_pHalfLinks1=TrajClass.GetHalfLinks();
        m_pHalfLinks2=TrajClass.GetMHalfLinks();
        m_pInc=TrajClass.GetInclusion();
    }
    else if(file.at(file.size()-1)=='q' && file.at(file.size()-2)=='.' )
    {
        
        S.FastReadQFile(file);
        bool topohealth= S.GetTopologyHealth();
        if(topohealth==false)
        {
            std::cout<<" error: Provided TS file is bad \n";
        }
        m_pAllV.clear();
        m_pInc.clear();
        m_pAllT.clear();
        m_pAllLinks.clear();
        m_pHalfLinks1.clear();
        m_pHalfLinks2.clear();
        m_pAllV=S.GetVertex();
        m_pAllT=S.GetTriangle();
        m_pAllLinks=S.GetLinks();
        m_pHalfLinks1=S.GetHalfLinks();
        m_pHalfLinks2=S.GetMHalfLinks();
        
    }
    else if(file.at(file.size()-1)=='i' && file.at(file.size()-2)=='s' && file.at(file.size()-3)=='t')
    {
        m_pAllV.clear();
        m_pInc.clear();
        m_pAllT.clear();
        m_pAllLinks.clear();
        m_pHalfLinks1.clear();
        m_pHalfLinks2.clear();
        
        TSI.ReadTSI(file);
        m_pAllV=TSI.GetVertex();
        m_pAllT=TSI.GetTriangle();
        m_pAllLinks=TSI.GetLinks();
        m_pHalfLinks1=TSI.GetHalfLinks();
        m_pHalfLinks2=TSI.GetMHalfLinks();
        m_pInc=TSI.GetInclusion();
    }
    else
    {
        std::cout<<" error: TS Unknown File Format "<<file<<"\n";
        OK=false;
    }
//=================================
    
    std::cout<<" We are trying to smoothing the TS for low scale geo \n";
    m_minRoughness = 0.4;
    if(OK==true)
    {
        UpdateGeometry();
        srand (8753);
        
        Surface_Mosaicing S(m_MosAlType, true);
        double DR=0.02;
        double DX = 0.1;
        for (std::vector<links *>::iterator it = m_pHalfLinks1.begin() ; it != m_pHalfLinks1.end(); ++it)
        {
            double linklength,midpointdistance;
            S.RoughnessOfALink((*it), &linklength, &midpointdistance);
            
            double oldroughness = midpointdistance-m_minRoughness*linklength;
            if(oldroughness>0)
            {
                vertex * pv1=(*it)->GetV1();
                vertex * pv2=(*it)->GetV2();
                Vec3D P10(pv1->GetVXPos(),pv1->GetVYPos(),pv1->GetVZPos());
                Vec3D P20(pv2->GetVXPos(),pv2->GetVYPos(),pv2->GetVZPos());

                pv1->UpdateOwnInclusion(true);
                pv2->UpdateOwnInclusion(true);

                bool stop=false;
                std::cout<<" Link with ID "<<(*it)->GetID()<<" and ver1 "<<pv1->GetVID()<<"  ver2 "<<pv1->GetVID()<<"\n";
                int loop= 0;
                while(stop==false)
                {
                    loop++;
                    double dx=1-2*((double(rand()%2000000)/2000000.0));           // Inside a cube with the side length of R
                    double dy=1-2*((double(rand()%2000000)/2000000.0));
                    double dz=1-2*((double(rand()%2000000)/2000000.0));
                    double chosever=((double(rand()%2000000)/2000000.0));

                    std::vector <double> C1=pv2->GetCurvature();
                    std::vector <double> C2=pv1->GetCurvature();
                    double A1=pv2->GetArea();
                    double A2=pv1->GetArea();
                    double E1=A1*(C1.at(0)+C1.at(1))*(C1.at(0)+C1.at(1))+A2*(C2.at(0)+C2.at(1))*(C2.at(0)+C2.at(1));


                    vertex *TV;
                    if(chosever>0.5)
                        TV=pv2;
                    else
                        TV=pv1;
                    
                    

                        VertexMove  vmove(TV, DR*dx, DR*dy, DR*dz,m_pBox);
                        vmove.Move();
                        C1.clear();C2.clear();
                        C1=pv2->GetCurvature();
                        C2=pv1->GetCurvature();
                        A1=pv2->GetArea();
                        A2=pv1->GetArea();
                        double E2=A1*(C1.at(0)+C1.at(1))*(C1.at(0)+C1.at(1))+A2*(C2.at(0)+C2.at(1))*(C2.at(0)+C2.at(1));
                        
                        S.RoughnessOfALink((*it), &linklength, &midpointdistance);
                        double roughness = midpointdistance-m_minRoughness*linklength;
                    
                    Vec3D P1(pv1->GetVXPos(),pv1->GetVYPos(),pv1->GetVZPos());
                    Vec3D P2(pv2->GetVXPos(),pv2->GetVYPos(),pv2->GetVZPos());
                    
                    (P1-P10).norm();
                    (P2-P20).norm();
                    
                        if(roughness<oldroughness  && ((P1-P10).norm())<DX && ((P2-P20).norm())<DX && E2-E1<0.1)
                            oldroughness = roughness;
                        else
                            vmove.RejectMove();

                        if((roughness<0 ||loop>10000))
                        {
                            std::cout<<" the roughness is reduced to "<<roughness<<"  with "<<loop<<" iterration \n";
                            stop=true;
                        }




                    
                }

            }

        }
        

        

        
    }
    
    
    WriteFiles vtu(m_pBox);
    std::string fi = "Smooth.vtu";
    vtu.Writevtu(m_pAllV,m_pAllT,m_pAllLinks,fi);
    
    std::ofstream output;
    output.open("smoothtopo.q");
    
    
    output<<std::fixed;
    output<<std::setprecision( Precision )<<(*m_pBox)(0)<<"   "<<(*m_pBox)(1)<<"   "<<(*m_pBox)(2)<<"   \n";
    output<<m_pAllV.size()<<"\n";
    for (int i=0;i<m_pAllV.size();i++)
    {
        vertex* a=m_pAllV.at(i);
        output<<std::setprecision( Precision )<<i<<"  "<<a->GetVXPos()<<" "<<a->GetVYPos()<<" "<<a->GetVZPos()<<" "<<0<<"\n";

    }
    output<< m_pAllT.size()<<"\n";
    for (int i=0;i<m_pAllT.size();i++)
    {

        triangle* a=m_pAllT.at(i);
        if(a->GetRepresentation()==true)
            output<<i<<"   "<<(a->GetV1())->GetVID()<<" "<<(a->GetV2())->GetVID()<<" "<<(a->GetV3())->GetVID()<<" "<<1<<"\n";
        else
            output<<i<<"   "<<(a->GetV1())->GetVID()<<" "<<(a->GetV2())->GetVID()<<" "<<(a->GetV3())->GetVID()<<" "<<0<<"\n";
    }
}



