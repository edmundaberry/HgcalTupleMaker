#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_HGCClusterProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElement.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlockElementCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFRecHitFraction.h"

HgcalTupleMaker_HGCClusterProducer::HgcalTupleMaker_HGCClusterProducer(const edm::ParameterSet& iConfig) :
  rawInputTag (iConfig.getParameter<edm::InputTag>("RawCandInputTag"))
{
  produces<reco::PFClusterCollection>("HGCEEClusters");
  produces<reco::PFClusterCollection>("HGCHEFClusters");
  produces<reco::PFClusterCollection>("HGCHEBClusters");
}


void HgcalTupleMaker_HGCClusterProducer::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<reco::PFCandidateCollection> pfCands;
  iEvent.getByLabel(rawInputTag, pfCands);
  reco::PFCandidateCollection::const_iterator it_pfCand     = pfCands -> begin();
  reco::PFCandidateCollection::const_iterator it_pfCand_end = pfCands -> end();
  
  std::auto_ptr<reco::PFClusterCollection> outputHGCEEPFClusters (new reco::PFClusterCollection()); 
  std::auto_ptr<reco::PFClusterCollection> outputHGCHEFPFClusters(new reco::PFClusterCollection()); 
  std::auto_ptr<reco::PFClusterCollection> outputHGCHEBPFClusters(new reco::PFClusterCollection()); 

  for (; it_pfCand != it_pfCand_end; ++it_pfCand){
    
    const reco::PFCandidate::ElementsInBlocks & elementsInBlocks = it_pfCand -> elementsInBlocks();
    reco::PFCandidate::ElementsInBlocks::const_iterator it_elementInBlock     = elementsInBlocks.begin();
    reco::PFCandidate::ElementsInBlocks::const_iterator it_elementInBlock_end = elementsInBlocks.end  ();
    

    for (; it_elementInBlock != it_elementInBlock_end; ++it_elementInBlock){
      reco::PFBlockRef blockRef = it_elementInBlock -> first;
      unsigned element_index = it_elementInBlock -> second;
      const reco::PFBlockElement * element = & blockRef -> elements()[element_index];
      
      reco::PFBlockElement::Type elementType = element -> type();
      
      if ( elementType != reco::PFBlockElement::HGC_ECAL  && 
	   elementType != reco::PFBlockElement::HGC_HCALF && 
	   elementType != reco::PFBlockElement::HGC_HCALB ) continue;
      
      const reco::PFBlockElementCluster* elementCluster = dynamic_cast<const reco::PFBlockElementCluster *>( element );
      const reco::PFClusterRef& clusterRef = elementCluster -> clusterRef();
      
      if      ( elementType != reco::PFBlockElement::HGC_ECAL  ) outputHGCEEPFClusters  -> push_back ( * clusterRef );
      else if ( elementType != reco::PFBlockElement::HGC_HCALF ) outputHGCHEFPFClusters -> push_back ( * clusterRef );
      else if ( elementType != reco::PFBlockElement::HGC_HCALB ) outputHGCHEBPFClusters -> push_back ( * clusterRef );
      
    }
  }

  iEvent.put ( outputHGCEEPFClusters , "HGCEEClusters");
  iEvent.put ( outputHGCHEFPFClusters, "HGCHEFClusters");
  iEvent.put ( outputHGCHEBPFClusters, "HGCHEBClusters");

}
