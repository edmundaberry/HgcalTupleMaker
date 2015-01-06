import FWCore.ParameterSet.Config as cms

from RecoJets.Configuration.RecoPFJets_cff import ca8PFJetsCHSPruned, ca8PFJetsCHS

ca4PFJetsCHS = ca8PFJetsCHS.clone( rParam = 0.4 )

ca4PFJetsCHSPrunedRCut5 = ca8PFJetsCHSPruned.clone( rParam = 0.4 )
ca4PFJetsCHSPrunedRCut5.jetCollInstanceName = cms.string("SubJets")
ca4PFJetsCHSPrunedRCut5.writeCompound = cms.bool(True)
ca4PFJetsCHSPrunedRCut5.jetPtMin = cms.double(1.)


hgcalTuplePFCA4PrunedJets = cms.EDProducer("HgcalTupleMaker_PFJets",
    RawJetInputTag      = cms.InputTag("ca4PFJetsCHS"),
    TrimmedJetInputTags = cms.VInputTag(("ca4PFJetsCHSPrunedRCut5")),
    SubjetInputTags     = cms.VInputTag(("ca4PFJetsCHSPrunedRCut5", "SubJets")),
    PFCandInputTag      = cms.InputTag("selectPFCands"),
    RCutFactorNames     = cms.vstring (("R5")),                                              
    RCutFactors         = cms.vdouble ((0.5)),                                           
    Prefix = cms.string('PFCA4Jet'),
    Suffix = cms.string(''),
    MaxSize = cms.uint32(100)
)

myJetProducerSequence         = cms.Sequence ( ca4PFJetsCHS )
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

    


