

#include <stdio.h>
#include "GenerateUnitCells.h"
#include "Nfunction.h"
//#include "links.h"
GenerateUnitCells::GenerateUnitCells(std::vector< bead* > bead,Argument *pArgu,Vec3D *pBox)
{
	m_pBox=pBox;
    m_pAllBead=bead;
    m_pArgu=pArgu;
     m_CNTSize=1.0;

}


GenerateUnitCells::~GenerateUnitCells()
{
    
}

int GenerateUnitCells::IDFromIndex(int i,int j,int k)
{

int n=i+m_Nx*m_Ny*k+m_Nx*j;

return n;
}

void GenerateUnitCells::Generate()
{
   
    m_AllCNTCells.clear();
    m_CNTCellSize.clear();
    m_CNTCellNo.clear();
    
    
    
    
    //==================
    //====
    //==================
    Nfunction f;
    std::vector <double> Box;
    Box.push_back((*m_pBox)(0));
    Box.push_back((*m_pBox)(1));
    Box.push_back((*m_pBox)(2));
    
    double CNTSize=m_CNTSize;
    
    
    
    /// The CNT cell should not be smaller then one; for dynamic box we should be carefull
    
    m_Nx=int(Box.at(0)/CNTSize);
    m_Ny=int(Box.at(1)/CNTSize);
    m_Nz=int(Box.at(2)/CNTSize);
    


    if(Box.at(0)/double(m_Nx)==CNTSize)
    {
        
    }
    else
    {
        //m_Nx=m_Nx+1; this is wrong it makes the box smaller 
        std::string A="CNT cell in x direction has the size of "+f.Int_to_String(Box.at(0)/double(m_Nx));
       // std::cout<<A<<"\n";

    }
    
    if(Box.at(1)/double(m_Ny)==CNTSize)
    {
        
    }
    else
    {
       // m_Ny=m_Ny+1;
        std::string A="CNT cell in y direction has the size of "+f.Int_to_String(Box.at(1)/double(m_Ny));
      //  std::cout<<A<<"\n";
    }
    if(Box.at(2)/double(m_Nz)==CNTSize)
    {
        
    }
    else
    {
      //  m_Nz=m_Nz+1;
        std::string A="CNT cell in z direction has the size of "+f.Int_to_String(Box.at(2)/double(m_Nz));
       // std::cout<<A<<"\n";
    }
    
    
    
    
    m_CNTCellSize.push_back(Box.at(0)/double(m_Nx));
    m_CNTCellSize.push_back(Box.at(1)/double(m_Ny));
    m_CNTCellSize.push_back(Box.at(2)/double(m_Nz));
    m_CNTCellNo.push_back(m_Nx);
    m_CNTCellNo.push_back(m_Ny);
    m_CNTCellNo.push_back(m_Nz);
    

    //======================================
    //====== Adding beads to CNT
    //=======================================
    
    
    int m=0;
    
    for (std::vector<bead *>::iterator it1 = m_pAllBead.begin() ; it1 != m_pAllBead.end(); ++it1)
    {

        
        double x=(*it1)->GetXPos();
        double y=(*it1)->GetYPos();
        double z=(*it1)->GetZPos();
        
        int nx=int(x/m_CNTCellSize.at(0));
        int ny=int(y/m_CNTCellSize.at(1));
        int nz=int(z/m_CNTCellSize.at(2));
        int id=IDFromIndex(nx,ny,nz);

        std::string cellid =f.Int_to_String(nx)+" "+f.Int_to_String(ny)+" "+f.Int_to_String(nz);

        std::map <std::string, UnitCell >::iterator it = m_AllCNTCells.find(cellid);
      //  std::cout<<m_AllCNTCells.size()<<"  here is arrived4 \n";

        if (it == m_AllCNTCells.end())
        {
            UnitCell T(m,nx,ny,nz);
            m_AllCNTCells.insert (it, std::pair<std::string, UnitCell >(cellid,T));
            m++;
        }
        std::map <std::string, UnitCell >::iterator it2 = m_AllCNTCells.find(cellid);
        (it2->second).AddtoVertexList(*it1);
        
        

        
    }
    
    
}
bool GenerateUnitCells::anythingaround (Vec3D PX)
{
    Nfunction f;
    bool any=false;
    double cutoff2 = 0.5;
    int nx=PX(0)/m_CNTCellSize.at(0);
    int ny=PX(1)/m_CNTCellSize.at(1);
    int nz=PX(2)/m_CNTCellSize.at(2);
    
    
    for (int i=-1;i<2;i++)
    {
        for (int j=-1;j<2;j++)
        {
            for (int k=-1;k<2;k++)
            {
                int mx=(nx+i+m_CNTCellNo.at(0))%(m_CNTCellNo.at(0));
                int my=(ny+j+m_CNTCellNo.at(1))%(m_CNTCellNo.at(1));
                int mz=(nz+k+m_CNTCellNo.at(2))%(m_CNTCellNo.at(2));
                
              //  std::cout<<mx<<"  "<<my<<"  "<<mz<<"\n";
                
                std::string cellid =f.Int_to_String(mx)+" "+f.Int_to_String(my)+" "+f.Int_to_String(mz);
                std::map <std::string, UnitCell >::iterator it = m_AllCNTCells.find(cellid);
                
               if (it != m_AllCNTCells.end())
               {

                std::vector<bead*> cbeads = (it->second).GetBeadList();
                for (std::vector<bead *>::iterator it2 = cbeads.begin() ; it2 != cbeads.end(); ++it2)
                {
                    Vec3D X;
                    X(0) = (*it2)->GetXPos();
                    X(1) = (*it2)->GetYPos();
                    X(2) = (*it2)->GetZPos();
                    
                    double dist2=dist2between2Points(X,PX);

                    if(dist2<cutoff2)
                    {
                        any =true;
                        break;
                    }
                    
                }
               }
            }
            
        }
    }

    
    return any;
}
double GenerateUnitCells::dist2between2Points(Vec3D X1,Vec3D X2)
{
    
    double dist2=0;
    
    double x1=X1(0);
    double y1=X1(1);
    double z1=X1(2);
    
    double x2=X2(0);
    double y2=X2(1);
    double z2=X2(2);
    
    
    double dx=x2-x1;
    double dy=y2-y1;
    double dz=z2-z1;
    
    if(fabs(dx)>(*m_pBox)(0)/2.0)
    {
        if(dx<0)
            dx=(*m_pBox)(0)+dx;
        else if(dx>0)
            dx=dx-(*m_pBox)(0);
    }
    if(fabs(dy)>(*m_pBox)(1)/2.0)
    {
        if(dy<0)
            dy=(*m_pBox)(1)+dy;
        else if(dy>0)
            dy=dy-(*m_pBox)(1);
    }
    if(fabs(dz)>(*m_pBox)(2)/2.0)
    {
        if(dz<0)
            dz=(*m_pBox)(2)+dz;
        else if(dz>0)
            dz=dz-(*m_pBox)(2);
    }
    
    dist2=dx*dx+dy*dy+dz*dz;
    return dist2;
}


