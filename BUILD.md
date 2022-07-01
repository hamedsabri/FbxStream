# Build

#### 1. source build environment

```
source build.env
```

#### 2. How To use build.py script

##### Arguments

There are 3 arguments that must be passed to the script: 

| Flags                 | Description                                                                           |
|--------------------   |-------------------------------------------------------------------------------------- |
|  --fbx-location   	| directory where FBX library is located.                                             	|
|  --fmt-location   	| directory where FMT library is located.                                             	|
| workspace_location    | directory where the project use as a workspace to build and install examples  		|

##### Stages

| Flag                 | Description                                                                           |
|--------------------  |--------------------------------------------------------------------------------------------------- |
|   --stages           | comma-separated list of stages can also be passed to the build system. By default "clean, configure, build, install" stages are executed if this argument is not set. |

| Options       | Description                                                                                   |
|-----------    |---------------------------------------------------                                            |
| clean         | clean build                                                                                   |
| configure     | call this stage every time a cmake file is modified                                           |
| build         | builds the project                                                                            |
| install       | installs the project                                                                          |

example
```
--stages=configure,build,install
```
***NOTE:*** All the flags can be followed by either ```space``` or ```=```

##### Variant

supported build variants: debug, release, relWithDebInfo can be passed via `--variant` flag.
