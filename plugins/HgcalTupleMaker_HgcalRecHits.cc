#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_HgcalRecHits.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/HGCRecHit/interface/HGCRecHitCollections.h"
#include "Geometry/FCalGeometry/interface/HGCalGeometry.h"

HgcalTupleMaker_HgcalRecHits::HgcalTupleMaker_HgcalRecHits(const edm::ParameterSet& iConfig) :
    inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
    geoTag  (iConfig.getParameter<std::string>  ("GeometryTag")),
    prefix  (iConfig.getParameter<std::string>  ("Prefix")),
    suffix  (iConfig.getParameter<std::string>  ("Suffix")),
    isHGEE  (iConfig.getParameter<bool>         ("isHGEE"))
{
  produces <std::vector<double> > ( prefix + "Energy"    + suffix );
  produces <std::vector<double> > ( prefix + "Time"      + suffix );
  
  produces <std::vector<int>    > ( prefix + "Cell"      + suffix );
  produces <std::vector<int>    > ( prefix + "Layer"     + suffix );
  produces <std::vector<int>    > ( prefix + "Subdet"    + suffix );
  produces <std::vector<int>    > ( prefix + "Sector"    + suffix );
  produces <std::vector<int>    > ( prefix + "SubSector" + suffix );

  produces <std::vector<double> > ( prefix + "X"         + suffix );
  produces <std::vector<double> > ( prefix + "Y"         + suffix );
  produces <std::vector<double> > ( prefix + "Z"         + suffix );
  produces <std::vector<double> > ( prefix + "R"         + suffix );
  produces <std::vector<double> > ( prefix + "Eta"       + suffix );
  produces <std::vector<double> > ( prefix + "Phi"       + suffix );

  produces <std::vector<double> > ( prefix + "Corner1Eta"+ suffix );
  produces <std::vector<double> > ( prefix + "Corner1Phi"+ suffix );
  produces <std::vector<double> > ( prefix + "Corner2Eta"+ suffix );
  produces <std::vector<double> > ( prefix + "Corner2Phi"+ suffix );
  produces <std::vector<double> > ( prefix + "Corner3Eta"+ suffix );
  produces <std::vector<double> > ( prefix + "Corner3Phi"+ suffix );
  produces <std::vector<double> > ( prefix + "Corner4Eta"+ suffix );
  produces <std::vector<double> > ( prefix + "Corner4Phi"+ suffix );

}

void HgcalTupleMaker_HgcalRecHits::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> > energy    ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> > time      ( new std::vector<double>()  );
					
  std::auto_ptr<std::vector<int> >    cell      ( new std::vector<int>   ()  );
  std::auto_ptr<std::vector<int> >    layer     ( new std::vector<int>   ()  );
  std::auto_ptr<std::vector<int> >    subdet    ( new std::vector<int>   ()  );
  std::auto_ptr<std::vector<int> >    sector    ( new std::vector<int>   ()  );
  std::auto_ptr<std::vector<int> >    subSector ( new std::vector<int>   ()  );

  std::auto_ptr<std::vector<double> > R         ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> > X         ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> > Y         ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> > Z         ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> > Eta       ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> > Phi       ( new std::vector<double>()  );

  std::auto_ptr<std::vector<double> > corner1Eta( new std::vector<double>()  ); 
  std::auto_ptr<std::vector<double> > corner1Phi( new std::vector<double>()  ); 
  std::auto_ptr<std::vector<double> > corner2Eta( new std::vector<double>()  ); 
  std::auto_ptr<std::vector<double> > corner2Phi( new std::vector<double>()  ); 
  std::auto_ptr<std::vector<double> > corner3Eta( new std::vector<double>()  ); 
  std::auto_ptr<std::vector<double> > corner3Phi( new std::vector<double>()  ); 
  std::auto_ptr<std::vector<double> > corner4Eta( new std::vector<double>()  );  
  std::auto_ptr<std::vector<double> > corner4Phi( new std::vector<double>()  );  
    
  edm::Handle<HGCRecHitCollection> rechits;
  iEvent.getByLabel(inputTag, rechits);

  edm::ESHandle<HGCalGeometry> geoHandle;
  iSetup.get<IdealGeometryRecord>().get(geoTag,geoHandle);
  const HGCalGeometry& hgcGeo = *geoHandle;
  
  if( rechits.isValid() ) {
    edm::LogInfo("HgcalTupleMaker_HgcalRecHitsInfo") << "Total # HgcalRecHits: " << rechits->size();
    
    for( HGCRecHitCollection::const_iterator it = rechits->begin(); it != rechits->end(); ++it ) {
      
      const DetId       detid   ( it -> detid() );
      const GlobalPoint position( std::move( hgcGeo.getPosition( detid ) ) );
      const HGCalGeometry::CornersVec corners( std::move( hgcGeo.getCorners( detid ) ) );
      

      if (isHGEE){
	HGCEEDetId hgdetid ( detid );
	cell      -> push_back(hgdetid.cell     ());
	layer     -> push_back(hgdetid.layer    ());
	subdet    -> push_back(hgdetid.subdet   ());
	sector    -> push_back(hgdetid.sector   ());
	subSector -> push_back(hgdetid.subsector());
      } 

      else { 
	HGCHEDetId hgdetid ( detid );
	cell      -> push_back(hgdetid.cell     ());
	layer     -> push_back(hgdetid.layer    ());
	subdet    -> push_back(hgdetid.subdet   ());
	sector    -> push_back(hgdetid.sector   ());
	subSector -> push_back(hgdetid.subsector());
      }
      
      double r = std::sqrt(position.x()*position.x() +
			   position.y()*position.y());

      energy -> push_back( it -> energy() );
      time   -> push_back( it -> time()   );
      X      -> push_back( position.x()   );
      Y      -> push_back( position.y()   );
      Z      -> push_back( position.z()   );
      R      -> push_back( r );
      Eta    -> push_back( position.eta() );
      Phi    -> push_back( position.phi() );
	     
      corner1Eta -> push_back ( corners[0].eta());
      corner1Phi -> push_back ( corners[0].phi());
      corner2Eta -> push_back ( corners[1].eta());
      corner2Phi -> push_back ( corners[1].phi());
      corner3Eta -> push_back ( corners[2].eta());
      corner3Phi -> push_back ( corners[2].phi());
      corner4Eta -> push_back ( corners[3].eta());
      corner4Phi -> push_back ( corners[3].phi());

    }
  } else {
    edm::LogError("HgcalTupleMaker_HgcalRecHitsError") << "Error! Can't get the product " << inputTag;
  }

  iEvent.put(energy   , prefix + "Energy"    + suffix );
  iEvent.put(time     , prefix + "Time"      + suffix );

  iEvent.put(cell     , prefix + "Cell"      + suffix );
  iEvent.put(layer    , prefix + "Layer"     + suffix );
  iEvent.put(subdet   , prefix + "Subdet"    + suffix );
  iEvent.put(sector   , prefix + "Sector"    + suffix );
  iEvent.put(subSector, prefix + "SubSector" + suffix );
  
  iEvent.put(X        , prefix + "X"         + suffix );
  iEvent.put(Y        , prefix + "Y"         + suffix );
  iEvent.put(Z        , prefix + "Z"         + suffix );
  iEvent.put(R        , prefix + "R"         + suffix );
  iEvent.put(Eta      , prefix + "Eta"       + suffix );
  iEvent.put(Phi      , prefix + "Phi"       + suffix );

  iEvent.put(corner1Eta, prefix + "Corner1Eta" + suffix );
  iEvent.put(corner1Phi, prefix + "Corner1Phi" + suffix );
  iEvent.put(corner2Eta, prefix + "Corner2Eta" + suffix );
  iEvent.put(corner2Phi, prefix + "Corner2Phi" + suffix );
  iEvent.put(corner3Eta, prefix + "Corner3Eta" + suffix );
  iEvent.put(corner3Phi, prefix + "Corner3Phi" + suffix );
  iEvent.put(corner4Eta, prefix + "Corner4Eta" + suffix );
  iEvent.put(corner4Phi, prefix + "Corner4Phi" + suffix );

}

