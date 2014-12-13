# caeplugin-AeroFXPost
An Pointwise CAE plugin that exports a AERO-F / XPOST grid.

![AeroFXPost][Logo]

Aero-F is a finite volume CFD code developed by the Farhat Research Group (www.stanford.edu/group/frg). It features capabilities for coupled multi-physics aeroelastic and aerothermoelastic simulations, and parallel implementaton using domain-decomposition methods. The source code is published online for the convenience of group members and approved third party users. It is neither "public domain" (in the sense of the intellectual property rights being forfeited) nor "open source" (in the sense of being freely licensed) unless otherwise explicitly noted in individual source code files. Stanford University makes no representations or warranties, express or implied, nor assumes any liability for the use of this software.

## Limitations

Only the `Unspecified` BC and VC physical types are supported.


## Building the Plugin

This plugin was created using version 1.0 R7 of the Pointwise CAE Plugin SDK.
However, it should build properly with newer versions of the SDK.

To build the AeroFXPost plugin you must integrate this source code into 
your local PluginSDK installation by following these steps.

* Download and install the [Pointwise Plugin SDK][SDKdownload].
* Configure and validate the SDK following the [SDK's instructions][SDKdocs].
* Create a AeroFXPost plugin project using the mkplugin script: `mkplugin -uns -cpp AeroFXPost`
* Replace the project's generated files with the files from this repository.
* Follow the platform specific instuctions below.

### Building the Plugin with Microsoft Visual Studio

* Open the Plugin SDK Visual Studio solution file.
 * For VS2008 open `PluginSDK\PluginSDK.sln`
 * For VS2012 open `PluginSDK\PluginSDK_vs2012.sln`
* Add the existing CaeUnsAeroFXPost project file to the solution *Plugins* folder.
 * For VS2008 add `PluginSDK\src\plugins\CaeUnsAeroFXPost\CaeUnsAeroFXPost.vcproj`
 * For VS2012 add `PluginSDK\src\plugins\CaeUnsAeroFXPost\CaeUnsAeroFXPost.vcxproj`

### Building the Plugin with Mac OS/X and Linux

* Nothing more needs to be done.


## Disclaimer
Plugins are freely provided. They are not supported products of
Pointwise, Inc. Some plugins have been written and contributed by third
parties outside of Pointwise's control.

TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, POINTWISE DISCLAIMS
ALL WARRANTIES, EITHER EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED
TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
PURPOSE, WITH REGARD TO THESE SCRIPTS. TO THE MAXIMUM EXTENT PERMITTED
BY APPLICABLE LAW, IN NO EVENT SHALL POINTWISE BE LIABLE TO ANY PARTY
FOR ANY SPECIAL, INCIDENTAL, INDIRECT, OR CONSEQUENTIAL DAMAGES
WHATSOEVER (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF BUSINESS
INFORMATION, OR ANY OTHER PECUNIARY LOSS) ARISING OUT OF THE USE OF OR
INABILITY TO USE THESE SCRIPTS EVEN IF POINTWISE HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGES AND REGARDLESS OF THE FAULT OR NEGLIGENCE OF
POINTWISE.

[Logo]: https://raw.github.com/dbgarlisch/CaeUnsAeroFXPost/master/logo_aerofxpost.png  "AeroFXPost Logo"
[SDKdocs]: http://www.pointwise.com/plugins
[SDKdownload]: http://www.pointwise.com/plugins/#sdk_downloads
