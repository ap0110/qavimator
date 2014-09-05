#!/bin/sh

cd "$( cd "$( dirname "$0" )" && pwd )"

LD_LIBRARY_PATH=.:./platforms
export LD_LIBRARY_PATH

./qavimator "$@"
