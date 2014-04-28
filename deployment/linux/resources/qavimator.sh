#!/bin/sh

LD_LIBRARY_PATH=.:./platforms
export LD_LIBRARY_PATH

./do_not_run_directly "$@"
