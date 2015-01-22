#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_HGCRecHitProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"
#include "DataFormats/ForwardDetId/interface/ForwardSubdetector.h"

HgcalTupleMaker_HGCRecHitProducer::HgcalTupleMaker_HGCRecHitProducer(const edm::ParameterSet& iConfig) :
  rawInputTag         (iConfig.getParameter<edm::InputTag>("RawClusterInputTag")),
  hgceeRecHitInputTag (iConfig.getParameter<edm::InputTag>("HGCEERecHitInputTag")),
  hgchebRecHitInputTag(iConfig.getParameter<edm::InputTag>("HGCHEBRecHitInputTag")),
  hgchefRecHitInputTag(iConfig.getParameter<edm::InputTag>("HGCHEFRecHitInputTag"))
{
  produces<HGCRecHitCollection>("HGCEERecHits" );
  produces<HGCRecHitCollection>("HGCHEBRecHits");
  produces<HGCRecHitCollection>("HGCHEFRecHits");
}


void HgcalTupleMaker_HGCRecHitProducer::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<reco::PFClusterCollection> clusters;
  iEvent.getByLabel(rawInputTag, clusters);
  reco::PFClusterCollection::const_iterator it_cluster     = clusters -> begin();
  reco::PFClusterCollection::const_iterator it_cluster_end = clusters -> end  ();
  
  edm::Handle<HGCRecHitCollection> inputHGCEERecHits;
  iEvent.getByLabel(hgceeRecHitInputTag, inputHGCEERecHits);

  edm::Handle<HGCRecHitCollection> inputHGCHEFRecHits;
  iEvent.getByLabel(hgchefRecHitInputTag, inputHGCHEFRecHits);

  edm::Handle<HGCRecHitCollection> inputHGCHEBRecHits;
  iEvent.getByLabel(hgchebRecHitInputTag, inputHGCHEBRecHits);

  HGCRecHitCollection::const_iterator it_recHit;
  
  std::auto_ptr<HGCRecHitCollection> outputHGCEERecHits (new HGCRecHitCollection());
  std::auto_ptr<HGCRecHitCollection> outputHGCHEFRecHits(new HGCRecHitCollection());
  std::auto_ptr<HGCRecHitCollection> outputHGCHEBRecHits(new HGCRecHitCollection());
  
  for (; it_cluster != it_cluster_end; ++it_cluster){
    
    const std::vector<std::pair<DetId, float> >& hitsAndFractions = it_cluster -> hitsAndFractions();
    std::vector<std::pair<DetId, float> >::const_iterator it_hitAndFraction     = hitsAndFractions.begin();
    std::vector<std::pair<DetId, float> >::const_iterator it_hitAndFraction_end = hitsAndFractions.end();
    
    for (; it_hitAndFraction != it_hitAndFraction_end; ++it_hitAndFraction){

      const DetId detid = it_hitAndFraction -> first;

      if     ( detid.det() == DetId::Forward && detid.subdetId() == HGCEE ) {
	it_recHit = inputHGCEERecHits -> find ( detid );
	if ( it_recHit != inputHGCEERecHits -> end() ) outputHGCEERecHits -> push_back ( *it_recHit );
	else { std::cout << "Can't find rechit..." << std::endl; }
      }

      else if ( detid.det() == DetId::Forward && detid.subdetId() == HGCHEF ) {
	it_recHit = inputHGCHEFRecHits -> find ( detid );
	if ( it_recHit != inputHGCHEFRecHits -> end() ) outputHGCHEFRecHits -> push_back ( *it_recHit );
	else { std::cout << "Can't find rechit..." << std::endl; }
      }
      
      else if ( detid.det() == DetId::Forward && detid.subdetId() == HGCHEB ) {
	it_recHit = inputHGCHEBRecHits -> find ( detid );
	if ( it_recHit != inputHGCHEBRecHits -> end() ) outputHGCHEBRecHits -> push_back ( *it_recHit );
	else { std::cout << "Can't find rechit..." << std::endl; } 
      }

      else { 
	std::cout << "Can't find rechit" << std::endl;
	std::cout << "\tCluster collection:  " << rawInputTag    << std::endl;
	std::cout << "\tRecHit  subdetector: " << detid.det() << "|" << detid.subdetId() << std::endl;
      }
    }	
  }

  iEvent.put ( outputHGCEERecHits , "HGCEERecHits"  );
  iEvent.put ( outputHGCHEBRecHits, "HGCHEBRecHits" );
  iEvent.put ( outputHGCHEFRecHits, "HGCHEFRecHits" );

}
