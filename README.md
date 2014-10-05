ARToolKitPlus
=============

*ARToolKit* is a software library that can be used to calculate
camera position and orientation relative to physical markers
in real time. This enables the easy development of a wide range
of Augmented Reality applications.

*ARToolKitPlus* is an extended version of ARToolKit's vision code
that adds features, but breaks compatibility due to a new
class-based API. ARToolKitPlus does not come with integrated
support for VRML nor does it come with a camera library.

advantages over the classical ARToolKit:
* easier C++ based API
* support for 4096 binary based markers - no need to design new markers yourself
* supports RGB565, Gray images
* "Robust Planar Pose" algorithm that reduces jitter while tracking
* uses modern text-file based camera calibration
* Vignetting Compensation
* automatic Thresholding
* Hull based tracking in multi marker mode

advantages over ARToolkitPlus 2.1.1
* support for reading OpenCV calibration files (requires boost)
* runtime-configurable code
* non templated, reusable library
* faster compilation times
* interface for tracking multiple individual markers
* Hull based tracking for multi marker mode
* cleaned up and easier to use interface

More information can be found at:
http://studierstube.org/handheld_ar/artoolkitplus.php

Please read the F.A.Q. before sending questions:
http://answers.launchpad.net/artoolkitplus/+faqs

For any open issues please visit:
http://launchpad.net/artoolkitplus
