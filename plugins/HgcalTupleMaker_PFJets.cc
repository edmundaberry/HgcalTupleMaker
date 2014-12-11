#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFJets.h"
#include "HGCALANA/HgcalTupleMaker/interface/JetTools.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "HGCALANA/HgcalTupleMaker/interface/JetTools.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Math/interface/deltaR.h"

struct same_pf_cand_as { 
  reco::PFCandidate cand1;;
  same_pf_cand_as( const reco::PFCandidate& c ) : cand1 ( c ) {}
  bool operator () ( const reco::PFCandidate& cand2 ) const {
    if ( cand1.pt ()        != cand2.pt ()        ) return false;
    if ( cand1.eta()        != cand2.eta()        ) return false;
    if ( cand1.phi()        != cand2.phi()        ) return false;
    if ( cand1.particleId() != cand2.particleId() ) return false;
    return true;
  }
};

struct same_subjet_as { 
  reco::Jet cand1;;
  same_subjet_as( const reco::Jet& c ) : cand1 ( c ) {}
  bool operator () ( const reco::Jet& cand2 ) const {
    if ( cand1.pt () != cand2.pt () ) return false;
    if ( cand1.eta() != cand2.eta() ) return false;
    if ( cand1.phi() != cand2.phi() ) return false;
    return true;
  }
};

HgcalTupleMaker_PFJets::HgcalTupleMaker_PFJets(const edm::ParameterSet& iConfig) :
    inputTag        (iConfig.getParameter<edm::InputTag>("InputTag")),
    subjetInputTag  (iConfig.getUntrackedParameter<edm::InputTag>("SubjetInputTag")),
    candInputTag    (iConfig.getParameter<edm::InputTag>("PFCandInputTag")),
    prefix          (iConfig.getParameter<std::string>  ("Prefix")),
    suffix          (iConfig.getParameter<std::string>  ("Suffix")),
    maxSize         (iConfig.getParameter<unsigned int> ("MaxSize")),
    hasSubjets      (iConfig.getParameter<bool>         ("HasSubjets")),
    isSubjets       (iConfig.getParameter<bool>         ("IsSubjets"))
{
  produces <std::vector<double> >            ( prefix + "Eta"           + suffix );
  produces <std::vector<double> >            ( prefix + "Phi"           + suffix );
  produces <std::vector<double> >            ( prefix + "P"             + suffix );
  produces <std::vector<double> >            ( prefix + "Pt"            + suffix );
  produces <std::vector<double> >            ( prefix + "Mass"          + suffix );
  produces <std::vector<double> >            ( prefix + "Energy"        + suffix );

  if ( hasSubjets ){
    produces <std::vector<std::vector<int> > > ( prefix + "SubjetIndices" + suffix );
  }

  else { 
    produces <std::vector<std::vector<int> > > ( prefix + "PFCandIndices" + suffix );
    
    if ( !isSubjets ){
      produces <std::vector<double> >            ( prefix + "Tau1"          + suffix );
      produces <std::vector<double> >            ( prefix + "Tau2"          + suffix );
      produces <std::vector<double> >            ( prefix + "Tau3"          + suffix );
      produces <std::vector<double> >            ( prefix + "NSubJ"         + suffix );
    }
  }
}

void HgcalTupleMaker_PFJets::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> >               eta           ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               phi           ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               p             ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               pt            ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               mass          ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               energy        ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               tau1          ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               tau2          ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               tau3          ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               nsubj         ( new std::vector<double>() );
  std::auto_ptr<std::vector<std::vector< int > > >  pfCandIndices ( new std::vector< std::vector< int > >() );
  std::auto_ptr<std::vector<std::vector< int > > >  subjetIndices ( new std::vector< std::vector< int > >() );

  edm::Handle<edm::View<reco::Jet> > jets;
  edm::Handle<edm::View<reco::Jet> > subjets;
  edm::Handle<reco::PFCandidateCollection> pfCands;
  
  iEvent.getByLabel(inputTag, jets);
  edm::View<reco::Jet>::const_iterator it     = jets -> begin();
  edm::View<reco::Jet>::const_iterator it_end = jets -> end  ();
  
  edm::View<reco::Jet>::const_iterator        firstSubjet, lastSubjet, thisSubjet;
  reco::PFCandidateCollection::const_iterator firstPFCand, lastPFCand, thisPFCand;
  
  if ( hasSubjets ) {
    iEvent.getByLabel(subjetInputTag, subjets);
    firstSubjet = subjets -> begin();
    lastSubjet  = subjets -> end();  
  }

  if (!hasSubjets){
    iEvent.getByLabel(candInputTag, pfCands);
    firstPFCand = pfCands -> begin();
    lastPFCand  = pfCands -> end();  
  }
    
  if( jets.isValid() ) {
    edm::LogInfo("HgcalTupleMaker_PFJetsInfo") << "Total # Jets: " << jets->size();
    
    for(; it != it_end; ++it){
      
      if(eta->size() >= maxSize) break;
      
      eta    -> push_back( it -> eta      () );
      phi    -> push_back( it -> phi      () );
      p      -> push_back( it -> p        () );
      pt     -> push_back( it -> pt       () );
      mass   -> push_back( it -> mass     () );
      energy -> push_back( it -> energy   () );
      pfCandIndices -> push_back( std::vector<int> () );
      subjetIndices -> push_back( std::vector<int> () );

      size_t last_index = pfCandIndices -> size() - 1;
      
      if ( hasSubjets ){

	size_t last_index = subjetIndices -> size() - 1;
	int nSubjets = it -> numberOfDaughters();
	
	for (int iSubjet = 0; iSubjet < nSubjets; ++iSubjet){
	  reco::PFJet const * subjet = dynamic_cast<reco::PFJet const *>(it -> daughter ( iSubjet ));
	  thisSubjet = std::find_if( firstSubjet, lastSubjet, same_subjet_as ( * subjet ));
	  (*subjetIndices)[last_index].push_back ( thisSubjet - firstSubjet );
	}
      }
      
      else { 

	reco::PFJet const * pfjet = dynamic_cast<reco::PFJet const *>( &*it);
	
	std::vector<reco::PFCandidatePtr> constituents = pfjet -> getPFConstituents();
	std::vector<reco::PFCandidatePtr>::const_iterator i_constituent = constituents.begin();
	std::vector<reco::PFCandidatePtr>::const_iterator constituent_end = constituents.end();
	
	for (; i_constituent != constituent_end; ++i_constituent){
	  thisPFCand = std::find_if( firstPFCand, lastPFCand, same_pf_cand_as ( *i_constituent ));
	  (*pfCandIndices)[last_index].push_back ( thisPFCand - firstPFCand );
	}
	
	if ( !isSubjets ) { 
	  double tmp_tau1  = JetTools::getTau(1, *pfjet);
	  double tmp_tau2  = JetTools::getTau(2, *pfjet);
	  double tmp_tau3  = JetTools::getTau(3, *pfjet);
	  double tmp_nsubj = ( tmp_tau1 > 0. ) ? tmp_tau2 / tmp_tau1 : -1.0;
	  
	  tau1  -> push_back( tmp_tau1  );
	  tau2  -> push_back( tmp_tau2  );
	  tau3  -> push_back( tmp_tau3  );
	  nsubj -> push_back( tmp_nsubj );
	}
      }
    }
  } else {
    edm::LogError("HgcalTupleMaker_PFJetsError") << "Error! Can't get the product " << inputTag;
  }

  iEvent.put( eta           , prefix + "Eta"           + suffix );
  iEvent.put( phi           , prefix + "Phi"           + suffix );
  iEvent.put( p             , prefix + "P"             + suffix );
  iEvent.put( pt            , prefix + "Pt"            + suffix );
  iEvent.put( mass          , prefix + "Mass"          + suffix );
  iEvent.put( energy        , prefix + "Energy"        + suffix );

  if ( hasSubjets ) {
    iEvent.put( subjetIndices , prefix + "SubjetIndices" + suffix );
  }

  else { 
    iEvent.put( pfCandIndices , prefix + "PFCandIndices" + suffix );

    if ( !isSubjets ){
      iEvent.put( tau1          , prefix + "Tau1"          + suffix );
      iEvent.put( tau2          , prefix + "Tau2"          + suffix );
      iEvent.put( tau3          , prefix + "Tau3"          + suffix );
      iEvent.put( nsubj         , prefix + "NSubJ"         + suffix );
    }
  }


}

