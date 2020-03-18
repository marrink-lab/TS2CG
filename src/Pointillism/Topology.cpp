

#include <iomanip>
#include "Topology.h"
#include "Nfunction.h"

Topology::Topology(Vec3D *pBox, double * pminangle, double * pLmin , double * pLmax)
{
    m_pBox=pBox;
    m_Box(0)=(*pBox)(0);
    m_Box(1)=(*pBox)(1);
    m_Box(2)=(*pBox)(2);
    
    
    
    
    
    m_pLmin2 = pLmin;
    m_pLmax2 = pLmax;
    m_pminAngle = pminangle;
    
    m_TopologyHealth = true;
    
    
    
    
    
    
}

Topology::~Topology()
{
    
}












void Topology::Write(std::vector< vertex* > ver, std::vector< triangle* > triangle1,  std::string Filename)
{
    
    int pres=Precision;
    std::ofstream output;
    if(Filename.at(Filename.size()-1)!='q' || Filename.at(Filename.size()-2)!='.')
        Filename = Filename +".q";
        
    output.open(Filename.c_str());
    
    
    output<<std::fixed;
    output<<std::setprecision( pres )<<(*m_pBox)(0)<<"   "<<(*m_pBox)(1)<<"   "<<(*m_pBox)(2)<<"   \n";
    output<<ver.size()<<"\n";
    for (int i=0;i<ver.size();i++)
    {
        vertex* a=ver.at(i);
        int clusterid=a->GetClusterID();
        output<<std::setprecision( pres )<<i<<"  "<<a->GetVXPos()<<" "<<a->GetVYPos()<<" "<<a->GetVZPos()<<" "<<clusterid<<"\n";
        
        
    }
    output<< triangle1.size()<<"\n";
    for (int i=0;i<triangle1.size();i++)
    {
        
        
        triangle* a=triangle1.at(i);
        
        if(a->GetRepresentation()==true)
            output<<i<<"   "<<(a->GetV1())->GetVID()<<" "<<(a->GetV2())->GetVID()<<" "<<(a->GetV3())->GetVID()<<" "<<1<<"\n";
        else
            output<<i<<"   "<<(a->GetV1())->GetVID()<<" "<<(a->GetV2())->GetVID()<<" "<<(a->GetV3())->GetVID()<<" "<<0<<"\n";
    }
    
    
}
double Topology::LengthBetweenTwoVertex(vertex* v1, vertex* v2)
{
    
    
    Vec3D Box;
    Box(0)= (*m_pBox)(0);
    Box(1)= (*m_pBox)(1);
    Box(2)= (*m_pBox)(2);
    
    double x2=v2->GetVXPos();
    double y2=v2->GetVYPos();
    double z2=v2->GetVZPos();
    double x1=v1->GetVXPos();
    double y1=v1->GetVYPos();
    double z1=v1->GetVZPos();
    double dx=x2-x1;
    double dy=y2-y1;
    double dz=z2-z1;
    
    
    if(fabs(dx)>Box(0)/2.0)
    {
        
        
        if(dx<0)
            dx=Box(0)+dx;
        else if(dx>0)
            dx=dx-Box(0);
        
        
    }
    if(fabs(dy)>Box(1)/2.0)
    {
        
        
        if(dy<0)
            dy=Box(1)+dy;
        else if(dy>0)
            dy=dy-Box(1);
        
        
    }
    
    if(fabs(dz)>Box(2)/2.0)
    {
        if(dz<0)
            dz=Box(2)+dz;
        else if(dz>0)
            dz=dz-Box(2);
    }
    
    
    double l2=dx*dx+dy*dy+dz*dz;
    
    
    
    
    
    
    return l2;
}

bool   Topology::CheckFaceAngle(links * l)
{
    bool is=true;
    
    vertex* v1 = l->GetV1();
    vertex* v2 = l->GetV2();
    vertex* v3 = l->GetV3();
    vertex* v4 = (l->GetMirrorLink())->GetV3();
    
    
    
    Vec3D   N1=CalculateNormal(v1,v2,v3);
    Vec3D   N2=CalculateNormal(v1,v4,v2);
    
    
    
    
    
    if(N1.dot(N1,N2)<(*m_pminAngle))
    {
        is=false;
    }
    
    
    
    return is;
}
Vec3D   Topology::CalculateNormal(vertex* v1 ,vertex* v2 ,vertex* v3)
{
    Vec3D N;
    
    
    
    double x1=v1->GetVXPos();
    double y1=v1->GetVYPos();
    double z1=v1->GetVZPos();
    double x2=v2->GetVXPos();
    double y2=v2->GetVYPos();
    double z2=v2->GetVZPos();
    double x3=v3->GetVXPos();
    double y3=v3->GetVYPos();
    double z3=v3->GetVZPos();
    
    Vec3D Box=(*m_pBox);
    
    
    
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
    
    
    
    Vec3D V1(dx1,dy1,dz1);
    Vec3D V2(dx2,dy2,dz2);
    N=V1*V2;
    double area=N.norm();
    N=N*(1.0/area);
    
    return N;
}
void Topology::FastReadQFile(std::string Filename)
{

    //============================================
    //====
    //===================================
    Nfunction f;
    std::ifstream input;
    input.open(Filename.c_str());
    double Lx,Ly,Lz;
    input>>Lx>>Ly>>Lz;
        m_Box(0)=Lx;
        m_Box(1)=Ly;
        m_Box(2)=Lz;
    
    (*m_pBox)(0)=Lx ; (*m_pBox)(1)=Ly ; (*m_pBox)(2)=Lz;

    std::string Line,str;
    int nover=0;
    int notri=0;
    input>>nover;
    getline(input,str);
    double x,y,z;
    int id;
    int molid;
    for (int i=0;i<nover;i++)
    {
       // input>>id>>x>>y>>z>>molid;
        getline(input,str);
        std::vector<std::string> Line = f.split(str);

        if(Line.size()<4)
        {
            std::string A=" Error: Something unusual in the TS file ";
            std::cout<<A<<"\n";
            f.Write_One_ErrorMessage(A);
            m_TopologyHealth = false;
        }
        if(Line.size()>5)
        {
            if((Line.at(5)).size()>0)
            {
                if((Line.at(5)).at(0)!=';')
                {
                    std::string A=" Error:  "+Line.at(5)+ " at line "+f.Int_to_String(i+2);
                    std::cout<<A<<"\n";
                    f.Write_One_ErrorMessage(A);
                    m_TopologyHealth = false;
                }
                
            }

        }
        id = f.String_to_Int(Line.at(0));
        x = f.String_to_Double(Line.at(1));
        y = f.String_to_Double(Line.at(2));
        z = f.String_to_Double(Line.at(3));
        
        
        if(Line.size()==5)
        molid = f.String_to_Int(Line.at(4));
        else
        molid = 0;
        
        vertex v(id,x,y,z);
        v.UpdateBox(m_pBox);
        v.UpdateClusterID(molid);
        m_Vertex.push_back(v);
    }
    input>>notri;
    
    getline(input,str);

    int v1,v2,v3,rep;
    for (int i=0;i<notri;i++)
    {
        bool pr=true;
      //  input>>id>>v1>>v2>>v3>>rep;
        
        getline(input,str);
        std::vector<std::string> Line = f.split(str);
        
        if(Line.size()<5)
        {
        std::cout<<"Error: Something wrong with triangle list at line "<<i<<"\n";
        }
        
        id=f.String_to_Int(Line.at(0));
        v1=f.String_to_Int(Line.at(1));
        v2=f.String_to_Int(Line.at(2));
        v3=f.String_to_Int(Line.at(3));
        rep=f.String_to_Int(Line.at(4));

      //  std::cout<<id<<"  "<<v1<<"  "<<v2<<"   "<<v3<<"\n";


        
        triangle T(id,&(m_Vertex.at(v1)),&(m_Vertex.at(v2)),&(m_Vertex.at(v3)));
        if(rep == 0)
        {
            pr=false;               T.UpdateRepresentation(pr);
        }
        
        m_Triangle.push_back(T);
    }
    
    input.close();

    for (std::vector<vertex>::iterator it = m_Vertex.begin() ; it != m_Vertex.end(); ++it)
    {
        
        m_pAllV.push_back(&(*it));
    }
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
    
    
    
}
