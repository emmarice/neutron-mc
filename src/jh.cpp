#include "jh.h"
#include <shtring.h>


MCstats::MCstats()
{
    m_fission_sites = allocate(0,0);
}
void MCstats::setDims(float a_x, float a_y, int grains)
{
  m_row = (int)a_x*grains;
  m_col = (int)a_y*grains;
  m_grains = grains;
}

MCstats::MCstats(float a_x, float a_y, int grains)
{
    m_row = (int)a_x*grains;
    m_col = (int)a_y*grains;
    m_grains = grains;
    m_fission_sites=allocate(m_row,m_col);
}

void MCstats::deallocate()
{
    for(int row =0; row<m_row ;row++){
      delete [] m_fission_sites[row];
    }
    delete [] m_fission_sites;
}

float ** MCstats::allocate(int rows, int cols)
{
    int r =  rows;
    int c = cols;

    float ** new_sites=  new float*[rows];
    for (int row =0; row<rows; row++){
      new_sites[row]= new float[cols];
      // Have to set cols*4 since each float is 4 bytes
      memset(new_sites[row],0,cols*4);
      std::cout << new_sites[row] << std::endl;
    }
    return new_sites;
}

MCstats::getRow(int row)
{
  if (row > m_row){
    std::cerr <<" ERROR: The row at index " <<row<< " is out of bounds"<<std::endl;
  } else {
    int * rowStart = m_fission_sites[row];
    return rowStart;
  }

}
// This function adds a new fission site wherever the neutron fissions
MCstats::setFissionSite(neutron::neutron a_neutron)
{
  int xloc = (int)m_grains*a_neutron.m_x;
  int yloc = (int)m_grains*a_neutron.m_y;
  if (yloc > m_col){
    std::cerr <<" ERROR: The column at index " <<yloc<< " is out of bounds"<<std::endl;
  } else {
    m_fission_sites.getRow(xloc)[yloc] +=1.0;
  }
}

MCstats::setFissionSite(neutron::neutron a_neutron, float a_n)
{
  float n = a_n;
  int xloc = (int)m_grains*a_neutron.m_x;
  int yloc = (int)m_grains*a_neutron.m_y;
  if (yloc > m_col){
    std::cerr <<" ERROR: The column at index " <<yloc<< " is out of bounds"<<std::endl;
  } else {
    m_fission_sites.getRow(xloc)[yloc] += n;
  }
}
MCstats::~MCstats()
{
    deallocate();
}