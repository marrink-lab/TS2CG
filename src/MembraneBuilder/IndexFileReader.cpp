

#include <stdio.h>
#include "IndexFileReader.h"
#include "Nfunction.h"
IndexFileReader::IndexFileReader(std::string filename)
{


    if( filename.at(filename.size()-4)=='.')
    {
        if(filename.at(filename.size()-3)=='n' && filename.at(filename.size()-2)=='d' && filename.at(filename.size()-1)=='x' )
        {
            m_IndexFileName = filename;
        }
        else
            std::cout<<" the format of the index file is not supported "<<std::endl;
        
    }
    else
    {
        m_IndexFileName=filename+".ndx";
    }
    
    GenerateGroupsFromIndex();

}
IndexFileReader::~IndexFileReader()
{
    
}
void IndexFileReader::GenerateGroupsFromIndex()
{
    Nfunction f;
    std::ifstream ndxfile;
    ndxfile.open(m_IndexFileName.c_str());
    
    
    std::string str = "No";
    std::string str1 = "No";
    std::string gname;
    
    int s=0;
    while (true)
    {
        if(ndxfile.eof())
            break;
        
      //  std::cout<<str1<<" we are here \n";
        ndxfile>>str1;
        if(str1=="[" && str!="[")
        {
        ndxfile>>gname>>str;
            IndexGroup group(gname);
            
            s++;
           // std::cout<<gname<<"  "<<s<<"    1 1\n";
            while(true)
            {
                ndxfile>>str;
                if(str=="[" || ndxfile.eof())
                    break;
                group.AddBeadToGroup(f.String_to_Int(str));

            }
            m_GroupIndex.push_back(group);

        }
        else if( str=="[")
        {
            gname = str1;
            ndxfile>>str;
            IndexGroup group(gname);
            
            s++;
           // std::cout<<gname<<"  "<<s<<"\n";
            while(true)
            {
                ndxfile>>str;
                if(str=="[" || ndxfile.eof())
                    break;
                group.AddBeadToGroup(f.String_to_Int(str));
                
            }
            m_GroupIndex.push_back(group);

        }
        else
        {
            std::getline(ndxfile,str1);
            std::cout<<"Line containing "<<str1<<" is ignored "<<std::endl;
        }
    }
    
   
    
    for (std::vector<IndexGroup>::iterator it = m_GroupIndex.begin() ; it != m_GroupIndex.end(); ++it)
    {
        
        m_pGroupIndex.push_back(&(*it));
    }
    ndxfile.close();
}














