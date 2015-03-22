import FWCore.ParameterSet.Config as cms

from RecoJets.Configuration.RecoPFJets_cff import ca8PFJetsCHSPruned, ca8PFJetsCHS, ak5PFJetsCHS

ca4PFJetsCHS = ca8PFJetsCHS.clone( rParam = 0.4 )

ca4PFJetsCHSPrunedRCut5 = ca8PFJetsCHSPruned.clone( rParam = 0.4 )
ca4PFJetsCHSPrunedRCut5.jetCollInstanceName = cms.string("SubJets")
ca4PFJetsCHSPrunedRCut5.writeCompound = cms.bool(True)
ca4PFJetsCHSPrunedRCut5.jetPtMin = cms.double(1.)

ak0p5PFJetsCHS = ak5PFJetsCHS.clone( rParam = 0.05 )
ak1p0PFJetsCHS = ak5PFJetsCHS.clone( rParam = 0.10 )
ak1p5PFJetsCHS = ak5PFJetsCHS.clone( rParam = 0.15 )

hgcalTuplePFAK0p5Jets = cms.EDProducer("HgcalTupleMaker_PFJets",
    RawJetInputTag      = cms.InputTag("ak0p5PFJetsCHS"),
    TrimmedJetInputTags = cms.VInputTag(),
    SubjetInputTags     = cms.VInputTag(),
    PFCandInputTag      = cms.InputTag("selectPFCands"),
    RCutFactorNames     = cms.vstring (),
    RCutFactors         = cms.vdouble (),
    Prefix = cms.string('PFAK0p5Jet'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(10)
)

hgcalTuplePFAK1p0Jets = cms.EDProducer("HgcalTupleMaker_PFJets",
    RawJetInputTag      = cms.InputTag("ak1p0PFJetsCHS"),
    TrimmedJetInputTags = cms.VInputTag(),
    SubjetInputTags     = cms.VInputTag(),
    PFCandInputTag      = cms.InputTag("selectPFCands"),
    RCutFactorNames     = cms.vstring (),
    RCutFactors         = cms.vdouble (),
    Prefix = cms.string('PFAK1p0Jet'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(10)
)

hgcalTuplePFAK1p5Jets = cms.EDProducer("HgcalTupleMaker_PFJets",
    RawJetInputTag      = cms.InputTag("ak1p5PFJetsCHS"),
    TrimmedJetInputTags = cms.VInputTag(),
    SubjetInputTags     = cms.VInputTag(),
    PFCandInputTag      = cms.InputTag("selectPFCands"),
    RCutFactorNames     = cms.vstring (),
    RCutFactors         = cms.vdouble (),
    Prefix = cms.string('PFAK1p5Jet'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(10)
)


hgcalTuplePFCA4PrunedJets = cms.EDProducer("HgcalTupleMaker_PFJets",
    RawJetInputTag      = cms.InputTag("ca4PFJetsCHS"),
    TrimmedJetInputTags = cms.VInputTag(("ca4PFJetsCHSPrunedRCut5")),
    SubjetInputTags     = cms.VInputTag(("ca4PFJetsCHSPrunedRCut5", "SubJets")),
    PFCandInputTag      = cms.InputTag("selectPFCands"),
    RCutFactorNames     = cms.vstring (("R5")),                                              
    RCutFactors         = cms.vdouble ((0.5)),                                           
    Prefix = cms.string('PFCA4Jet'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(10)
)

myJetProducerSequence         = cms.Sequence ( ca4PFJetsCHS * ak0p5PFJetsCHS * ak1p0PFJetsCHS * ak1p5PFJetsCHS )
myPrunedJetProducerSequence   = cms.Sequence ( ca4PFJetsCHSPrunedRCut5 ) 
myPrunedJetTupleMakerSequence = cms.Sequence ( hgcalTuplePFCA4PrunedJets ) 
                               
def makePrunedJetTupleMakersForRCuts ( process, rcut_factors ) :

    oldJetProducerName = "ca4PFJetsCHSPrunedRCut5"
    process.MessageLogger.suppressError =  cms.untracked.vstring( "ca4PFJetsCHS", oldJetProducerName )
    
    for rcut_factor in rcut_factors:   
        
        # Associate the rcut_factor (float) with a string
        factorString = str(rcut_factor).replace("0.","")

        # Create the new jet producer module from the old jet producer module
        newJetProducerName = oldJetProducerName.replace("5", factorString)
        setattr(process, newJetProducerName, ca4PFJetsCHSPrunedRCut5.clone() )

        # Set the new jet producer module's parameters
        newJetProducerModule = getattr( process, newJetProducerName )
        newJetProducerModule.rcut_factor = rcut_factor
        
        # Put the new jet producer into the sequence
        myPrunedJetProducerSequence.replace ( getattr(process, oldJetProducerName), 
                                              getattr(process, oldJetProducerName) + getattr(process,newJetProducerName))
        
        # Add the new jet producer to the tags to be investigated by the tuple maker module
        hgcalTuplePFCA4PrunedJets.TrimmedJetInputTags.append ( cms.InputTag( newJetProducerName ) )
        hgcalTuplePFCA4PrunedJets.SubjetInputTags.append ( cms.InputTag( newJetProducerName, "SubJets" ) )
        hgcalTuplePFCA4PrunedJets.RCutFactorNames.append ( "R" + factorString ) 
        hgcalTuplePFCA4PrunedJets.RCutFactors.append ( rcut_factor ) 

        # Silence message logger
        process.MessageLogger.suppressError.append ( newJetProducerName )

    


