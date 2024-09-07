#!/bin/bash
#
#	Copyright 2024 James Burlingame
#
#	Licensed under the Apache License, Version 2.0 (the "License");
#	you may not use this file except in compliance with the License.
#	You may obtain a copy of the License at
#
#	    https://www.apache.org/licenses/LICENSE-2.0
#
#	Unless required by applicable law or agreed to in writing, software
#	distributed under the License is distributed on an "AS IS" BASIS,
#	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#	See the License for the specific language governing permissions and
#	limitations under the License.
#

[ -f "${TESTDIR:?The TESTDIR environment variable is not set}/test-common.sh" ] || exit 99
[ -x "${INTEL_HEX_LOADER:?The INTEL_HEX_LOADER environment variable is not set}" ] || exit 99

source "${TESTDIR}/test-common.sh"

Setup

HEX_INPUT_FILE1="${HEX_DIR}/bad-hex-address-04.hex"

# run actual test and capture results
"${INTEL_HEX_LOADER}" \
    --output "${TEST_NAME}.com" \
    "${HEX_INPUT_FILE1}" \
        < /dev/null > "${TEST_NAME}.out" 2> "${TEST_NAME}.err"
CODE="$?"
echo "${CODE}" > "${TEST_NAME}.code"

CheckResults
