
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
      virtual void beginJob() override;
      virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      virtual void endJob() override;

      // ----------member data ---------------------------

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


      edm::EDGetTokenT<edm::DetSetVector<CTPPSPixelLocalTrack>> pixelLocalTrackToken_;
      edm::EDGetTokenT<edm::DetSetVector<TotemRPLocalTrack>> stripLocalTrackToken_;
      edm::EDGetTokenT<edm::DetSetVector<CTPPSDiamondLocalTrack>> diamondLocalTrackToken_;

      edm::EDGetTokenT<std::vector<pat::Jet> > pat_jet_token;
      edm::EDGetTokenT<std::vector<pat::Muon> > pat_muon_token;
      edm::EDGetTokenT<std::vector<pat::Electron> > pat_electron_token;
      //edm::EDGetTokenT<std::vector<reco::Vertex> > PV_token;

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

      TH2D* Arm1VsDijetXi_;
      TH2D* Arm2VsDijetXi_;
      TH2D* Arm1VsDileptonXi_;
      TH2D* Arm2VsDileptonXi_;

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

  desc.add<edm::InputTag>("jetInputTag",   edm::InputTag("slimmedJets")) // TODO: change default to the reco value, not the miniaod value
    ->setComment("input tag of the jet collection");
  desc.add<edm::InputTag>("electronInputTag",   edm::InputTag("slimmedElectrons")) // TODO: change default to the reco value, not the miniaod value
    ->setComment("input tag of the electron collection");
  desc.add<edm::InputTag>("muonInputTag",   edm::InputTag("slimmedMuons")) // TODO: change default to the reco value, not the miniaod value
    ->setComment("input tag of the muon collection");

  descriptions.add("ctppsXiAnalyzer", desc);
  return;
}

CTPPSXiAnalyzer::CTPPSXiAnalyzer(const edm::ParameterSet& iConfig):
  EDAnalyzer(iConfig),
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
  usePixel_   (false),
  useStrip_   (false),
  useDiamond_ (false)
{
  usesResource("TFileService");

  if(detectorBitset_ | 1)
    usePixel_ = true;
  if(detectorBitset_ | 2)
    useStrip_ = true;
  if(detectorBitset_ | 4)
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
    {}
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


  Arm1Xi_ = fs->make<TH1D>("h_arm1xi","Arm1 Xi;Xi;Events",100,0,1);
  Arm2Xi_ = fs->make<TH1D>("h_arm2xi","Arm2 Xi;Xi;Events",100,0,1);



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

CTPPSXiAnalyzer::~CTPPSXiAnalyzer()= default;

void CTPPSXiAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  std::vector<double> arm1Xis;
  std::vector<double> arm2Xis;

  if(usePixel_)
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
          double x_mm = track.getX();
          // Here apply corrections to x
          double xi = (x_mm*0.001)/7.5; // Convert to meters and use nominal optics x = D * xi, D ~

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

  if(useStrip_)
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
          double x_mm = track.getX();
          // Here apply corrections to x
          double xi = (x_mm*0.001)/7.5; // Convert to meters and use nominal optics x = D * xi, D ~

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
          double x_mm = track.getX();
          // Here apply corrections to x
          double xi = (x_mm*0.001)/7.5; // Convert to meters and use nominal optics x = D * xi, D ~

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

  return;
}



//define this as a plug-in
DEFINE_FWK_MODULE(CTPPSXiAnalyzer);
