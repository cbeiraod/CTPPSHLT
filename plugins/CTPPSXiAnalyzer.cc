
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/JetReco/interface/PFJet.h"

#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"

#include "DataFormats/Common/interface/DetSet.h"
#include "DataFormats/Common/interface/DetSetVector.h"

#include "DataFormats/CTPPSDetId/interface/CTPPSPixelDetId.h"
#include "DataFormats/CTPPSDetId/interface/TotemRPDetId.h"
#include "DataFormats/CTPPSDetId/interface/CTPPSDiamondDetId.h"

#include "DataFormats/HLTReco/interface/TriggerTypeDefs.h"
#include "DataFormats/HLTReco/interface/TriggerFilterObjectWithRefs.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/CTPPSReco/interface/CTPPSPixelLocalTrack.h"     // pixel
#include "DataFormats/CTPPSReco/interface/TotemRPLocalTrack.h"        // strip
#include "DataFormats/CTPPSReco/interface/CTPPSDiamondLocalTrack.h"   // diamond

#include "TLorentzVector.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TTree.h"
#include "TFile.h"

#include <sstream>
#include <vector>
#include <utility> // for std::pair





class CTPPSXiAnalyzer : public edm::one::EDAnalyzer<edm::one::SharedResources>
//class CTPPSXiAnalyzer : public edm::one::EDAnalyzer<>
{
   public:
      explicit CTPPSXiAnalyzer(const edm::ParameterSet&);
      ~CTPPSXiAnalyzer();

      static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);


   private:
      //virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

      std::string suffix_;
      double minXi_;
      double maxXi_;
      int binsXi_;
      double minMass_;
      double maxMass_;
      int binsMass_;
      double minRapidity_;
      double maxRapidity_;
      int binsRapidity_;

      edm::InputTag pixelLocalTrackInputTag_; // Input tag identifying the pixel detector
      edm::InputTag stripLocalTrackInputTag_; // Input tag identifying the strip detector
      edm::InputTag diamondLocalTrackInputTag_; // Input tag identifying the diamond detector

      unsigned int detectorBitset_;

      bool usePATObjects_;
      bool xiFromDijet_;
      bool xiFromDilepton_;

      edm::InputTag jetInputTag_;
      edm::InputTag electronInputTag_;
      edm::InputTag muonInputTag_;

      bool saveTTree_;


      edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelLocalTrack>> pixelLocalTrackToken_;
      edm::EDGetTokenT<edm::DetSetVector<TotemRPLocalTrack>> stripLocalTrackToken_;
      edm::EDGetTokenT<edm::DetSetVector<CTPPSDiamondLocalTrack>> diamondLocalTrackToken_;

      edm::EDGetTokenT<std::vector<pat::Jet> > pat_jet_token;
      edm::EDGetTokenT<std::vector<pat::Muon> > pat_muon_token;
      edm::EDGetTokenT<std::vector<pat::Electron> > pat_electron_token;
      //edm::EDGetTokenT<std::vector<reco::Vertex> > PV_token;

      edm::EDGetTokenT<std::vector<reco::PFJet> > jet_token;

      bool usePixel_;
      bool useStrip_;
      bool useDiamond_;


  /*edm::EDGetTokenT<pat::TriggerObjectStandAloneCollection> trigobjectsMINIAODToken_;
  edm::EDGetTokenT<edm::TriggerResults> trgresultsORIGToken_;
  edm::EDGetTokenT<trigger::TriggerEvent> trigobjectsRAWToken_;
  edm::EDGetTokenT<edm::TriggerResults>  trgresultsHLT2Token_;

  edm::EDGetTokenT<trigger::TriggerFilterObjectWithRefs> et_Filter_Token_;
  edm::EDGetTokenT<trigger::TriggerFilterObjectWithRefs> showershape_Filter_Token_;
  edm::EDGetTokenT<trigger::TriggerFilterObjectWithRefs> dphi_Filter_Token_;

  edm::EDGetTokenT<edm::AssociationMap<edm::OneToValue<std::vector<reco::RecoEcalCandidate>, float > > > showershape_Var_Token_;
  edm::EDGetTokenT<edm::AssociationMap<edm::OneToValue<std::vector<reco::RecoEcalCandidate>, float > > > hovere_Var_Token_;
  edm::EDGetTokenT<edm::AssociationMap<edm::OneToValue<std::vector<reco::RecoEcalCandidate>, float > > > trackiso_Var_Token_;// */

      edm::Service<TFileService> fs;
      TH1D* Arm1Xi_;
      TH1D* Arm2Xi_;
      TH1D* Arm1DijetXi_;
      TH1D* Arm2DijetXi_;
      TH1D* Arm1DileptonXi_;
      TH1D* Arm2DileptonXi_;
      TH1D* Arm1XiMatchedDijet_;
      TH1D* Arm2XiMatchedDijet_;
      TH1D* Arm1XiMatchedDilepton_;
      TH1D* Arm2XiMatchedDilepton_;

      TH2D* Arm1VsDijetXi_;
      TH2D* Arm2VsDijetXi_;
      TH2D* Arm1VsDileptonXi_;
      TH2D* Arm2VsDileptonXi_;
      TH2D* Arm1VsDijetXiMatched_;
      TH2D* Arm2VsDijetXiMatched_;
      TH2D* Arm1VsDileptonXiMatched_;
      TH2D* Arm2VsDileptonXiMatched_;

      TH1D* ProtonMass_;
      TH1D* ProtonRapidity_;
      TH2D* ProtonMassVsRapidity_;

      TH1D* ProtonMassMatchedDijet_;
      TH1D* ProtonRapidityMatchedDijet_;
      TH2D* ProtonMassVsRapidityMatchedDijet_;

      TH1D* ProtonMassMatchedDilepton_;
      TH1D* ProtonRapidityMatchedDilepton_;
      TH2D* ProtonMassVsRapidityMatchedDilepton_;

      TH1D* DijetMass_;
      TH1D* DijetRapidity_;
      TH2D* DijetMassVsRapidity_;

      TH1D* DileptonMass_;
      TH1D* DileptonRapidity_;
      TH2D* DileptonMassVsRapidity_;

      TH2D* MassVsDijet_;
      TH2D* RapidityVsDijet_;

      TH2D* MassVsDilepton_;
      TH2D* RapidityVsDilepton_;

      TH1D* DijetMassFraction_;
      TH1D* DijetRapidityDifference_;

      TH1D* DileptonMassFraction_;
      TH1D* DileptonRapidityDifference_;

      TFile* protonTFile_;
      TTree* protonTTree_;

      unsigned int protonTTreeArm_;
      double protonTTreeRawX_;
      double protonTTreeX_;
      double protonTTreeXi_;
      unsigned int protonTTreeDetector_;
      /*double diprotonArm1X_;
      double diprotonArm2X_;
      double diprotonArm1Xi_;
      double diprotonArm2Xi_;
      double diprotonMass_;
      double diprotonRapidity_;
      double dijetMass_;
      double dijetRapidity_;
      double dijetArm1Xi_;
      double dijetArm2Xi_;*/
};

void CTPPSXiAnalyzer::fillDescriptions(edm::ConfigurationDescriptions& descriptions)
{
  edm::ParameterSetDescription desc;

  desc.add<edm::InputTag>("pixelLocalTrackInputTag",   edm::InputTag("ctppsPixelLocalTracks"))
    ->setComment("input tag of the pixel local track collection");
  desc.add<edm::InputTag>("stripLocalTrackInputTag",   edm::InputTag("totemRPLocalTrackFitter"))
    ->setComment("input tag of the strip local track collection");
  desc.add<edm::InputTag>("diamondLocalTrackInputTag", edm::InputTag("ctppsDiamondLocalTracks"))
    ->setComment("input tag of the diamond local track collection");

  desc.add<unsigned int>("detectorBitset", static_cast<unsigned int>(1)) // TODO: Consider changing to 5 once the diamonds are implemented
    ->setComment("bitset of which detector types to consider: bit 1 -> pixel; bit 2 -> strips; bit 3 -> diamonds. eg. the value 1 will only consider the pixel detectors, the value 5 will consider the diamond and pixel detectors");

  desc.add<bool>("usePATObjects", false)
    ->setComment("whether to use the PAT collection of objects or not (particularly useful if using MINIAOD instead of RAW)"); // TODO: implement this when set to false

  desc.add<bool>("xiFromDijet", false)
    ->setComment("whether to reconstruct the xi values from the dijet system");

  desc.add<bool>("xiFromDilepton", false)
    ->setComment("whether to reconstruct the xi values from the dilepton system"); // TODO: implement this when set to true

  desc.add<std::string>("suffix", std::string(""))
    ->setComment("String to add to the end of the name of the plots produced");

  desc.add<double>("minXi", 0.0)
    ->setComment("The minimum number on the Xi axis of the plots");
  desc.add<double>("maxXi", 1.0)
    ->setComment("The maximum number on the Xi axis of the plots");
  desc.add<int>("binsXi", 100)
    ->setComment("The bins on the Xi axis of the plots");

  desc.add<double>("minMass", 100.0)
    ->setComment("The minimum number on the Mass axis of the plots");
  desc.add<double>("maxMass", 400.0)
    ->setComment("The maximum number on the Mass axis of the plots");
  desc.add<int>("binsMass", 100)
    ->setComment("The bins on the Mass axis of the plots");

  desc.add<double>("minRapidity", -1.8)
    ->setComment("The minimum number on the Rapidity axis of the plots");
  desc.add<double>("maxRapidity", 1.8)
    ->setComment("The maximum number on the Rapidity axis of the plots");
  desc.add<int>("binsRapidity", 100)
    ->setComment("The bins on the Rapidity axis of the plots");

  // "slimmedJets"
  desc.add<edm::InputTag>("jetInputTag",   edm::InputTag("ak4PFJets")) // TODO: check if the reco tag is the same as used online for the trigger (it is probably hltAK4PFJetsCorrected)
    ->setComment("input tag of the jet collection");
  desc.add<edm::InputTag>("electronInputTag",   edm::InputTag("slimmedElectrons")) // TODO: change default to the reco value, not the miniaod value
    ->setComment("input tag of the electron collection");
  desc.add<edm::InputTag>("muonInputTag",   edm::InputTag("slimmedMuons")) // TODO: change default to the reco value, not the miniaod value
    ->setComment("input tag of the muon collection");

  desc.add<bool>("saveTTree", false)
    ->setComment("whether to save the values in a ttree");

  descriptions.add("ctppsXiAnalyzer", desc);
  return;
}

CTPPSXiAnalyzer::CTPPSXiAnalyzer(const edm::ParameterSet& iConfig):
  suffix_                    (iConfig.getParameter< std::string   > ("suffix")),
  minXi_                     (iConfig.getParameter< double        > ("minXi")),
  maxXi_                     (iConfig.getParameter< double        > ("maxXi")),
  binsXi_                    (iConfig.getParameter< int           > ("binsXi")),
  minMass_                   (iConfig.getParameter< double        > ("minMass")),
  maxMass_                   (iConfig.getParameter< double        > ("maxMass")),
  binsMass_                  (iConfig.getParameter< int           > ("binsMass")),
  minRapidity_               (iConfig.getParameter< double        > ("minRapidity")),
  maxRapidity_               (iConfig.getParameter< double        > ("maxRapidity")),
  binsRapidity_              (iConfig.getParameter< int           > ("binsRapidity")),
  pixelLocalTrackInputTag_   (iConfig.getParameter< edm::InputTag > ("pixelLocalTrackInputTag")),
  stripLocalTrackInputTag_   (iConfig.getParameter< edm::InputTag > ("stripLocalTrackInputTag")),
  diamondLocalTrackInputTag_ (iConfig.getParameter< edm::InputTag > ("diamondLocalTrackInputTag")),
  detectorBitset_            (iConfig.getParameter< unsigned int  > ("detectorBitset")),
  usePATObjects_             (iConfig.getParameter< bool          > ("usePATObjects")),
  xiFromDijet_               (iConfig.getParameter< bool          > ("xiFromDijet")),
  xiFromDilepton_            (iConfig.getParameter< bool          > ("xiFromDilepton")),
  jetInputTag_               (iConfig.getParameter< edm::InputTag > ("jetInputTag")),
  electronInputTag_          (iConfig.getParameter< edm::InputTag > ("electronInputTag")),
  muonInputTag_              (iConfig.getParameter< edm::InputTag > ("muonInputTag")),
  saveTTree_                 (iConfig.getParameter< bool          > ("saveTTree")),
  usePixel_   (false),
  useStrip_   (false),
  useDiamond_ (false)
{
  usesResource("TFileService");

  if(detectorBitset_ & 1)
    usePixel_ = true;
  if(detectorBitset_ & 2)
    useStrip_ = true;
  if(detectorBitset_ & 4)
    useDiamond_ = true;

  if(usePixel_)
    pixelLocalTrackToken_   = consumes<edm::DetSetVector<CTPPSPixelLocalTrack>>(pixelLocalTrackInputTag_);
  if(useStrip_)
    stripLocalTrackToken_   = consumes<edm::DetSetVector<TotemRPLocalTrack>>(stripLocalTrackInputTag_);
  if(useDiamond_)
    diamondLocalTrackToken_ = consumes<edm::DetSetVector<CTPPSDiamondLocalTrack>>(diamondLocalTrackInputTag_);

  if(xiFromDijet_)
  {
    if(usePATObjects_)
    {
      pat_jet_token = consumes< std::vector<pat::Jet> >( jetInputTag_ );
    }
    else
    {
      jet_token = consumes< std::vector<reco::PFJet> >( jetInputTag_ );
    }
  }

  if(xiFromDilepton_)
  {
    if(usePATObjects_)
    {
      pat_muon_token = consumes<std::vector<pat::Muon> >( muonInputTag_ );
      pat_electron_token = consumes<std::vector<pat::Electron> >( electronInputTag_ );
    }
    else
    {}
  }

  //PV_token = consumes<std::vector<reco::Vertex> > (edm::InputTag("offlineSlimmedPrimaryVertices"));

  auto buildName = [](std::string base, std::string suffix = "") -> std::string
  {
    if(suffix == "")
      return base;
    else
      return base + "_" + suffix;
  };

  if(detectorBitset_ != 0)
  {
    Arm1Xi_ = fs->make<TH1D>(buildName("h_arm1xi", suffix_).c_str(),"Arm1 Xi;#xi_{proton};Events", binsXi_, minXi_, maxXi_);
    Arm2Xi_ = fs->make<TH1D>(buildName("h_arm2xi", suffix_).c_str(),"Arm2 Xi;#xi_{proton};Events", binsXi_, minXi_, maxXi_);

    ProtonMass_           = fs->make<TH1D>(buildName("h_protonMass", suffix_).c_str(),"Proton Mass;M_{proton};Events", binsMass_, minMass_, maxMass_);
    ProtonRapidity_       = fs->make<TH1D>(buildName("h_protonRapidity", suffix_).c_str(),"Proton Rapidity;y_{proton};Events", binsRapidity_, minRapidity_, maxRapidity_);
    ProtonMassVsRapidity_ = fs->make<TH2D>(buildName("h_proton_massVSrapidity", suffix_).c_str(),"Proton Mass vs Rapidity;M_{proton};y_{proton}", binsMass_, minMass_, maxMass_, binsRapidity_, minRapidity_, maxRapidity_);

    ProtonMassMatchedDijet_           = fs->make<TH1D>(buildName("h_protonMass_matched", suffix_).c_str(),"Proton Mass;M_{proton};Events", binsMass_, minMass_, maxMass_);
    ProtonRapidityMatchedDijet_       = fs->make<TH1D>(buildName("h_protonRapidity_matched", suffix_).c_str(),"Proton Rapidity;y_{proton};Events", binsRapidity_, minRapidity_, maxRapidity_);
    ProtonMassVsRapidityMatchedDijet_ = fs->make<TH2D>(buildName("h_proton_massVSrapidity_matched", suffix_).c_str(),"Proton Mass vs Rapidity;M_{proton};y_{proton}", binsMass_, minMass_, maxMass_, binsRapidity_, minRapidity_, maxRapidity_);
  }

  if(xiFromDijet_)
  {
    Arm1XiMatchedDijet_ = fs->make<TH1D>(buildName("h_arm1xi_matcheddijet", suffix_).c_str(),"Arm1 Xi;#xi_{proton};Events", binsXi_, minXi_, maxXi_);
    Arm2XiMatchedDijet_ = fs->make<TH1D>(buildName("h_arm2xi_matcheddijet", suffix_).c_str(),"Arm2 Xi;#xi_{proton};Events", binsXi_, minXi_, maxXi_);

    Arm1DijetXi_ = fs->make<TH1D>(buildName("h_arm1dijetxi", suffix_).c_str(),"Arm1 Dijet Xi;#xi_{dijet};Events", binsXi_, minXi_, maxXi_);
    Arm2DijetXi_ = fs->make<TH1D>(buildName("h_arm2dijetxi", suffix_).c_str(),"Arm2 Dijet Xi;#xi_{dijet};Events", binsXi_, minXi_, maxXi_);

    Arm1VsDijetXi_ = fs->make<TH2D>(buildName("h_arm1vsdijetxi", suffix_).c_str(), "Arm1 Xi Vs Dijet Xi;#xi_{proton};#xi_{dijet}", binsXi_, minXi_, maxXi_, binsXi_, minXi_, maxXi_);
    Arm2VsDijetXi_ = fs->make<TH2D>(buildName("h_arm2vsdijetxi", suffix_).c_str(), "Arm2 Xi Vs Dijet Xi;#xi_{proton};#xi_{dijet}", binsXi_, minXi_, maxXi_, binsXi_, minXi_, maxXi_);

    Arm1VsDijetXiMatched_ = fs->make<TH2D>(buildName("h_arm1vsdijetxi_matched", suffix_).c_str(), "Arm1 Xi Vs Dijet Xi;#xi_{proton};#xi_{dijet}", binsXi_, minXi_, maxXi_, binsXi_, minXi_, maxXi_);
    Arm2VsDijetXiMatched_ = fs->make<TH2D>(buildName("h_arm2vsdijetxi_matched", suffix_).c_str(), "Arm2 Xi Vs Dijet Xi;#xi_{proton};#xi_{dijet}", binsXi_, minXi_, maxXi_, binsXi_, minXi_, maxXi_);

    DijetMass_           = fs->make<TH1D>(buildName("h_dijetMass", suffix_).c_str(), "Dijet Mass;M_{dijet};Events", binsMass_, minMass_, maxMass_);
    DijetRapidity_       = fs->make<TH1D>(buildName("h_dijetRapidity", suffix_).c_str(), "Dijet Rapidity;y_{dijet};Events", binsRapidity_, minRapidity_, maxRapidity_);
    DijetMassVsRapidity_ = fs->make<TH2D>(buildName("h_dijet_massVSrapidity", suffix_).c_str(),"Dijet Mass vs Rapidity;M_{dijet};y_{dijet}", binsMass_, minMass_, maxMass_, binsRapidity_, minRapidity_, maxRapidity_);

    MassVsDijet_ = fs->make<TH2D>(buildName("h_massVSdijet", suffix_).c_str(), "Mass;M_{proton};M_{dijet}", binsMass_, minMass_, maxMass_, binsMass_, minMass_, maxMass_);
    RapidityVsDijet_ = fs->make<TH2D>(buildName("h_rapidityVSdijet", suffix_).c_str(), "Rapidity;y_{proton};y_{dijet}", binsRapidity_, minRapidity_, maxRapidity_, binsRapidity_, minRapidity_, maxRapidity_);

    DijetMassFraction_ = fs->make<TH1D>(buildName("h_dijetMassFraction", suffix_).c_str(), "Mass Fraction;M_{dijet}/M_{proton};Events", 100, 0.0, 2.0);
    DijetRapidityDifference_ = fs->make<TH1D>(buildName("h_dijetRapidityDifference", suffix_).c_str(), "Rapidity Difference;y_{dijet}-y_{proton}", 100, -1.0, 1.0);
  }

  if(xiFromDilepton_)
  {
    Arm1XiMatchedDilepton_ = fs->make<TH1D>(buildName("h_arm1xi_matcheddilepton", suffix_).c_str(),"Arm1 Xi;#xi_{proton};Events", binsXi_, minXi_, maxXi_);
    Arm2XiMatchedDilepton_ = fs->make<TH1D>(buildName("h_arm2xi_matcheddilepton", suffix_).c_str(),"Arm2 Xi;#xi_{proton};Events", binsXi_, minXi_, maxXi_);

    Arm1DileptonXi_ = fs->make<TH1D>(buildName("h_arm1dileptonxi", suffix_).c_str(),"Arm1 Dilepton Xi;#xi_{dilepton};Events", binsXi_, minXi_, maxXi_);
    Arm2DileptonXi_ = fs->make<TH1D>(buildName("h_arm2dileptonxi", suffix_).c_str(),"Arm2 Dilepton Xi;#xi_{dilepton};Events", binsXi_, minXi_, maxXi_);

    Arm1VsDileptonXi_ = fs->make<TH2D>(buildName("h_arm1vsdileptonxi", suffix_).c_str(), "Arm1 Xi Vs Dilepton Xi;#xi_{proton};#xi_{dilepton}", binsXi_, minXi_, maxXi_, binsXi_, minXi_, maxXi_);
    Arm2VsDileptonXi_ = fs->make<TH2D>(buildName("h_arm2vsdileptonxi", suffix_).c_str(), "Arm2 Xi Vs Dilepton Xi;#xi_{proton};#xi_{dilepton}", binsXi_, minXi_, maxXi_, binsXi_, minXi_, maxXi_);

    Arm1VsDileptonXiMatched_ = fs->make<TH2D>(buildName("h_arm1vsdileptonxi_matched", suffix_).c_str(), "Arm1 Xi Vs Dilepton Xi;#xi_{proton};#xi_{dilepton}", binsXi_, minXi_, maxXi_, binsXi_, minXi_, maxXi_);
    Arm2VsDileptonXiMatched_ = fs->make<TH2D>(buildName("h_arm2vsdileptonxi_matched", suffix_).c_str(), "Arm2 Xi Vs Dilepton Xi;#xi_{proton};#xi_{dilepton}", binsXi_, minXi_, maxXi_, binsXi_, minXi_, maxXi_);

    DileptonMass_           = fs->make<TH1D>(buildName("h_dileptonMass", suffix_).c_str(), "Dijet Mass;M_{dilepton};Events", binsMass_, minMass_, maxMass_);
    DileptonRapidity_       = fs->make<TH1D>(buildName("h_dileptonRapidity", suffix_).c_str(), "Dijet Rapidity;y_{dilepton};Events", binsRapidity_, minRapidity_, maxRapidity_);
    DileptonMassVsRapidity_ = fs->make<TH2D>(buildName("h_dilepton_massVSrapidity", suffix_).c_str(),"Dijet Mass vs Rapidity;M_{dilepton};y_{dilepton}", binsMass_, minMass_, maxMass_, binsRapidity_, minRapidity_, maxRapidity_);

    MassVsDilepton_ = fs->make<TH2D>(buildName("h_massVSdilepton", suffix_).c_str(), "Mass;M_{proton};M_{dilepton}", binsMass_, minMass_, maxMass_, binsMass_, minMass_, maxMass_);
    RapidityVsDilepton_ = fs->make<TH2D>(buildName("h_rapidityVSdilepton", suffix_).c_str(), "Rapidity;y_{proton};y_{dilepton}", binsRapidity_, minRapidity_, maxRapidity_, binsRapidity_, minRapidity_, maxRapidity_);

    DileptonMassFraction_ = fs->make<TH1D>(buildName("h_dileptonMassFraction", suffix_).c_str(), "Mass Fraction;M_{dilepton}/M_{proton};Events", 100, 0.0, 2.0);
    DileptonRapidityDifference_ = fs->make<TH1D>(buildName("h_dileptonRapidityDifference", suffix_).c_str(), "Rapidity Difference;y_{dilepton}-y_{proton}", 100, -1.0, 1.0);
  }

  if(saveTTree_)
  {
    TDirectory* cwd = gDirectory;

    if(detectorBitset_ != 0)
    {
      //protonTTree_ = fs->makeAndRegister<TTree>(buildName("protonTTree", suffix_).c_str(), "Proton TTree");
      TDirectory* outDir = fs->getBareDirectory();
      outDir->cd();
      //protonTFile_ = new TFile("protons.root", "RECREATE");
      //protonTFile_->cd();
      protonTTree_ = new TTree(buildName("protonTTree", suffix_).c_str(), "Proton TTree");

      protonTTree_->Branch("detector_id", &protonTTreeDetector_);
      protonTTree_->Branch("arm_id", &protonTTreeArm_);
      protonTTree_->Branch("raw_x", &protonTTreeRawX_);
      protonTTree_->Branch("x", &protonTTreeX_);
      protonTTree_->Branch("xi", &protonTTreeXi_);
    }

    if(xiFromDijet_)
    {}

    if(xiFromDilepton_)
    {}

    cwd->cd();
  }



  std::stringstream message;
  message << "CTPPSXiAnalyzer: ";
  if(xiFromDijet_ || xiFromDilepton_)
  {
    message << "PAT collections - " << ((usePATObjects_)?("True"):("False"));
    if(xiFromDijet_)
      message << "; using dijet - " << jetInputTag_.encode();
    if(xiFromDilepton_)
      message << "; using dilepton, electron - " << electronInputTag_.encode() << "; muon - " << muonInputTag_.encode();
  }
  else
  {
    message << "None";
  }

  LogDebug("") << message.str();

  return;
}

CTPPSXiAnalyzer::~CTPPSXiAnalyzer()=default;

void CTPPSXiAnalyzer::endJob()
{
  if(saveTTree_)
  {
    TDirectory* cwd = gDirectory;

    if(detectorBitset_ != 0)
    {
      TDirectory* outDir = fs->getBareDirectory();
      outDir->cd();
      //protonTFile_->cd();
      protonTTree_->Write();
      //protonTFile_->Write();
      //protonTFile_->Close();
    }

    cwd->cd();
  }

  return;
}

struct protonReco
{
  size_t arm1Idx;
  size_t arm2Idx;
  double xi1;
  double xi2;
  double mass;
  double rapidity;
};

void CTPPSXiAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  std::vector<double> arm1Xis;
  std::vector<double> arm2Xis;
  std::vector<protonReco> protonPairs;
  double arm1DijetXi = -1;
  double arm2DijetXi = -1;
  double jetMass = -1;
  double jetRapidity = -999;
  //double arm1DileptonXi = -1;
  //double arm2DileptonXi = -1;

  if(usePixel_) // Pixels correspond to RP 220 in 2017 data
  {
    edm::Handle<edm::DetSetVector<CTPPSPixelLocalTrack>> pixelTracks;
    iEvent.getByToken(pixelLocalTrackToken_, pixelTracks);

    for(const auto &rpv : (*pixelTracks))
    {
      CTPPSPixelDetId id(rpv.id);

      for(auto & track : rpv)
      {
        if(track.isValid())
        {
          double x_mm = track.getX0();
          // Here apply corrections to x
          x_mm = x_mm + (-42.05);

          double xi = (x_mm*0.001)/7.5; // Convert to meters and use nominal optics x = D * xi, D ~ 7.5

          if(saveTTree_)
          {
            protonTTreeRawX_ = track.getX0();
            protonTTreeX_ = x_mm;
            protonTTreeXi_ = xi;
            protonTTreeArm_ = static_cast<unsigned int>(id.arm());
            protonTTreeDetector_ = 1;

            protonTTree_->Fill();
          }

          if(x_mm < 0)
            continue;

          if(id.arm() == 0)
          {
            arm1Xis.push_back(xi);
            Arm1Xi_->Fill(xi);
          }
          if(id.arm() == 1)
          {
            arm2Xis.push_back(xi);
            Arm2Xi_->Fill(xi);
          }
        }
      }
    }
  }

  if(useStrip_) // Strips correspond to RP 210 in 2017 data
  {
    edm::Handle<edm::DetSetVector<TotemRPLocalTrack>> stripTracks;
    iEvent.getByToken(stripLocalTrackToken_, stripTracks);

    for(const auto &rpv : (*stripTracks))
    {
      TotemRPDetId id(rpv.id);

      for(auto & track : rpv)
      {
        if(track.isValid())
        {
          double x_mm = track.getX0();
          // Here apply corrections to x
          if(id.arm() == 0) // arm45
            x_mm = x_mm + (-3.7);
          if(id.arm() == 1) // arm56
            x_mm = x_mm + (-2.75);

          double xi = (x_mm*0.001)/7.5; // Convert to meters and use nominal optics x = D * xi, D ~ 7.5

          if(saveTTree_)
          {
            protonTTreeRawX_ = track.getX0();
            protonTTreeX_ = x_mm;
            protonTTreeXi_ = xi;
            protonTTreeArm_ = static_cast<unsigned int>(id.arm());
            protonTTreeDetector_ = 2;

            protonTTree_->Fill();
          }

          if(x_mm < 0)
            continue;

          if(id.arm() == 0)
          {
            arm1Xis.push_back(xi);
            Arm1Xi_->Fill(xi);
          }
          if(id.arm() == 1)
          {
            arm2Xis.push_back(xi);
            Arm2Xi_->Fill(xi);
          }
        }
      }
    }
  }

  if(useDiamond_)
  {
    edm::Handle<edm::DetSetVector<CTPPSDiamondLocalTrack>> diamondTracks;
    iEvent.getByToken(diamondLocalTrackToken_, diamondTracks);

    for(const auto &rpv : (*diamondTracks))
    {
      CTPPSDiamondDetId id(rpv.id);

      for(auto & track : rpv)
      {
        if(track.isValid())
        {
          double x_mm = track.getX0();
          // Here apply corrections to x
          double xi = (x_mm*0.001)/7.5; // Convert to meters and use nominal optics x = D * xi, D ~ 7.5

          if(saveTTree_)
          {
            protonTTreeRawX_ = track.getX0();
            protonTTreeX_ = x_mm;
            protonTTreeXi_ = xi;
            protonTTreeArm_ = static_cast<unsigned int>(id.arm());
            protonTTreeDetector_ = 3;

            protonTTree_->Fill();
          }

          if(id.arm() == 0)
          {
            arm1Xis.push_back(xi);
            Arm1Xi_->Fill(xi);
          }
          if(id.arm() == 1)
          {
            arm2Xis.push_back(xi);
            Arm2Xi_->Fill(xi);
          }
        }
      }
    }
  }

  for(size_t i = 0; i < arm1Xis.size(); ++i)
  {
    double xi1 = arm1Xis[i];
    for(size_t j = 0; j < arm2Xis.size(); ++j)
    {
      double xi2 = arm2Xis[j];

      protonReco tmp;
      tmp.arm1Idx = i;
      tmp.arm2Idx = j;
      tmp.xi1 = xi1;
      tmp.xi2 = xi2;
      tmp.mass = std::sqrt(xi1*xi2)*13000.0;
      tmp.rapidity = std::log(xi1/xi2)/2.0;

      ProtonMass_->Fill(tmp.mass);
      ProtonRapidity_->Fill(tmp.rapidity);
      ProtonMassVsRapidity_->Fill(tmp.mass, tmp.rapidity);

      protonPairs.push_back(std::move(tmp));
    }
  }

  if(xiFromDijet_)
  {
    if(usePATObjects_)
    {
      edm::Handle< std::vector<pat::Jet> > jets;
      iEvent.getByToken(pat_jet_token, jets);
      if(jets->size() >= 2)
      {
        pat::Jet jet1 = (*jets)[0];
        pat::Jet jet2 = (*jets)[1];

        TLorentzVector jet1_vec(jet1.px(), jet1.py(), jet1.pz(), jet1.pt());
        TLorentzVector jet2_vec(jet2.px(), jet2.py(), jet2.pz(), jet2.pt());
        TLorentzVector system = jet1_vec + jet2_vec;

        double mass = system.M();
        double rapidity = system.Rapidity();

        double xi1 = std::sqrt(mass*mass * std::exp(2.0 * rapidity) / (13000.0*13000.0));
        double xi2 = xi1 * std::exp(-2.0 * rapidity);

        Arm1DijetXi_->Fill(xi1);
        Arm2DijetXi_->Fill(xi2);

        arm1DijetXi = xi1;
        arm2DijetXi = xi2;
        jetMass = mass;
        jetRapidity = rapidity;
      }
    }
    else
    {
      edm::Handle< std::vector<reco::PFJet> > jets;
      iEvent.getByToken(jet_token, jets);

      if(jets->size() >= 2)
      {
        reco::Jet jet1 = (*jets)[0];
        reco::Jet jet2 = (*jets)[1];

        TLorentzVector jet1_vec(jet1.px(), jet1.py(), jet1.pz(), jet1.pt());
        TLorentzVector jet2_vec(jet2.px(), jet2.py(), jet2.pz(), jet2.pt());
        TLorentzVector system = jet1_vec + jet2_vec;

        double mass = system.M();
        double rapidity = system.Rapidity();

        double xi1 = std::sqrt(mass*mass * std::exp(2.0 * rapidity) / (13000.0*13000.0));
        double xi2 = xi1 * std::exp(-2.0 * rapidity);

        Arm1DijetXi_->Fill(xi1);
        Arm2DijetXi_->Fill(xi2);

        arm1DijetXi = xi1;
        arm2DijetXi = xi2;
        jetMass = mass;
        jetRapidity = rapidity;
      }
    }

    if(arm1DijetXi > 0)
    {
      double closestXi = -2;
      for(auto& xi: arm1Xis)
      {
        if(std::abs(xi - arm1DijetXi) < std::abs(closestXi - arm1DijetXi))
          closestXi = xi;
      }
      if(closestXi > 0)
        Arm1VsDijetXi_->Fill(closestXi, arm1DijetXi);
    }

    if(arm2DijetXi > 0)
    {
      double closestXi = -2;
      for(auto& xi: arm2Xis)
      {
        if(std::abs(xi - arm2DijetXi) < std::abs(closestXi - arm2DijetXi))
          closestXi = xi;
      }
      if(closestXi > 0)
        Arm2VsDijetXi_->Fill(closestXi, arm2DijetXi);
    }

    if(jetMass > 0)
    {
      DijetMass_->Fill(jetMass);
      DijetRapidity_->Fill(jetRapidity);
      DijetMassVsRapidity_->Fill(jetMass, jetRapidity);

      int match = -1;
      double minDistance = 0;
      for(size_t i = 0; i < protonPairs.size(); ++i)
      {
        double protonMass = protonPairs[i].mass;
        double protonRapidity = protonPairs[i].rapidity;

        double massDifference = jetMass - protonMass;
        double rapidityDifference = jetRapidity - protonRapidity;
        double distance = std::sqrt(std::pow(massDifference, 2) - std::pow(rapidityDifference, 2));

        if(match == -1)
        {
          minDistance = distance;
          match = i;
        }
        else
        {
          if(distance < minDistance)
          {
            minDistance = distance;
            match = i;
          }
        }
      }

      if(match > -1)
      {
        ProtonMassMatchedDijet_->Fill(protonPairs[match].mass);
        ProtonRapidityMatchedDijet_->Fill(protonPairs[match].rapidity);
        ProtonMassVsRapidityMatchedDijet_->Fill(protonPairs[match].mass, protonPairs[match].rapidity);

        Arm1XiMatchedDijet_->Fill(protonPairs[match].xi1);
        Arm2XiMatchedDijet_->Fill(protonPairs[match].xi2);

        Arm1VsDijetXiMatched_->Fill(protonPairs[match].xi1, arm1DijetXi);
        Arm2VsDijetXiMatched_->Fill(protonPairs[match].xi2, arm2DijetXi);

        MassVsDijet_->Fill(protonPairs[match].mass, jetMass);
        RapidityVsDijet_->Fill(protonPairs[match].rapidity, jetRapidity);

        DijetMassFraction_->Fill(jetMass/protonPairs[match].mass);
        DijetRapidityDifference_->Fill(jetRapidity - protonPairs[match].rapidity);
      }
    }
  }

  return;
}



//define this as a plug-in
DEFINE_FWK_MODULE(CTPPSXiAnalyzer);
