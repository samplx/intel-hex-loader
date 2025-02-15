#!/bin/bash

cp tests/test001.sh tests/test.sh.in

for n in \
    001 002 003 004 005 006 007 008 009 \
    010 011 012 013 014 015 016 017 018 019 \
    020 021 022 023 024 025 026 027 028 029 \
    030 031 032 033 034 035 036 037 038 039 \
    040 041 042 043 044 045 046 047 048 049 \
    050 051 052 053 054 055 056 057 058 059
do
    mkdir -p "tests/expected/test${n}"
    rm -f "tests/test${n}.sh"
    cp tests/test.sh.in "tests/test${n}.sh"
    chmod +x "tests/test${n}.sh"
done

