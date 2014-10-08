#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFCands.h"
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

HgcalTupleMaker_PFCands::HgcalTupleMaker_PFCands(const edm::ParameterSet& iConfig) :
    inputTag      (iConfig.getParameter<edm::InputTag>("InputTag")),
    hgceeInputTag (iConfig.getParameter<edm::InputTag>("hgceeInputTag")),
    hgchefInputTag(iConfig.getParameter<edm::InputTag>("hgchefInputTag")),
    hgchebInputTag(iConfig.getParameter<edm::InputTag>("hgchebInputTag")),
    prefix        (iConfig.getParameter<std::string>  ("Prefix")),
    suffix        (iConfig.getParameter<std::string>  ("Suffix")) {
  produces <std::vector<double> >              (prefix + "Pt"                   + suffix );
  produces <std::vector<double> >              (prefix + "Eta"                  + suffix );
  produces <std::vector<double> >              (prefix + "Phi"                  + suffix );
  produces <std::vector<int   > >              (prefix + "Id"                   + suffix );
  produces <std::vector<std::vector<int> > >   (prefix + "HGCEEClusterIndex"    + suffix );
  produces <std::vector<std::vector<int> > >   (prefix + "HGCHEBClusterIndex"   + suffix );
  produces <std::vector<std::vector<int> > >   (prefix + "HGCHEFClusterIndex"   + suffix );
}

void HgcalTupleMaker_PFCands::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> >            pt                 ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >            eta                ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >            phi                ( new std::vector<double>() );
  std::auto_ptr<std::vector<int   > >            id                 ( new std::vector<int>   () );
  std::auto_ptr<std::vector<std::vector<int> > > hgceeClusterIndex  ( new std::vector<std::vector<int> > () );
  std::auto_ptr<std::vector<std::vector<int> > > hgchebClusterIndex ( new std::vector<std::vector<int> > () );
  std::auto_ptr<std::vector<std::vector<int> > > hgchefClusterIndex ( new std::vector<std::vector<int> > () );
  
  edm::Handle<reco::PFCandidateCollection> pfCands;
  iEvent.getByLabel(inputTag, pfCands);

  edm::Handle<reco::PFClusterCollection> hgceeClusters;
  iEvent.getByLabel(hgceeInputTag, hgceeClusters);
  
  edm::Handle<reco::PFClusterCollection> hgchefClusters;
  iEvent.getByLabel(hgchefInputTag, hgchefClusters);
  
  edm::Handle<reco::PFClusterCollection> hgchebClusters;
  iEvent.getByLabel(hgchebInputTag, hgchebClusters);

  reco::PFClusterCollection::const_iterator it_cluster;
  reco::PFClusterCollection::const_iterator first_HGCEECluster  = hgceeClusters  -> begin();
  reco::PFClusterCollection::const_iterator first_HGCHEBCluster = hgchebClusters -> begin();
  reco::PFClusterCollection::const_iterator first_HGCHEFCluster = hgchefClusters -> begin();
  reco::PFClusterCollection::const_iterator last_HGCEECluster   = hgceeClusters  -> end();
  reco::PFClusterCollection::const_iterator last_HGCHEBCluster  = hgchebClusters -> end();
  reco::PFClusterCollection::const_iterator last_HGCHEFCluster  = hgchefClusters -> end();
  
  if( pfCands.isValid() ) {
    edm::LogInfo("HgcalTupleMaker_PFCandsInfo") << "Total # PFCands: " << pfCands->size();
    
    for( reco::PFCandidateCollection::const_iterator it = pfCands->begin(); it != pfCands->end(); ++it ) {
      
      pt  -> push_back( it -> pt        () );
      eta -> push_back( it -> eta       () );
      phi -> push_back( it -> phi       () );
      id  -> push_back( it -> particleId() );

      hgceeClusterIndex  -> push_back ( std::vector<int> () );
      hgchebClusterIndex -> push_back ( std::vector<int> () );
      hgchefClusterIndex -> push_back ( std::vector<int> () );

      size_t last_entry = hgchefClusterIndex -> size();

      // First get all of the elements in all of the blocks in this PFCandidate
      
      const reco::PFCandidate::ElementsInBlocks & elementsInBlocks = it -> elementsInBlocks();
      reco::PFCandidate::ElementsInBlocks::const_iterator it_elementInBlock     = elementsInBlocks.begin();
      reco::PFCandidate::ElementsInBlocks::const_iterator it_elementInBlock_end = elementsInBlocks.end  ();
      
      // Loop over elements

      for (; it_elementInBlock != it_elementInBlock_end; ++it_elementInBlock){
	reco::PFBlockRef blockRef = it_elementInBlock -> first;
	unsigned element_index = it_elementInBlock -> second;
	const reco::PFBlockElement * element = & blockRef -> elements()[element_index];
	
	reco::PFBlockElement::Type elementType = element -> type();
	
	// If this element isn't from the HGCAL, skip it

	if ( elementType != reco::PFBlockElement::HGC_ECAL  && 
	     elementType != reco::PFBlockElement::HGC_HCALF && 
	     elementType != reco::PFBlockElement::HGC_HCALB ) continue;

	// Calorimeter PF elements are element clusters

	const reco::PFBlockElementCluster* elementCluster = dynamic_cast<const reco::PFBlockElementCluster *>( element );

	// Get the reference to the cluster

	const reco::PFClusterRef& clusterRef = elementCluster -> clusterRef();

	// Find where the cluster is in the list

	if ( elementType == reco::PFBlockElement::HGC_ECAL ){
	  it_cluster = std::find(first_HGCEECluster, last_HGCEECluster, *clusterRef);
	  int index = it_cluster == last_HGCEECluster ? -1 : it_cluster - first_HGCEECluster;
	  (*hgceeClusterIndex)[last_entry - 1].push_back(index);
	}
	
	else if ( elementType == reco::PFBlockElement::HGC_HCALF ){
	  it_cluster = std::find(first_HGCHEFCluster, last_HGCHEFCluster, *clusterRef);
	  int index = it_cluster == last_HGCHEFCluster ? -1 : it_cluster - first_HGCHEFCluster;
	  (*hgchefClusterIndex)[last_entry - 1].push_back(index);
	}

	else if ( elementType == reco::PFBlockElement::HGC_HCALB ){
	  it_cluster = std::find(first_HGCHEBCluster, last_HGCHEBCluster, *clusterRef);
	  int index = it_cluster == last_HGCHEBCluster ? -1 : it_cluster - first_HGCHEBCluster;
	  (*hgchebClusterIndex)[last_entry - 1].push_back(index);
	}
      }
    }
  } else {
    edm::LogError("HgcalTupleMaker_PFCandsError") << "Error! Can't get the product " << inputTag;
  }
  
  iEvent.put( pt                  , prefix + "Pt"                   + suffix );
  iEvent.put( eta                 , prefix + "Eta"                  + suffix );
  iEvent.put( phi                 , prefix + "Phi"                  + suffix );
  iEvent.put( id                  , prefix + "Id"                   + suffix );
  iEvent.put( hgceeClusterIndex   , prefix + "HGCEEClusterIndex"     + suffix );
  iEvent.put( hgchebClusterIndex  , prefix + "HGCHEBClusterIndex"    + suffix );
  iEvent.put( hgchefClusterIndex  , prefix + "HGCHEFClusterIndex"    + suffix );

}
