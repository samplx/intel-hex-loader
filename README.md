# intel-hex-loader

This is a program to create a [CP/M](https://en.wikipedia.org/wiki/CP/M)
*COM* file from one or more
[Intel HEX](https://en.wikipedia.org/wiki/Intel_HEX) format files.
It can optionally act as a binary patch by using an Intel HEX file in order
to update the contents of an existing *COM* file.

In general, the tool is used like the **CP/M** command `LOAD`, which
loads a Intel HEX format file and create a *COM* file.
The program `HEXCOM` acts much the same.
It can also be used to match what on CP/M would be a `DDT` session,
followed by the `SAVE` command to persist the results.

## Options

Almost all options have a short, single character version: e.g. `-o` as
well as a long version, e.g. `--output`.

The `--version` option has no short version. It describes the version.

The `--help` or `-h` option provides a short description of how to use the tool.

The `--output` or `-o` option has a required argument,
the full name of the output *COM* file.
This "option" is required, you must set the output filename.

The `--patch` or `-p` option is used to define an existing *COM* file.
Thie file is read into memory, and then any *HEX* files are loaded on top
of it. This results in a binary patch of the existing file.

**WARNING: You cannot use the same file as output and patch files.**

The `--size` or `-s` option is used to define the upper limit to the
saved file. For example, a `--size 32` option would cause the memory
from `0x0100` to `0x20FF` to be saved in the output file. You could also
use `--size 0x20` if that makes more sense.

This is the same
as if the `SAVE 32` command was given to the `CCP` after a `DDT` session.
If the size is not defined, the tool will default to the maximum address
where data was saved.

The program always saves the *COM* file in "page" size (256 byte) chunks.
It will not create a zero-length file.

The `--verbose` or `-v` option is used to provide additional feedback.


## Examples

### A *COM* file from a *HEX* file

The typical usage, creating a *COM* file from a single *HEX* file.


```bash
mac80 dump.asm
./intel-hex-loader --output dump.com dump.hex
```

### A *COM* file from multiple *HEX* files

This usage mimics a `DDT` session to load the individual *HEX* files, followed by a warm reboot and a `SAVE` of a number of pages to an output *COM* file.

```bash
mac80 as0com.asm
mac80 as1io.asm
mac80 as2scan.asm
mac80 as3sym.asm
mac80 as4sear.asm
mac80 as5oper.asm
mac80 as6main.asm
mac80 asm1420.asm
intel-hex-loader --size 32 --output asm80.com \
    as0com.hex as1io.hex as2scan.hex as3sym.hex \
    as4sear.hex as5oper.hex as6main.hex asm1420.hex
```

### Patch a *COM* file

This usage mimics a `DDT` session where a *COM* file is loaded followed by one or more *HEX* file patches.

**REMEMBER** you cannot use the same file as the output file (`--output` or `-o`),
as well as the source patch file (`--patch` or `-p`) option. In other words:

**In-place updates are not supported**!

```bash
mac80 asm1420.asm
intel-hex-loader --output new-asm80.com \
    --patch asm80.com asm1420.hex
```
