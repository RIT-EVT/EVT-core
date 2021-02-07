# EVT Sample Source

## Introduction

This project is a sample of what the architecture of the core EVT firmware
could look like. The goal is for this code to be the replacement for mbed in
EVT applications and to maintain the shared functionality that all EVT targets
share.

The key points of the project are listed below. If it is decided that this
architecture would benefit the team in the long term, then the process of
porting our existing code over to this system will begin.

## Key Points

* CMake used for building project, allows this repo to be dependency in many
other projects
* Building no longer reliant on mbed.
* Testing of core functionality (io abstractions, utilities, etc) no longer
coupled with specific targets (TMS, BMS, etc)
* Version control of the core code base independent of the various targets.

## Next Steps

In the case that this architecture is chosen then this repo will be moved to
an EVT controlled location (either Bitbucket or Github). After that, the
existing code will be ported over.

Each target (TMS, BMS, LVSS) will then be given a dedicated repo and be treated
as independent projects. This way testing, version controlling, and
documentation will all be decoupled.
