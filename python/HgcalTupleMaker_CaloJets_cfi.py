import FWCore.ParameterSet.Config as cms

hgcalTupleCaloJets = cms.EDProducer("HgcalTupleMaker_CaloJets",
    InputTag = cms.InputTag('ak5CaloJets'),
    Prefix = cms.string('CaloJet'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(10)
)
