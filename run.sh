#!/bin/sh
THREADS=$((1))
./ecb.elf $THREADS;
./cbc.elf $THREADS;
./cfb.elf $THREADS;
./ctr.elf $THREADS