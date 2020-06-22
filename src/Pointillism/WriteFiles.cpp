


#include "WriteFiles.h"


WriteFiles::WriteFiles(Vec3D *Box)
{

m_pBox=(Box);












}

WriteFiles::~WriteFiles()
{
    
}

void WriteFiles::Writevtu(std::vector< vertex* > ver, std::vector< triangle* > triangle1,  std::vector< links* > links1, std::string Filename)
{
    
    Vec3D m_Box=*m_pBox;
    
    
    // First make all the triangles visualizable
    for (std::vector<triangle *>::iterator it = triangle1.begin() ; it != triangle1.end(); ++it)
    {
        
        (*it)->UpdateRepresentation(true);
    }
    // then check the trinagle visualizablity
    for (std::vector<links *>::iterator it = links1.begin() ; it != links1.end(); ++it)
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
        
        if(fabs(dx)>m_Box(0)/2.0 || fabs(dy)>m_Box(1)/2.0 || fabs(dz)>m_Box(2)/2.0)
        {
            
            rep=false;
        }
        if(rep==false)
        {
            ((*it)->GetTriangle())->UpdateRepresentation(rep);
            
        }
        
    }
    
    
    
    
    
    int numv=ver.size();
    int numtri=triangle1.size();
    int numtrirep=0;
    
    for (int i=0;i<triangle1.size();i++)
    {
        triangle* a=triangle1.at(i);
        if(a->GetRepresentation()==true)
        {numtrirep++;}
        
    }
    
    
    
    std::ofstream Output;
    Output.open(Filename.c_str());
    
    
    
    
    Output<<"<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"BigEndian\">"<<"\n";
    Output<<"  <UnstructuredGrid>"<<"\n";
    Output<<"    <Piece NumberOfPoints=\""<<numv<<"\" NumberOfCells=\""<<numtrirep<<"\">"<<"\n";
    Output<<"      <PointData Scalars=\"scalars\">"<<"\n";

        Output<<"        <DataArray type=\"Float32\" Name=\"inc\" Format=\"ascii\">"<<"\n";
        
        for (int i=0;i<numv;i++)
        {
            vertex* a=ver.at(i);
            if(a->VertexOwnInclusion()==true)
            {
                    Output<<"          "<<1<<"\n";
            }
            else
                Output<<"          "<<0<<"\n";
            
        }
        Output<<"        </DataArray>"<<"\n";

    
    WriteInclusion("dir", ver, &Output);
    
    
    
    
    Output<<"      </PointData>"<<"\n";
    Output<<"      <Points>"<<"\n";
    Output<<"        <DataArray type=\"Float32\" NumberOfComponents=\"3\" Format=\"ascii\">"<<"\n";
    
    
    for (int i=0;i<numv;i++)
    {
        vertex* a=ver.at(i);
        Output<<"          "<<a->GetVXPos()<<" "<<a->GetVYPos()<<" "<<a->GetVZPos()<<" "<<"\n";
        
        
    }
    Output<<"        </DataArray>"<<"\n";
    Output<<"      </Points>"<<"\n";
    
    Output<<"      <Cells>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"connectivity\" Format=\"ascii\">"<<"\n";
    
    
    for (int i=0;i<numtri;i++)
    {
        
        
        triangle* a=triangle1.at(i);
        
        if(a->GetRepresentation()==true)
            Output<<"           "<<(a->GetV1())->GetVID()<<" "<<(a->GetV2())->GetVID()<<" "<<(a->GetV3())->GetVID()<<" "<<"\n";
        
        
    }
    Output<<"        </DataArray>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"offsets\" Format=\"ascii\">"<<"\n";
    int ofset=3;
    Output<<"          ";
    for (int i=0;i<numtrirep;i++)
    {
        
        Output<<ofset+3*i<<" ";
        
        
    }
    Output<<"\n";
    
    Output<<"        </DataArray>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"types\" Format=\"ascii\">"<<"\n";
    Output<<"          ";
    for (int i=0;i<numtrirep;i++)
    {
        
        Output<<5<<" ";
        
        
    }
    Output<<"\n";
    
    
    
    
    
    
    
    Output<<"        </DataArray>"<<"\n";
    Output<<"      </Cells>"<<"\n";
    Output<<"    </Piece>"<<"\n";
    Output<<"  </UnstructuredGrid>"<<"\n";
    Output<<"</VTKFile> "<<"\n";
    
    
}
/////////////////////////

void WriteFiles::Writefullvtu(std::vector< vertex* > ver, std::vector< triangle* > triangle1,  std::vector< links* > links1, std::string Filename)
{
    
    Vec3D m_Box=*m_pBox;
    
    // First make all the triangles visualizable
    for (std::vector<triangle *>::iterator it = triangle1.begin() ; it != triangle1.end(); ++it)
    {
        
        (*it)->UpdateRepresentation(true);
    }
    // then check the trinagle visualizablity
    for (std::vector<links *>::iterator it = links1.begin() ; it != links1.end(); ++it)
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
            triangle * t=(*it)->GetTriangle();
            t->UpdateRepresentation(rep);
            
            
        }
        
    }
    
    
    
    
    
    int numv=ver.size();
    int numtri=triangle1.size();
    int numtrirep=0;
    
    for (int i=0;i<triangle1.size();i++)
    {
        triangle* a=triangle1.at(i);
        if(a->GetRepresentation()==true)
        {numtrirep++;}
        
    }
    
    
    
    std::ofstream Output;
    Output.open(Filename.c_str());
    
    
    
    
    Output<<"<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"BigEndian\">"<<"\n";
    Output<<"  <UnstructuredGrid>"<<"\n";
    Output<<"    <Piece NumberOfPoints=\""<<numv<<"\" NumberOfCells=\""<<numtrirep<<"\">"<<"\n";
    Output<<"      <PointData Scalars=\"scalars\">"<<"\n";
    Output<<"        <DataArray type=\"Float32\" Name=\"inc\" Format=\"ascii\">"<<"\n";
    
    for (int i=0;i<numv;i++)
    {
        vertex* a=ver.at(i);
        if(a->VertexOwnInclusion()==true)
            Output<<"          "<<1<<"\n";
        else
            Output<<"          "<<0<<"\n";
        
    }
    Output<<"        </DataArray>"<<"\n";
    //============================================= cuvrature c1
    Output<<"        <DataArray type=\"Float32\" Name=\"c1\" Format=\"ascii\">"<<"\n";
    
    for (int i=0;i<numv;i++)
    {
        vertex* a=ver.at(i);
        std::vector <double> c=a->GetCurvature();
            Output<<"          "<<c.at(0)<<"\n";

        
    }
    Output<<"        </DataArray>"<<"\n";
    //============================================= cuvrature c2

    Output<<"        <DataArray type=\"Float32\" Name=\"c2\" Format=\"ascii\">"<<"\n";
    
    for (int i=0;i<numv;i++)
    {
        vertex* a=ver.at(i);
        std::vector <double> c=a->GetCurvature();
        Output<<"          "<<c.at(1)<<"\n";
        
        
    }
    Output<<"        </DataArray>"<<"\n";
    //============================================= inclsuion v
    WriteInclusion("dir", ver, &Output);
    //============================================= principal direction 1

    (Output)<<std::fixed;
    (Output)<<std::setprecision( Precision );
    Output<<"        <DataArray type=\"Float32\" Name=\" t1 \"  NumberOfComponents=\"3\" Format=\"ascii\">"<<"\n";
    for (std::vector<vertex*>::iterator itr = ver.begin() ; itr != ver.end(); ++itr)
    {

            Tensor2 LG= (*itr)->GetL2GTransferMatrix();
            Vec3D L(1,0,0);
        Vec3D G=LG*L;
            Output<<"  "<<G(0)<<"      "<<G(1)<<"      "<<G(2)<<"\n";

        
    }
    Output<<"        </DataArray>"<<"\n";

    //============================================= principal direction  2
    (Output)<<std::fixed;
    (Output)<<std::setprecision( Precision );
    Output<<"        <DataArray type=\"Float32\" Name=\" t2 \"  NumberOfComponents=\"3\" Format=\"ascii\">"<<"\n";
    for (std::vector<vertex*>::iterator itr = ver.begin() ; itr != ver.end(); ++itr)
    {
        
        Tensor2 LG= (*itr)->GetL2GTransferMatrix();
        Vec3D L(0,1,0);
        Vec3D G=LG*L;
        Output<<"  "<<G(0)<<"      "<<G(1)<<"      "<<G(2)<<"\n";
        
        
    }
    Output<<"        </DataArray>"<<"\n";
    
    //============================================= normal
    (Output)<<std::fixed;
    (Output)<<std::setprecision( Precision );
    Output<<"        <DataArray type=\"Float32\" Name=\" normal \"  NumberOfComponents=\"3\" Format=\"ascii\">"<<"\n";
    for (std::vector<vertex*>::iterator itr = ver.begin() ; itr != ver.end(); ++itr)
    {
        
        Vec3D N= (*itr)->GetNormalVector();

        Output<<"  "<<N(0)<<"      "<<N(1)<<"      "<<N(2)<<"\n";
        
        
    }
    Output<<"        </DataArray>"<<"\n";

       //============================================= vertices points
    Output<<"      </PointData>"<<"\n";
    Output<<"      <Points>"<<"\n";
    Output<<"        <DataArray type=\"Float32\" NumberOfComponents=\"3\" Format=\"ascii\">"<<"\n";
    
    
    for (int i=0;i<numv;i++)
    {
        vertex* a=ver.at(i);
        Output<<"          "<<a->GetVXPos()<<" "<<a->GetVYPos()<<" "<<a->GetVZPos()<<" "<<"\n";
        
        
    }
    Output<<"        </DataArray>"<<"\n";
    Output<<"      </Points>"<<"\n";
    
    Output<<"      <Cells>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"connectivity\" Format=\"ascii\">"<<"\n";
    
    
    for (int i=0;i<numtri;i++)
    {
        
        
        triangle* a=triangle1.at(i);
        
        if(a->GetRepresentation()==true)
            Output<<"           "<<(a->GetV1())->GetVID()<<" "<<(a->GetV2())->GetVID()<<" "<<(a->GetV3())->GetVID()<<" "<<"\n";
        
        
    }
    Output<<"        </DataArray>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"offsets\" Format=\"ascii\">"<<"\n";
    int ofset=3;
    Output<<"          ";
    for (int i=0;i<numtrirep;i++)
    {
        
        Output<<ofset+3*i<<" ";
        
        
    }
    Output<<"\n";
    
    Output<<"        </DataArray>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"types\" Format=\"ascii\">"<<"\n";
    Output<<"          ";
    for (int i=0;i<numtrirep;i++)
    {
        
        Output<<5<<" ";
        
        
    }
    Output<<"\n";
    
    
    
    
    
    
    
    Output<<"        </DataArray>"<<"\n";
    Output<<"      </Cells>"<<"\n";
    Output<<"    </Piece>"<<"\n";
    Output<<"  </UnstructuredGrid>"<<"\n";
    Output<<"</VTKFile> "<<"\n";
    
    
}

void WriteFiles::WriteInclusion(std::string id, std::vector<vertex* > ver, std::ofstream *Output)
{
    double cc=0;
    (*Output)<<std::fixed;
    (*Output)<<std::setprecision( Precision );
    *Output<<"        <DataArray type=\"Float32\" Name=\""<<id<<"\"  NumberOfComponents=\"3\" Format=\"ascii\">"<<"\n";
    for (std::vector<vertex*>::iterator itr = ver.begin() ; itr != ver.end(); ++itr)
    {
        if((*itr)->VertexOwnInclusion()==true)
        {
            inclusion* in=(*itr)->GetInclusion();
            Vec3D D=(in)->GetLDirection();
            D=((*itr)->GetL2GTransferMatrix())*D;

            *Output<<"  "<<D(0)<<"      "<<D(1)<<"      "<<D(2)<<"\n";
        }
        else
        *Output<<"  "<<cc<<"    "<<cc<<"    "<<cc<<"\n";
        
    }
    *Output<<"        </DataArray>"<<"\n";
    
}
void WriteFiles::WritevtuNochange(std::vector< vertex* > ver, std::vector< triangle* > triangle1,  std::vector< links* > links1, std::string Filename)
{
    
    Vec3D m_Box=*m_pBox;
    
    // First make all the triangles visualizable

    
    
    
    int numv=ver.size();
    int numtri=triangle1.size();
    int numtrirep=0;
    
    for (int i=0;i<triangle1.size();i++)
    {
        triangle* a=triangle1.at(i);
        if(a->GetRepresentation()==true)
        {numtrirep++;}
        
    }
    
    
    
    std::ofstream Output;
    Output.open(Filename.c_str());
    
    
    
    
    Output<<"<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"BigEndian\">"<<"\n";
    Output<<"  <UnstructuredGrid>"<<"\n";
    Output<<"    <Piece NumberOfPoints=\""<<numv<<"\" NumberOfCells=\""<<numtrirep<<"\">"<<"\n";
    Output<<"      <PointData Scalars=\"scalars\">"<<"\n";
    Output<<"        <DataArray type=\"Float32\" Name=\"inc\" Format=\"ascii\">"<<"\n";
    
    for (int i=0;i<numv;i++)
    {
        vertex* a=ver.at(i);
        if(a->VertexOwnInclusion()==true)
            Output<<"          "<<1<<"\n";
        else
            Output<<"          "<<0<<"\n";
        
    }
    Output<<"        </DataArray>"<<"\n";
    
    // std::string R= (*itr)->GetName();
    WriteInclusion("dir", ver, &Output);
    
    /*for (std::vector<RigidInclusion*>::iterator itr = inc.begin() ; itr != inc.end(); ++itr)
     {
     std::string R= (*itr)->GetName();
     WriteInclusion(R, ver, &Output);
     }*/
    
    
    Output<<"      </PointData>"<<"\n";
    Output<<"      <Points>"<<"\n";
    Output<<"        <DataArray type=\"Float32\" NumberOfComponents=\"3\" Format=\"ascii\">"<<"\n";
    
    
    for (int i=0;i<numv;i++)
    {
        vertex* a=ver.at(i);
        Output<<"          "<<a->GetVXPos()<<" "<<a->GetVYPos()<<" "<<a->GetVZPos()<<" "<<"\n";
        
        
    }
    Output<<"        </DataArray>"<<"\n";
    Output<<"      </Points>"<<"\n";
    
    Output<<"      <Cells>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"connectivity\" Format=\"ascii\">"<<"\n";
    
    
    for (int i=0;i<numtri;i++)
    {
        
        
        triangle* a=triangle1.at(i);
        
        if(a->GetRepresentation()==true)
            Output<<"           "<<(a->GetV1())->GetVID()<<" "<<(a->GetV2())->GetVID()<<" "<<(a->GetV3())->GetVID()<<" "<<"\n";
        
        
    }
    Output<<"        </DataArray>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"offsets\" Format=\"ascii\">"<<"\n";
    int ofset=3;
    Output<<"          ";
    for (int i=0;i<numtrirep;i++)
    {
        
        Output<<ofset+3*i<<" ";
        
        
    }
    Output<<"\n";
    
    Output<<"        </DataArray>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"types\" Format=\"ascii\">"<<"\n";
    Output<<"          ";
    for (int i=0;i<numtrirep;i++)
    {
        
        Output<<5<<" ";
        
        
    }
    Output<<"\n";
    
    
    
    
    
    
    
    Output<<"        </DataArray>"<<"\n";
    Output<<"      </Cells>"<<"\n";
    Output<<"    </Piece>"<<"\n";
    Output<<"  </UnstructuredGrid>"<<"\n";
    Output<<"</VTKFile> "<<"\n";
    
    
}
void WriteFiles::Writevtunew(std::vector< vertex* > ver, std::vector< triangle* > triangle1,  std::string Filename)
{
    
    Vec3D m_Box=*m_pBox;
    double L=m_Box(0);
    if(m_Box(1)<L)
    L=m_Box(1);
    if(m_Box(2)<L)
    L=m_Box(2);
    L=L/2;

    // First make all the triangles visualizable
    for (std::vector<triangle *>::iterator it = triangle1.begin() ; it != triangle1.end(); ++it)
    {
        
        (*it)->UpdateRepresentation(true);
        vertex * v1=(*it)->GetV1();
        vertex * v2=(*it)->GetV2();
        vertex * v3=(*it)->GetV3();
        
        Vec3D X1(v1->GetVXPos(),v1->GetVYPos(),v1->GetVZPos());
        Vec3D X2(v2->GetVXPos(),v2->GetVYPos(),v2->GetVZPos());
        Vec3D X3(v3->GetVXPos(),v3->GetVYPos(),v3->GetVZPos());
        
        bool rep=true;
        
        if((X1-X2).norm()>L || (X3-X2).norm()>L || (X1-X3).norm()>L)
        {
            
            rep=false;
            ((*it))->UpdateRepresentation(rep);

        }

    }

    
    
    
    
    
    int numv=ver.size();
    int numtri=triangle1.size();
    int numtrirep=0;
    
    for (int i=0;i<triangle1.size();i++)
    {
        triangle* a=triangle1.at(i);
        if(a->GetRepresentation()==true)
        {numtrirep++;}
        
    }
    
    
    
    std::ofstream Output;
    Output.open(Filename.c_str());
    
    
    
    
    Output<<"<VTKFile type=\"UnstructuredGrid\" version=\"0.1\" byte_order=\"BigEndian\">"<<"\n";
    Output<<"  <UnstructuredGrid>"<<"\n";
    Output<<"    <Piece NumberOfPoints=\""<<numv<<"\" NumberOfCells=\""<<numtrirep<<"\">"<<"\n";
    Output<<"      <PointData Scalars=\"scalars\">"<<"\n";
    
    Output<<"        <DataArray type=\"Float32\" Name=\"inc\" Format=\"ascii\">"<<"\n";
    
    for (int i=0;i<numv;i++)
    {
        vertex* a=ver.at(i);
        if(a->VertexOwnInclusion()==true)
        {
            Output<<"          "<<1<<"\n";
        }
        else
        Output<<"          "<<0<<"\n";
        
    }
    Output<<"        </DataArray>"<<"\n";
    
    
    WriteInclusion("dir", ver, &Output);
    
    
    
    
    Output<<"      </PointData>"<<"\n";
    Output<<"      <Points>"<<"\n";
    Output<<"        <DataArray type=\"Float32\" NumberOfComponents=\"3\" Format=\"ascii\">"<<"\n";
    
    
    for (int i=0;i<numv;i++)
    {
        vertex* a=ver.at(i);
        Output<<"          "<<a->GetVXPos()<<" "<<a->GetVYPos()<<" "<<a->GetVZPos()<<" "<<"\n";
        
        
    }
    Output<<"        </DataArray>"<<"\n";
    Output<<"      </Points>"<<"\n";
    
    Output<<"      <Cells>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"connectivity\" Format=\"ascii\">"<<"\n";
    
    
    for (int i=0;i<numtri;i++)
    {
        
        
        triangle* a=triangle1.at(i);
        
        if(a->GetRepresentation()==true)
        Output<<"           "<<(a->GetV1())->GetVID()<<" "<<(a->GetV2())->GetVID()<<" "<<(a->GetV3())->GetVID()<<" "<<"\n";
        
        
    }
    Output<<"        </DataArray>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"offsets\" Format=\"ascii\">"<<"\n";
    int ofset=3;
    Output<<"          ";
    for (int i=0;i<numtrirep;i++)
    {
        
        Output<<ofset+3*i<<" ";
        
        
    }
    Output<<"\n";
    
    Output<<"        </DataArray>"<<"\n";
    Output<<"        <DataArray type=\"Int32\" Name=\"types\" Format=\"ascii\">"<<"\n";
    Output<<"          ";
    for (int i=0;i<numtrirep;i++)
    {
        
        Output<<5<<" ";
        
        
    }
    Output<<"\n";
    
    
    
    
    
    
    
    Output<<"        </DataArray>"<<"\n";
    Output<<"      </Cells>"<<"\n";
    Output<<"    </Piece>"<<"\n";
    Output<<"  </UnstructuredGrid>"<<"\n";
    Output<<"</VTKFile> "<<"\n";
    
    
}
