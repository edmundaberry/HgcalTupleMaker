#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_GenParticles.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "TMath.h"

#include <algorithm>

int HgcalTupleMaker_GenParticles::hasPartonDaughter ( const reco::GenParticleCollection::const_iterator & it_part ){
  
  reco::GenParticleRefVector daughters = it_part -> daughterRefVector();
  reco::GenParticleRefVector::const_iterator it_daught     = daughters.begin();
  reco::GenParticleRefVector::const_iterator it_daught_end = daughters.end  ();

  bool hasPartDaughter = false;
  for(; it_daught != it_daught_end; ++it_daught){
    int abs_daughter_pdg = abs(it_daught -> get() -> pdgId());
    
    if ( abs_daughter_pdg <= 6 || 
	 abs_daughter_pdg == 9 || 
	 abs_daughter_pdg == 21 ) { 
      hasPartDaughter = true;
      break;
    }
  }
  
  return int ( hasPartDaughter );
}

int HgcalTupleMaker_GenParticles::getMotherIndex (const reco::GenParticleCollection::const_iterator & it_part,
						  const edm::Handle<reco::GenParticleCollection> & particles,
						  int i_mother){
  
  const reco::Candidate* mother = it_part -> mother(i_mother);

  reco::GenParticleCollection::const_iterator it_motherPart     = particles -> begin();
  reco::GenParticleCollection::const_iterator it_motherPart_end = particles -> end();
  
  int mother_index = -1;
  for (; it_motherPart != it_motherPart_end; ++it_motherPart ){
    if (mother == &(*it_motherPart)){
      mother_index = std::distance(particles->begin(), it_motherPart);
      break;
    }
  }
  return mother_index;
  
}

int HgcalTupleMaker_GenParticles::getNumStat3Daughters ( const reco::GenParticleCollection::const_iterator & it_part ){
  
  int retval = 0;
  int numDaughters = it_part -> numberOfDaughters ();
  for (int i = 0; i < numDaughters; ++i){
    int daughter_status = it_part -> daughter (i) -> status();
    if ( daughter_status == 3 ) retval++;
  }
  return retval;
}
		
int HgcalTupleMaker_GenParticles::getStat3DaughterIndex (const reco::GenParticleCollection::const_iterator & it_part,
							 const edm::Handle<reco::GenParticleCollection> & particles,
							 int i_daughter){
  
  const reco::Candidate* daughter = it_part -> daughter(i_daughter);
  int daughter_status = daughter -> status();

  if ( daughter_status != 3 ) return -1;

  reco::GenParticleCollection::const_iterator it_daughterPart     = particles -> begin();
  reco::GenParticleCollection::const_iterator it_daughterPart_end = particles -> end();
  
  int daughter_index = -1;
  for (; it_daughterPart != it_daughterPart_end; ++it_daughterPart ){
    if (daughter == &(*it_daughterPart)){
      daughter_index = std::distance(particles->begin(), it_daughterPart);
      break;
    }
  }
  return daughter_index;
  
}
		

HgcalTupleMaker_GenParticles::HgcalTupleMaker_GenParticles(const edm::ParameterSet& iConfig) :
    inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
    prefix  (iConfig.getParameter<std::string>  ("Prefix")),
    suffix  (iConfig.getParameter<std::string>  ("Suffix")),
    maxSize (iConfig.getParameter<unsigned int> ("MaxSize"))
{
  produces <std::vector<int> >    ( prefix + "Status"            + suffix );
  produces <std::vector<double> > ( prefix + "Eta"               + suffix );
  produces <std::vector<double> > ( prefix + "Phi"               + suffix );
  produces <std::vector<double> > ( prefix + "P"                 + suffix );
  produces <std::vector<double> > ( prefix + "Px"                + suffix );
  produces <std::vector<double> > ( prefix + "Py"                + suffix );
  produces <std::vector<double> > ( prefix + "Pz"                + suffix );
  produces <std::vector<double> > ( prefix + "Pt"                + suffix );
  produces <std::vector<double> > ( prefix + "Energy"            + suffix );
  produces <std::vector<int> >    ( prefix + "PdgId"             + suffix );
  produces <std::vector<double> > ( prefix + "VX"                + suffix );
  produces <std::vector<double> > ( prefix + "VY"                + suffix );
  produces <std::vector<double> > ( prefix + "VZ"                + suffix );
  produces <std::vector<int> >    ( prefix + "NumDaught"         + suffix );
  produces <std::vector<int> >    ( prefix + "NumDaught3"        + suffix );
  produces <std::vector<int> >    ( prefix + "NumMother"         + suffix );
  produces <std::vector<int> >    ( prefix + "Mother1Index"      + suffix );
  produces <std::vector<int> >    ( prefix + "Mother2Index"      + suffix );
  produces <std::vector<int> >    ( prefix + "HasPartonDaughter" + suffix );
  
}

void HgcalTupleMaker_GenParticles::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> >  eta             ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  phi             ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  p               ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  px              ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  py              ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  pz              ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  pt              ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  energy          ( new std::vector<double>()  );
  std::auto_ptr<std::vector<int> >     pdgId           ( new std::vector<int>   ()  );
  std::auto_ptr<std::vector<double> >  vx              ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  vy              ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  vz              ( new std::vector<double>()  );
  std::auto_ptr<std::vector<int> >     numDaught       ( new std::vector<int>   ()  );
  std::auto_ptr<std::vector<int> >     numDaught3      ( new std::vector<int>   ()  );
  std::auto_ptr<std::vector<int> >     numMother       ( new std::vector<int>   ()  );
  std::auto_ptr<std::vector<int> >     status          ( new std::vector<int>   ()  );
  std::auto_ptr<std::vector<int> >     mother1Index    ( new std::vector<int>   ()  );
  std::auto_ptr<std::vector<int> >     mother2Index    ( new std::vector<int>   ()  );
  std::auto_ptr<std::vector<int> >     hasPartDaughter ( new std::vector<int>   ()  );
  
  if( !iEvent.isRealData() ) {
    edm::Handle<reco::GenParticleCollection> genParticles;
    iEvent.getByLabel(inputTag, genParticles);

    reco::GenParticleCollection::const_iterator it        = genParticles -> begin(); 
    reco::GenParticleCollection::const_iterator it_mother = genParticles -> begin();
    reco::GenParticleCollection::const_iterator it_end    = genParticles -> end();
    
    if( genParticles.isValid() ) {
      edm::LogInfo("HgcalTupleMaker_GenParticlesInfo") << "Total # GenParticles: " << genParticles->size();

      for( reco::GenParticleCollection::const_iterator it = genParticles->begin(); it != genParticles->end(); ++it ) {
	
        // if(eta->size() >= maxSize) break;

	int max = std::max ( int(it -> numberOfMothers()), 2);
	int motherIndex[2] = { -999, -999 };
	
	for ( int i_mother = 0; i_mother <= max; ++i_mother )
	  motherIndex[i_mother] = getMotherIndex ( it, genParticles, i_mother );
	
        status          -> push_back ( it -> status            () );
        eta             -> push_back ( it -> eta               () );
        phi             -> push_back ( it -> phi               () );
        p               -> push_back ( it -> p                 () );
        px              -> push_back ( it -> px                () );
        py              -> push_back ( it -> py                () );
        pz              -> push_back ( it -> pz                () );
        pt              -> push_back ( it -> pt                () );
        energy          -> push_back ( it -> energy            () );
        pdgId           -> push_back ( it -> pdgId             () );
        vx              -> push_back ( it -> vx                () );
        vy              -> push_back ( it -> vy                () );
        vz              -> push_back ( it -> vz                () );
        numDaught       -> push_back ( it -> numberOfDaughters () );
	numDaught3      -> push_back ( getNumStat3Daughters (it)  );
	numMother       -> push_back ( it -> numberOfMothers   () );
        mother1Index    -> push_back ( motherIndex[0] );
        mother2Index    -> push_back ( motherIndex[1] );
	hasPartDaughter -> push_back ( hasPartonDaughter ( it ) );
      }
    } else {
      edm::LogError("HgcalTupleMaker_GenParticlesError") << "Error! Can't get the product " << inputTag;
    }
  }
  
  iEvent.put( status,          prefix + "Status"            + suffix );
  iEvent.put( eta,             prefix + "Eta"               + suffix );
  iEvent.put( phi,             prefix + "Phi"               + suffix );
  iEvent.put( p,               prefix + "P"                 + suffix );
  iEvent.put( px,              prefix + "Px"                + suffix );
  iEvent.put( py,              prefix + "Py"                + suffix );
  iEvent.put( pz,              prefix + "Pz"                + suffix );
  iEvent.put( pt,              prefix + "Pt"                + suffix );
  iEvent.put( energy,          prefix + "Energy"            + suffix );
  iEvent.put( pdgId,           prefix + "PdgId"             + suffix );
  iEvent.put( vx,              prefix + "VX"                + suffix );
  iEvent.put( vy,              prefix + "VY"                + suffix );
  iEvent.put( vz,              prefix + "VZ"                + suffix );
  iEvent.put( numDaught,       prefix + "NumDaught"         + suffix );
  iEvent.put( numDaught3,      prefix + "NumDaught3"        + suffix );
  iEvent.put( numMother,       prefix + "NumMother"         + suffix );
  iEvent.put( mother1Index,    prefix + "Mother1Index"      + suffix );
  iEvent.put( mother2Index,    prefix + "Mother2Index"      + suffix );
  iEvent.put( hasPartDaughter, prefix + "HasPartonDaughter" + suffix );
}

