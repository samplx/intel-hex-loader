# Testcases

This document describes the *ad hoc* tests for the `intel-hex-loader` tool.

## Overview

There is a **bash** shell script for each of the tests. They will execute
the tool, and gather the standard output, standard error, any `.COM` file
that was produced, as well as the **exit code** from the execution.
The are compared to any corresponding **expected** result files.
If the expected results exist, they are checked. If they do not
exist it does not prevent the test from passing.

## Test Descriptions

### test001

An error. No command-line arguments.

### test002

A valid `--help` command.

### test003

A valid `-h` command.

### test004

An error. An input hex file, but no `--output` and no `-o` option.

### test005

An error. A valid `--output file` option. No input hex file.

### test006

An error. A valid `--output file` option. Input hex file is missing.

### test007

An warning. A valid `--output file` option. Input hex file is empty.

### test008

An warning. A valid `--output file` option. Input hex file has no colon ':'.

### test009

An warning. A valid `--output file` option. Input hex file has a single valid zero-length, type '00' record.

### test010

An warning. A valid `--output file` option. Input hex file has a single valid type '01' (EOF) record.

### test011

An error. A valid `--output file` option.
Input hex file has an invalid character '/' in the first digit of the byte count.

### test012

An error. A valid `--output file` option.
Input hex file has an invalid character ':' in the first digit of the byte count.

### test013

An error. A valid `--output file` option.
Input hex file has an invalid character '@' in the second digit of the byte count.

### test014

An error. A valid `--output file` option.
Input hex file has an invalid character 'G' in the first digit of the byte count.

### test015

An error. A valid `--output file` option.
Input hex file has an invalid character ` in the second digit of the byte count.

### test016

An error. A valid `--output file` option.
Input hex file has an invalid character 'g' in the second digit of the byte count.

### test017

An error. A valid `--output file` option.
Input hex file has an invalid character '!' in the first digit of the address.

### test018

An error. A valid `--output file` option.
Input hex file has an invalid character '!' in the second digit of the address.

### test019

An error. A valid `--output file` option.
Input hex file has an invalid character '!' in the third digit of the address.

### test020

An error. A valid `--output file` option.
Input hex file has an invalid character '!' in the fourth digit of the address.

### test021

An error. A valid `--output file` option.
Input hex file has an invalid character '!' in the first digit of the record type.

### test022

An error. A valid `--output file` option.
Input hex file has an invalid character '!' in the second digit of the record type.

### test023

An error. A valid `--output file` option.
Input hex file has an invalid character '!' in the first digit of a data byte.

### test024

An error. A valid `--output file` option.
Input hex file has an invalid character '!' in the second digit of the checksum.

### test025

An error. A valid `--output file` option.
Input hex file has an invalid checksum.

### test026

A valid `--output file` option.
Input hex file has valid input for a single byte '5E' at address '0100'.

### test027

A valid `--output file` option.
Input hex file has valid input for a single byte '5E' at address '0100',
with a trailing 'E5' after the checksum.

### test028

A valid `--output file` option.
Two input hex files have valid input for a single byte at address '0100'.
The contents of the first file ('FF') are overwritten by the second ('AA').

### test029

A valid `--output file` option.
Two input hex files with valid input. First has 16 bytes at address '0100', with
the first byte being '21'. The second has a single byte of '55' at address '0100'.

### test030

A valid `--output file` option.
Two input hex files with valid input. First has 16 bytes at address '0100'.
The second has a 16 bytes at address '0200'.

### test031

A valid `--output file` option. A valid `--size 1` option.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.

### test032

A valid `--output file` option. A valid `--size 2` option.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.

### test033

A valid `--output file` option. A valid `-s 2` option.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.

### test034

A valid `--output file` option. An invalid valid `--size 2x` option.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.

### test034

A valid `--output file` option. An invalid valid `--size 0xag` option.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.

### test035

A valid `--output file` option. An invalid valid `--size 08` option.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.

### test036

A valid `--output file` option. An invalid valid `-s 2x` option.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.

### test037

A valid `--output file` option. A valid `-s 012` option.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.

### test038

A valid `--output file` option. A valid `--size 0xA` option.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.

### test039

A valid `--output file` option. A valid `--size 3` option.
Two input hex files with valid input. First has 16 bytes at address '0100'.
The second has a 16 bytes at address '0200'.

### test040

An error. An input hex file, but no `--output` and no `-o` option.
A valid `--verbose` option.

### test041

An error. A valid `--output file` option. No input hex file.
A valid `--verbose` option.

### test042

An error. A valid `--output file` option. Input hex file is missing.
A valid `--verbose` option.

### test043

An warning. A valid `--output file` option. Input hex file is empty.
A valid `--verbose` option.

### test044

An warning. A valid `--output file` option. Input hex file has no colon ':'.
A valid `--verbose` option.

### test045

An warning. A valid `--output file` option. Input hex file has a single valid zero-length, type '00' record.
A valid `--verbose` option.

### test046

An warning. A valid `--output file` option. Input hex file has a single valid type '01' (EOF) record.
A valid `-v` option.

### test047

An error. A valid `--output file` option.
Input hex file has an invalid character '/' in the first digit of the byte count.
A valid `--verbose` option.

### test048

An error. A valid `--output file` option.
Input hex file has an invalid checksum.
A valid `--verbose` option.

### test049

A valid `--output file` option.
Input hex file has valid input for a single byte '5E' at address '0100'.
A valid `--verbose` option.

### test050

A valid `--output file` option.
Input hex file has valid input for a single byte '5E' at address '0100',
with a trailing 'E5' after the checksum.
A valid `--verbose` option.

### test051

A valid `--output file` option.
Two input hex files have valid input for a single byte at address '0100'.
The contents of the first file ('E5') are overwritten by the second ('AA').
A valid `--verbose` option.

### test052

A valid `--output file` option.
Two input hex files have valid input for a single byte at address '0100'.
The contents of the first file ('E5') are overwritten by the second ('AA').
A valid `-v` option.

### test053

A valid `-o file` option. A valid `-s 2` option.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.
A valid `-v` option.

### test054

A valid `--output file` option. A valid `--size 2` option.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.
A valid `--verbose` option.

### test055

A valid `--output file` option.
A `--patch file` option with a missing patch file.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.
A valid `--verbose` option.

### test056

A valid `--output file` option.
A `--patch file` option with a 256 byte patch file full of '55' bytes.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.
A valid `--verbose` option.

### test057

A valid `--output file` option. A valid `--size 1` option.
A `--patch file` option with a 512 byte patch file full of 'AA' bytes.
An input hex file with valid input, 16 bytes at address '0100', with
the first byte being '3F'.
A valid `--verbose` option.

