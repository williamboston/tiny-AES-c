#!/bin/sh
THREADS=$((4))
./ecb.elf $THREADS;
./cbc.elf $THREADS;
./cfb.elf $THREADS;
./ctr.elf $THREADS