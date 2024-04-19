# AudioPrism
AudioPrism is an Arduino-based audio analysis library for interpreting the results of Fourier transforms. Premade spectral analysis tools are provided for identifying prominent frequencies, classifying the periodicity of a signal, detecting transients, and more. AudioPrism is flexible and expandible; the object-oriented structure of the library provides a chasis to implement custom analysis modules on. This document explains how to install and use AudioPrism's provided tools and how to create audio analysis tools within the AudioPrism framework.
# Table of Contents
- [Getting Started](#Getting-Started)
  - [Prerequisites](#Prerequisites)
  - [Installation](#Installation)
  - [Configuration](#Configuration)
- [Provided Modules](#Provided-Modules)
  - [MaxAmplitude](#MaxAmplitude)
  - [TotalAmplitude](#TotalAmplitude)
  - [MeanAmplitude](#MeanAmplitude)
  - [Centroid](#Centroid)
  - [Noisiness](#Noisiness)
  - [DeltaAmplitudes](#DeltaAmplitudes)
  - [SalientFreqs](#SalientFreqs)
  - [PercussionDetection](#PercussionDetection)
  - [MajorPeaks](#MajorPeaks)
  - [BreadSlicer](#BreadSlicer)
- [Classes](#Classes)
  - [Class Hierarchy Overview](#Class-Hierarchy-Overview)
  - [AnalysisModule Class](#AnalysisModule-Class)
  - [ModuleInterface Class](#ModuleInterface-Class)
- [Creating Modules](#Creating-Modules)
  - [Atomic Modules](#Creating-an-Atomic-Module)
  - [Composite Modules](#Creating-a-Composite-Module)
- [Files](#Files)
- [Examples](#Examples)
# Getting Started
## Prerequisites
AudioPrism is an Arduino library intended for use in [ArduinoIDE](https://www.arduino.cc/en/software) and on Arduino / Arduino-compatible boards. AudioPrism may be used in other environments and C++ projects, but setup information and tutorial steps will pertain to Arduino and ArduinoIDE.

The AudioPrism library provides analysis tools that operate on the results of a Fourier transform; it does not provide a way to perform a fourier transform on data. There are free external libraries for Arduino that offer this functionality:
- [ArduinoFFT](https://www.arduino.cc/reference/en/libraries/arduinofft/) (recommended)
- [EasyFFT](https://projecthub.arduino.cc/abhilashpatel121/easyfft-fast-fourier-transform-fft-for-arduino-03724d)
  
## Installation
Installing AudioPrism is as simple as copying the repository into ArduinoIDE's 'libraries' folder. The 'libraries' folder is located at `Documents\Arduino\libraries` on both Windows and MacOS. Below are two methods to download and install AudioPrism.
### Clone the Repository (recommended)
Cloning the repository with git is recommended to stay up-to-date with changes. Inside a shell, navigate to the ArduinoIDE 'libraries' folder and run `git clone ___`. 
### Download as a .zip
Instead of cloning the repository, you can download AudioPrism's files as a .zip. Download AudioPrism as a .zip, extract the files, and place them in the ArduinoIDE 'libraries' folder. This is not the recommended approach, as it makes it more difficult to receive updates.

## Configuration
With the AudioPrism library installed, you can include it in your Arduino Sketch in .cpp, .h, or .ino files with `#include <AudioPrism>`. To get proper analysis results from modules in AudioPrism, you must provide the audio sample rate and the FFT window size used to generate the frequency domain information that will be input to the modules. These constants are  necessary to the function of AudioPrism modules, as the audio sample rate and FFT window size are used to calculate the frequencies associated with each bin of a frequency spectrum resulting from the FFT operation; failing to synchronize these constants between your external FFT library and AudioPrism will render the analysis results of AudioPrism useless.

## Example
```c++
#include <AudioPrism>

setup () {
  // show configuration steps
}

loop () {
  // show basic usage
}
```
# Provided Modules
Modules in this library range in complexity, from atomic modules with no modifiable parameters to compositions of modules that contain submodules and have many modifiable parameters. This list of included modules is roughly ordered by complexity, with simpler modules first. 

## MaxAmplitude
The MaxAmplitude module finds the amplitude of the frequency bin with the greatest amplitude in an input frequency spectrum.
### Parameters
MaxAmplitude has no modifiable analysis parameters.
### Return Type
`.getOutput()` returns a float value, the maximum amplitude found by the most recent call to `.doAnalysis()`.
### Submodules
MaxAmplitude does not rely on any submodules.

## TotalAmplitude
The TotalAmplitude module sums the amplitudes of the bins of a frequency spectrum, calculating the total energy of the input audio used to generate that frequency spectrum.
### Parameters
TotalAmplitude has no modifiable parameters.
### Return Type
`getOutput()` returns a float value, the amplitude sum calculated in the most recent call to `.doAnalysis()`.
### Submodules
TotalAmplitude does not rely on any submodules.

## MeanAmplitude
The MeanAmplitude module calculates the average amplitude across the bins in an input frequency spectrum.
### Parameters
MeanAmplitude has no modifiable parameters.
### Return Type
`.getOutput()` returns a float value, the amplitude average calculated in the most recent call to `.doAnalysis()`.
### Submodules
MeanAmplitude contains a private instance of [TotalAmplitude](#TotalAmplitude). The output of this TotalAmplitude module is divided by the range of bins in the frequency spectrum to calculate the average amplitude.

## Centroid
The Centroid module calculates the center of mass of a frequency spectrum. This is a weighted average of the frequencies present in the input audio. The centroid can be used to characterize the brightness of the input, as a greater presence of high-frequency energy raises the centroid and a greater presence of low-frequency energy lowers the centroid.
### Parameters
Centroid has no modifiable parameters.
### Return Type
`getOutput()` returns an integer value, the frequency of the center of mass of the input audio spectrum calculated in the most recent call to `.doAnalysis()`.
### Submodules
Centroid does not rely on any submodules.

## Noisiness
The Noisiness module calculates the spectral entropy of a frequency spectrum, which is a measure of the spread or randomness of the energy in a frequency spectrum. White noise is characterized by a frequency spectrum with energy evenly distributed across the frequency range; pure periodic signals like sine waves are characterized by frequency spectrums with a sharp spike at a single point in the frequency range.
### Parameters
Noisiness has no modifiable parameters.
### Return Type
`.getOutput()` returns a float value between 0 and 1, where 0 represents a minimally entropic audio spectrum (as generated by a periodic signal) and 1 represents a maximally entropic audio spectrum (as generated by a noisy signal).

## DeltaAmplitudes
The DeltaAmplitudes module calculates the change in amplitude for each bin in a frequency spectrum.
### Parameters
DeltaAmplitudes has no modifiable parameters.
### Return Type
`getOutput()` returns a reference to an array of float values, representing the change in amplitude for each bin in a frequency spectrum. The size of this array is equal to the size of the input array. The memory for this array is allocated by DeltaAmplitudes' default constructor and is freed by DeltaAmplitudes' default destructor.

## SalientFreqs
The SalientFreqs module finds the `n` bins with the greatest change in amplitude from one audio spectrum to another. By default, SalientFreqs finds the three bins with the greatest change in amplitude.
### Parameters
`numFreqs`, the number of salient frequencies to find, can be set by using the parameterized constructor `SalientFreqs(int n)` or by invoking the member function `.changeNumFreqs(int n)`. The default value for numFreqs, set when using the default constructor, is 3.
```c++
// set numFreqs with the constructor
SalientFreqs sal_freqs = SalientFreqs(8); // set numFreqs to 8
```
```c++
// change numFreqs after construction
SalientFreqs sal_freqs = SalientFreqs(); // default numFreqs value: 3
sal_freqs.setNumFreqs(8);                // set numFreqs to 8
```
#### Return Type
`.getOutput()` returns a reference to an array of integer values, the indices of the frequency bins with the greatest change in amplitude. The size of this array is equal to numFreqs and is automatically updated when the member function `.changeNumFreqs(int n)` is invoked. The memory for this array is allocated upon construction, reallocated when `.changeNumFreqs(int n)` is invoked, and freed by the default destructor.

## PercussionDetection
PercussionDetection detects transients (signal spikes, like percussion in music) by considering three factors: the instantaneous total volume, the change in volume, and the instantaneous noisiness of an audio spectrum. These factors are weighed against modifiable threshold values, and a transient is detected if and only if it meets all three thresholds.
### Parameters
PercussionDetection has modifiable threshold values for each of its three factors:
1. `loudness_threshold` is a float value representing the instantaneous total amplitude required to satisfy the volume condition. Setting this threshold defines the minimum volume a transient must reach to be detected.
```c++
PercussionDetection pd = PercussionDetection(); // default loudness threshold: 300
pd.setLoudnessThreshold(200); // change loudness threshold to 200
```
2. `delta_threshold` is a float value representing the change in amplitude from one moment to another required to satisfy the change-in-volume condition. Setting this threshold defines how sharply a transient must spike up in volume to be detected.
```c++
PercussionDetection pd = PercussionDetection(); // default delta threshold: 150
pd.setDeltaThreshold(200); // raise delta threshold to 200
```
3. `noisiness_threshold` is a 0-1 float value representing the instantaneous spectral entropy required to satisfy the noisiness condition. Setting this threshold defines how percussive (with high spectral entropy taken as an indicator of percussiveness) a transient must be to be detected. Visit the [Noisiness](#Noisiness) module for further explanation of spectral entropy.
```c++
PercussionDetection pd = PercussionDetection(); // default noisiness threshold: 0.7
pd.setNoisinessThreshold(0.2); // lower noisiness threshold to 0.2
```
<!--EOL-->
These thresholds can be set with the optional constructor `PercussionDetection(float loudness_t, float delta_t, float noise_t)` or by calling the individual member functions `.setLousnessThreshold(float loudness_t)`, `.setDeltaThreshold(float delta_t)`, and `.setNoisinessThreshold(float noise_t)`. Setting any threshold to 0 effectively excludes it from consideration, so PercussionDetection can be configured to consider only 1 or any combination of 2 factors.
```c++
// create an instance of percussion detection with the following parameters:
// loudness_threshold:   250
// delta_threshold:      100
// noisiness_threshld:   0 (disabled)
PercussionDetection pd = PercussionDetection(250, 100 0);
```
### Return Type
`getOutput()` returns a boolean value representing whether or not the start of a transient was detected in the most recent call to `.doAnalysis()`.
### Submodules
PercussionDetection uses three submodules:
1. A private instance of [TotalAmplitude](#TotalAmplitude) is used to measure the total energy of the audio spectrum. 
2. A private instance of [DeltaAmplitudes](#DeltaAmplitudes) is used to measure the total change in energy of the audio spectrum.
3. A private instance of [Noisiness](#Noisiness) is used to measure the periodicity of the signal that generated the audio spectrum.

## MajorPeaks
### Parameters
### Return Type

## BreadSlicer
The BreadSlicer module breaks down an input frequency spectrum into a set of configurable, non-overlapping, frequency slices, sums the amplitudes within each slice, and returns the list of sums ordered from lowest to highest frequency slice.
### Parameters

### Return Type
`getOutput()` returns a reference to an array of float values, the amplitude sums from the most recent analysis. This array has a length equal to the number of slices selected. The memory for this array is allocated by the default constructor and is freed by the default destructor.

# Classes
AudioPrism utilizes an object-oriented design to implement its analysis tools to maintain data access control, standardize common parameters and functionality, offer a framework for creating additional modules, and allow the creation of high-order modules composed of other analysis modules. This section describes the relationship between AudioPrism's classes and how to use them to implement analysis modules.
## Class Hierarchy Overview
AudioPrism provides a base class AnalysisModule and a critical component ModuleInterface, which are used to implement individual analysis modules, each of which is its own class. 
1. The AnalysisModule base class stores the parameters and functions that are common to all analysis modules, as well as the virtual `.doAnalysis()` function that must be implemented in analysis modules to give them their unique functionality. For details on the base class's data and functions, see [AnalysisModule Class](#AnalysisModuleClass).
2. The ModuleInterface class is a templated class derived from the AnalysisModule base class; it is used to manage analysis modules' output, which can be any data type. For more information about the implementation of and need for this interface, see [ModuleInterface Class](#ModuleInterfaceClass).
<!--EOL-->
Individual analysis modules are implemented as their own classes that must derive from ModuleInterface with an output type specified. For more information about implementing analysis modules, see [Module Classes](#ModuleClasses)
## AnalysisModule Class
The `AnalysisModule` class serves as the base class for all the individual modules. This is where the functionality and properties common to all modules are located. 
### AnalysisModule Data
Key constants like the audio sample rate and FFT window size are stored here, which together define the analysis context in which a specific module operates. The sample rate should be at minimum double the maximum frequency that you want modules to handle. The FFT window size must be a power of 2.
### AnalysisModule Member Functions
#### doAnalysis
`doAnalysis()` is a pure virtual function that must be implemented in individual modules. This is where the unique functionality of analysis modules is defined. This function is provided as a pure virtual function in the base class so that synchronous analysis can be easily performed by looping over a set of analysis modules and triggering their analysis functions.
```c++
// perform synchronous analysis on a set of analysis modules
for (int i=0; i<numModules; i++) {
  // modules[] is an array of analysis modules derived from the AnalysisModule base class
  // .doAnalysis() can perform a different function for each derived analysis module
  modules[i].doAnalysis(input);
}
```
#### setAnalaysisRangeByFreq/Bin
The base class implements functions that allow you to constrain a module's frequency range that it will look at during analysis. `.setAnalysisRangeByFreq(int lower, int upper)` sets the lower and upper frequency bounds by frequency. The provided frequency values will be rounded to the nearest frequency bin indices based on the audio sample rate and FFT window size. `.setAnalysisRangeByBin(int lower, int upper)` sets the lower and upper frequency bounds from explicitly provided bin indices.
```c++
// assume sample rate is 8192 and the FFT window size is 256

// set analysis range to 0-1000 Hz (this will convert to bins 0 - 31)
// Lower bin: (256 / 8192) * 0 = 0
// Upper bin: (256 / 8192) * 1000 = 31
derived_module.setAnalysisRangeByFreq(0, 1000);

// set analysis range to bins 10 - 25 (this corresponds to 320 - 800 Hz)
// Lower frequency: (8192 / 256) * 10 = 340 Hz
// Upper frequency: (8192 / 256) * 25 = 800 Hz
derived_module.setAnalysisRangeByBin(10, 25);
```
#### addSubmodule
The `.addSubmodule()` member function connects a submodule's parameters (audio context, analysis range, etc.) to a parent module. When implementing a module that is composed of submodules, the submodules should be privately declared within the parent module, and the parent module's constructor should invoke `.addSubmodule()` on itself with a reference to the submodule to be added. Once a submodule is added, changes to the parent module's parameters will be automatically propagated down to its submodules (and submodules will propagate changes down to their submodules, if any). For example usage, see [Creating Modules](#CreatingModules).
## ModuleInterface Class
The ModuleInterface class is necessary to connect an analysis module to the AnalysisModule base class while permitting variable output types and maintaining the ability to keep arrays of analysis modules. ModuleInterface is a templated class that derives from the AnalysisModule base class, and its only job is to store the output of the most recent analysis performed by an analysis module.
### ModuleInterface Data
The ModuleInterface template class contains a protected variable `output`. This is where an analysis module must store the result of its implementation of `.doAnalysis()`, and it should only be modified inside a module deriving from ModuleInterface. By selecting a type T for the ModuleInterface class, you are selecting the `output` type of the derived analysis module. This can be any type, including references to multidimensional arrays. 
### ModuleInterface Member Functions
`.getOutput()` is used to retrieve the value of `output`. This getter function is necessary because `output` is a protected variable inside the ModuleInterface class.

# Creating Modules
To implement an analysis module, declare a new class that inherits from ModuleInterface with an output type specified. Make a public definition for the virtual `doAnalysis()` function, with a `const float **` input type, and store the result of the analysis in the `output` variable at the end. The `output` variable will have a data type equal to the type you specified for ModuleInterface. 
## Creating an Atonic Module
For atomic modules (modules containing no submodules), no constructor or destructor needs to be explicitly defined.
```c++
// atomic module with no internal data or submodules
class MyAtomicModule : ModuleInterface<float> {
public:
  // all analysis modules must implement a version of doAnalysis
  void doAnalysis (const float **input) {
    // do something with input

    // store the analysis result in 'output'
    // this can be retrieved outside the module with .getOutput()
    output = result;
  }
}
```
## Creating a Composite Module
For composite modules (modules containing submodules), declare a private instance of the submodule(s) and define a constructor that invokes the `addSubmodule()` function with a reference to the submodule(s). This allows properties of the parent module to be propagated to its submodules.
```c++
class MyCompositeModule : ModuleInterface<float> {
private:
  TotalAmplitude total_amp = TotalAmplitude();
public:
  // modules containing submodules must register them in a constructor
  MyCompositeModule () {
    this->addSubmodule(&totalAmp);
  }

  // all analysis modules must implement a version of doAnalysis
  void doAnalysis (const float **input) {
    totalAmp.doAnalysis(input);
    float total = totalAmp.getOutput();
    // do something with total...

    // store the analysis result in 'output'
    // this can be retrieved outside the module with .getOutput()
    output = result;
  }
}
```

# Files
## AnalysisModule.*
## modules/*

# Examples
