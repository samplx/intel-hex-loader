# shellcheck disable=SC2148

# this file should be sourced by the actual test

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

# check requirements. fail fast. fail hard (99)
[ -d "${TESTDIR}" ] || exit 99
[ -d "${TESTDIR}/com-files" ] || exit 99
[ -d "${TESTDIR}/expected" ] || exit 99
[ -d "${TESTDIR}/hex-files" ] || exit 99
[ -x "${INTEL_HEX_LOADER}" ] || exit 99

export COM_DIR="tests/com-files"
export EXPECTED_DIR="tests/expected"
export HEX_DIR="tests/hex-files"

VERBOSE="${VERBOSE:="no"}"
PROGRAM_NAME="${0##*/}"
TEST_NAME="${PROGRAM_NAME%.sh}"

CURRENT_DIR="$(readlink -e .)"
RESULTS_DIR="$(mktemp -d test-runner-output.XXXXXXXXX)"

function E() {
    if [ "${VERBOSE}" = "yes" ]
    then
        echo "$*"
        "$@"
    fi
}

function Setup() {
    local real_testdir
    local real_loader
    real_testdir="$(readlink -e "${TESTDIR}")"
    real_loader="$(readlink -e "${INTEL_HEX_LOADER}")"
    ln -s "${real_testdir}" "${RESULTS_DIR}/tests"
    ln -s "${real_loader}" "${RESULTS_DIR}/intel-hex-loader"
    # override for actual test script -- always local to match expected results
    INTEL_HEX_LOADER="./intel-hex-loader"
    TESTDIR="tests"
    cd "${RESULTS_DIR}" || exit 99
}

function CleanUp() {
    if [ "${KEEP_TEMPORARIES}" = "yes" ]
    then
        echo " NOTE: results remain in $RESULTS_DIR"
    else
        cd "${CURRENT_DIR}" || exit 99
        rm --force --recursive "${RESULTS_DIR}"
    fi
}

function CheckResults() {
    local RESULT
    local DETAILS
    RESULT="PASS"
    DETAILS=""

    if [ -f "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.out" ]
    then
        if ! cmp -s "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.out" "./${TEST_NAME}.out"
        then
            RESULT="FAIL"
            DETAILS="Output. ${DETAILS}"
            E diff -u "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.out" "./${TEST_NAME}.out"
        fi
    fi

    if [ -f "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.err" ]
    then
        if ! cmp -s "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.err" "./${TEST_NAME}.err"
        then
            RESULT="FAIL"
            DETAILS="Error output. ${DETAILS}"
            E diff -u "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.err" "./${TEST_NAME}.err"
        fi
    fi

    if [ -f "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.com.missing" ] && [ -f "./${TEST_NAME}.com" ]
    then
        RESULT="FAIL"
        DETAILS="Found unexpected COM output. ${DETAILS}"
    elif [ -f "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.com" ]
    then
        if [ ! -f "./${TEST_NAME}.com" ]
        then
            RESULT="FAIL"
            DETAILS="Missing COM output. ${DETAILS}"
        elif ! cmp -s "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.com" "./${TEST_NAME}.com"
        then
            RESULT="FAIL"
            DETAILS="COM output. ${DETAILS}"
            E cmp -l "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.com" "./${TEST_NAME}.com"
        fi
    fi

    if [ -f "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.code" ]
    then
        EXPECTED_CODE="$(cat "${EXPECTED_DIR}/${TEST_NAME}/${TEST_NAME}.code")"
        ACTUAL_CODE="$(cat "./${TEST_NAME}.code")"
        if [ "$EXPECTED_CODE" -ne "$ACTUAL_CODE" ]
        then
            RESULT="FAIL"
            DETAILS="Exit code. ${DETAILS}"
            E echo "EXPECTED_CODE=${EXPECTED_CODE}, ACTUAL_CODE=${ACTUAL_CODE}"
        fi
    fi

    CleanUp

    if [ "$RESULT" = "FAIL" ]
    then
        echo "FAIL ${TEST_NAME}:${DETAILS}"
        exit 1
    fi
    echo "PASS ${TEST_NAME}"
    exit 0
}
