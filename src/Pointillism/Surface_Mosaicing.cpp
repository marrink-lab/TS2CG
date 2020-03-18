


#include "Surface_Mosaicing.h"
#include "Nfunction.h"
#include "VMDOutput.h"
#include "WriteFiles.h"
#include "Trajectory.h"
#include "Curvature.h"
Surface_Mosaicing::Surface_Mosaicing(std::string altype, bool smooth)
{
        m_AlgorithmType = altype;
        m_smooth = smooth;
}Surface_Mosaicing::Surface_Mosaicing()
{
    m_AlgorithmType = "Type1";
    m_smooth = false;
}
void Surface_Mosaicing::PerformMosaicing(Vec3D * pBox, std::vector<vertex* > verm , std::vector<triangle* > ptri, std::vector<links* > plinks, std::vector<inclusion* > inc, int iteration)
{

    m_Inc = inc;
    iteration=iteration-1;
m_Box=*pBox;
m_pBox = pBox;
m_pTri =ptri;
m_pVers = verm;
m_pLinks = plinks;


    MosaicOneRound();
    UpdateGeometry();

   /* if(iteration ==0)
    {
    std::string tfile="Mosaic.dat";
    Trajectory T(pBox);
    T.Write(-1,tfile,m_pFV,m_pFT,m_pFL,inc);
    
    std::string file="Mosaic.vtu";
    std::string file2="orginal.vtu";
    WriteFiles VTU(pBox);
    VTU.Writevtu(m_pFV, m_pFT,  m_pFL, file);
  //  VTU.Writevtu(m_pVers, m_pTri,  m_pLinks, file2);

   VMDOutput VMD(m_Box,m_pFV, m_pTMLH1, file);
   VMD.WriteGro();
    }*/
    
    

    
}

Surface_Mosaicing::~Surface_Mosaicing()
{
    
}
void Surface_Mosaicing::MosaicOneRound()
{
    
    m_TMV.clear();
    m_TMT.clear();
    m_TML.clear();
    
    
//=========================================
//===== First we copy the old vertex
//=========================================
    for (std::vector<vertex *>::iterator it = m_pVers.begin() ; it != m_pVers.end(); ++it)
    {
        double x=(*it)->GetVXPos();
        double y=(*it)->GetVYPos();
        double z=(*it)->GetVZPos();
        int id=(*it)->GetVID();
        vertex v(id,x,y,z);
        if((*it)->VertexOwnInclusion()==true)
        {
            v.UpdateOwnInclusion(true);
            v.UpdateInclusion((*it)->GetInclusion());
        }
        v.UpdateBox(m_pBox);
        m_TMV.push_back(v);
    }
//=========================================
    //===== for each link, generate a mid vertex
//=========================================
    int id=m_TMV.size();
    for (std::vector<links *>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
    {

        double x,y,z;
        
        BestEstimateOfMidPointPossition((*it), &x, &y,&z);
        
        vertex v(id,x,y,z);
        v.UpdateBox(m_pBox);
        m_TMV.push_back(v);
        id++;
    }
    id=m_pVers.size();
    for (std::vector<links *>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
    {
        (*it)->UpdateV0(&(m_TMV.at(id)));
        ((*it)->GetMirrorLink())->UpdateV0(&(m_TMV.at(id)));
        id++;
    }
//=========================================
//===== now update nighbour vertex of the copied old vertex
//=========================================
    std::vector<vertex >::iterator itn = m_TMV.begin();
    for (std::vector<vertex *>::iterator it = m_pVers.begin() ; it != m_pVers.end(); ++it)
    {
        std::vector<links* > LN=(*it)->GetVLinkList();
        for (std::vector<links *>::iterator it2 = LN.begin() ; it2 != LN.end(); ++it2)
        {
            vertex *pv=(*it2)->GetV0();
            (*itn).AddtoNeighbourVertex(pv);
        }
        
        itn++;
        
    }
//=========================================
//===== now update nighbour vertex of the new vertex
//=========================================
    for (std::vector<links *>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
    {
        vertex *pv=(*it)->GetV0();

        links * ml=(*it)->GetMirrorLink();
        links * l1 = (*it)->GetNeighborLink1();
        links * l2 = (*it)->GetNeighborLink2();
        links * l3 = ml->GetNeighborLink1();
        links * l4 = ml->GetNeighborLink2();
        
        vertex *pv1=l1->GetV0();
        pv->AddtoNeighbourVertex(pv1);
        
        vertex *pv2=l2->GetV0();
        pv->AddtoNeighbourVertex(pv2);
        
        vertex *pv3=l3->GetV0();
        pv->AddtoNeighbourVertex(pv3);
        
        vertex *pv4=l4->GetV0();
        pv->AddtoNeighbourVertex(pv4);
        
        int id1=((*it)->GetV1())->GetVID();
        int id2=((*it)->GetV2())->GetVID();
        
        
        pv->AddtoNeighbourVertex(&(m_TMV.at(id1)));
        pv->AddtoNeighbourVertex(&(m_TMV.at(id2)));

    }
//=========================================
//===== generate new triangles
//=========================================
    int tid=0;
    for (std::vector<links *>::iterator it = m_pLinks.begin() ; it != m_pLinks.end(); ++it)
    {
        triangle *t1=(*it)->GetTriangle();
        if(t1->GetGotMashed()==false)
        {
            links * l1 = (*it)->GetNeighborLink1();
            links * l2 = (*it)->GetNeighborLink2();
            vertex *V1=&(m_TMV.at(((*it)->GetV1())->GetVID()));
            vertex *V2=&(m_TMV.at(((*it)->GetV2())->GetVID()));
            vertex *V3=&(m_TMV.at(((*it)->GetV3())->GetVID()));
            vertex *VM0=(*it)->GetV0();
            vertex *VM1=l1->GetV0();
            vertex *VM2=l2->GetV0();
            triangle T1(tid,V1,VM0,VM2);
            tid++;
            triangle T2(tid,VM0,V2,VM1);
            tid++;
            triangle T3(tid,VM0,VM1,VM2);
            tid++;
            triangle T4(tid,VM2,VM1,V3);
            tid++;
            m_TMT.push_back(T1);
            m_TMT.push_back(T2);
            m_TMT.push_back(T3);
            m_TMT.push_back(T4);
            t1->UpdateGotMashed(true);
        }
        links * lm=(*it)->GetMirrorLink();
        triangle *t2=lm->GetTriangle();

        if(t2->GetGotMashed()==false)
        {
            links * l1 = (lm)->GetNeighborLink1();
            links * l2 = (lm)->GetNeighborLink2();
            vertex *V1=&(m_TMV.at(((lm)->GetV1())->GetVID()));
            vertex *V2=&(m_TMV.at(((lm)->GetV2())->GetVID()));
            vertex *V3=&(m_TMV.at(((lm)->GetV3())->GetVID()));
            vertex *VM0=(lm)->GetV0();
            vertex *VM1=l1->GetV0();
            vertex *VM2=l2->GetV0();
            triangle T1(tid,V1,VM0,VM2);
            tid++;
            triangle T2(tid,VM0,V2,VM1);
            tid++;
            triangle T3(tid,VM0,VM1,VM2);
            tid++;
            triangle T4(tid,VM2,VM1,V3);
            tid++;
            m_TMT.push_back(T1);
            m_TMT.push_back(T2);
            m_TMT.push_back(T3);
            m_TMT.push_back(T4);
            t2->UpdateGotMashed(true);
        }
        
    }
//=================================
//=== add neighbour triangles to vertex
//==========================================
    for (std::vector<triangle >::iterator it = m_TMT.begin() ; it != m_TMT.end(); ++it)
    {
        vertex *v1 = (*it).GetV1();
        vertex *v2 = (*it).GetV2();
        vertex *v3 = (*it).GetV3();
        
        v1->AddtoTraingleList(&(*it));
        v2->AddtoTraingleList(&(*it));
        v3->AddtoTraingleList(&(*it));


    }
    
//=================================
//=== generate new link list
//==========================================
    int lid=0;
    for (std::vector<triangle >::iterator it = m_TMT.begin() ; it != m_TMT.end(); ++it)
    {
        vertex* v1=(*it).GetV1();
        vertex* v2=(*it).GetV2();
        vertex* v3=(*it).GetV3();
        
        links l1(lid,v1,v2,&(*it));
        l1.UpdateV3(v3);
        
        lid++;
        links l2(lid,v2,v3,&(*it));
        l2.UpdateV3(v1);
        
        lid++;
        links l3(lid,v3,v1,&(*it));
        l3.UpdateV3(v2);
        m_TML.push_back(l1);
        m_TML.push_back(l2);
        m_TML.push_back(l3);
        
        lid++;

        
    }
    
    lid=0;
    for (std::vector<triangle >::iterator it = m_TMT.begin() ; it != m_TMT.end(); ++it)
    {
        vertex* v1=(*it).GetV1();
        vertex* v2=(*it).GetV2();
        vertex* v3=(*it).GetV3();
        
        
        int id1=lid;
        lid++;
        int id2=lid;

        lid++;
        int id3=lid;
        lid++;

        links * l1=&(m_TML.at(id1));
        links * l2=&(m_TML.at(id2));
        links * l3=&(m_TML.at(id3));
        l1->UpdateNeighborLink1(l2);
        l1->UpdateNeighborLink2(l3);
        l2->UpdateNeighborLink1(l3);
        l2->UpdateNeighborLink2(l1);
        l3->UpdateNeighborLink1(l1);
        l3->UpdateNeighborLink2(l2);
        
        v1->AddtoLinkList(l1);
        v2->AddtoLinkList(l2);
        v3->AddtoLinkList(l3);
        
    }
    //=============================
    //=====  temprory: copy the links and triangles into pointers
    //=============================================
    for (std::vector<vertex >::iterator it = m_TMV.begin() ; it != m_TMV.end(); ++it)
    {
        m_pFV.push_back(&(*it));
    }
    for (std::vector<triangle >::iterator it = m_TMT.begin() ; it != m_TMT.end(); ++it)
    {
        m_pFT.push_back(&(*it));
    }
    for (std::vector<links >::iterator it = m_TML.begin() ; it != m_TML.end(); ++it)
    {
        m_pFL.push_back(&(*it));
    }
    
//====================================================================
//========================== Adding mieror links
//============================================================================
    for (std::vector<vertex* >::iterator it = m_pFV.begin() ; it != m_pFV.end(); ++it)
    {
        
        std::vector<links*> nvl=(*it)->GetVLinkList();
        

        for (std::vector<links*>::iterator it2 =nvl.begin() ; it2 != nvl.end(); ++it2)
        {

            if((*it2)->GetMirrorFlag()==false)
            {

                vertex * v2=(*it2)->GetV2();

                std::vector<links*> nvl2=v2->GetVLinkList();
                for (std::vector<links*>::iterator it3 =nvl2.begin() ; it3 != nvl2.end(); ++it3)
                {
                    if(((*it2)->GetV1())->GetVID()==((*it3)->GetV2())->GetVID() && ((*it2)->GetV2())->GetVID()==((*it3)->GetV1())->GetVID() )
                    {
                        if((*it2)->GetID()!=(*it3)->GetID())
                        {
                            (*it3)->UpdateMirrorLink((*it2));
                            (*it2)->UpdateMirrorLink((*it3));
                            (*it3)->UpdateMirrorFlag(true);
                            (*it2)->UpdateMirrorFlag(true);
                            
                            m_pTMLH1.push_back((*it2));
                            m_pTMLH2.push_back((*it3));
                        }
                        else
                        {
                            std::cout<<"Error: this should not happen "<<std::endl;
                        }
                        
                        
                        
                    }
                    
                }
            }
            
            
        }
        
    }
    
 /*   for (std::vector<links*>::iterator it = m_pFL.begin() ; it != m_pFL.end(); ++it)
    {
        
       if((*it)->GetMirrorFlag()==false)
        for (std::vector<links*>::iterator it2 =it+1 ; it2 != m_pFL.end(); ++it2)
        {
            
            if(((*it2)->GetV1())->GetVID()==((*it)->GetV2())->GetVID() && ((*it2)->GetV2())->GetVID()==((*it)->GetV1())->GetVID() && (*it2)->GetID()!=(*it)->GetID())
            {
                
                
                (*it)->UpdateMirrorLink((*it2));
                (*it2)->UpdateMirrorLink((*it));
                (*it)->UpdateMirrorFlag(true);
                (*it2)->UpdateMirrorFlag(true);
                
                m_pTMLH1.push_back((*it2));
                m_pTMLH2.push_back((*it));
  
            }
        }
        
    }*/
//=====
    

}

void Surface_Mosaicing::BestEstimateOfMidPointPossition(links *l, double *X, double *Y,double *Z)
{
    double x=0;
    double y=0;
    double z=0;
    vertex * pv1=l->GetV1();
    vertex * pv2=l->GetV2();
    Vec3D *pBox=pv1->GetBox();
    double x1=pv1->GetVXPos();
    double y1=pv1->GetVYPos();
    double z1=pv1->GetVZPos();
               
    double x2=pv2->GetVXPos();
    double y2=pv2->GetVYPos();
    double z2=pv2->GetVZPos();

    double xmid=(x1+x2)/2.0;
    double ymid=(y1+y2)/2.0;
    double zmid=(z1+z2)/2.0;
    

        if(fabs(x1-x2)>(*pBox)(0)/2)
                xmid=xmid+(*pBox)(0)/2;
        if(fabs(y1-y2)>(*pBox)(1)/2)
                ymid=ymid+(*pBox)(1)/2;
        if(fabs(z1-z2)>(*pBox)(2)/2)
            zmid=zmid+(*pBox)(2)/2;


    
    


     Vec3D geodesic_dir(x2-x1,y2-y1,z2-z1);

     for (int i=0;i<3;i++)
     {
         if(fabs(geodesic_dir(i))>(*pBox)(i)/2)
         {
             if(geodesic_dir(i)<0)
             geodesic_dir(i)=geodesic_dir(i)+(*pBox)(i);
             else if(geodesic_dir(i)>0)
             geodesic_dir(i)=geodesic_dir(i)-(*pBox)(i);
         }
     }
    
    double Linklenght= geodesic_dir.norm();
    geodesic_dir = geodesic_dir*(1/geodesic_dir.norm());

     Vec3D Lo_geoV1=(pv1->GetG2LTransferMatrix())*geodesic_dir;
     Lo_geoV1(2)=0;
     Lo_geoV1=Lo_geoV1*(1/(Lo_geoV1.norm()));
     Vec3D Glo_geoV1=(pv1->GetL2GTransferMatrix())*Lo_geoV1;
    
    Vec3D Lo_geoV2=(pv2->GetG2LTransferMatrix())*geodesic_dir;
    Lo_geoV2(2)=0;
    Lo_geoV2=Lo_geoV2*(1/(Lo_geoV2.norm()));
    Vec3D Glo_geoV2=(pv2->GetL2GTransferMatrix())*Lo_geoV2;



    
    
    Tensor2 Hous;
    Vec3D Zk;
    Zk(2)=1.0;
    double SignT=1;
    
    if((1+geodesic_dir(2))>(1-geodesic_dir(2)))
    {
        Zk=Zk+geodesic_dir;
        SignT=-1;
        
    }
    else if((1+geodesic_dir(2))<=(1-geodesic_dir(2)))
    {
        Zk=Zk-geodesic_dir;
    }
    
    Zk=Zk*(1.0/Zk.norm());
    
    
    Tensor2 I('I');
    Tensor2 W=Hous.makeTen(Zk);
    
    Hous=(I-(W*(W.Transpose(W)))*2)*SignT;

    Vec3D t_2=Hous*Glo_geoV2;
    Vec3D t_1=Hous*Glo_geoV1;

    t_2=t_2*(1/t_2(2));
    t_1=t_1*(1/t_1(2));
 
    
    
    t_2=t_2*(Linklenght/2.0);
    t_1=t_1*(Linklenght/2.0);
    
    /// test case to see if inclduign curvature make it better
    Vec3D Dr(0,0,0);
    
    if(m_AlgorithmType == "Type2")
    {


    Vec3D N1=pv1->GetNormalVector();
    Vec3D N2=pv2->GetNormalVector();
    std::vector <double> C1=pv1->GetCurvature();
    std::vector <double> C2=pv2->GetCurvature();

        double Cos1=Lo_geoV1(0);
        double Sin1=Lo_geoV1(1);
        
        double Cos2=Lo_geoV2(0);
        double Sin2=Lo_geoV2(1);

        double Curve1=C1.at(0)*Cos1*Cos1+C1.at(1)*Sin1*Sin1;
        double Curve2=C2.at(0)*Cos2*Cos2+C2.at(1)*Sin2*Sin2;
        
        
        double D2X_1=Curve1*(t_1.dot(t_1,t_1))*(2*N1(2)*t_1(0)/Linklenght-N1(0));
        double D2Y_1=Curve1*(t_1.dot(t_1,t_1))*(2*N1(2)*t_1(1)/Linklenght-N1(1));
        double D2X_2=Curve2*(t_2.dot(t_2,t_2))*(2*N2(2)*t_2(0)/Linklenght-N2(0));
        double D2Y_2=Curve2*(t_2.dot(t_2,t_2))*(2*N2(2)*t_2(1)/Linklenght-N2(1));
        

    
    
    
    
    double X_0=(D2X_1+D2X_2+5*(t_1(0)-t_2(0)))/16.0;
    double Y_0=(D2Y_1+D2Y_2+5*(t_1(1)-t_2(1)))/16.0;
  
        
        Dr(0)=X_0;
        Dr(1)=Y_0;
    }
  else if(m_AlgorithmType == "Type1")
  {
 
      Dr(0)=(t_1(0)-t_2(0))/4;
      Dr(1)=(t_1(1)-t_2(1))/4;

  }
  else
    {
        std::cout<<"Error: 12344! \n";
    }
    // For highly rough surfaces
    {
        double drsize=Dr.norm();
       // std::cout<<drsize<<"   "<<Linklenght <<"\n";
     if(m_smooth==true)
     {
         if(drsize>0.5*Linklenght)
         {
             
             Dr = Dr*(0.2*Linklenght/drsize);
         }
     }
    else
    {
        if(drsize>0.5*Linklenght)
        {
            // no much doing
           /* pv1->UpdateOwnInclusion(true);
            pv1->UpdateInclusion(m_Inc.at(0));
            pv2->UpdateOwnInclusion(true);
            pv2->UpdateInclusion(m_Inc.at(0));*/
          
          std::cout<<"warning: the surfaces is very rough, you may use option -smooth \n";
        }
    }
        
        
    }
   // Dr = Dr*0.0;
    Vec3D GDr=(Hous.Transpose(Hous))*Dr;
    
    x=xmid+GDr(0);
    y=ymid+GDr(1);
    z=zmid+GDr(2);

    
    *X=x;
    *Y=y;
    *Z=z;
    
}
void  Surface_Mosaicing::UpdateGeometry()
{
    
    for (std::vector<triangle *>::iterator it = m_pFT.begin() ; it != m_pFT.end(); ++it)
        (*it)->UpdateNormal_Area(m_pBox);
    for (std::vector<links *>::iterator it = m_pTMLH1.begin() ; it != m_pTMLH1.end(); ++it)
    {
        (*it)->UpdateNormal();
        (*it)->UpdateShapeOperator(m_pBox);
    }
    for (std::vector<vertex *>::iterator it = m_pFV.begin() ; it != m_pFV.end(); ++it)
        Curvature P(*it);
    
}
// This is for minimazation
void Surface_Mosaicing::RoughnessOfALink(links *l, double *linklength, double *midpointdistance)
{
    double x=0;
    double y=0;
    double z=0;
    vertex * pv1=l->GetV1();
    vertex * pv2=l->GetV2();
    Vec3D *pBox=pv1->GetBox();
    double x1=pv1->GetVXPos();
    double y1=pv1->GetVYPos();
    double z1=pv1->GetVZPos();
    
    double x2=pv2->GetVXPos();
    double y2=pv2->GetVYPos();
    double z2=pv2->GetVZPos();
    
    double xmid=(x1+x2)/2.0;
    double ymid=(y1+y2)/2.0;
    double zmid=(z1+z2)/2.0;
    
    
    if(fabs(x1-x2)>(*pBox)(0)/2)
        xmid=xmid+(*pBox)(0)/2;
    if(fabs(y1-y2)>(*pBox)(1)/2)
        ymid=ymid+(*pBox)(1)/2;
    if(fabs(z1-z2)>(*pBox)(2)/2)
        zmid=zmid+(*pBox)(2)/2;
    
    
    
    
    
    
    Vec3D geodesic_dir(x2-x1,y2-y1,z2-z1);
    
    for (int i=0;i<3;i++)
    {
        if(fabs(geodesic_dir(i))>(*pBox)(i)/2)
        {
            if(geodesic_dir(i)<0)
                geodesic_dir(i)=geodesic_dir(i)+(*pBox)(i);
            else if(geodesic_dir(i)>0)
                geodesic_dir(i)=geodesic_dir(i)-(*pBox)(i);
        }
    }
    
    double Linklenght= geodesic_dir.norm();
    geodesic_dir = geodesic_dir*(1/geodesic_dir.norm());
    
    Vec3D Lo_geoV1=(pv1->GetG2LTransferMatrix())*geodesic_dir;
    Lo_geoV1(2)=0;
    Lo_geoV1=Lo_geoV1*(1/(Lo_geoV1.norm()));
    Vec3D Glo_geoV1=(pv1->GetL2GTransferMatrix())*Lo_geoV1;
    
    Vec3D Lo_geoV2=(pv2->GetG2LTransferMatrix())*geodesic_dir;
    Lo_geoV2(2)=0;
    Lo_geoV2=Lo_geoV2*(1/(Lo_geoV2.norm()));
    Vec3D Glo_geoV2=(pv2->GetL2GTransferMatrix())*Lo_geoV2;
    
    
    
    
    
    Tensor2 Hous;
    Vec3D Zk;
    Zk(2)=1.0;
    double SignT=1;
    
    if((1+geodesic_dir(2))>(1-geodesic_dir(2)))
    {
        Zk=Zk+geodesic_dir;
        SignT=-1;
        
    }
    else if((1+geodesic_dir(2))<=(1-geodesic_dir(2)))
    {
        Zk=Zk-geodesic_dir;
    }
    
    Zk=Zk*(1.0/Zk.norm());
    
    
    Tensor2 I('I');
    Tensor2 W=Hous.makeTen(Zk);
    
    Hous=(I-(W*(W.Transpose(W)))*2)*SignT;
    
    Vec3D t_2=Hous*Glo_geoV2;
    Vec3D t_1=Hous*Glo_geoV1;
    
    t_2=t_2*(1/t_2(2));
    t_1=t_1*(1/t_1(2));
    
    
    
    t_2=t_2*(Linklenght/2.0);
    t_1=t_1*(Linklenght/2.0);
    
    /// test case to see if inclduign curvature make it better
    Vec3D Dr(0,0,0);
    
    if(m_AlgorithmType == "Type2")
    {
        
        
        Vec3D N1=pv1->GetNormalVector();
        Vec3D N2=pv2->GetNormalVector();
        std::vector <double> C1=pv1->GetCurvature();
        std::vector <double> C2=pv2->GetCurvature();
        
        double Cos1=Lo_geoV1(0);
        double Sin1=Lo_geoV1(1);
        
        double Cos2=Lo_geoV2(0);
        double Sin2=Lo_geoV2(1);
        
        double Curve1=C1.at(0)*Cos1*Cos1+C1.at(1)*Sin1*Sin1;
        double Curve2=C2.at(0)*Cos2*Cos2+C2.at(1)*Sin2*Sin2;
        
        
        double D2X_1=Curve1*(t_1.dot(t_1,t_1))*(2*N1(2)*t_1(0)/Linklenght-N1(0));
        double D2Y_1=Curve1*(t_1.dot(t_1,t_1))*(2*N1(2)*t_1(1)/Linklenght-N1(1));
        double D2X_2=Curve2*(t_2.dot(t_2,t_2))*(2*N2(2)*t_2(0)/Linklenght-N2(0));
        double D2Y_2=Curve2*(t_2.dot(t_2,t_2))*(2*N2(2)*t_2(1)/Linklenght-N2(1));
        
        
        
        
        
        
        double X_0=(D2X_1+D2X_2+5*(t_1(0)-t_2(0)))/16.0;
        double Y_0=(D2Y_1+D2Y_2+5*(t_1(1)-t_2(1)))/16.0;
        
        
        Dr(0)=X_0;
        Dr(1)=Y_0;
    }
    else if(m_AlgorithmType == "Type1")
    {
        
        Dr(0)=(t_1(0)-t_2(0))/4;
        Dr(1)=(t_1(1)-t_2(1))/4;
        
    }
    else
    {
        std::cout<<"Error: 12344! \n";
    }

        double drsize=Dr.norm();
        *linklength = Linklenght;
        *midpointdistance = drsize;

    
}










