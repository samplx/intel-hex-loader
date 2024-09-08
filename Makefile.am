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

## Process this file with automake to produce Makefile.in

ACLOCAL_AMFLAGS = -I m4

bin_PROGRAMS = intel-hex-loader

intel_hex_loader_SOURCES = intel-hex-loader.c

AM_TESTS_ENVIRONMENT = \
	TESTDIR='$(srcdir)/tests'; export TESTDIR; \
	INTEL_HEX_LOADER='$(builddir)/intel-hex-loader'; export INTEL_HEX_LOADER; \
	VERBOSE='yes'; export VERBOSE;

TESTS = \
	tests/test001.sh \
	tests/test002.sh \
	tests/test003.sh \
	tests/test004.sh \
	tests/test005.sh \
	tests/test006.sh \
	tests/test007.sh \
	tests/test008.sh \
	tests/test009.sh \
	tests/test010.sh \
	tests/test011.sh \
	tests/test012.sh \
	tests/test013.sh \
	tests/test014.sh \
	tests/test015.sh \
	tests/test016.sh \
	tests/test017.sh \
	tests/test018.sh \
	tests/test019.sh \
	tests/test020.sh \
	tests/test021.sh \
	tests/test022.sh \
	tests/test023.sh \
	tests/test024.sh \
	tests/test025.sh \
	tests/test026.sh \
	tests/test027.sh \
	tests/test028.sh \
	tests/test029.sh \
	tests/test030.sh \
	tests/test031.sh \
	tests/test032.sh \
	tests/test033.sh \
	tests/test034.sh \
	tests/test035.sh \
	tests/test036.sh \
	tests/test037.sh \
	tests/test038.sh \
	tests/test039.sh \
	tests/test040.sh \
	tests/test041.sh \
	tests/test042.sh \
	tests/test043.sh \
	tests/test044.sh \
	tests/test045.sh \
	tests/test046.sh \
	tests/test047.sh \
	tests/test048.sh \
	tests/test049.sh \
	tests/test050.sh \
	tests/test051.sh \
	tests/test052.sh \
	tests/test053.sh \
	tests/test054.sh \
	tests/test055.sh \
	tests/test056.sh \
	tests/test057.sh \
	tests/test058.sh

