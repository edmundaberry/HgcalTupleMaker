#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_HGCRecHitProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"

HgcalTupleMaker_HGCRecHitProducer::HgcalTupleMaker_HGCRecHitProducer(const edm::ParameterSet& iConfig) :
  rawInputTag   (iConfig.getParameter<edm::InputTag>("RawClusterInputTag")),
  recHitInputTag(iConfig.getParameter<edm::InputTag>("RecHitInputTag"))
{
  produces<HGCRecHitCollection>();
}


void HgcalTupleMaker_HGCRecHitProducer::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<reco::PFClusterCollection> clusters;
  iEvent.getByLabel(rawInputTag, clusters);
  reco::PFClusterCollection::const_iterator it_cluster     = clusters -> begin();
  reco::PFClusterCollection::const_iterator it_cluster_end = clusters -> end  ();
  
  edm::Handle<HGCRecHitCollection> inputRecHits;
  iEvent.getByLabel(recHitInputTag, inputRecHits);

  HGCRecHitCollection::const_iterator it_recHit;

  std::auto_ptr<HGCRecHitCollection> outputRecHits(new HGCRecHitCollection());
  
  for (; it_cluster != it_cluster_end; ++it_cluster){
    
    const std::vector<std::pair<DetId, float> >& hitsAndFractions = it_cluster -> hitsAndFractions();
    std::vector<std::pair<DetId, float> >::const_iterator it_hitAndFraction     = hitsAndFractions.begin();
    std::vector<std::pair<DetId, float> >::const_iterator it_hitAndFraction_end = hitsAndFractions.end();
    
    for (; it_hitAndFraction != it_hitAndFraction_end; ++it_hitAndFraction){
      const DetId  detid    = it_hitAndFraction -> first;
      it_recHit = inputRecHits -> find ( detid );
      outputRecHits -> push_back ( * it_recHit );
    }
	
  }

  iEvent.put ( outputRecHits );

}
