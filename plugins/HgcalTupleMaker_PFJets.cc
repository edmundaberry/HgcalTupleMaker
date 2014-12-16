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
  rawInputTag       (iConfig.getParameter<edm::InputTag>("RawJetInputTag")),
  trimmedInputTags  (iConfig.getParameter<std::vector<edm::InputTag> >("TrimmedJetInputTags")),
  subjetInputTags   (iConfig.getParameter<std::vector<edm::InputTag> >("SubjetInputTags")),  
  pfcandInputTag    (iConfig.getParameter<edm::InputTag>("PFCandInputTag")),
  rcutFactorStrings (iConfig.getParameter<std::vector<std::string> >("RCutFactorNames")),
  rcutFactors       (iConfig.getParameter<std::vector<double> >("RCutFactors")),
  prefix            (iConfig.getParameter<std::string>  ("Prefix")),
  suffix            (iConfig.getParameter<std::string>  ("Suffix")),
  maxSize           (iConfig.getParameter<unsigned int> ("MaxSize")){
  
  produces <std::vector<double> >               ( prefix + "RCutFactors"     + suffix );
  produces <std::vector<std::string> >          ( prefix + "RCutFactorNames" + suffix );
  produces <std::vector<double> >               ( prefix + "Eta"             + suffix );
  produces <std::vector<double> >               ( prefix + "Phi"             + suffix );
  produces <std::vector<double> >               ( prefix + "P"               + suffix );
  produces <std::vector<double> >               ( prefix + "Pt"              + suffix );
  produces <std::vector<double> >               ( prefix + "Energy"          + suffix );
  produces <std::vector<double> >               ( prefix + "RawMass"         + suffix );
  produces <std::vector<int> >                  ( prefix + "RawNPFCands"     + suffix );
  produces <std::vector<double> >               ( prefix + "Tau1"            + suffix );
  produces <std::vector<double> >               ( prefix + "Tau2"            + suffix );
  produces <std::vector<double> >               ( prefix + "Tau3"            + suffix );
  produces <std::vector<double> >               ( prefix + "NSubJ"           + suffix );
  produces <std::vector<std::vector<int> > >    ( prefix + "PFCandIndices"   + suffix );
  produces <std::vector<std::vector<double> > > ( prefix + "TrimmedMass"     + suffix );
  produces <std::vector<std::vector<double> > > ( prefix + "TrimmedMassDrop" + suffix );
  produces <std::vector<std::vector<int> > >    ( prefix + "TrimmedNPFCands" + suffix );
  produces <std::vector<std::vector<int> > >    ( prefix + "TrimmedNSubjets" + suffix );

}

edm::View<reco::Jet>::const_iterator 
matchJet ( const reco::Jet & rawJet,  const edm::Handle<edm::View<reco::Jet> > & trimmedJetHandle ){
  
  edm::View<reco::Jet>::const_iterator it_trimmedJet     = trimmedJetHandle -> begin();
  edm::View<reco::Jet>::const_iterator it_trimmedJet_end = trimmedJetHandle -> end  ();
  edm::View<reco::Jet>::const_iterator retval;
  
  double rawEta = rawJet.eta();
  double rawPhi = rawJet.phi();
  double minDR = 999.;
  for (; it_trimmedJet != it_trimmedJet_end ; ++it_trimmedJet ){
    double dr = deltaR ( it_trimmedJet -> eta(), it_trimmedJet -> phi(), rawEta, rawPhi );
    if ( dr < minDR ) {
      retval = it_trimmedJet;
      minDR = dr;
    }
  }
  return retval;
}

void HgcalTupleMaker_PFJets::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  std::auto_ptr<std::vector<double> >               eta             ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               phi             ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               p               ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               pt              ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               energy          ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               rawMass         ( new std::vector<double>() );
  std::auto_ptr<std::vector<int> >                  rawNCands       ( new std::vector<int>   () );
  std::auto_ptr<std::vector<double> >               tau1            ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               tau2            ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               tau3            ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               nSubJ           ( new std::vector<double>() );
  std::auto_ptr<std::vector<double> >               rCutFactors     ( new std::vector<double>() );
  std::auto_ptr<std::vector<std::string> >          rCutFactorStrings ( new std::vector<std::string>() );
  std::auto_ptr<std::vector<std::vector< int > > >  pfCandIndices   ( new std::vector<std::vector<int> >   () );
  std::auto_ptr<std::vector<std::vector<double> > > trimmedMass     ( new std::vector<std::vector<double> >() );
  std::auto_ptr<std::vector<std::vector<double> > > trimmedMassDrop ( new std::vector<std::vector<double> >() );
  std::auto_ptr<std::vector<std::vector<int> > >    trimmedNCands   ( new std::vector<std::vector<int> >   () );
  std::auto_ptr<std::vector<std::vector<int> > >    trimmedNSubjets ( new std::vector<std::vector<int> >   () );

  edm::Handle<reco::PFCandidateCollection> pfCands;
  reco::PFCandidateCollection::const_iterator firstPFCand, lastPFCand, thisPFCand;
  iEvent.getByLabel(pfcandInputTag, pfCands);
  firstPFCand = pfCands -> begin();
  lastPFCand  = pfCands -> end();  
  
  edm::Handle<edm::View<reco::Jet> > rawJets;
  iEvent.getByLabel(rawInputTag, rawJets);
  edm::View<reco::Jet>::const_iterator it_rawJet     = rawJets -> begin();
  edm::View<reco::Jet>::const_iterator it_rawJet_end = rawJets -> end  ();
  
  std::vector<edm::Handle<edm::View<reco::Jet> > > trimmedJetHandles;
  std::vector<edm::Handle<edm::View<reco::Jet> > > subJetHandles;
  
  int nTrimmed = trimmedInputTags.size();

  for (int iTrimmed = 0; iTrimmed < nTrimmed; ++iTrimmed){
    edm::Handle<edm::View<reco::Jet> > tmp_trimmedJetHandle;
    edm::Handle<edm::View<reco::Jet> > tmp_subJetHandle;
    
    iEvent.getByLabel(trimmedInputTags[iTrimmed], tmp_trimmedJetHandle);
    iEvent.getByLabel(subjetInputTags [iTrimmed], tmp_subJetHandle);
    
    trimmedJetHandles.push_back (tmp_trimmedJetHandle);
    subJetHandles    .push_back (tmp_subJetHandle);
    rCutFactors ->    push_back (rcutFactors[iTrimmed]);
    rCutFactorStrings ->push_back (rcutFactorStrings [iTrimmed]);
  }
  
  for (; it_rawJet != it_rawJet_end; ++it_rawJet){
    
    reco::PFJet const * rawPFJet = dynamic_cast<reco::PFJet const *>( &*it_rawJet);

    double tmp_tau1  = JetTools::getTau(1, *rawPFJet);
    double tmp_tau2  = JetTools::getTau(2, *rawPFJet);
    double tmp_tau3  = JetTools::getTau(3, *rawPFJet);
    double tmp_nsubj = ( tmp_tau1 > 0. ) ? tmp_tau2 / tmp_tau1 : -1.0;
       
    eta       -> push_back( it_rawJet -> eta      () );
    phi       -> push_back( it_rawJet -> phi      () );
    p         -> push_back( it_rawJet -> p        () );
    pt        -> push_back( it_rawJet -> pt       () );
    energy    -> push_back( it_rawJet -> energy   () );
    rawMass   -> push_back( it_rawJet -> mass     () );
    rawNCands -> push_back( rawPFJet  -> getPFConstituents ().size());
    tau1      -> push_back( tmp_tau1  );
    tau2      -> push_back( tmp_tau2  );
    tau3      -> push_back( tmp_tau3  );
    nSubJ     -> push_back( tmp_nsubj );

    pfCandIndices   -> push_back( std::vector<int>    () );
    trimmedMass     -> push_back( std::vector<double> () );
    trimmedMassDrop -> push_back( std::vector<double> () );
    trimmedNCands   -> push_back( std::vector<int>    () );
    trimmedNSubjets -> push_back( std::vector<int>    () );
    
    size_t last_index = pfCandIndices -> size() - 1;

    std::vector<reco::PFCandidatePtr> constituents = rawPFJet -> getPFConstituents();
    std::vector<reco::PFCandidatePtr>::const_iterator i_constituent = constituents.begin();
    std::vector<reco::PFCandidatePtr>::const_iterator constituent_end = constituents.end();
    
    for (; i_constituent != constituent_end; ++i_constituent){
      thisPFCand = std::find_if( firstPFCand, lastPFCand, same_pf_cand_as ( *i_constituent ));
      (*pfCandIndices)[last_index].push_back ( thisPFCand - firstPFCand );
    }
    
    for (int iTrimmed = 0; iTrimmed < nTrimmed; ++iTrimmed){
      edm::View<reco::Jet>::const_iterator trimmedJet = matchJet ( * it_rawJet, trimmedJetHandles[iTrimmed] );
      int nSubjets = trimmedJet -> numberOfDaughters();
      int nTrimmedPFCands = 0;
      double subjetMassMaximum = -1.;
      for (int iSubjet = 0; iSubjet < nSubjets; ++iSubjet){
       	reco::PFJet const * subjet = dynamic_cast<reco::PFJet const *>(trimmedJet -> daughter ( iSubjet ));
       	nTrimmedPFCands += (subjet -> getPFConstituents ().size());
	if ( subjet -> mass() > subjetMassMaximum ) subjetMassMaximum = subjet -> mass();
      }
      double massDrop = -1.;
      if ( subjetMassMaximum > 0. && trimmedJet -> mass() > 0.) 
	massDrop = subjetMassMaximum / trimmedJet -> mass();
      (*trimmedMassDrop)[last_index].push_back ( massDrop );
      (*trimmedMass)    [last_index].push_back ( trimmedJet -> mass() );
      (*trimmedNCands)  [last_index].push_back ( nTrimmedPFCands );
      (*trimmedNSubjets)[last_index].push_back ( nSubjets );
    }
  }

  iEvent.put( rCutFactors     , prefix + "RCutFactors"      + suffix );
  iEvent.put( rCutFactorStrings , prefix + "RCutFactorNames"  + suffix );
  iEvent.put( eta             , prefix + "Eta"              + suffix );
  iEvent.put( phi             , prefix + "Phi"              + suffix );
  iEvent.put( p               , prefix + "P"                + suffix );
  iEvent.put( pt              , prefix + "Pt"               + suffix );
  iEvent.put( energy          , prefix + "Energy"           + suffix );
  iEvent.put( rawMass         , prefix + "RawMass"          + suffix );
  iEvent.put( rawNCands       , prefix + "RawNPFCands"      + suffix );
  iEvent.put( tau1            , prefix + "Tau1"             + suffix );
  iEvent.put( tau2            , prefix + "Tau2"             + suffix );
  iEvent.put( tau3            , prefix + "Tau3"             + suffix );
  iEvent.put( nSubJ           , prefix + "NSubJ"            + suffix );
  iEvent.put( pfCandIndices   , prefix + "PFCandIndices"    + suffix );
  iEvent.put( trimmedMass     , prefix + "TrimmedMass"      + suffix );
  iEvent.put( trimmedMassDrop , prefix + "TrimmedMassDrop"  + suffix );
  iEvent.put( trimmedNCands   , prefix + "TrimmedNPFCands"  + suffix );
  iEvent.put( trimmedNSubjets , prefix + "TrimmedNSubjets"  + suffix );

}

