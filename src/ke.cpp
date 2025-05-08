#include "../include/ke.h"
#include <fstream>
#include <cmath>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <sstream>
// puts the class in its basic state
materialManager::materialManager()
{

}
void materialManager::addCX(std::string a_matName,std::string a_CXType,
                     std::string a_fileName)
{
  std::ifstream cxFile(a_fileName);
  double En;
  double Cx;
  std::string line;
  std::string tempWord;
  getline(cxFile,line);
  getline(cxFile,line);
  getline(cxFile,line);
  while (getline(cxFile,line))
  {
    std::stringstream strSec(line);
    getline(strSec,tempWord,';');
    En=stod(tempWord);
    getline(strSec,tempWord,';');
    Cx=stod(tempWord);
    m_crossX[a_matName][a_CXType].push_back({En,Cx});
  }
}
void materialManager::addMaterial(std::string a_matName,
                    std::vector<std::pair<std::string,std::string>> a_files)
{

  for(auto fi:a_files)
  {
    addCX(a_matName,fi.first,fi.second);
    if(fi.first!="tot")
    {
      m_matTypes[a_matName].push_back(fi.first);
    }
    m_mats.push_back(a_matName);
  }
}
std::pair<double,double> materialManager::findBound(neutron a_neutron, double a_d)
{
  double angle=a_neutron.getAngle();
  double oldx=a_neutron.getPos().first;
  double oldy=a_neutron.getPos().second;
  double xlb=-100;
  double ylb=-100;
  double xhb=-100;
  double yhb=-100;
  std::pair<double,double> newP =a_neutron.getSteppedPos(a_d);
  double newx=newP.first;
  double newy=newP.second;
  for(std::string reg : m_regions)
  {
    double xLow=m_geo[reg].second.first.first;
    double xHigh=m_geo[reg].second.first.second;
    double yLow=m_geo[reg].second.second.first;
    double yHigh=m_geo[reg].second.second.second;
    if(newx>=xLow && newx<xHigh && newy>=yLow && newy<yHigh)
    {
      xlb=xLow;
      xhb=xHigh;
      ylb=yLow;
      yhb=yHigh;
      continue;
    }
  }
  if(xlb==-100)
  {
    // std::cout<<"void"<<std::endl;
    return {-100,-100};
  }
  // solve function 
  double yintLow=(newy-oldy)/(newx-oldx)*(xlb-oldx)+oldy;
  double yintHigh=(newy-oldy)/(newx-oldx)*(xhb-oldx)+oldy;
  double xintLow=(newx-oldx)/(newy-oldy)*(ylb-oldy)+oldx;
  double xintHigh=(newx-oldx)/(newy-oldy)*(yhb-oldy)+oldx;
  if(yintLow>ylb && yintLow<yhb)
  {
    // std::cout<<"left side"<<std::endl;
    return {xlb+1e-8,yintLow};
  }
  if(yintHigh>ylb && yintHigh<yhb)
  {
    // std::cout<<" right side"<<std::endl;
    return {xhb-1e-8,yintHigh};
  }
  if(xintLow>xlb && xintLow<xhb)
  {
    // std::cout<<"bottom"<<std::endl;
    return {xintLow,ylb+1e-8};
  }
  if(xintHigh>xlb && xintHigh<xhb)
  {
    // std::cout<<"top"<<std::endl;
    return {xintHigh+1e-8,yhb};
  }
  else
  {
    std::cout<<"error, probably corner??"<<std::endl;
    return {xlb,ylb};
  }
}

void materialManager::addNu(std::string a_matName,double a_nu)
{
  m_nuBar[a_matName]=a_nu;
}
void materialManager::addDensity(std::string a_matName,double a_rho, double a_A)
{
  if (a_A<0)
  {
    m_matDens[a_matName]=a_rho;
  }
  else
  {
    m_matDens[a_matName]=a_rho/a_A;
    m_As[a_matName]=a_A;
  }
}
std::string materialManager::matFinder(double a_x, double a_y)
{
  for(std::string reg : m_regions)
  {
    double xLow=m_geo[reg].second.first.first;
    double xHigh=m_geo[reg].second.first.second;
    double yLow=m_geo[reg].second.second.first;
    double yHigh=m_geo[reg].second.second.second;
    if(a_x>=xLow && a_x<xHigh && a_y>=yLow && a_y<yHigh)
    {
      return m_geo[reg].first;
    }
  }
  return "void";
}
std::string materialManager::matFinder(std::pair<double, double> a_pos)
{
  double x=a_pos.first;
  double y=a_pos.second;
  std::string outp=matFinder(x,y);
  return outp;
}
double materialManager::getCX(std::string a_matName,std::string a_type
                        ,double a_En)
{
  bool foundH=false;
  double eLow=0;
  double cxLow=0;
  double eHigh=0;
  double cxHigh=0;
  double En=a_En*1000; //covert to eV

  for(std::pair enX: m_crossX[a_matName][a_type])
  {
    if(En==enX.first)
    {
      return enX.second;
    }
    if(En<enX.first)
    {
      if(!foundH)
      {
        eHigh=enX.first;
        cxHigh=enX.second;
        foundH=true;
      }
      continue;
    }
    else if(En>enX.first)
    {
      eLow=enX.first;
      cxLow=enX.second;
    }
  }
  double efrac=(En-eLow)/(eHigh-eLow);
  return cxLow+efrac*(cxHigh-cxLow);
}
double materialManager::getCXTot(std::string a_matName,double a_En)
{
  // double totalCX=0;
  // for (std::string ty:m_matTypes[a_matName])
  // {
  //   totalCX=totalCX+getCX(a_matName,ty,a_En);
  // }
  // return totalCX;
  return getCX(a_matName,"tot",a_En);
}
double materialManager::getDist(std::string a_matName,double a_En,double a_eta)
{
  double totCX=getCXTot(a_matName,a_En)*0.6022*m_matDens[a_matName];
  return -1*std::log(1-a_eta)/totCX;
}
std::string materialManager::getReactionType(double a_eta,std::string a_matName,
                                double a_En)
{
  double totCX=getCXTot(a_matName,a_En);
  double num=0;
  for (std::string ty:m_matTypes[a_matName])
  {
    num=num+getCX(a_matName,ty,a_En);
    if(a_eta<=num/totCX)
    {
      return ty;
    }
  }
  // std::cout<<"something went wrong in reaction sample"<<std::endl;
  return "scat";
}
double materialManager::getScatEn(double a_En, double a_eta,std::string a_matName)
{
  double alpha=pow(((m_As[a_matName]-1)/(m_As[a_matName])),2);
  return alpha*a_En+a_eta*(a_En-alpha*a_En);
}
 int materialManager::getFisInfo(std::string a_matName,double a_eta)
{
  double nu=m_nuBar[a_matName];
  int nuInt =int(nu);
  int nF=0;
  std::vector<double> es;
  if(a_eta<=nu-nuInt)
  {
    nF=nuInt+1;
  }
  else
  {
    nF=nuInt;
  }
  return nF;
}
void materialManager::addShape(std::string a_reg, std::string a_mat,double a_xLow, double a_yLow,
                   double a_xHigh, double a_yHigh)
{
  m_regions.push_back(a_reg);
  std::pair<std::pair<double,double>,std::pair<double,double>> posPair=
                                    {{a_xLow,a_xHigh},{a_yLow,a_yHigh}};
  std::pair<std::string,std::pair<std::pair<double,double>,std::pair<double,double>>> outPair=
    {a_mat,posPair};
  m_geo[a_reg]=outPair;
}
std::pair<double,double> materialManager::addShapeFromFile(std::string a_fileName)
{
  std::ifstream shapeFile(a_fileName);
  std::string reg;
  std::string mat;
  double xlow;
  double ylow;
  double xhigh;
  double yhigh;
  double xHest=0;
  double yHest=0;
  std::string line;
  std::string tempWord;
  while (getline(shapeFile,line))
  {
    // std::cout<<line<<std::endl;
    std::stringstream linStream(line);
    getline(linStream,tempWord,',');
    reg=tempWord;
    getline(linStream,tempWord,',');
    mat=tempWord;
    getline(linStream,tempWord,',');
    xlow=std::stod(tempWord);
    getline(linStream,tempWord,',');
    ylow=std::stod(tempWord);
    getline(linStream,tempWord,',');
    xhigh=std::stod(tempWord);
    getline(linStream,tempWord,',');
    yhigh=std::stod(tempWord);
    std::cout<<"adding:"<<mat<<std::endl;
    addShape(reg,mat,xlow,ylow,xhigh,yhigh);
    if(xhigh>xHest)
    {
      xHest=xhigh;
    }
    if(yhigh>yHest)
    {
      yHest=yhigh;
    }
  }
  std::pair<double,double> outp={xHest,yHest};
  return outp;
}
std::map<std::string,std::map<std::string,std::vector<std::pair<double,double>>>> materialManager::getCXS()
{
  return m_crossX;
}
std::map<std::string,double> materialManager::getDens()
{
  return m_matDens;
}
std::map<std::string,double> materialManager::getNus()
{
  return m_nuBar;
}
tallies::tallies()
{
  // nothing here
}
tallies::~tallies()
{
  // nothing here
}
tallies::tallies(state a_state,materialManager* a_mats,int a_fisnum,int a_fisNor)
{
  double wieght=1.0/a_fisNor;
  // double wieght=1.0;
  // std::cout<<wieght<<std::endl;
  m_colEst=0;
  m_absEst=0;
  m_pathEst=0;

  m_num=a_state.getNumParticles();
  std::map<std::string,std::map<std::string,std::vector<std::pair<double,double>>>> crossX=a_mats->getCXS();
  std::map<std::string,double> matDens=a_mats->getDens();
  std::map<std::string,double> nuBar = a_mats->getNus();
  int escp=0;
  int fis=0;
  int cap=0;
  double en=0;
  int alive=0;
  for(neutron nu : a_state.getParticles())
  {
    int death=nu.getDeath();
    if(death>-1)
    {
      if(death==0)
      {
        escp+=1;
      }
      if(death==1)
      {
        cap+=1;
      }
      if(death==2)
      {
        fis+=1;
      }
    }
    else
    {
      en+=nu.getE();
      alive+=1;
    }
    std::vector<pair<std::string,double>> paths=nu.getPathEst();
    for (pair<std::string,double> path : paths)
        {
          std::string matP=path.first;
          if (crossX[matP].find("fis") == crossX[matP].end())
            {}
          else
          {
            double cxP=a_mats->getCX(matP,"fis",nu.getE());
            // std::cout<<"cx:"<<cxP<<" path:"<<path.second<<" wieght"<<wieght<<" nu"<<nuBar[matP]<<std::endl;
            m_pathEst+=path.second*cxP*nuBar[matP]*wieght;//*nu.getWeight();
          }
        }
    for(std::pair<std::string,double> col: nu.getCols())
    {
      std::string matP=col.first;
      if (crossX[matP].find("fis") == crossX[matP].end())
        {}
      else
      {
        double en=col.second;
        m_colEst+=wieght*nuBar[matP]*a_mats->getCX(matP,"fis",en)/(a_mats->getCX(matP,"tot",en));
      }
    }
    for(std::pair<std::string,double> abs: nu.getAbs())
    {
      std::string matP=abs.first;
      double en=abs.second;
      if (crossX[matP].find("fis") == crossX[matP].end())
        {}
      else
      {
        // std::cout<<a_mats->getCX(matP,"fis",en)<<std::endl;
        // std::cout<<a_mats->getCX(matP,"fis",en)+a_mats->getCX(matP,"abs",en)<<std::endl;
        m_absEst+=wieght*nuBar[matP]*a_mats->getCX(matP,"fis",en)/(a_mats->getCX(matP,"fis",en)+a_mats->getCX(matP,"abs",en));
        // std::cout<<m_colEst<<std::endl;
      }
    }
  }
  m_avEnergy=en/alive;
  m_avEscape=escp/double(m_num);
  m_avFis=fis/double(m_num);
  m_avCap=cap/double(m_num);
  m_avSurvive=alive/double(m_num);
}
double tallies::getAvEn()
{
  return m_avEnergy;
}
double tallies::getEscape()
{
  return m_avEscape;
}
double tallies::getFis()
{
  return m_avFis;
}
double tallies::getCap()
{
  return m_avCap;
}
double tallies::getAlive()
{
  return m_avSurvive;
}
double tallies::getColEst()
{
  return m_colEst;
}
double tallies::getAbsEst()
{
  return m_absEst;
}
double tallies::getPathEst()
{
  return m_pathEst;
}
int tallies::getNum()
{
  return m_num;
}
reducedState::reducedState()
{
  // nothing to do here
}
void reducedState::addReduced(tallies a_tally)
{
  m_reducedStates.push_back(a_tally);
}
std::vector<tallies> reducedState::getTallies()
{
  return m_reducedStates;
}
randomGen::randomGen(int a_x)
{
  m_a=104729;
  m_b=8675309;
  m_m=int(std::pow(2,32));
  m_xi=a_x;
}
randomGen::randomGen()
{
  m_a=104729;
  m_b=8675309;
  m_m=int(std::pow(2,64));
  m_xi=8675;
}
void randomGen::setSeed(int a_x)
{
  m_xi=a_x;
}
void randomGen::setPars(int a_a,int a_b,int a_m)
{
  m_a=a_a;
  m_b=a_b;
  m_m=a_m;
}
int randomGen::getRand()
{
  m_xi=(m_a*m_xi+m_b)%m_m;
  return m_xi;
}
double randomGen::getNormRand()
{
  m_xi=(m_a*m_xi+m_b)%m_m;
  double nor=double(m_xi)/double(m_m);
  return nor;
}

void reducedState::addMC(double a_k, double a_ent)
{
  m_ks.push_back(a_k);
  m_entropy.push_back(a_ent);
}
std::vector<double> reducedState::getK()
{
  return m_ks;
}
std::vector<double> reducedState::getEnt()
{
  return m_entropy;
}