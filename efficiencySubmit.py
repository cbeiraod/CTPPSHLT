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
  parser.add_argument('-q', '--queue', default="1nd", help='On which queue to submit the job')
  parser.add_argument('--cmssw', default="/afs/cern.ch/work/c/cbeiraod/CT-PPS/TestCTPPSTrig/CMSSW_9_4_0_pre3/src", help='directory of the CMSSW environment')
  parser.add_argument('--setup', default="/afs/cern.ch/work/c/cbeiraod/CT-PPS", help="Directory where the setup file is located")
  parser.add_argument('-o', '--outDirectory', required=True, help='Name of the output directory')

  args = parser.parse_args()

  if not args.dryRun:
    print "You did not enable dry run. You are on your own!"

  inputFiles = {
    "base": "list_cff.py",
    "double": "list_cff2.py",
    "longer1": "list_cff3.py",
    "longer2": "list_cff4.py",
  }

  submitOrder = [
                  "base",
                  "double",
                  "longer1",
                  "longer2",
                ]

  maxTracks = {
    "allTracks": -1,
    "1Track": 1,
    "2Tracks": 2,
    "4Tracks": 4,
    "6Tracks": 6,
    "8Tracks": 8,
    "10Tracks": 10,
  }

  for fileSet in submitOrder:
    assure_path_exists(args.outDirectory + "/" + fileSet + "/dummy")
    for trackSet in maxTracks:
      basePath = os.path.realpath(args.outDirectory + "/" + fileSet + "/" + trackSet)
      logDirectory = os.path.realpath(os.getcwd() + "/logs/" + fileSet + "/" + trackSet)
      assure_path_exists(basePath + "/dummy")
      assure_path_exists(logDirectory + "/dummy")

      with open(basePath + "/job.sh", 'w') as jobFile:
        jobFile.write("#!/bin/bash\n\n")

        jobFile.write("echo \"Changing to the setup directory\"\n")
        jobFile.write("cd " + os.path.realpath(args.setup) + "\n\n")

        jobFile.write("echo \"Running setup\"\n")
        jobFile.write(". setup.sh\n\n")

        jobFile.write('echo "Changing to CMSSW directory"\n')
        jobFile.write('cd ' + os.path.realpath(args.cmssw) + '\n\n')

        jobFile.write('echo "Setting up CMSSW"\n')
        jobFile.write('#cmsenv\n')
        jobFile.write('eval `scramv1 runtime -sh`\n\n')

        jobFile.write('echo "Changing to the working directory"\n')
        jobFile.write('cd ' + basePath + '\n\n')

        jobFile.write('echo "Running the stuff"\n')
        jobFile.write('cmsRun run_steamflow_cfg.py &> log_run_steamflow\n\n')

        jobFile.write('echo "Done!"\n')

        mode = os.fstat(jobFile.fileno()).st_mode
        mode |= 0o111
        os.fchmod(jobFile.fileno(), mode & 0o7777)

      with open("./theMaxTracks.py", 'w') as maxTracksPython:
        maxTracksPython.write("maxTracksPerArm = " + str(maxTracks[trackSet]) + "\n")

      prepConfig = "edmConfigDump efficiencies.py > " + basePath + "/hlt_config.py"

      prepInput = "cp " + inputFiles[fileSet] + " " + basePath + "/list_cff.py"

      prepProcessor = "cp run_steamflow_cfg.py " + basePath + "/run_steamflow_cfg.py"

      submissionCmd = "bsub -q " + args.queue + " -r -e " + logDirectory + "/err.log -o " + logDirectory + "/out.log " + basePath + "/job.sh"

      if args.verbose:
        print prepConfig
      if not args.dryRun:
        p = subprocess.Popen(prepConfig, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate()
      else:
        print "Would run: " + prepConfig

      if args.verbose:
        print prepInput
      if not args.dryRun:
        p = subprocess.Popen(prepInput, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate()
      else:
        print "Would run: " + prepInput

      if args.verbose:
        print prepProcessor
      if not args.dryRun:
        p = subprocess.Popen(prepProcessor, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate()
      else:
        print "Would run: " + prepProcessor

      if args.verbose:
        print submissionCmd
      if not args.dryRun:
        p = subprocess.Popen(submissionCmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate()
      else:
        print "Would run: " + submissionCmd



