 #if !defined(AFX_VMDOutput_CPP_7F4A21B7_C13C_1223_TF2E_124095086234__INCLUDED_)
#define AFX_VMDOutput_CPP_7F4A21B7_C13C_1223_TF2E_124095086234__INCLUDED_


#include "VMDOutput.h"


VMDOutput::VMDOutput(Vec3D Box, std::vector<vertex* > pver , std::vector<links* > plinks, std::string Filename)
{

m_Box=Box;

m_pVers=pver;
m_pLinks=plinks;



m_Filename=Filename;







}

VMDOutput::~VMDOutput()
{
    
}












void VMDOutput::WriteGro()
{


        for (std::vector<links *>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
        {
		(*it)->UpdateVisualize(true);
	}
        for (std::vector<links *>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
        {
		vertex * v1=(*it)->GetV1();
		vertex * v2=(*it)->GetV2();
            vertex * v3=(*it)->GetV3();


		double x1=v1->GetVXPos();
		double x2=v2->GetVXPos();
		double y1=v1->GetVYPos();
		double y2=v2->GetVYPos();
		double z1=v1->GetVZPos();
		double z2=v2->GetVZPos();
		double dx=x2-x1;
		double dy=y2-y1;
		double dz=z2-z1;
		bool rep=true;

		if(fabs(dx)>m_Box(0)/2.0)
		{
			rep=false;
		}
		else if(fabs(dy)>m_Box(1)/2.0)
		{
			rep=false;
		}
		else if(fabs(dz)>m_Box(2)/2.0)
		{
			rep=false;
		} 
		if(rep==false)
		{
			
			(*it)->UpdateVisualize(rep);
		

		}


	}




std::string Filename=m_Filename+".gro";


	FILE *fgro;
    fgro = fopen(Filename.c_str(), "w");

    
                                                     
    							/// resid  res name   noatom   x   y   z
                                                     const char* Title="Network";
                                                     int Size=m_pVers.size();
                                                     
                                                       fprintf(fgro,  "%s\n",Title);
                                                       fprintf(fgro, "%5d\n",Size);
                                                     int i=0;
          for (std::vector<vertex *>::iterator it = m_pVers.begin() ; it != m_pVers.end(); ++it)
           {
		i++;
		double x=(*it)->GetVXPos();
		double y=(*it)->GetVYPos();
		double z=(*it)->GetVZPos();

		if((*it)->VertexOwnInclusion()==false)
		{
            const char* B="C";
            const char* A="Ver";
            int resid=0;
		  fprintf(fgro, "%5d%5s%5s%5d%8.3f%8.3f%8.3f%8.4f%8.4f%8.4f\n",resid,A,B,i,x/5.0,y/5.0,z/5.0,0.0,0.0,0.0 );
		}
		else
		{
            const char* B="O";
            const char* A=(((*it)->GetInclusion())->GetType()).c_str();
            int resid=((*it)->GetInclusion())->GetTypeID();
    		fprintf(fgro, "%5d%5s%5s%5d%8.3f%8.3f%8.3f%8.4f%8.4f%8.4f\n",resid,A,B,i,x/5.0,y/5.0,z/5.0,0.0,0.0,0.0 );
		}
               if(i>20000)
                   i=0;
           }




   fprintf(fgro,  "%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f\n",m_Box(0)/5,m_Box(1)/5,m_Box(2)/5,0.0,0.0,0.0,0.0,0.0,0.0 );
   fclose(fgro);
//=================================== 
//========= make the topology file
//=======================
std::string topfile=m_Filename+".top";

                                  std::ofstream Topfile;
                                  Topfile.open(topfile.c_str());


Topfile<<"[ defaults ] \n";
Topfile<<"1 1 \n";
Topfile<<"[ atomtypes ] \n";
Topfile<<"CT1 72.0 0.000 A 0.0 0.0  \n";





Topfile<<"[ moleculetype ] \n";
Topfile<<" surface    1 \n";
Topfile<<" [atoms] \n";
i=0;
          for (std::vector<vertex *>::iterator it = m_pVers.begin() ; it != m_pVers.end(); ++it)
           {
		i++;
               int resid=(*it)->GetClusterID();

		Topfile<<i<<" 	CT1 	"<<resid <<"	Sur 	C 	"<<i<<" 	0  \n";

           }
Topfile<<" [bonds] \n";

          for (std::vector<links *>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
           {
		i++;
		if((*it)->GetVisualize()==true)
		{
		int id1=((*it)->GetV1())->GetVID();
		int id2=((*it)->GetV2())->GetVID();
		Topfile<<id1+1<<"   "<<id2+1<<" 	1 	1  	20000\n";
		}

           }

Topfile<<"[ system ] \n";
Topfile<<" triangluated surface \n";

Topfile<<"[ molecules ] \n";
Topfile<<" surface 1 \n";




}

/////////////////////////
void VMDOutput::WriteGro2()
{
    
    
    for (std::vector<links *>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
    {
        (*it)->UpdateVisualize(true);
    }
    for (std::vector<links *>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
    {
        vertex * v1=(*it)->GetV1();
        vertex * v2=(*it)->GetV2();
        vertex * v3=(*it)->GetV3();
        
        
        double x1=v1->GetVXPos();
        double x2=v2->GetVXPos();
        double y1=v1->GetVYPos();
        double y2=v2->GetVYPos();
        double z1=v1->GetVZPos();
        double z2=v2->GetVZPos();
        double dx=x2-x1;
        double dy=y2-y1;
        double dz=z2-z1;
        bool rep=true;
        
        if(fabs(dx)>m_Box(0)/2.0)
        {
            rep=false;
        }
        else if(fabs(dy)>m_Box(1)/2.0)
        {
            rep=false;
        }
        else if(fabs(dz)>m_Box(2)/2.0)
        {
            rep=false;
        }
        if(rep==false)
        {
            
            (*it)->UpdateVisualize(rep);
            
            
        }
        
        
    }
    
    
    
    
    std::string Filename=m_Filename+".gro";
    
    
    FILE *fgro;
    fgro = fopen(Filename.c_str(), "w");
    
    
    
    /// resid  res name   noatom   x   y   z
    const char* Title="Network";
    int Size=m_pVers.size();
    
    fprintf(fgro,  "%s\n",Title);
    fprintf(fgro, "%5d\n",Size);
    int i=0;
    for (std::vector<vertex *>::iterator it = m_pVers.begin() ; it != m_pVers.end(); ++it)
    {
        i++;
        double x=(*it)->GetVXPos();
        double y=(*it)->GetVYPos();
        double z=(*it)->GetVZPos();
        
        if((*it)->VertexOwnInclusion()==false)
        {
            const char* B="C";
            const char* A="Ver";
            int resid=0;
            fprintf(fgro, "%5d%5s%5s%5d%8.3f%8.3f%8.3f%8.4f%8.4f%8.4f\n",resid,A,B,i,x,y,z,0.0,0.0,0.0 );
        }
        else
        {
            const char* B="O";
            const char* A=(((*it)->GetInclusion())->GetType()).c_str();
            int resid=((*it)->GetInclusion())->GetTypeID();
            fprintf(fgro, "%5d%5s%5s%5d%8.3f%8.3f%8.3f%8.4f%8.4f%8.4f\n",resid,A,B,i,x,y,z,0.0,0.0,0.0 );
        }
        
        if(i>20000)
            i=0;
    }
    
    
    
    
    fprintf(fgro,  "%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f%10.5f\n",m_Box(0),m_Box(1),m_Box(2),0.0,0.0,0.0,0.0,0.0,0.0 );
    fclose(fgro);
 
    
    
}
#endif



