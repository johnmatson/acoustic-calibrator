# Acoustic Calibrator
>A real-time embedded audio system that filters a raw audio signal to counteract the unique acoustic frequency characteristics of a room. Developed by John Matson and Alexander Mueller in fall 2020 for BCIT's ELEX 7820, taught by David Romalo.

## Usage
The callibrator is designed for the TMS320F28027 Piccolo Launchpad with integrated dual-sample 12-bit ADC and the external TLV5616CP 12-bit DAC. To begin calibration, perform the following steps:
1. Scale & shift a line-level mono audio source so that it is centered around 1.67 V with a maximum peak-to-peak voltage of 3.3 V.
2. Connect this scaled & shifted audio source to ADC input A (Launchpad pin J1.7).
3. Setup a mono refernce mic in the listening position of choice.
4. Amplify the mic signal and connect the line-level mono mic signal to ADC input B (Launchpad pin J1.8).
5. 
6. Connect the DAC output (pin 7) to the speaker for playback.
7. Connect the Launchpad to your computer and compile the project to the device in Code Composer Studio.

The callibrator should now counteract the acoustic signature of the room the audio is played back in, in real-time. Please note that sample rate is currently set to 5 kHz. The system will be optimized to operate at a suitable sample rate for audio playback in a future release. Also note that the callibrator output may oscillate under current operation. Improved averging algorithms will be included in a future release to resolve this issue.

## Contents
* [Background](#background)
* [Overview](#overview)
* [Implementation](#implementation)

## Background
An important issue in audio engineering is the effect that room acoustics have on an audio stream that actually reaches the listeners’ ears. While a song might sound great in isolation, the acoustics of a room might have such a drastic influence on the signal that the music sounds terrible as it is eventually perceived.

## Overview
The audio calibrator has two audio inputs; one for the reference audio that is to be played in a room, and another for the recorded audio that is picked up by a reference microphone placed in the room. We analyze the frequency components of both audio inputs via 32-point FFTs, which are cast into four larger frequency bins corresponding to our filter bands. By taking the quotient of the reference and recorded audio streams, we are able to determine the differences between the two audio streams in each frequency band. With these computed ratios, we manipulate the gains of a 4-band filter – also known as a graphic equalizer – to compensate for the frequency manipulation of the room acoustics and output a filtered version of the reference audio signal.

## Implementation
The calibrator's firmware is written in C and implemented using TI's SYS/BIOS RTOS.

### System Diagram
![](readme-assets/block-diagram.png)
