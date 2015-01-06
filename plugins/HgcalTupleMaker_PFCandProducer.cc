#include "HGCALANA/HgcalTupleMaker/interface/HgcalTupleMaker_PFCandProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

HgcalTupleMaker_PFCandProducer::HgcalTupleMaker_PFCandProducer(const edm::ParameterSet& iConfig) :
  rawInputTag (iConfig.getParameter<edm::InputTag>("RawJetInputTag")),
  maxSize     (iConfig.getParameter<unsigned int> ("MaxSize"))
{
  produces<reco::PFCandidateCollection>();
}

void HgcalTupleMaker_PFCandProducer::
produce(edm::Event& iEvent, const edm::EventSetup& iSetup) {

  edm::Handle<edm::View<reco::Jet> > rawJets;
  iEvent.getByLabel(rawInputTag, rawJets);
  edm::View<reco::Jet>::const_iterator it_rawJet     = rawJets -> begin();
  edm::View<reco::Jet>::const_iterator it_rawJet_end = rawJets -> end  ();
  
  std::auto_ptr<reco::PFCandidateCollection> outputPFCands(new reco::PFCandidateCollection()); 
  
  unsigned int iJet = 0;

  for (; it_rawJet != it_rawJet_end; ++it_rawJet){
    
    iJet++;
    if ( iJet > maxSize ) break;
    
    reco::PFJet const * rawPFJet = dynamic_cast<reco::PFJet const *>( &*it_rawJet);
    std::vector<reco::PFCandidatePtr> constituents = rawPFJet -> getPFConstituents();
    std::vector<reco::PFCandidatePtr>::const_iterator i_constituent = constituents.begin();
    std::vector<reco::PFCandidatePtr>::const_iterator constituent_end = constituents.end();
    
    for (; i_constituent != constituent_end; ++i_constituent)
      outputPFCands -> push_back ( *i_constituent );
  }

  iEvent.put(outputPFCands);
  
}
