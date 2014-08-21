#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_HgcalRecHits.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"

HgcalTupleMaker_HgcalRecHits::HgcalTupleMaker_HgcalRecHits(const edm::ParameterSet& iConfig) :
    inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
    prefix  (iConfig.getParameter<std::string>  ("Prefix")),
    suffix  (iConfig.getParameter<std::string>  ("Suffix"))
{
  produces <std::vector<double> > ( prefix + "Energy" + suffix );
  produces <std::vector<double> > ( prefix + "Time"   + suffix );
}

void HgcalTupleMaker_HgcalRecHits::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> > energy ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> > time   ( new std::vector<double>()  );

  edm::Handle<HGCRecHitCollection> rechits;
  iEvent.getByLabel(inputTag, rechits);
  
  if( rechits.isValid() ) {
    edm::LogInfo("HgcalTupleMaker_HgcalRecHitsInfo") << "Total # HgcalRecHits: " << rechits->size();
    
    for( HGCRecHitCollection::const_iterator it = rechits->begin(); it != rechits->end(); ++it ) {
      
      energy -> push_back( it -> energy () );
      time   -> push_back( it -> time   () );
      
    }
  } else {
    edm::LogError("HgcalTupleMaker_HgcalRecHitsError") << "Error! Can't get the product " << inputTag;
  }

  iEvent.put( energy, prefix + "Energy" + suffix );
  iEvent.put( energy, prefix + "Time"   + suffix );

}

