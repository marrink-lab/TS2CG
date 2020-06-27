
#include <time.h>
#include <iomanip>
#include "Traj_XXX.h"


Traj_XXX::Traj_XXX(Vec3D *pBox)
{
 m_pBox=pBox;
m_Condition=true;
    Nfunction f;
    m_tsiPrecision = f.Int_to_String(18)+"."+f.Int_to_String(10);
}

Traj_XXX::~Traj_XXX()
{
    
}
void Traj_XXX::WriteTSI(int step ,  std::string filename , std::vector< vertex* > pver, std::vector< triangle* > ptriangle,  std::vector< inclusion* > pinc)
{
    FILE * output;
    output = fopen((filename).c_str(), "w");
    fprintf(output,"%5d\n",step);
    std::string format = "%"+m_tsiPrecision+"lf%"+m_tsiPrecision+"lf%"+m_tsiPrecision+"lf\n";
    fprintf(output,format.c_str(),(*m_pBox)(0),(*m_pBox)(1),(*m_pBox)(2));
    
    
    const char* ver="vertex";
    int size=pver.size();
    fprintf(output,"%s%20d\n",ver,size);
     format = "%5d%"+m_tsiPrecision+"lf%"+m_tsiPrecision+"lf%"+m_tsiPrecision+"lf\n";
    for (std::vector<vertex *>::iterator it = pver.begin() ; it != pver.end(); ++it)
    fprintf(output,format.c_str(),(*it)->GetVID(),(*it)->GetVXPos(),(*it)->GetVYPos(),(*it)->GetVZPos());
    
    const char* tri="triangle";
    size = ptriangle.size();
    fprintf(output,"%s%20d\n",tri,size);
    for (std::vector<triangle *>::iterator it = ptriangle.begin() ; it != ptriangle.end(); ++it)
    fprintf(output,"%5d%5d%5d%5d\n",(*it)->GetTriID(),((*it)->GetV1())->GetVID(),((*it)->GetV2())->GetVID(),((*it)->GetV3())->GetVID());
    

    const char* inc="inclusion";
    size = pinc.size();
    fprintf(output,"%s%20d\n",inc,size);
    format = "%5d%5d%5d%"+m_tsiPrecision+"lf%"+m_tsiPrecision+"lf\n";
    for (std::vector<inclusion *>::iterator it = pinc.begin() ; it != pinc.end(); ++it)
    fprintf(output,format.c_str(),(*it)->GetID(),(*it)->GetTypeID(),((*it)->Getvertex())->GetVID(),((*it)->GetLDirection())(0),((*it)->GetLDirection())(1));
    
    fclose(output);

}
void Traj_XXX::ReadTSI(std::string filename)
{
    //=======================
    //===== Clear the storage
    m_Vertex.clear();
    m_Triangle.clear();
    m_Links.clear();
    m_Inclusion.clear();
    m_pAllV.clear();
    m_pAllT.clear();
    m_pLinks.clear();
    m_pHL.clear();
    m_pMHL.clear();
    m_pInclusion.clear();
    //======================
    Nfunction f;
    char str[10];
    int step,nver,ntr,ninc,id,tid,vid;
    double x,y,z;

    int v1,v2,v3;
    FILE * input;
    input = fopen(filename.c_str(), "r");
    int rf = fscanf(input, "%d" ,&step );
    rf = fscanf(input, "%lf%lf%lf" ,&x,&y,&z );
    (*m_pBox)(0) = x;
    (*m_pBox)(1) = y;
    (*m_pBox)(2) = z;

    rf = fscanf(input, "%s%d" ,str,&nver );


    
    for (int i=0;i<nver;i++)
    {
        rf = fscanf(input, "%d%lf%lf%lf" ,&id,&x,&y,&z);
        vertex v(id,x,y,z);
        v.UpdateBox(m_pBox);
        v.UpdateClusterID(1);
        m_Vertex.push_back(v);
    }
    rf = fscanf(input, "%s%d" ,str,&ntr );
    
    for (int i=0;i<ntr;i++)
    {
        rf = fscanf(input, "%d%d%d%d" ,&id,&v1,&v2,&v3);
        bool pr=true;
        triangle T(id,&(m_Vertex.at(v1)),&(m_Vertex.at(v2)),&(m_Vertex.at(v3)));

        m_Triangle.push_back(T);
    }
    rf = fscanf(input, "%s%d" ,str,&ninc );
    for (int i=0;i<ninc;i++)
    {
        Vec3D D;
        rf = fscanf(input, "%d%d%d%lf%lf" ,&id,&tid,&vid,&x,&y);
        D(0)=x;D(1)=y;D(2)=0;
        inclusion Tinc(id);
        Tinc.Updatevertex(&(m_Vertex.at(vid)));
        Tinc.UpdateType("P"+f.Int_to_String(tid),tid);
        Tinc.UpdateLocalDirection(D);
        m_Inclusion.push_back(Tinc);
        (m_Vertex.at(vid)).UpdateOwnInclusion(true);
        


    }
    fclose(input);
    for (std::vector<inclusion>::iterator it = m_Inclusion.begin() ; it != m_Inclusion.end(); ++it)
    m_pInclusion.push_back(&(*it));
    //======= we finished the reading.
    for (std::vector<vertex>::iterator it = m_Vertex.begin() ; it != m_Vertex.end(); ++it)
        m_pAllV.push_back(&(*it));

    int li=-1;
    
    for (std::vector<triangle>::iterator it = m_Triangle.begin() ; it != m_Triangle.end(); ++it)
    {
        
        m_pAllT.push_back(&(*it));
        (it->GetV1())->AddtoTraingleList(&(*it));
        (it->GetV1())->AddtoNeighbourVertex((it->GetV2()));
        (it->GetV2())->AddtoTraingleList(&(*it));
        (it->GetV2())->AddtoNeighbourVertex((it->GetV3()));
        (it->GetV3())->AddtoTraingleList(&(*it));
        (it->GetV3())->AddtoNeighbourVertex((it->GetV1()));
        
        /// create links
        li++;
        int id1=li;
        li++;
        int id2=li;
        li++;
        int id3=li;
        
        links l1(id1,it->GetV1(),it->GetV2(),&(*it));
        l1.UpdateV3(it->GetV3());
        
        links l2(id2,it->GetV2(),it->GetV3(),&(*it));
        l2.UpdateV3(it->GetV1());
        
        links l3(id3,it->GetV3(),it->GetV1(),&(*it));
        l3.UpdateV3(it->GetV2());
        m_Links.push_back(l1);
        m_Links.push_back(l2);
        m_Links.push_back(l3);
        
    }
     li=-1;
    for (std::vector<triangle>::iterator it = m_Triangle.begin() ; it != m_Triangle.end(); ++it)
    {
        li++;
        int id1=li;
        li++;
        int id2=li;
        li++;
        int id3=li;
        links * l1=&(m_Links.at(id1));
        links * l2=&(m_Links.at(id2));
        links * l3=&(m_Links.at(id3));
        l1->UpdateNeighborLink1(l2);
        l1->UpdateNeighborLink2(l3);
        l2->UpdateNeighborLink1(l3);
        l2->UpdateNeighborLink2(l1);
        l3->UpdateNeighborLink1(l1);
        l3->UpdateNeighborLink2(l2);
        
        
        (it->GetV1())->AddtoLinkList(l1);
        (it->GetV2())->AddtoLinkList(l2);
        (it->GetV3())->AddtoLinkList(l3);

    }
    for (std::vector<links>::iterator it = m_Links.begin() ; it != m_Links.end(); ++it)
    {
        bool foundM=false;
        if((it)->GetMirrorFlag()==true)
        {
            m_pMHL.push_back(it->GetMirrorLink());
            m_pHL.push_back(&(*it));
            foundM = true;
        }
        else
        {
            vertex *v1=it->GetV1();
            vertex *v2=it->GetV2();
            
            std::vector<links*>  lList = v2->GetVLinkList();
            for (std::vector<links*>::iterator it2 = lList.begin() ; it2 != lList.end(); ++it2)
            {
                if(((*it2)->GetV2())->GetVID()==v1->GetVID())
                {
                    it->UpdateMirrorLink((*it2));
                    (*it2)->UpdateMirrorLink(&(*it));
                    it->UpdateMirrorFlag(true);
                    (*it2)->UpdateMirrorFlag(true);
                    foundM = true;
                    break;
                }
            }
        }
        if(foundM == false)
        {
            std::string A=" Warning : This system does not look like a closed system or the triangles orination are not consistent ";
            std::cout<<A<<"\n";
            f.Write_One_LogMessage(A);
        }
        
    }
    int nomirror=0;
    for (std::vector<links>::iterator it = m_Links.begin() ; it != m_Links.end(); ++it)
    {
        if(it->GetMirrorFlag()==false)
        {
            nomirror++;
        }
        
    }
    if(nomirror!=0)
    {
        std::string A=" Warning : there is links without mirror ";
        std::cout<<nomirror<<" links without mirror \n";
        f.Write_One_LogMessage(A);
    }
    for (std::vector<links>::iterator it = m_Links.begin() ; it != m_Links.end(); ++it)
    {
        
        m_pLinks.push_back(&(*it));
    }
    
    for (std::vector<inclusion*>::iterator it = m_pInclusion.begin() ; it != m_pInclusion.end(); ++it)
    {
        ((*it)->Getvertex())->UpdateInclusion(*it);
    }

}


