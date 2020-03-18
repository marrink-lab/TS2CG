
#include <time.h>
#include <iomanip>
#include "Trajectory.h"


Trajectory::Trajectory(Vec3D *pBox)
{
 m_pBox=pBox;



m_TrajectoryCondition=true;




}

Trajectory::~Trajectory()
{
    
}

void Trajectory::Write(int step ,  std::string filename , std::vector< vertex* > pver, std::vector< triangle* > ptriangle,  std::vector< links* > plinks, std::vector< inclusion* > pinc)
{
    
    
    int pres=Precision;
    
    
    
    
    
    std::ofstream File;
    File.open(filename.c_str());
    m_pAllV=pver;
    m_pLinks=plinks;
    m_pAllT=ptriangle;
    
    int nver=m_pAllV.size();
    int ntri=m_pAllT.size();
    int nli=m_pLinks.size();
    int ninc=pinc.size();
    
    File<<std::fixed;
    File<<"Step  "<<step<<"  \n";
    File<<"Box  "<<std::setprecision( pres )<<(*m_pBox)(0)<<"  "<<(*m_pBox)(1)<<"  "<<(*m_pBox)(2)<<"  \n";
    File<<nver<<"   "<<ntri<<"  "<<nli<<" "<<ninc<<"\n";
    
    
    std::vector<int> inctypeid;
    std::vector<std::string> inctype;
    for (std::vector<inclusion *>::iterator it = pinc.begin() ; it != pinc.end(); ++it)
    {
        bool picked=false;
        for (std::vector<int>::iterator it1 = inctypeid.begin() ; it1 != inctypeid.end(); ++it1)
        {
            if((*it1)==(*it)->GetTypeID())
                picked=true;
        }
        if(picked==false)
        {
            inctypeid.push_back((*it)->GetTypeID());
            inctype.push_back((*it)->GetType());
        }
    }
    

    File<<"No_InclusionType  "<<inctypeid.size()<<"   \n";
    for (int i=0;i<inctypeid.size();i++)
    {
        File<<inctypeid.at(i)<<"   "<<inctype.at(i)<<"   ";
    }
    File<<"\n";
    
    for (std::vector<vertex *>::iterator it = pver.begin() ; it != pver.end(); ++it)
    {
        
        (*it)->WriteVertexToFile(&File);
        
        
    }
    
    for (std::vector<triangle *>::iterator it = ptriangle.begin() ; it != ptriangle.end(); ++it)
    {
        
        (*it)->WriteTriangleToFile(&File);
        
    }
    for (std::vector<links *>::iterator it = plinks.begin() ; it != plinks.end(); ++it)
    {
        
        (*it)->WriteLinkToFile(&File);
        
    }
    for (std::vector<inclusion *>::iterator it = pinc.begin() ; it != pinc.end(); ++it)
    {
        
        (*it)->WriteInclusionToFile(&File);
        
    }
    
    File<<"END \n";
    File.close();
    
    
}
void Trajectory::Read(std::string filename)
{


m_Vertex.clear();
m_Triangle.clear();
m_Links.clear();
m_pAllV.clear();
m_pAllT.clear();
m_pLinks.clear();
m_pHL.clear();
m_pMHL.clear();
m_pInclusion.clear();
m_Inclusion.clear();




	std::ifstream file(filename.c_str());

     if(file)
	{
		
		bool fco=CheckFile(filename);
		if(fco!=true)
		m_TrajectoryCondition=false;
	}
	else
	{
		m_TrajectoryCondition=false;

	}

file.close();
if(m_TrajectoryCondition==true)
{

ReadTrajectoryFile(filename);


}
else
{
std::string sms="the Trajectory file is broken ";
m_f.Write_One_LogMessage(sms);

}

}

bool Trajectory::CheckFile(std::string file1)
{
bool is=true;
     	std::ifstream input;
    	input.open(file1.c_str());


	std::string name;

	while(true)
	{
		input>>name;
		
		if(input.eof())
		{

			if(name!="END")
			{
			   is=false;
			
			}
		break;
		}

	}

input.close();
return is;

}
void Trajectory::ReadTrajectoryFile(std::string filename)
{
    	std::ifstream input;
    	input.open(filename.c_str());
	std::string box,step,temstr;
	double  lx,ly,lz;
  	input>>step>>m_iniStep;
        input>>box>>lx>>ly>>lz;
	if(box=="Box" && step=="Step")
	{


		(*m_pBox)(0)=lx;
		(*m_pBox)(1)=ly;
		(*m_pBox)(2)=lz;

	}
	else
	{
	   m_TrajectoryCondition=false;
		std::string sms="Trajectory file is broken ";
		m_f.Write_One_LogMessage(sms);
		
	}
    int ntri,nver,nlink,ninc;
    
    input>>nver>>ntri>>nlink>>ninc;
    
    int noinctype=0;
    int inctypeid;
    input>>temstr>>noinctype;
    if(temstr=="No_InclusionType")
    {
        for (int i=0;i<noinctype;i++)
        {
            input>>inctypeid>>temstr;
            m_Inclusion_TypeID.push_back(inctypeid);
            m_Inclusion_Type.push_back(temstr);
        }
    }
    else
    {
        m_TrajectoryCondition=false;
        std::string sms="restart file is broken ";
        m_f.Write_One_LogMessage(sms);
    }
    
    for (int i=0;i<nver;i++)
    {
        vertex v(i);
        v.UpdateBox(m_pBox);
        
        m_Vertex.push_back(v);
    }
    for (int i=0;i<ntri;i++)
    {
        triangle t(i);
        m_Triangle.push_back(t);
    }
    for (int i=0;i<nlink;i++)
    {
        links l(i);
        m_Links.push_back(l);
    }
    for (int i=0;i<ninc;i++)
    {
        inclusion inc(i);
        m_Inclusion.push_back(inc);
    }
    double  t=clock();
    for (std::vector<vertex>::iterator it = m_Vertex.begin() ; it != m_Vertex.end(); ++it)
    {
        m_pAllV.push_back(&(*it));
    }
    
    for (std::vector<links>::iterator it = m_Links.begin() ; it != m_Links.end(); ++it)
    {
        m_pLinks.push_back(&(*it));
    }
    
    for (std::vector<triangle>::iterator it = m_Triangle.begin() ; it != m_Triangle.end(); ++it)
    {
        m_pAllT.push_back(&(*it));
    }
    for (std::vector<inclusion>::iterator it = m_Inclusion.begin() ; it != m_Inclusion.end(); ++it)
    {
        m_pInclusion.push_back(&(*it));
    }
  //  t=clock()-t;
   // std::cout<<" This takes long " <<((float)t)/CLOCKS_PER_SEC<<" s\n";
   // t=clock();

    for (std::vector<vertex*>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
    {
        (*it)->ReadVertexFromFile(&input,m_pAllV, m_pLinks, m_pAllT, m_pInclusion);
    }
 //   t=clock()-t;
  //  std::cout<<" vertex read " <<((float)t)/CLOCKS_PER_SEC<<" s\n";
   // t=clock();

    for (std::vector<triangle*>::iterator it = m_pAllT.begin() ; it != m_pAllT.end(); ++it)
    {
        (*it)->ReadTriangleFromFile(&input,m_pAllV);
    }
//    t=clock()-t;
 //   std::cout<<" tri read " <<((float)t)/CLOCKS_PER_SEC<<" s\n";
  //  t=clock();
    for (std::vector<links*>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
    {
        (*it)->ReadLinkFromFile(&input,m_pAllV,m_pLinks,m_pAllT);
    }
 //   t=clock()-t;
  //  std::cout<<" link read " <<((float)t)/CLOCKS_PER_SEC<<" s\n";
   // t=clock();
    for (std::vector<inclusion*>::iterator it = m_pInclusion.begin() ; it != m_pInclusion.end(); ++it)
    {
        (*it)->ReadInclusionFromFile(&input,m_pAllV);
    }
    for (std::vector<links *>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
    {
        if((*it)->GetLinkSide()==1)
            m_pHL.push_back((*it));
        else
            m_pMHL.push_back((*it));
        
    }
    
    
    
    
    
    input>>temstr;
	if(temstr!="END")
	{
		std::string sms="the Trajectory file does not end correctly with END. It ends with "+temstr;
		m_f.Write_One_ErrorMessage(sms);
		m_TrajectoryCondition=false;
	}


}

