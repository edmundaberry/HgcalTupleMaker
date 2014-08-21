import FWCore.ParameterSet.Config as cms

hgcalTupleGenJets = cms.EDProducer("HgcalTupleMaker_GenJets",
    InputTag = cms.InputTag('ak5GenJets'),
    Prefix = cms.string('GenJet'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(10)
)
