#!/usr/bin/env bash

valgrind build/main
echo ""
echo ""
echo ""
valgrind build/tcc/main
