#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFJets.h"
#include "HGCALANA/HgcalTupleMaker/interface/JetTools.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "HGCALANA/HgcalTupleMaker/interface/JetTools.h"


HgcalTupleMaker_PFJets::HgcalTupleMaker_PFJets(const edm::ParameterSet& iConfig) :
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
  produces <std::vector<double> > ( prefix + "Tau1"   + suffix );
  produces <std::vector<double> > ( prefix + "Tau2"   + suffix );
  produces <std::vector<double> > ( prefix + "Tau3"   + suffix );
}


void HgcalTupleMaker_PFJets::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> >  eta   ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  phi   ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  p     ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  pt    ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  energy( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  tau1  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  tau2  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  tau3  ( new std::vector<double>()  );
  std::auto_ptr<std::vector<double> >  nsubj ( new std::vector<double>()  );

  edm::Handle<reco::PFJetCollection> pfJets;
  iEvent.getByLabel(inputTag, pfJets);
  
  if( pfJets.isValid() ) {
    edm::LogInfo("HgcalTupleMaker_PFJetsInfo") << "Total # PFJets: " << pfJets->size();
    
    for( reco::PFJetCollection::const_iterator it = pfJets->begin(); it != pfJets->end(); ++it ) {
      
      if(eta->size() >= maxSize) break;
      
      eta    -> push_back( it -> eta      () );
      phi    -> push_back( it -> phi      () );
      p      -> push_back( it -> p        () );
      pt     -> push_back( it -> pt       () );
      energy -> push_back( it -> energy   () );

      double tmp_tau1  = JetTools::getTau(1, *it);
      double tmp_tau2  = JetTools::getTau(2, *it);
      double tmp_tau3  = JetTools::getTau(3, *it);
      double tmp_nsubj = ( tmp_tau1 > 0. ) ? tmp_tau2 / tmp_tau1 : -1.0;
      
      tau1   -> push_back( tmp_tau1  );
      tau2   -> push_back( tmp_tau2  );
      tau3   -> push_back( tmp_tau3  );
      nsubj  -> push_back( tmp_nsubj );

    }
  } else {
    edm::LogError("HgcalTupleMaker_PFJetsError") << "Error! Can't get the product " << inputTag;
  }

  iEvent.put( eta   , prefix + "Eta"    + suffix );
  iEvent.put( phi   , prefix + "Phi"    + suffix );
  iEvent.put( p     , prefix + "P"      + suffix );
  iEvent.put( pt    , prefix + "Pt"     + suffix );
  iEvent.put( energy, prefix + "Energy" + suffix );
  iEvent.put( tau1  , prefix + "Tau1"   + suffix );
  iEvent.put( tau2  , prefix + "Tau2"   + suffix );
  iEvent.put( tau3  , prefix + "Tau3"   + suffix );
  iEvent.put( nsubj , prefix + "NSubJ"  + suffix );

}

