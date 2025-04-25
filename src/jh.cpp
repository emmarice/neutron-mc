#include "jh.h"


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

MCstats::MCstats(int rows, int cols)
{
    m_fission_sites=allocate(rows,cols);
}

void MCstats::deallocate()
{
    for(int row =0; row<m_row ;row++){
      delete [] m_fission_sites[row];
    }
    delete [] m_fission_sites;
}

int ** MCstats::allocate(int rows, int cols)
{
    int r =  rows;
    int c = cols;

    int ** new_sites=  new int*[rows];
    for (int row =0; row<rows; row++){
      new_sites[row]= new int[cols];
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
    m_fission_sites.getRow(xloc)[yloc] +=1;
  }
}

MCstats::~MCstats()
{
    deallocate();
}