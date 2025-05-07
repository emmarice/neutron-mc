#include "../include/jh.h"
#include <string.h>
#include <iostream>
#include <numbers>

/*
MCstats::MCstats()
{
    m_fission_sites = allocate(0,0);
}
*/
void MCstats::setDims(float a_x, float a_y, int grains)
{
  m_row = (int)a_x*grains;
  m_col = (int)a_y*grains;
  m_grains = grains;
}

MCstats::MCstats(float a_x, float a_y, int grains=1)
{
    m_row = (int)a_x*grains;
    m_col = (int)a_y*grains;
    m_grains = grains;
    std::cout<< "Number of cells per cm is: "<< m_grains <<std::endl;
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
    for (int row =0; row<r; row++){
      new_sites[row]= new float[cols];
      // Have to set cols*4 since each float is 4 bytes
      memset(new_sites[row],0,cols*4);
    }
    return new_sites;
}

float * MCstats::getRow(int row)
{
  if (row > m_row){
    std::cerr <<" ERROR: The row at index " <<row<< " is out of bounds"<<std::endl;
    return NULL;
  } else {
    float * rowStart = m_fission_sites[row];
    return rowStart;
  }

}
// This function adds a new fission site wherever the neutron fissions
void MCstats::setFissionSite(neutron a_neutron)
{
  int xloc = (int)a_neutron.getPos().first*m_grains;
  int yloc = (int)a_neutron.getPos().second*m_grains;
  if (yloc > m_col){
    std::cerr <<" ERROR: The column at index " <<yloc<< " is out of bounds"<<std::endl;
  } else {
    m_fission_sites[xloc][yloc] +=1.0;
    m_totalParts+=1;
  }
}

void MCstats::setFissionSite(neutron a_neutron, int a_n)
{
  int n = a_n;
  int xloc = (int)m_grains*a_neutron.getPos().first;
  int yloc = (int)m_grains*a_neutron.getPos().second;
  if (yloc > m_col){
    std::cerr <<" ERROR: The column at index " <<yloc<< " is out of bounds"<<std::endl;
  } else {
    m_fission_sites[xloc][yloc] += n;
    m_totalParts += n;
  }
}
void MCstats::printFissionSites()
{
  std::cout<< "Trying to print"<< std::endl;
  for (int row = 0; row< m_row; row++){
    for (int col =0; col < m_col; col++){
      std::cout << m_fission_sites[row][col] << " ";
    }
    std::cout<<"Row: " << row<<std::endl;
  }
}
MCstats::~MCstats()
{
    deallocate();
}

double MCstats::sampleEnergy(randomGen * a_rand){
  double minE=1e-8;
  double maxE=15.0;
  double maxP=0.358206;
  bool reject=true;
  double en;
  while(reject)
  {
    double eta1=a_rand->getNormRand();
    double eta2=a_rand->getNormRand();
    double xx=minE+eta1*(maxE-minE);
    double p=0.453*std::exp(-1.036*xx)*std::sinh(std::pow(2.29*xx,1/2));
    if(eta2*maxP<=p)
    {
      reject=false;
      en =xx/1000; //convert to kev
    }
  }
 return en;
}

void MCstats::normalizeSites(){
  for (int row = 0; row< m_row; row++){
    for (int col =0; col < m_col; col++){
      m_normSites.push_back(m_fission_sites[row][col]/m_totalParts);
      m_locations.push_back(std::make_pair(row,col));
    }
  }
}

state MCstats::nextState(int a_numParticles,randomGen * rgen){
  state next; 
  int numN = 0;
  double runningTot = 0.0;
  normalizeSites();
  for (int i = 0; i<a_numParticles; i++){
    double rando = rgen->getNormRand();
    for (int j = 0; j<m_normSites.size(); j++){
      runningTot+=m_normSites[j];
      if (rando <= runningTot){
        neutron n;
        double xpos= (m_locations[j].first + rgen->getNormRand())/m_grains;
        double ypos= (m_locations[j].second + rgen->getNormRand())/m_grains;
        n.setE(sampleEnergy(rgen));
        n.setAngle(rgen->getNormRand()*4*std::acos(0.0));
        n.setPos(xpos,ypos);
        next.addNeutron(n);
        break;
      }
    }
  }
  return next;
}

int MCstats::getTotalFissions(){
  return m_totalParts;
}

double MCstats::getEntropy(){
  double entropy=0.0;
  for (int i = 0; i<m_normSites.size(), i++){
    entropy += m_normSites[i]*std::log(m_normSites)
  }
  return entropy;
}
  /* Create a running total of the amount of fission sites in the next generation
   The probability for a neutron to be born in the next set of 
   fission sites is the normalized discretized PDF of the current sites
   nextState returns a vector of neutrons for the next trail run
   shannon entropy is the bin I/all bins so we should save this as a variable


}

*/