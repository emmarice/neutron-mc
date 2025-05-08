#include "../include/jh.h"
#include <string.h>
#include <iostream>
#include <numbers>
#include <cmath>
/*
MCstats::MCstats()
{
    m_fission_sites = allocate(0,0);
}
*/
void MCstats::setDims(double a_x, double a_y, int grains)
{
  m_row = (int)a_x*grains;
  m_col = (int)a_y*grains;
  m_grains = grains;
}

MCstats::MCstats(double a_x, double a_y, int grains=1)
{
    m_row = (int)a_x*grains;
    m_col = (int)a_y*grains;
    std::cout << "max Y is: " <<a_y << std::endl;
    std::cout << "max collumn is: " <<m_col << std::endl;
    m_grains = grains;
    std::cout<< "Number of cells per cm is: "<< m_grains <<std::endl;
    m_fission_sites=allocate(m_row,m_col);
}

void MCstats::clear()
{
  m_totalParts = 0;
  m_normSites.clear();
  m_locations.clear();
  for (int row =0; row<m_row; row++)
  {
  // Have to set cols*4 since each double is 4 bytes
    memset(m_fission_sites[row],0,m_col*4);
  }
}

void MCstats::deallocate()
{
    for(int row =0; row<m_row ;row++){
      delete [] m_fission_sites[row];
    }
    delete [] m_fission_sites;
}

double ** MCstats::allocate(int rows, int cols)
{
    int r =  rows;
    int c = cols;

    double ** new_sites=  new double*[rows];
    for (int row =0; row<r; row++){
      new_sites[row]= new double[cols];
      // Have to set cols*4 since each double is 4 bytes
      memset(new_sites[row],0,cols*4);
    }
    return new_sites;
}

double * MCstats::getRow(int row)
{
  if (row > m_row){
    std::cerr <<" ERROR: The row at index " <<row<< " is out of bounds"<<std::endl;
    return NULL;
  } else {
    double * rowStart = m_fission_sites[row];
    return rowStart;
  }

}
// This function adds a new fission site wherever the neutron fissions
void MCstats::setFissionSite(neutron a_neutron)
{
  int xloc = (int)a_neutron.getPos().first*m_grains;
  int yloc = (int)a_neutron.getPos().second*m_grains;
  if (yloc > m_col)
  {
    std::cout <<" ERROR: The value of max Y is " <<m_col <<std::endl;
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
void MCstats::saveFissionSites(std::string a_of)
{
  std::ofstream outFile(a_of);
  std::cout<< "Trying to print"<< std::endl;
  for (int row = 0; row< m_row; row++){
    for (int col =0; col < m_col; col++)
    {
      outFile << m_fission_sites[row][col]<<" ";
    }
    outFile<<"\n";
  }
  outFile.close();
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
      en =xx*1000; //convert to kev
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
  // std::cout<<"next"<<std::endl;
  state next; 
  int numN = 0;
  double runningTot = 0.0;
  normalizeSites();
  for (int i = 0; i<a_numParticles; i++)
  {
    double rando = rgen->getNormRand();
    for (int j = 0; j<m_normSites.size(); j++)
    {
      runningTot+=m_normSites[j];
      if (rando <= runningTot)
      {
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
  // std::cout<<next.getNumParticles()<<std::endl;
  // std::cout<<next.getParticles()[35].getE()<<std::endl;
  return next;
}

int MCstats::getTotalFissions(){
  return m_totalParts;
}

double MCstats::getEntropy(){
  double entropy=0.0;
  for (int i = 0; i<m_normSites.size(); i++){
    if (m_normSites[i] >0.0){
      // std::cout<<"here"<<std::endl;
      entropy += -1*m_normSites[i]*std::log(m_normSites[i]);
    }
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
std::pair<double, double> MCstats::getStats(std::vector<double> a_vec)
{
  double mu=0;
  double var=0;
  int n=0;
  for(double val : a_vec)
  {
    n++;
    mu+=val;
  }
  mu=mu/n;
  for(double val : a_vec)
  {
    var+=std::pow(val-mu,2);
  }
  var=std::pow(var/(n-1),0.5);
  std::pair<double, double> outp = {mu,var};
  return outp;
}