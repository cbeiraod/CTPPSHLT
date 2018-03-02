import os,subprocess,ROOT
import pickle
import re
import time
import shutil

def assure_path_exists(path):
  dir = os.path.dirname(path)
  if not os.path.exists(dir):
    os.makedirs(dir)

if __name__ == "__main__":
  import argparse
  import os
  import sys

  parser = argparse.ArgumentParser(description='Process the command line options')
  parser.add_argument('-d', '--dryRun', action='store_true', help='Do a dry run (i.e. do not actually run the potentially dangerous commands but print them to the screen)')
  parser.add_argument('-v', '--verbose', action='store_true', help='Whether to print verbose output')
  parser.add_argument('-o', '--outDirectory', required=True, help='Name of the output directory')
  parser.add_argument('-i', '--inputFile', required=True, help='Name of the input file')

  args = parser.parse_args()

  toPlot = {
    "all": "ctppsAll",
    "coinc": "ctppsCoincidince",
    "l1": "ctppsCoincidenceL1Double",
    "full": "ctppsDiPFJetAve260CoincidenceDouble",
  }

  plots = {
    "Arm1_Xi": {
      "name": "h_arm1xi",
      "logy": False,
      "logz": False,
      "opt": "",
    },
    "Arm2_Xi": {
      "name": "h_arm2xi",
      "logy": False,
      "logz": False,
      "opt": "",
    },
    "Proton_Mass": {
      "name": "h_protonMass",
      "logy": False,
      "logz": False,
      "opt": "",
    },
    "Proton_Rapidity": {
      "name": "h_protonRapidity",
      "logy": False,
      "logz": False,
      "opt": "",
    },
    "Proton_MassVRapidity": {
      "name": "h_proton_massVSrapidity",
      "logy": False,
      "logz": False,
      "opt": "colz",
    },
    "Dijet_Arm1_Xi": {
      "name": "h_arm1dijetxi",
      "logy": True,
      "logz": False,
      "opt": "",
    },
    "Dijet_Arm2_Xi": {
      "name": "h_arm2dijetxi",
      "logy": True,
      "logz": False,
      "opt": "",
    },
    "Dijet_Mass": {
      "name": "h_dijetMass",
      "logy": False,
      "logz": False,
      "opt": "",
    },
    "Dijet_Rapidity": {
      "name": "h_dijetRapidity",
      "logy": False,
      "logz": False,
      "opt": "",
    },
    "Dijet_MassVRapidity": {
      "name": "h_dijet_massVSrapidity",
      "logy": False,
      "logz": False,
      "opt": "colz",
    },
    "Arm1_Xi_DijetVProton": {
      "name": "h_arm1vsdijetxi_matched",
      "logy": False,
      "logz": False,
      "opt": "colz",
    },
    "Arm2_Xi_DijetVProton": {
      "name": "h_arm2vsdijetxi_matched",
      "logy": False,
      "logz": False,
      "opt": "colz",
    },
    "Mass_DijetVProton": {
      "name": "h_massVSdijet",
      "logy": False,
      "logz": False,
      "opt": "colz",
    },
    "Rapidity_DijetVProton": {
      "name": "h_rapidityVSdijet",
      "logy": False,
      "logz": False,
      "opt": "colz",
    },
    "DijetMassFraction": {
      "name": "h_dijetMassFraction",
      "logy": False,
      "logz": False,
      "opt": "",
    },
    "DijetRapidityDifference": {
      "name": "h_dijetRapidityDifference",
      "logy": False,
      "logz": False,
      "opt": "",
    },
  }

  import tdrstyle

  file = ROOT.TFile.Open(args.inputFile, "READ")
  assure_path_exists(args.outDirectory + "/dummy")
  if args.verbose:
    print file

  for condition in toPlot:
    plotDirectory = file.Get(toPlot[condition])
    if args.verbose:
      print "  ", plotDirectory
    for plotName in plots:
      hist = plotDirectory.Get(plots[plotName]["name"])
      if args.verbose:
        print "    ", plot

      canvas = ROOT.TCanvas(plotName + "_canvas", plotName + "_canvas", 800, 450)
      if plots[plotName]["logy"]:
        canvas.SetLogy()
      if plots[plotName]["logz"]:
        canvas.SetLogz()
      canvas.SetRightMargin(0.08)
      canvas.SetLeftMargin(0.08)

      if plots[plotName]["opt"] is not "":
        hist.Draw(plots[plotName]["opt"])
      else:
        hist.Draw()

      canvas.Update()
      canvas.SaveAs(args.outDirectory + "/" + condition + "_" + plotName + ".png")
      canvas.SaveAs(args.outDirectory + "/" + condition + "_" + plotName + ".pdf")
