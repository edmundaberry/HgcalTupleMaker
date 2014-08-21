import FWCore.ParameterSet.Config as cms

hgcalTuplePFJets = cms.EDProducer("HgcalTupleMaker_PFJets",
    InputTag = cms.InputTag('ca8PFJetsCHS'),
    Prefix = cms.string('PFJet'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(10)
)
