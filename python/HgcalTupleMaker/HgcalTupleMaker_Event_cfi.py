import FWCore.ParameterSet.Config as cms

hgcalTupleEvent = cms.EDProducer("HgcalTupleMaker_Event",
   RhoInputTag = cms.InputTag("kt6PFJets", "rho")
)
