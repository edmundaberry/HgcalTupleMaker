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
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"

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
  produces <std::vector<std::vector<int> > >   (prefix + "HGCEERecHitIndex"     + suffix );
  produces <std::vector<std::vector<int> > >   (prefix + "HGCHEBRecHitIndex"    + suffix );
  produces <std::vector<std::vector<int> > >   (prefix + "HGCHEFRecHitIndex"    + suffix );
  produces <std::vector<std::vector<double> > >(prefix + "HGCEERecHitFraction"  + suffix );
  produces <std::vector<std::vector<double> > >(prefix + "HGCHEBRecHitFraction" + suffix );
  produces <std::vector<std::vector<double> > >(prefix + "HGCHEFRecHitFraction" + suffix );
}

void HgcalTupleMaker_PFCands::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> >               pt                   ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               eta                  ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               phi                  ( new std::vector<double>() );
  std::auto_ptr<std::vector<int   > >               id                   ( new std::vector<int>   () );
  std::auto_ptr<std::vector<std::vector<int> > >    hgceeRecHitIndex     ( new std::vector<std::vector<int> > () );
  std::auto_ptr<std::vector<std::vector<int> > >    hgchebRecHitIndex    ( new std::vector<std::vector<int> > () );
  std::auto_ptr<std::vector<std::vector<int> > >    hgchefRecHitIndex    ( new std::vector<std::vector<int> > () );
  std::auto_ptr<std::vector<std::vector<double> > > hgceeRecHitFraction  ( new std::vector<std::vector<double> > () );
  std::auto_ptr<std::vector<std::vector<double> > > hgchebRecHitFraction ( new std::vector<std::vector<double> > () );
  std::auto_ptr<std::vector<std::vector<double> > > hgchefRecHitFraction ( new std::vector<std::vector<double> > () );
  
  edm::Handle<reco::PFCandidateCollection> pfCands;
  iEvent.getByLabel(inputTag, pfCands);
  
  edm::Handle<HGCRecHitCollection>  hgceeRecHits;
  iEvent.getByLabel(hgceeInputTag,  hgceeRecHits);
				    
  edm::Handle<HGCRecHitCollection>  hgchefRecHits;
  iEvent.getByLabel(hgchefInputTag, hgchefRecHits);
				    
  edm::Handle<HGCRecHitCollection>  hgchebRecHits;
  iEvent.getByLabel(hgchebInputTag, hgchebRecHits);
  
  HGCRecHitCollection::const_iterator it_recHit;
  HGCRecHitCollection::const_iterator first_HGCEERecHit  = hgceeRecHits  -> begin();
  HGCRecHitCollection::const_iterator first_HGCHEBRecHit = hgchebRecHits -> begin();
  HGCRecHitCollection::const_iterator first_HGCHEFRecHit = hgchefRecHits -> begin();
  
  if( pfCands.isValid() ) {
    edm::LogInfo("HgcalTupleMaker_PFCandsInfo") << "Total # PFCands: " << pfCands->size();
    
    for( reco::PFCandidateCollection::const_iterator it = pfCands->begin(); it != pfCands->end(); ++it ) {

      pt  -> push_back( it -> pt        () );
      eta -> push_back( it -> eta       () );
      phi -> push_back( it -> phi       () );
      id  -> push_back( it -> particleId() );

      hgceeRecHitIndex     -> push_back ( std::vector<int>    () );
      hgchebRecHitIndex    -> push_back ( std::vector<int>    () );
      hgchefRecHitIndex    -> push_back ( std::vector<int>    () );
      hgceeRecHitFraction  -> push_back ( std::vector<double> () );
      hgchebRecHitFraction -> push_back ( std::vector<double> () );
      hgchefRecHitFraction -> push_back ( std::vector<double> () );

      size_t last_entry = hgchefRecHitFraction -> size();

      // Get RecHits from HGCAL
      
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

	// Each cluster is a collection of rechits fractions
	// "fractions" because rechits can be shared between multiple clusters
	// Loop over these
	
	const std::vector<reco::PFRecHitFraction>& recHitFractions = elementCluster -> clusterRef() -> recHitFractions();
	std::vector<reco::PFRecHitFraction>::const_iterator it_recHitFraction     = recHitFractions.begin();
	std::vector<reco::PFRecHitFraction>::const_iterator it_recHitFraction_end = recHitFractions.end();

	for (; it_recHitFraction != it_recHitFraction_end; ++it_recHitFraction){
	  
	  // This rechit might be null!
	  if (it_recHitFraction -> recHitRef().isNull()) continue;
	  
	  const unsigned detid    = it_recHitFraction -> recHitRef() -> detId();
	  const double   fraction = it_recHitFraction -> fraction();

	  if       ( elementType == reco::PFBlockElement::HGC_ECAL ){ 
	    it_recHit = hgceeRecHits -> find ( detid );
	    int index = it_recHit - first_HGCEERecHit;
	    (*hgceeRecHitIndex    )[last_entry - 1].push_back (index);
	    (*hgceeRecHitFraction )[last_entry - 1].push_back (fraction);
	  }
	  else if  ( elementType == reco::PFBlockElement::HGC_HCALF ){ 
	    it_recHit = hgchefRecHits -> find ( detid );
	    int index = it_recHit - first_HGCHEFRecHit;
	    (*hgchefRecHitIndex   )[last_entry - 1].push_back (index);
	    (*hgchefRecHitFraction)[last_entry - 1].push_back (fraction);
	  }
	  else if  ( elementType == reco::PFBlockElement::HGC_HCALB ){ 
	    it_recHit = hgchebRecHits -> find ( detid );
	    int index = it_recHit - first_HGCHEBRecHit;
	    (*hgchebRecHitIndex   )[last_entry - 1].push_back (index);
	    (*hgchebRecHitFraction)[last_entry - 1].push_back (fraction);
	  }
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
  iEvent.put( hgceeRecHitIndex    , prefix + "HGCEERecHitIndex"     + suffix );
  iEvent.put( hgchebRecHitIndex   , prefix + "HGCHEBRecHitIndex"    + suffix );
  iEvent.put( hgchefRecHitIndex   , prefix + "HGCHEFRecHitIndex"    + suffix );
  iEvent.put( hgceeRecHitFraction , prefix + "HGCEERecHitFraction"  + suffix );
  iEvent.put( hgchebRecHitFraction, prefix + "HGCHEBRecHitFraction" + suffix );
  iEvent.put( hgchefRecHitFraction, prefix + "HGCHEFRecHitFraction" + suffix );

}
