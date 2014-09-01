#ifndef HgcalTupleMaker_GenParticles_h
#define HgcalTupleMaker_GenParticles_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"

class HgcalTupleMaker_GenParticles : public edm::EDProducer {
 public:
  explicit HgcalTupleMaker_GenParticles(const edm::ParameterSet&);
  
  int getMotherIndex (const reco::GenParticleCollection::const_iterator & it_part,
		      const edm::Handle<reco::GenParticleCollection> & particles,
		      int i_mother);

  int getStat3DaughterIndex (const reco::GenParticleCollection::const_iterator & it_part,
			     const edm::Handle<reco::GenParticleCollection> & particles,
			     int i_daughter);
  

  int getNumStat3Daughters ( const reco::GenParticleCollection::const_iterator & it_part );
  	


  int hasPartonDaughter ( const reco::GenParticleCollection::const_iterator & it_part );
  
    

 private:
  void produce( edm::Event &, const edm::EventSetup & );
  const edm::InputTag   inputTag;
  const std::string     prefix,suffix;
  const unsigned int    maxSize;
};

#endif
