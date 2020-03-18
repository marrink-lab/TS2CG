

#include <stdio.h>
#include "VertexMove.h"
#include "Curvature.h"
//#include "links.h"
VertexMove::VertexMove(vertex *pvertex, double dx, double dy, double dz,Vec3D *Box):m_vertex(0)
{


m_pvertex=pvertex;
m_pBox=Box;
m_dx=dx;
m_dy=dy;
m_dz=dz;
    
m_pAVer=m_pvertex->GetVNeighbourVertex();
    m_oldX=m_pvertex->GetVXPos();
    m_oldY=m_pvertex->GetVYPos();
    m_oldZ=m_pvertex->GetVZPos();
	m_X=m_oldX+m_dx;
	m_Y=m_oldY+m_dy;
	m_Z=m_oldZ+m_dz;



}


VertexMove::~VertexMove()
{
    
}


void VertexMove::Move()
{
	    m_vertex=(*m_pvertex);

        for (std::vector<vertex *>::iterator it = m_pAVer.begin() ; it != m_pAVer.end(); ++it)
        {

		    m_AVer.push_back(*(*it));
        }
	
    m_pvertex->UpdateVXPos(m_X);
	m_pvertex->UpdateVYPos(m_Y);
	m_pvertex->UpdateVZPos(m_Z);

    std::vector<links *> pL=m_pvertex->GetVLinkList();

    for (std::vector<links *>::iterator it = pL.begin() ; it != pL.end(); ++it)
    {
        links *nl=(*it)->GetNeighborLink1();
        m_nLinks.push_back(*(*it));
        m_RingLinks.push_back(*nl);
        
        
    }


    
    //==== Update the ring triangle normal
    std::vector<triangle *> pT=m_pvertex->GetVTraingleList();
    for (std::vector<triangle *>::iterator it = pT.begin() ; it != pT.end(); ++it)
    {
        
        m_Triangle.push_back(*(*it));
        (*it)->UpdateNormal_Area(m_pBox);
        
    }
    
    for (std::vector<links *>::iterator it = pL.begin() ; it != pL.end(); ++it)
    {
        (*it)->UpdateNormal();
        (*it)->UpdateShapeOperator(m_pBox);
        links *nl=(*it)->GetNeighborLink1();
        nl->UpdateNormal();
        nl->UpdateShapeOperator(m_pBox);
        
        
    }
    //==== Update vertexes
    
    Curvature Q(m_pvertex);
    for (std::vector<vertex *>::iterator it = m_pAVer.begin() ; it != m_pAVer.end(); ++it)
        Curvature P(*it);

    

}

void VertexMove::RejectMove()
{

(*m_pvertex)=m_vertex;

int i=0;
        for (std::vector<vertex>::iterator it = m_AVer.begin() ; it != m_AVer.end(); ++it)
        {

		*(m_pAVer.at(i))=(*it);
		i++;
        }


    i=0;
    std::vector<links *> pL=m_pvertex->GetVLinkList();
    for (std::vector<links *>::iterator it = pL.begin() ; it != pL.end(); ++it)
    {
        links *nl=(*it)->GetNeighborLink1();
        
        
        *(*it)=m_nLinks.at(i);
        
        (*it)->GetMirrorLink()->PutNormal((*it)->GetNormal());
        (*it)->GetMirrorLink()->PutShapeOperator((*it)->GetBe(),(*it)->GetHe());
        
        *nl=m_RingLinks.at(i);
        
        nl->GetMirrorLink()->PutNormal(nl->GetNormal());
        nl->GetMirrorLink()->PutShapeOperator(nl->GetBe(),nl->GetHe());
        
        i++;
    }
    
    i=0;
    std::vector<triangle *> pT=m_pvertex->GetVTraingleList();
    for (std::vector<triangle *>::iterator it = pT.begin() ; it != pT.end(); ++it)
    {
        *(*it)=m_Triangle.at(i);
        
        i++;
    }

    std::vector<links>::iterator itr=m_LIntEChange.begin();
    for (std::vector<links *>::iterator it = m_pLIntEChange.begin() ; it != m_pLIntEChange.end(); ++it)
    {
        double en=(*itr).GetIntEnergy();
        (*it)->UpdateIntEnergy(en);
        ((*it)->GetMirrorLink())->UpdateIntEnergy(en);


        ++itr;
        
    }
}


