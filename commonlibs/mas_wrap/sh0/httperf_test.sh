#!/bin/sh
httperf --server zocromas.mastar.lan --http-version=1.0 --port=5005 --server-name zocromas.mastar.lan:5005  --uri=/HarryHarrison.jpeg --num-conns=1000 --num-calls=100
