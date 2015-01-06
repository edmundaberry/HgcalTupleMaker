#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFClusters.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"

HgcalTupleMaker_PFClusters::HgcalTupleMaker_PFClusters(const edm::ParameterSet& iConfig) :
    inputTag      (iConfig.getParameter<edm::InputTag>("InputTag")),
    recHitInputTag(iConfig.getParameter<edm::InputTag>("RecHits")),
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
  produces <std::vector<std::vector<int> > > (prefix + "RecHitIndex" + suffix );
  produces <std::vector<std::vector<double> > > (prefix + "RecHitFraction" + suffix );
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
  std::auto_ptr<std::vector<std::vector<int   > > > recHitIndex    ( new std::vector<std::vector<int   > > () );
  std::auto_ptr<std::vector<std::vector<double> > > recHitFraction ( new std::vector<std::vector<double> > () );

  edm::Handle<reco::PFClusterCollection> clusters;
  iEvent.getByLabel(inputTag, clusters);

  edm::Handle<HGCRecHitCollection> recHits;
  iEvent.getByLabel(recHitInputTag, recHits);

  HGCRecHitCollection::const_iterator it_recHit;
  HGCRecHitCollection::const_iterator first_recHit = recHits -> begin();  

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
      
      recHitIndex    -> push_back ( std::vector<int>    () );
      recHitFraction -> push_back ( std::vector<double> () );
      
      size_t last_entry = recHitIndex -> size();
      
      // Each cluster is a collection of rechits fractions
      // "fractions" because rechits can be shared between multiple clusters
      // Loop over these
	
      const std::vector<std::pair<DetId, float> >& hitsAndFractions = it -> hitsAndFractions();
      std::vector<std::pair<DetId, float> >::const_iterator it_hitAndFraction     = hitsAndFractions.begin();
      std::vector<std::pair<DetId, float> >::const_iterator it_hitAndFraction_end = hitsAndFractions.end();
      
      for (; it_hitAndFraction != it_hitAndFraction_end; ++it_hitAndFraction){
	
	const DetId  detid    = it_hitAndFraction -> first;
	const float  fraction = it_hitAndFraction -> second;
	
	it_recHit = recHits -> find ( detid );
	int index = it_recHit - first_recHit;
	(*recHitIndex    )[last_entry - 1].push_back (index);
	(*recHitFraction )[last_entry - 1].push_back (fraction);
	
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
  iEvent.put( recHitIndex, prefix + "RecHitIndex" + suffix );
  iEvent.put( recHitFraction, prefix + "RecHitFraction" + suffix );

}
