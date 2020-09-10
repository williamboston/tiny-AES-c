#!/bin/sh
THREADS=$((2))
./ecb.elf $THREADS;
./cbc.elf $THREADS;
./cfb.elf $THREADS;
./ctr.elf $THREADS