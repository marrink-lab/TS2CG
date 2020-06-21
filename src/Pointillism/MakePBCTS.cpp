

#include <iomanip>
#include "MakePBCTS.h"
#include "Nfunction.h"
#include "VMDOutput.h"
#include "WriteFiles.h"

MakePBCTS::MakePBCTS(std::string file)
{
    m_Health = true;
    m_pBox = &m_Box;
    //m_pLmin2 = pLmin;
    //m_pLmax2 = pLmax;
    //m_pminAngle = pminangle;
    Nfunction f;
    std::ifstream input;
    input.open(file.c_str());
    double Lx,Ly,Lz;
    input>>Lx>>Ly>>Lz;
    
    m_Box(0)=Lx;
    m_Box(1)=Ly;
    m_Box(2)=Lz;
    
    //=========================================================================
    //============#  Read the vertex section of the q file #==================
    //========================================================================
    
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
        getline(input,str);
        std::vector<std::string> Line = f.split(str);
        if(Line.size()<4)
        {
            std::cout<<" Error: Something unusual in the TS file at line "<<i+3<<"\n";
            m_Health = false;
        }
        if(Line.size()>5)
        {
            if((Line.at(5)).size()>0)
            {
                if((Line.at(5)).at(0)!=';')
                {
                    std::string A=" Error:  "+Line.at(5)+ " at line "+f.Int_to_String(i+3);
                    std::cout<<A<<"\n";
                    m_Health = false;
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
    for (std::vector<vertex>::iterator it = m_Vertex.begin() ; it != m_Vertex.end(); ++it)
    {
        m_pAllV.push_back(&(*it));
    }
    //=========================================================================
    //============#  Read the trinagle section of the q file #==================
    //========================================================================
    input>>notri;
    getline(input,str);
    int v1,v2,v3,rep;
    for (int i=0;i<notri;i++)
    {
        bool pr=true;
        getline(input,str);
        std::vector<std::string> Line = f.split(str);
        
        if(Line.size()<5)
        {
            std::cout<<"Error: Something wrong with triangle list at line "<<i+m_Vertex.size()+4<<"\n";
        }
        
        id=f.String_to_Int(Line.at(0));
        v1=f.String_to_Int(Line.at(1));
        v2=f.String_to_Int(Line.at(2));
        v3=f.String_to_Int(Line.at(3));
        rep=f.String_to_Int(Line.at(4));
        triangle T(id,&(m_Vertex.at(v1)),&(m_Vertex.at(v2)),&(m_Vertex.at(v3)));
        if(rep == 0)
        {
            pr=false;               T.UpdateRepresentation(pr);
        }
        
        m_Triangle.push_back(T);
    }
    
    input.close();
    //=========================================================================
    //============#  Create Links; add trinagles to vertex trinagle list #==================
    //========================================================================
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
    //=========================================================================
    //============#  add links nighbours #==================
    //========================================================================
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
    
    //=========================================================================
    //============#     Find mirror links and edge link  #==================
    //========================================================================
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
            m_pEdgeLink.push_back(&(*it));

        }
        
    }
    for (std::vector<links>::iterator it = m_Links.begin() ; it != m_Links.end(); ++it)
    {
        m_pLinks.push_back(&(*it));
    }
    //=========================================================================
    //============#     Analayze edge link  #==================
    //========================================================================
    if(m_pEdgeLink.size()==0)
    {
        std::cout<<"Note: TS looks fine, no correction is required \n";
        m_Health = false;
    }
    else
    {
        std::cout<<"---> Note: found "<<m_pEdgeLink.size()<<" links at the boundry \n";
        
        //========= see if close boundry
        Vec3D TBV;
        for (std::vector<links*>::iterator it = m_pEdgeLink.begin() ; it != m_pEdgeLink.end(); ++it)
        {
           TBV=TBV+LinkVec(*it);
        }
        if(TBV.norm()>0.00001)
        {
            std::cout<<"---> error: it seems the problem with the ts file is not just the boundry but somehow your triangles";
            std::cout<<"do not follow the same rotations.  Because "<<TBV.norm()<<" should be zero but it is not\n";
            m_Health = false;
        }
        //=== Find average link lenght, Note: this double calculate the links with mirror
        double avL=0;
        double minL = 1000000;

        for (std::vector<links*>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
        {
            avL+=(LinkVec(*it)).norm()/double(m_pLinks.size());
            if(minL>(LinkVec(*it)).norm())
            minL=(LinkVec(*it)).norm();
        }
        std::cout<<"---> Note: average link length is "<<avL<<"\n";
        //=== Find an estimate of the box
        Vec3D CM;
        Vec3D MinPos(10000000000,100000000000,1000000000000); // a vector that hold the minimum value of XYZ
        for (std::vector<vertex*>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
        {
            Vec3D Pos((*it)->GetVXPos(),(*it)->GetVYPos(),(*it)->GetVZPos());
            CM = CM + Pos*(1.0/double(m_pAllV.size()));
            
            for (int i=0;i<3;i++)
            {
                if(MinPos(i)>Pos(i))
                MinPos(i)=Pos(i);
            }
        }

        Lx=0;Ly=0;Lz=0;
           for (std::vector<vertex*>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
         {
             for (std::vector<vertex*>::iterator it1 = m_pAllV.begin() ; it1 != m_pAllV.end(); ++it1)
             {
                 Vec3D Pos1((*it)->GetVXPos(),(*it)->GetVYPos(),(*it)->GetVZPos());
                 Vec3D Pos2((*it1)->GetVXPos(),(*it1)->GetVYPos(),(*it1)->GetVZPos());
                 if(fabs(Pos1(0)-Pos2(0))>Lx)
                 Lx=fabs(Pos1(0)-Pos2(0));
                 if(fabs(Pos1(1)-Pos2(1))>Ly)
                Ly=fabs(Pos1(1)-Pos2(1));
                 if(fabs(Pos1(2)-Pos2(2))>Lz)
                 Lz=fabs(Pos1(2)-Pos2(2));
                 
                 
             }
         }
        Ly=Ly+minL; Lx=Lx+minL;  Lz=4*Lz;

        
        std::cout<<"---> initial box "<<(*m_pBox)(0)<<"   "<<(*m_pBox)(1)<<"   "<<(*m_pBox)(2)<<"   \n";
        std::cout<<"---> predicted box "<<Lx<<"   "<<Ly<<"   "<<Lz<<"   \n";
        (*m_pBox)(0) = Lx;
        (*m_pBox)(1) = Ly;
        (*m_pBox)(2) = Lz;
        for (std::vector<vertex*>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
        {
            Vec3D Pos((*it)->GetVXPos(),(*it)->GetVYPos(),(*it)->GetVZPos());
            (*it)->UpdateVXPos(Pos(0)-MinPos(0)+minL/2);
            (*it)->UpdateVYPos(Pos(1)-MinPos(1)+minL/2);
            (*it)->UpdateVZPos(Pos(2)-CM(2)+Lz/2);

        }

    }
    
    if(m_Health==true)
    {
       // m_pEdgeLink

        ///===================================
        //=== Find Permiter beads
        std::vector<vertex*>      pedgever;
        for (std::vector<links*>::iterator it = m_pEdgeLink.begin() ; it != m_pEdgeLink.end(); ++it)
            pedgever.push_back((*it)->GetV1());

        //=== Find edge beads
        vertex* V00,*V10,*V01,*V11;
        double dist00=m_Box(0);
        double dist10=m_Box(0);
        double dist01=m_Box(0);
        double dist11=m_Box(0);

        for (std::vector<vertex*>::iterator it = pedgever.begin() ; it != pedgever.end(); ++it)
        {
            Vec3D Pos((*it)->GetVXPos(),(*it)->GetVYPos(),0);
            Vec3D Pos00(0,0,0);
            Vec3D Pos10(m_Box(0),0,0);
            Vec3D Pos01(0,m_Box(1),0);
            Vec3D Pos11(m_Box(0),m_Box(1),0);
            
            if((Pos00-Pos).norm()<dist00)
            {
            V00 = (*it);
                dist00 = (Pos00-Pos).norm();
            }
            if((Pos10-Pos).norm()<dist10)
            {
                V10 = (*it);
                dist10 = (Pos10-Pos).norm();

            }
            if((Pos01-Pos).norm()<dist01)
            {
                V01 = (*it);
                dist01 = (Pos01-Pos).norm();

            }
            if((Pos11-Pos).norm()<dist11)
            {
                V11 = (*it);
                dist11 = (Pos11-Pos).norm();

            }
        }
        
        std::cout<<"edge Ver "<<V00->GetVID()<<"  "<<V10->GetVID()<<"  "<<V01->GetVID()<<"  "<<V11->GetVID()<<"  \n";
        // check if the system is clockwise or unti
        bool clockwise = true;
        std::vector<vertex*> VecV0X;  // to store chain of vertex at each edge
        std::vector<vertex*> VecV1X;
        std::vector<vertex*> VecV1Y;
        std::vector<vertex*> VecV0Y;
        {
            //========== Order the links
            vertex *v1 = V00;
            pedgever.clear(); // We clear it to make it ordered in the loop and starting from V00;
            while(true)
            {
                for (std::vector<links*>::iterator it = m_pEdgeLink.begin() ; it != m_pEdgeLink.end(); ++it)
                {
                    if(((*it)->GetV1())->GetVID()==v1->GetVID())
                    {
                        pedgever.push_back((*it)->GetV1());
                        m_pOrderEdgeLink.push_back(*it);
                        v1=(*it)->GetV2();
                        break;
                    }
                }
                if(v1==V00)
                break;
            }
            if(m_pOrderEdgeLink.size()!=m_pEdgeLink.size())
            {
                std::cout<<"error878: something wrong happened \n";
            }
            //
            int nn=0;
            for (std::vector<links*>::iterator it = m_pOrderEdgeLink.begin() ; it != m_pOrderEdgeLink.end(); ++it)
            {
                nn++;
                if((*it)->GetV1()==V01)
                {
                    clockwise = true;
                    break;
                }
                if((*it)->GetV1()==V10)
                {
                    clockwise = false;
                    break;
                }
            }
            std::cout<<"---> note: triangles are clockwise "<<clockwise<<"  \n";
            if(clockwise==false)
            {
                std::cout<<"---> Warning: the triangles are clockwise. We have not tested this condition, please check your output and \n";
                std::cout<<"---> If there was an issue contact weria.pezeshkian@gmail.com \n";


            }
            
            if(clockwise==false)
            {
                int chain = 20;
                for (std::vector<links*>::iterator it = m_pOrderEdgeLink.begin() ; it != m_pOrderEdgeLink.end(); ++it)
                {
                    if((*it)->GetV1()==V00)
                    {
                        chain = 0;
                    }
                    else if((*it)->GetV1()==V10)
                    {
                        VecV0X.push_back((*it)->GetV1());
                        chain = 1;
                    }
                    else if((*it)->GetV1()==V11)
                    {
                        VecV1Y.push_back((*it)->GetV1());
                        chain = 2;
                    }
                    else if((*it)->GetV1()==V01)
                    {
                        VecV1X.push_back((*it)->GetV1());
                        chain = 3;
                    }
                    if(chain==0)
                    {
                        VecV0X.push_back((*it)->GetV1());
                    }
                    else if(chain==1)
                    {
                        VecV1Y.push_back((*it)->GetV1());
                    }
                    else if(chain==2)
                    {
                        VecV1X.push_back((*it)->GetV1());
                    }
                    else if(chain==3)
                    {
                        VecV0Y.push_back((*it)->GetV1());
                    }
                    else
                    {
                        std::cout<<"error000 \n";
                    }
                    
                }
            }
            
            VecV0Y.push_back(V00);
        }
///================
        
        std::cout<<" 0 X vertex size "<<VecV0X.size()<<"\n";
        std::cout<<" 0 Y vertex size "<<VecV0Y.size()<<"\n";
        std::cout<<" 1 X vertex size "<<VecV1X.size()<<"\n";
        std::cout<<" 1 Y vertex size "<<VecV1Y.size()<<"\n";
        
        std::vector<triangle>    newT;
        int TID = m_pAllT.size();
        
        if(VecV0X.size()<=VecV1X.size())
        {
            int M = VecV1X.size();
            int DM=VecV1X.size()-VecV0X.size();
            int m=0;
            int R=VecV0X.size()/(DM+1);
            
            for (int i=0;i<VecV0X.size()-1;i++)
            {
            triangle t1(TID,VecV0X.at(i),VecV1X.at(M-i-1),VecV1X.at(M-i-2));
            newT.push_back(t1);
            TID++;
                
                if(i%R==0 && m<DM)
                {
                    M=M-1;
                    triangle t3(TID,VecV0X.at(i),VecV1X.at(M-i-1),VecV0X.at(i+1));
                    newT.push_back(t3);
                    TID++;
                    m++;
                    triangle t(TID,VecV0X.at(i+1),VecV1X.at(M-i-1),VecV1X.at(M-i-2));
                    newT.push_back(t);
                    TID++;
                }
                else
                {
                    triangle t2(TID,VecV0X.at(i),VecV1X.at(M-i-2),VecV0X.at(i+1));
                    newT.push_back(t2);
                    TID++;
                }


            }
        }
        if(VecV0X.size()>VecV1X.size())
        {
            int M = VecV1X.size();
            int DM=VecV0X.size()-VecV1X.size();
            int m=0;
            int R=VecV1X.size()/(DM+1);
            
            for (int i=0;i<VecV1X.size()-1;i++)
            {
                triangle t1(TID,VecV0X.at(i+m),VecV1X.at(M-i-1),VecV0X.at(i+1+m));
                newT.push_back(t1);
                TID++;
                
                if(i%R==0 && m<DM)
                {
                    triangle t3(TID,VecV0X.at(i+1+m),VecV1X.at(M-i-1),VecV1X.at(M-i-2));
                    newT.push_back(t3);
                    TID++;
                    triangle t(TID,VecV0X.at(i+1+m),VecV1X.at(M-i-2),VecV0X.at(i+2+m));
                    newT.push_back(t);
                    TID++;
                    m++;

                }
                else
                {
                    triangle t2(TID,VecV0X.at(i+1+m),VecV1X.at(M-i-1),VecV1X.at(M-i-2));
                    newT.push_back(t2);
                    TID++;
                }
                
                
            }
        }
        

        
        if(VecV1Y.size()<=VecV0Y.size())
        {
            int M = VecV0Y.size();
            int DM=VecV0Y.size()-VecV1Y.size();
            int m=0;
            int R=VecV1Y.size()/(DM+1);
            for (int i=0;i<VecV1Y.size()-1;i++)
            {

                triangle t2(TID,VecV0Y.at(M-i-1),VecV0Y.at(M-i-2),VecV1Y.at(i));
                newT.push_back(t2);
                TID++;

                

                if(i%R==0 && m<DM)
                {
                    M=M-1;
                    triangle t3(TID,VecV0Y.at(M-i-1),VecV0Y.at(M-i-2),VecV1Y.at(i+1));
                    newT.push_back(t3);
                    TID++;
                    m++;
                    triangle t(TID,VecV1Y.at(i+1),VecV1Y.at(i),VecV0Y.at(M-i-1));
                    newT.push_back(t);
                    TID++;
                }
                else
                {
                    triangle t(TID,VecV1Y.at(i+1),VecV1Y.at(i),VecV0Y.at(M-i-2));
                    newT.push_back(t);
                    TID++;
                }



            }
        }
            
            if(VecV1Y.size()>VecV0Y.size())
            {
                int M = VecV0Y.size();
                int DM=VecV1Y.size()-VecV0Y.size();
                int m=0;
                int R=VecV0Y.size()/(DM+1);
                for (int i=0;i<VecV0Y.size()-1;i++)
                {
                    
                    triangle t2(TID,VecV0Y.at(M-i-1),VecV1Y.at(i+1+m),VecV1Y.at(i+m));
                    newT.push_back(t2);
                    TID++;

                    if(i%R==0 && m<DM)
                    {
                        M=M-1;
                        triangle t3(TID,VecV1Y.at(i+1+m), VecV0Y.at(M-i-1),VecV0Y.at(M-i-2));
                        newT.push_back(t3);
                        TID++;
                        m++;
                        triangle t(TID,VecV1Y.at(i+2+m),VecV1Y.at(i+1+m),VecV0Y.at(M-i-2));
                        newT.push_back(t);
                        TID++;
                    }
                    else
                    {
                        triangle t(TID,VecV1Y.at(i+1+m),VecV0Y.at(M-i-1),VecV0Y.at(M-i-2));
                        newT.push_back(t);
                        TID++;
                    }
                }
            }

        
        //V00;
        triangle t(TID,V11,V01,V10);
        newT.push_back(t);
        TID++;
        
        triangle t2(TID,V10,V01,V00);
        newT.push_back(t2);
        TID++;
        
        
        
        for (std::vector<triangle>::iterator it = newT.begin() ; it != newT.end(); ++it)
        {
            m_pAllT.push_back(&(*it));
        }

        
        //===== Write Files
        std::string newfile = "new_"+file;
            VMDOutput vmd(m_Box, m_pAllV , m_pLinks ,"output");
            vmd.WriteGro2();
            
            Write(m_pAllV,m_pAllT,newfile);
            // to check if the links created well
            for (std::vector<vertex*>::iterator it = m_pAllV.begin() ; it != m_pAllV.end(); ++it)
            {
                Vec3D Pos((*it)->GetVXPos(),(*it)->GetVYPos(),(*it)->GetVZPos());
                (*it)->UpdateVXPos(Pos(0)-m_Box(0)/2);
                (*it)->UpdateVYPos(Pos(1)-m_Box(1)/2);
                
            }
            WriteFiles VTU(m_pBox);
            VTU.Writevtunew(m_pAllV,m_pAllT,"output.vtu");
  
        
        
        


    }
    
    
}

MakePBCTS::~MakePBCTS()
{
    
}
Vec3D MakePBCTS::LinkVec(links* l)
{
    vertex *v1 = l->GetV1();
    vertex *v2 = l->GetV2();

    double x2=v2->GetVXPos();
    double y2=v2->GetVYPos();
    double z2=v2->GetVZPos();
    double x1=v1->GetVXPos();
    double y1=v1->GetVYPos();
    double z1=v1->GetVZPos();
    double dx=x2-x1;
    double dy=y2-y1;
    double dz=z2-z1;

    Vec3D L(dx,dy,dz);
    return L;
}
void MakePBCTS::Write(std::vector< vertex* > ver, std::vector< triangle* > triangle1,  std::string Filename)
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
/*














double MakePBCTS::LengthBetweenTwoVertex(vertex* v1, vertex* v2)
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

bool   MakePBCTS::CheckFaceAngle(links * l)
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
Vec3D   MakePBCTS::CalculateNormal(vertex* v1 ,vertex* v2 ,vertex* v3)
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
*/
