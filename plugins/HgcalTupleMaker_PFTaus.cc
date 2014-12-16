#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFTaus.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"
#include "DataFormats/TauReco/interface/PFTauDiscriminator.h"

HgcalTupleMaker_PFTaus::HgcalTupleMaker_PFTaus(const edm::ParameterSet& iConfig) :
    inputTag(iConfig.getParameter<edm::InputTag>("InputTag")),
    decayModeFindingTag      (iConfig.getParameter<edm::InputTag>("DecayModeFindingTag")),   
    looseCombinedIsolationTag(iConfig.getParameter<edm::InputTag>("LooseCombinedIsolationTag")),
    muonRejectionTag         (iConfig.getParameter<edm::InputTag>("MuonRejectionTag")),	     
    prefix  (iConfig.getParameter<std::string>  ("Prefix")),
    suffix  (iConfig.getParameter<std::string>  ("Suffix")),
    maxSize (iConfig.getParameter<unsigned int> ("MaxSize"))
{
  produces <std::vector<double> > ( prefix + "P"      + suffix );
  produces <std::vector<double> > ( prefix + "Pt"     + suffix );
  produces <std::vector<double> > ( prefix + "Eta"    + suffix );
  produces <std::vector<double> > ( prefix + "Phi"    + suffix );
  produces <std::vector<int> >    ( prefix + "PassDecayModeFinding"       + suffix );
  produces <std::vector<int> >    ( prefix + "PassLooseCombinedIsolation" + suffix );
  produces <std::vector<int> >    ( prefix + "PassMuonRejection"          + suffix );
  produces <std::vector<int> >    ( prefix + "PassAll"                    + suffix );
} 

void HgcalTupleMaker_PFTaus::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> >  p                         ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  pt                        ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  eta                       ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  phi                       ( new std::vector<double>()  );
  std::auto_ptr<std::vector<int> >     passDecayModeFinding      ( new std::vector<int>()  );
  std::auto_ptr<std::vector<int> >     passLooseCombinedIsolation( new std::vector<int>()  );
  std::auto_ptr<std::vector<int> >     passMuonRejection         ( new std::vector<int>()  );
  std::auto_ptr<std::vector<int> >     passAll                   ( new std::vector<int>()  );

  edm::Handle<reco::PFTauCollection> taus;
  iEvent.getByLabel(inputTag, taus);

  edm::Handle<reco::PFTauDiscriminator> decayModeFinding;
  iEvent.getByLabel(decayModeFindingTag, decayModeFinding);
  
  edm::Handle<reco::PFTauDiscriminator> looseCombinedIsolation;
  iEvent.getByLabel(looseCombinedIsolationTag, looseCombinedIsolation);

  edm::Handle<reco::PFTauDiscriminator> muonRejection;
  iEvent.getByLabel(muonRejectionTag, muonRejection);
  
  if( taus.isValid() ) {
    edm::LogInfo("HgcalTupleMaker_PFTausInfo") << "Total # PFTaus: " << taus->size();
    
    unsigned nTaus = taus -> size();
    for ( unsigned iTau = 0; iTau < nTaus; ++iTau ) {
      reco::PFTauRef it(taus, iTau);

      p   -> push_back( it -> p  () );
      pt  -> push_back( it -> pt () );      
      eta -> push_back( it -> eta() );
      phi -> push_back( it -> phi() );

      int tmp_decayModeFinding       = (*decayModeFinding      )[it] > 0.5 ? 1 : 0;
      int tmp_looseCombinedIsolation = (*looseCombinedIsolation)[it] > 0.5 ? 1 : 0;
      int tmp_muonRejection          = (*muonRejection         )[it] > 0.5 ? 1 : 0;
      int tmp_all                    = ( tmp_decayModeFinding       == 1 &&
					 tmp_looseCombinedIsolation == 1 &&
					 tmp_muonRejection          == 1 ) ? 1 : 0;

      passDecayModeFinding       -> push_back ( tmp_decayModeFinding       );
      passLooseCombinedIsolation -> push_back ( tmp_looseCombinedIsolation );
      passMuonRejection          -> push_back ( tmp_muonRejection          );
      passAll                    -> push_back ( tmp_all                    ) ;
      
    }
  } else {
    edm::LogError("HgcalTupleMaker_PFTausError") << "Error! Can't get the product " << inputTag;
  }
  
  iEvent.put( p     , prefix + "P"      + suffix );
  iEvent.put( pt    , prefix + "Pt"     + suffix );
  iEvent.put( eta   , prefix + "Eta"    + suffix );
  iEvent.put( phi   , prefix + "Phi"    + suffix );
  iEvent.put( passDecayModeFinding      , prefix + "PassDecayModeFinding"       + suffix );
  iEvent.put( passLooseCombinedIsolation, prefix + "PassLooseCombinedIsolation" + suffix );
  iEvent.put( passMuonRejection         , prefix + "PassMuonRejection"          + suffix );
  iEvent.put( passAll                   , prefix + "PassAll"                    + suffix );
  
}
