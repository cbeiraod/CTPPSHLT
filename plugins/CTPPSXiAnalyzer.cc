
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
      //virtual void endJob() override;

      // ----------member data ---------------------------

      std::string suffix_;
      double min_;
      double max_;
      int bins_;

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

  desc.add<std::string>("suffix", std::string(""))
    ->setComment("String to add to the end of the name of the plots produced");

  desc.add<double>("min", 0.0)
    ->setComment("The minimum number on the axis of the plots");
  desc.add<double>("max", 0.0)
    ->setComment("The maximum number on the axis of the plots");
  desc.add<int>("bins", 0.0)
    ->setComment("The bins on the axis of the plots");

  // "slimmedJets"
  desc.add<edm::InputTag>("jetInputTag",   edm::InputTag("ak4PFJets")) // TODO: check if the reco tag is the same as used online for the trigger (it is probably hltAK4PFJetsCorrected)
    ->setComment("input tag of the jet collection");
  desc.add<edm::InputTag>("electronInputTag",   edm::InputTag("slimmedElectrons")) // TODO: change default to the reco value, not the miniaod value
    ->setComment("input tag of the electron collection");
  desc.add<edm::InputTag>("muonInputTag",   edm::InputTag("slimmedMuons")) // TODO: change default to the reco value, not the miniaod value
    ->setComment("input tag of the muon collection");

  descriptions.add("ctppsXiAnalyzer", desc);
  return;
}

CTPPSXiAnalyzer::CTPPSXiAnalyzer(const edm::ParameterSet& iConfig):
  suffix_                    (iConfig.getParameter< std::string   > ("suffix")),
  min_                       (iConfig.getParameter< double        > ("min")),
  max_                       (iConfig.getParameter< double        > ("max")),
  bins_                      (iConfig.getParameter< int           > ("bins")),
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

  if(detectorBitset_ != 0)
  {
    Arm1Xi_ = fs->make<TH1D>("h_arm1xi","Arm1 Xi;Xi;Events", bins_, min_ max_);
    Arm2Xi_ = fs->make<TH1D>("h_arm2xi","Arm2 Xi;Xi;Events", bins_, min_ max_);
  }

  if(xiFromDijet_)
  {
    Arm1DijetXi_ = fs->make<TH1D>("h_arm1dijetxi","Arm1 Dijet Xi;Xi;Events", bins_, min_ max_);
    Arm2DijetXi_ = fs->make<TH1D>("h_arm2dijetxi","Arm2 Dijet Xi;Xi;Events", bins_, min_ max_);

    Arm1VsDijetXi_ = fs->make<TH2D>("h_arm1vsdijetxi", "Arm1 Xi Vs Dijet Xi;xi_{proton};xi_{dijet}", bins_, min_ max_, bins_, min_ max_);
    Arm2VsDijetXi_ = fs->make<TH2D>("h_arm2vsdijetxi", "Arm2 Xi Vs Dijet Xi;xi_{proton};xi_{dijet}", bins_, min_ max_, bins_, min_ max_);
  }

  if(xiFromDilepton_)
  {
    Arm1DileptonXi_ = fs->make<TH1D>("h_arm1dileptonxi","Arm1 Dilepton Xi;Xi;Events", bins_, min_ max_);
    Arm2DileptonXi_ = fs->make<TH1D>("h_arm2dileptonxi","Arm2 Dilepton Xi;Xi;Events", bins_, min_ max_);

    Arm1VsDileptonXi_ = fs->make<TH2D>("h_arm1vsdileptonxi", "Arm1 Xi Vs Dilepton Xi;xi_{proton};xi_{dilepton}", bins_, min_ max_, bins_, min_ max_);
    Arm2VsDileptonXi_ = fs->make<TH2D>("h_arm2vsdileptonxi", "Arm2 Xi Vs Dilepton Xi;xi_{proton};xi_{dilepton}", bins_, min_ max_, bins_, min_ max_);
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

CTPPSXiAnalyzer::~CTPPSXiAnalyzer()= default;

void CTPPSXiAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  std::vector<double> arm1Xis;
  std::vector<double> arm2Xis;
  double arm1DijetXi = -1;
  double arm2DijetXi = -1;
  //double arm1DileptonXi = -1;
  //double arm2DileptonXi = -1;

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
          double x_mm = track.getX0();
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
          double x_mm = track.getX0();
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
          double x_mm = track.getX0();
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
  }

  return;
}



//define this as a plug-in
DEFINE_FWK_MODULE(CTPPSXiAnalyzer);
