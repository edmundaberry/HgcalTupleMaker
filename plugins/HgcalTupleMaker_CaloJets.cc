#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_CaloJets.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"

HgcalTupleMaker_CaloJets::HgcalTupleMaker_CaloJets(const edm::ParameterSet& iConfig) :
    inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
    prefix  (iConfig.getParameter<std::string>  ("Prefix")),
    suffix  (iConfig.getParameter<std::string>  ("Suffix")),
    maxSize (iConfig.getParameter<unsigned int> ("MaxSize"))
{
  produces <std::vector<double> > ( prefix + "Eta"    + suffix );
  produces <std::vector<double> > ( prefix + "Phi"    + suffix );
  produces <std::vector<double> > ( prefix + "P"      + suffix );
  produces <std::vector<double> > ( prefix + "Pt"     + suffix );
  produces <std::vector<double> > ( prefix + "Energy" + suffix );
}

void HgcalTupleMaker_CaloJets::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> >  eta   ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  phi   ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  p     ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  pt    ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  energy( new std::vector<double>()  );

  edm::Handle<reco::CaloJetCollection> caloJets;
  iEvent.getByLabel(inputTag, caloJets);
  
  if( caloJets.isValid() ) {
    edm::LogInfo("HgcalTupleMaker_CaloJetsInfo") << "Total # CaloJets: " << caloJets->size();
    
    for( reco::CaloJetCollection::const_iterator it = caloJets->begin(); it != caloJets->end(); ++it ) {
      
      if(eta->size() >= maxSize) break;
      
      eta    -> push_back( it -> eta      () );
      phi    -> push_back( it -> phi      () );
      p      -> push_back( it -> p        () );
      pt     -> push_back( it -> pt       () );
      energy -> push_back( it -> energy   () );
      
    }
  } else {
    edm::LogError("HgcalTupleMaker_CaloJetsError") << "Error! Can't get the product " << inputTag;
  }

  iEvent.put( eta   , prefix + "Eta"    + suffix );
  iEvent.put( phi   , prefix + "Phi"    + suffix );
  iEvent.put( p     , prefix + "P"      + suffix );
  iEvent.put( pt    , prefix + "Pt"     + suffix );
  iEvent.put( energy, prefix + "Energy" + suffix );

}

