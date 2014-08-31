#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFJets.h"
#include "HGCALANA/HgcalTupleMaker/interface/JetTools.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "HGCALANA/HgcalTupleMaker/interface/JetTools.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

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

HgcalTupleMaker_PFJets::HgcalTupleMaker_PFJets(const edm::ParameterSet& iConfig) :
    inputTag        (iConfig.getParameter<edm::InputTag>("InputTag")),
    candInputTag    (iConfig.getParameter<edm::InputTag>("PFCandInputTag")),
    prefix          (iConfig.getParameter<std::string>  ("Prefix")),
    suffix          (iConfig.getParameter<std::string>  ("Suffix")),
    maxSize         (iConfig.getParameter<unsigned int> ("MaxSize")),
    inputTagIsPruned(inputTag.label().find(std::string("Pruned")) != std::string::npos)
{
  produces <std::vector<double> >            ( prefix + "Eta"           + suffix );
  produces <std::vector<double> >            ( prefix + "Phi"           + suffix );
  produces <std::vector<double> >            ( prefix + "P"             + suffix );
  produces <std::vector<double> >            ( prefix + "Pt"            + suffix );
  produces <std::vector<double> >            ( prefix + "Mass"          + suffix );
  produces <std::vector<double> >            ( prefix + "Energy"        + suffix );
  produces <std::vector<double> >            ( prefix + "Tau1"          + suffix );
  produces <std::vector<double> >            ( prefix + "Tau2"          + suffix );
  produces <std::vector<double> >            ( prefix + "Tau3"          + suffix );
  produces <std::vector<double> >            ( prefix + "NSubJ"         + suffix );
  produces <std::vector<std::vector<int> > > ( prefix + "PFCandIndices" + suffix );

  if ( inputTagIsPruned ){
    produces <std::vector<int> >                    ( prefix + "NDaughters"   + suffix );
    produces <std::vector<double> >                 ( prefix + "MassDrop"     + suffix );
    produces <std::vector<std::vector< double > > > ( prefix + "DaughterPt"   + suffix );
    produces <std::vector<std::vector< double > > > ( prefix + "DaughterEta"  + suffix );
    produces <std::vector<std::vector< double > > > ( prefix + "DaughterPhi"  + suffix );
    produces <std::vector<std::vector< double > > > ( prefix + "DaughterMass" + suffix );
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

  std::auto_ptr<std::vector<int> >                  nDaughters    ( new std::vector<int>   () );
  std::auto_ptr<std::vector<double> >               massDrop      ( new std::vector<double>() );
  std::auto_ptr<std::vector<std::vector<double> > > daughterPt    ( new std::vector< std::vector<double> >() );
  std::auto_ptr<std::vector<std::vector<double> > > daughterEta   ( new std::vector< std::vector<double> >() );
  std::auto_ptr<std::vector<std::vector<double> > > daughterPhi   ( new std::vector< std::vector<double> >() );
  std::auto_ptr<std::vector<std::vector<double> > > daughterMass  ( new std::vector< std::vector<double> >() );
  
  edm::Handle<edm::View<reco::Jet> > jets;
  iEvent.getByLabel(inputTag, jets);

  edm::Handle<reco::PFCandidateCollection> pfCands;
  iEvent.getByLabel(candInputTag, pfCands);

  reco::PFCandidateCollection::const_iterator firstPFCand = pfCands -> begin();
  reco::PFCandidateCollection::const_iterator lastPFCand  = pfCands -> end();
  reco::PFCandidateCollection::const_iterator thisPFCand;
  
  edm::View<reco::Jet>::const_iterator it     = jets -> begin();
  edm::View<reco::Jet>::const_iterator it_end = jets -> end  ();
  
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
      
      double tmp_tau1  = -1.;
      double tmp_tau2  = -1.;
      double tmp_tau3  = -1.;
      double tmp_nsubj = -1.;

      reco::PFJet const * pfjet = dynamic_cast<reco::PFJet const *>( &*it);
      if ( pfjet ) { 
	tmp_tau1  = JetTools::getTau(1, *pfjet);
	tmp_tau2  = JetTools::getTau(2, *pfjet);
	tmp_tau3  = JetTools::getTau(3, *pfjet);
	tmp_nsubj = ( tmp_tau1 > 0. ) ? tmp_tau2 / tmp_tau1 : -1.0;

	size_t last_pfcand_index = pfCandIndices -> size();
	std::vector<reco::PFCandidatePtr> constituents = pfjet -> getPFConstituents();
	std::vector<reco::PFCandidatePtr>::const_iterator i_constituent = constituents.begin();
	std::vector<reco::PFCandidatePtr>::const_iterator constituent_end = constituents.end();
	
	for (; i_constituent != constituent_end; ++i_constituent){
	  thisPFCand = std::find_if( firstPFCand, lastPFCand, same_pf_cand_as ( *i_constituent ));
	  (*pfCandIndices)[last_pfcand_index - 1].push_back ( thisPFCand - firstPFCand );
	}
      }
      
      tau1  -> push_back( tmp_tau1  );
      tau2  -> push_back( tmp_tau2  );
      tau3  -> push_back( tmp_tau3  );
      nsubj -> push_back( tmp_nsubj );
      
      if ( inputTagIsPruned ) { 

	// Prepare vectors
	
	daughterPt   -> push_back ( std::vector<double> () );
	daughterEta  -> push_back ( std::vector<double> () );
	daughterPhi  -> push_back ( std::vector<double> () );
	daughterMass -> push_back ( std::vector<double> () );

	// Info about daughters

	int tmp_nDaughters = it -> numberOfDaughters();
	size_t last_daughter_index = daughterPt -> size();
	
	for (int iDaughter = 0; iDaughter < tmp_nDaughters; ++iDaughter){
	  reco::Candidate const * daughter = it -> daughter ( iDaughter );
	  (*daughterPt  )[last_daughter_index - 1].push_back( daughter -> pt  () );
	  (*daughterEta )[last_daughter_index - 1].push_back( daughter -> eta () );
	  (*daughterPhi )[last_daughter_index - 1].push_back( daughter -> phi () );
	  (*daughterMass)[last_daughter_index - 1].push_back( daughter -> mass() );
	}
	nDaughters -> push_back ( tmp_nDaughters );

	// Info about mass drop
	
	double tmp_massDrop = -1;
	if ( tmp_nDaughters > 1 ){
	  double da1_mass = (*daughterMass)[last_daughter_index - 1][0];
	  double da2_mass = (*daughterMass)[last_daughter_index - 1][1];
	  if ( da1_mass > da2_mass ) tmp_massDrop = da1_mass / it -> mass ();
	  else                       tmp_massDrop = da2_mass / it -> mass ();
	}
	massDrop -> push_back ( tmp_massDrop );
	
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
  iEvent.put( tau1          , prefix + "Tau1"          + suffix );
  iEvent.put( tau2          , prefix + "Tau2"          + suffix );
  iEvent.put( tau3          , prefix + "Tau3"          + suffix );
  iEvent.put( nsubj         , prefix + "NSubJ"         + suffix );
  iEvent.put( pfCandIndices , prefix + "PFCandIndices" + suffix );
  
  if ( inputTagIsPruned ) { 
    iEvent.put ( nDaughters   , prefix + "NDaughters"   + suffix );
    iEvent.put ( massDrop     , prefix + "MassDrop"     + suffix );
    iEvent.put ( daughterPt   , prefix + "DaughterPt"   + suffix );
    iEvent.put ( daughterEta  , prefix + "DaughterEta"  + suffix );
    iEvent.put ( daughterPhi  , prefix + "DaughterPhi"  + suffix );
    iEvent.put ( daughterMass , prefix + "DaughterMass" + suffix );
  }
}

