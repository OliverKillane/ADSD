## What is this?
A collection of scripts for getting values needed for SVM.

## To Setup
```bash
# from this directory
cd tools

python3.10 -m venv .venv

source .venv/bin/activate      # for bash
source .venv/bin/activate.fish # for fish

pip install -e .      # to use it
pip install -e .[dev] # for extra dev helpers (mypy, darker)
```
For convenience we can add the scripts directory to path to use `qua`,`tab` and `svm` anywhere.
```bash
# from the scripts directory
export PATH=$(pwd):$PATH
```

## The Tools
```bash
svm --help
qua --help
tab --help
exp --help
```