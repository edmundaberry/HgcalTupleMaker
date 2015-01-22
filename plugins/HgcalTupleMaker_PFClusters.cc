#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFClusters.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"
#include "DataFormats/ForwardDetId/interface/ForwardSubdetector.h"

HgcalTupleMaker_PFClusters::HgcalTupleMaker_PFClusters(const edm::ParameterSet& iConfig) :
    inputTag      (iConfig.getParameter<edm::InputTag>("InputTag")),
    hgceeRecHitInputTag(iConfig.getParameter<edm::InputTag>("HGCEERecHits")),
    hgchefRecHitInputTag(iConfig.getParameter<edm::InputTag>("HGCHEFRecHits")),
    hgchebRecHitInputTag(iConfig.getParameter<edm::InputTag>("HGCHEBRecHits")),
    prefix        (iConfig.getParameter<std::string>  ("Prefix")),
    suffix        (iConfig.getParameter<std::string>  ("Suffix")) {
  produces <std::vector<double> > ( prefix + "Pt"        + suffix );
  produces <std::vector<double> > ( prefix + "Energy"    + suffix );
  produces <std::vector<double> > ( prefix + "Time"      + suffix );
  produces <std::vector<int>    > ( prefix + "Layer"     + suffix );
  produces <std::vector<double> > ( prefix + "X"         + suffix );
  produces <std::vector<double> > ( prefix + "Y"         + suffix );
  produces <std::vector<double> > ( prefix + "Z"         + suffix );
  produces <std::vector<double> > ( prefix + "R"         + suffix );
  produces <std::vector<double> > ( prefix + "Eta"       + suffix );
  produces <std::vector<double> > ( prefix + "Phi"       + suffix );
  produces <std::vector<std::vector<int> > >    (prefix + "HGCEERecHitIndex"  + suffix );
  produces <std::vector<std::vector<int> > >    (prefix + "HGCHEBRecHitIndex" + suffix );
  produces <std::vector<std::vector<int> > >    (prefix + "HGCHEFRecHitIndex" + suffix );
  produces <std::vector<std::vector<double> > > (prefix + "HGCEERecHitFraction" + suffix );
  produces <std::vector<std::vector<double> > > (prefix + "HGCHEBRecHitFraction" + suffix );
  produces <std::vector<std::vector<double> > > (prefix + "HGCHEFRecHitFraction" + suffix );
}

void HgcalTupleMaker_PFClusters::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> > pt     ( new std::vector<double> () );
  std::auto_ptr<std::vector<double> > energy ( new std::vector<double> () );
  std::auto_ptr<std::vector<double> > time   ( new std::vector<double> () );
  std::auto_ptr<std::vector<int>    > layer  ( new std::vector<int>    () );
  std::auto_ptr<std::vector<double> > X	     ( new std::vector<double> () );
  std::auto_ptr<std::vector<double> > Y	     ( new std::vector<double> () );
  std::auto_ptr<std::vector<double> > Z	     ( new std::vector<double> () );
  std::auto_ptr<std::vector<double> > R	     ( new std::vector<double> () );
  std::auto_ptr<std::vector<double> > Eta    ( new std::vector<double> () );
  std::auto_ptr<std::vector<double> > Phi    ( new std::vector<double> () ); 
  std::auto_ptr<std::vector<std::vector<int   > > > hgceeRecHitIndex    ( new std::vector<std::vector<int   > > () );
  std::auto_ptr<std::vector<std::vector<int   > > > hgchebRecHitIndex   ( new std::vector<std::vector<int   > > () );
  std::auto_ptr<std::vector<std::vector<int   > > > hgchefRecHitIndex   ( new std::vector<std::vector<int   > > () );
  std::auto_ptr<std::vector<std::vector<double> > > hgceeRecHitFraction ( new std::vector<std::vector<double> > () );
  std::auto_ptr<std::vector<std::vector<double> > > hgchefRecHitFraction( new std::vector<std::vector<double> > () );
  std::auto_ptr<std::vector<std::vector<double> > > hgchebRecHitFraction( new std::vector<std::vector<double> > () );

  edm::Handle<reco::PFClusterCollection> clusters;
  iEvent.getByLabel(inputTag, clusters);

  edm::Handle<HGCRecHitCollection> hgceeRecHits;
  iEvent.getByLabel(hgceeRecHitInputTag, hgceeRecHits);

  edm::Handle<HGCRecHitCollection> hgchefRecHits;
  iEvent.getByLabel(hgchefRecHitInputTag, hgchefRecHits);

  edm::Handle<HGCRecHitCollection> hgchebRecHits;
  iEvent.getByLabel(hgchebRecHitInputTag, hgchebRecHits);

  HGCRecHitCollection::const_iterator it_recHit;
  HGCRecHitCollection::const_iterator first_hgceeRecHit  = hgceeRecHits -> begin();  
  HGCRecHitCollection::const_iterator first_hgchebRecHit = hgchebRecHits -> begin();  
  HGCRecHitCollection::const_iterator first_hgchefRecHit = hgchefRecHits -> begin();  

  HGCRecHitCollection::const_iterator last_hgceeRecHit  = hgceeRecHits -> end();  
  HGCRecHitCollection::const_iterator last_hgchebRecHit = hgchebRecHits -> end();  
  HGCRecHitCollection::const_iterator last_hgchefRecHit = hgchefRecHits -> end();  
  
  if( clusters.isValid() ) {
    edm::LogInfo("HgcalTupleMaker_PFClustersInfo") << "Total # PFClusters: " << clusters->size();
    
    for( reco::PFClusterCollection::const_iterator it = clusters->begin(); it != clusters->end(); ++it ) {
      
      pt     -> push_back ( it -> pt    () );
      energy -> push_back ( it -> energy() );
      time   -> push_back ( it -> time  () );
      layer  -> push_back ( it -> layer () );

      const math::XYZPoint & position = it -> position();

      X      -> push_back ( position.X()   );
      Y      -> push_back ( position.Y()   );
      Z      -> push_back ( position.Z()   );
      R      -> push_back ( position.R()   );
      Eta    -> push_back ( position.Eta() );
      Phi    -> push_back ( position.Phi() );
      
      hgceeRecHitIndex     -> push_back ( std::vector<int>    () );
      hgceeRecHitFraction  -> push_back ( std::vector<double> () );
      hgchebRecHitIndex    -> push_back ( std::vector<int>    () );
      hgchebRecHitFraction -> push_back ( std::vector<double> () );
      hgchefRecHitIndex    -> push_back ( std::vector<int>    () );
      hgchefRecHitFraction -> push_back ( std::vector<double> () );
      
      size_t last_entry = hgceeRecHitIndex -> size();
      
      // Each cluster is a collection of rechits fractions
      // "fractions" because rechits can be shared between multiple clusters
      // Loop over these
	
      const std::vector<std::pair<DetId, float> >& hitsAndFractions = it -> hitsAndFractions();
      std::vector<std::pair<DetId, float> >::const_iterator it_hitAndFraction     = hitsAndFractions.begin();
      std::vector<std::pair<DetId, float> >::const_iterator it_hitAndFraction_end = hitsAndFractions.end();
      
      for (; it_hitAndFraction != it_hitAndFraction_end; ++it_hitAndFraction){
	
	const DetId  detid    = it_hitAndFraction -> first;
	const float  fraction = it_hitAndFraction -> second;
	int index;
	
	if     ( detid.det() == DetId::Forward && detid.subdetId() == HGCEE ) {
	  it_recHit = hgceeRecHits -> find ( detid );
	  if ( it_recHit == last_hgceeRecHit ) std::cout << "Can't find HGCEE rechit from " << inputTag <<  std::endl;
	  index = it_recHit - first_hgceeRecHit;
	  (*hgceeRecHitIndex   )[last_entry - 1].push_back(index);
	  (*hgceeRecHitFraction)[last_entry - 1].push_back(fraction);
	}
	
	else if ( detid.det() == DetId::Forward && detid.subdetId() == HGCHEF ) {
	  it_recHit = hgchefRecHits -> find ( detid );
	  if ( it_recHit == last_hgchefRecHit ) std::cout << "Can't find HGCHEF rechit from " << inputTag << std::endl;
	  index = it_recHit - first_hgchefRecHit;
	  (*hgchefRecHitIndex   )[last_entry - 1].push_back(index);
	  (*hgchefRecHitFraction)[last_entry - 1].push_back(fraction);
	}
      
	else if ( detid.det() == DetId::Forward && detid.subdetId() == HGCHEB ) {
	  it_recHit = hgchebRecHits -> find ( detid );
	  if ( it_recHit == last_hgchebRecHit ) std::cout << "Can't find HGCHEB rechit from " << inputTag << std::endl;
	  index = it_recHit - first_hgchebRecHit;
	  (*hgchebRecHitIndex   )[last_entry - 1].push_back(index);
	  (*hgchebRecHitFraction)[last_entry - 1].push_back(fraction);
	}
	
	else { 
	  std::cout << "Can't find rechit (tuple maker)" << std::endl;
	  std::cout << "\tCluster collection:  " << inputTag    << std::endl;
	  std::cout << "\tRecHit  subdetector: " << detid.det() << "|" << detid.subdetId() << std::endl;
	}
      }
    }
  } else {
    edm::LogError("HgcalTupleMaker_PFClustersError") << "Error! Can't get the product " << inputTag;
  }

  iEvent.put( pt     , prefix + "Pt"        + suffix );
  iEvent.put( energy , prefix + "Energy"    + suffix );
  iEvent.put( time   , prefix + "Time"      + suffix );
  iEvent.put( layer  , prefix + "Layer"     + suffix );
  iEvent.put( X	     , prefix + "X"         + suffix );
  iEvent.put( Y	     , prefix + "Y"         + suffix );
  iEvent.put( Z	     , prefix + "Z"         + suffix );
  iEvent.put( R	     , prefix + "R"         + suffix );
  iEvent.put( Eta    , prefix + "Eta"       + suffix );
  iEvent.put( Phi    , prefix + "Phi"       + suffix );
  iEvent.put( hgceeRecHitIndex    , prefix + "HGCEERecHitIndex"  + suffix );
  iEvent.put( hgchebRecHitIndex   , prefix + "HGCHEBRecHitIndex" + suffix );
  iEvent.put( hgchefRecHitIndex   , prefix + "HGCHEFRecHitIndex" + suffix );
  iEvent.put( hgceeRecHitFraction , prefix + "HGCEERecHitFraction" + suffix );
  iEvent.put( hgchefRecHitFraction, prefix + "HGCHEBRecHitFraction" + suffix );
  iEvent.put( hgchebRecHitFraction, prefix + "HGCHEFRecHitFraction" + suffix );

}
