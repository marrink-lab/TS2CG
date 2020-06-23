

#include <stdio.h>
#include "Wall.h"
Wall::Wall()
{

    m_State = false;
    m_Density = 1;
    m_H = 0;
    m_BeadName = "WL";

}
Wall::~Wall()
{
    
}
void Wall::UpdateBox(Vec3D *x)
{
m_pBox=x;
}
void Wall::UpdateState(bool x)
{
    m_State = x;
}
void Wall::UpdateH(double x)
{
    m_H = x;
}
void Wall::UpdateDen(double x)
{
    m_Density = x;
}
void Wall::UpdateBeadName(std::string x)
{
    m_BeadName = x;
}
void Wall::PrintWallState()
{
    std::cout<<"---> Wall info \n";
    std::cout<<"---> state "<<m_State<<"   H   "<<m_H<<"   Density "<<m_Density<<"  Wall beads name "<<m_BeadName<<"\n";

 }
void Wall::CreateWall(std::vector<point*>  p1, std::vector<point*>  p2)
{
    if(m_State==true)
    {
        std::cout<<"We are creating a wall beads for this system with: \n";
        PrintWallState();
        
        
        
        //============ Make the wall top file
        std::string file="Wall.itp";
        FILE *fitp;
        fitp = fopen(file.c_str(), "w");
        const char* atype=" [ atomtypes ] ";
        const char* AA=" A ";
        fprintf(fitp,  "%s\n",atype);
        fprintf(fitp, "%5s%8.3f%8.3f%5s%8.3f%8.3f\n",m_BeadName.c_str(),72.0,0.00,AA,0.0,0.0);
        
        {
            const char* tem=" [ nonbond_params ] ";
            fprintf(fitp,  "%s\n",tem);
            const char* tem2=" C1     WL     1    0.47  0.500 ";
            fprintf(fitp,  "%s\n",tem2);
        }
        {
        const char* tem=" [ moleculetype ] ";
        fprintf(fitp,  "%s\n",tem);
        }
        {
            const char* tem=" Wall              1 ";
            fprintf(fitp,  "%s\n",tem);
        }
        {
            const char* tem=" [ atoms ] ";
            fprintf(fitp,  "%s\n",tem);
        }
        {
            std::string str =  "1      "+m_BeadName+"      1        wall  "+m_BeadName +"     1       0";
            const char* tem=str.c_str();
            fprintf(fitp,  "%s\n",tem);
        }
        {
            const char* tem1="[ position_restraints ]";
            fprintf(fitp,  "%s\n",tem1);
            const char* tem2="1    1 1000 1000 1000";
            fprintf(fitp,  "%s\n",tem2);
        }
        fclose (fitp);
        
        
        // m_AllWallBeads
        //======== Make the wall beads
        int i=0;
        int resid = 0;
    
        for (std::vector<point*>::iterator it = p1.begin() ; it != p1.end(); ++it)
        {
            int trand = rand()%100000000;
            double prob=double(trand)/100000000.0;
            int id=i;
            Vec3D X=(*it)->GetPos();
            Vec3D N=(*it)->GetNormal();
            X=X+N*(m_H);
            bead  tb(id, m_BeadName, m_BeadName, "Wall", resid,X(0), X(1),X(2));
            if(prob<m_Density)
            {
                m_AllWallBeads.push_back(tb);
                m_AllWallPoints.push_back(*it);
                i++;
                resid++;
            }

        }
        for (std::vector<point*>::iterator it = p2.begin() ; it != p2.end(); ++it)
        {
            int trand = rand()%100000000;
            double prob=double(trand)/100000000.0;
            int id=i;
            Vec3D X=(*it)->GetPos();
            Vec3D N=(*it)->GetNormal();
            X=X+N*(m_H);
            bead  tb(id, m_BeadName, m_BeadName, "Wall", resid,X(0), X(1),X(2));
            if(prob<m_Density)
            {
                m_AllWallBeads.push_back(tb);
                m_AllWallPoints.push_back(*it);
                i++;
                resid++;

            }


            
        }
        

        
    }
    else
    {
        
    }
    
}













