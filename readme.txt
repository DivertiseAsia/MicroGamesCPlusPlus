To build .apk file
1. use SDK tools version 25.2.5
2. apart from the target API, Android 4.0.3 (API 15) is needed
3. command "cocos deploy -p android" to compile and auto upload to you mobile 

other command 
command "cocos compile -p android" to compile code to apk file
command "cocos run -p android" to compile and auto upload and auto run after upload
command "cocos deploy-p android -m release" to deploy mode release 

last build 
use cocos2d-x version 3.14.1
use python version 2.7.5
use java sdk version 1.8.141
use NDK version 14b
use SDK tools version 26.0.0 rc2
use SDK platfrom version 9 (version min of build)
use SDK platfrom version 15 (version target of build)

change version android target 
  cd proj.android
  open project.properties change target version to version 15
  open AndroidManifest.xml change targetSdkVersion to version 15
  command "cocos deploy -p android"
finish 
test run on API version 15 up work 
(API 23 on my mobile , API 17,19,22 on visual studio emulator and nox emulator)



