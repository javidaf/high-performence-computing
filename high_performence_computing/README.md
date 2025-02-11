# Higher Performance Computing

## Prerequisites
- MinGW (Windows)
- SCons
- Python
- conda

### Installing MinGW
Download and install MinGW from [here](https://sourceforge.net/projects/mingw/)
Add the MinGW bin directory to the system PATH

### seting up the 'scons' environment
Create a new conda environment from the `environment.yml` file, this will also install 'scons' in the environment. 
```bash
conda create env -f environment.yml
conda activate c_hpc
```



## Building the Project
Build the project with the scons installed in the conda environment
so conda evnironment should be activated before running the following command
```bash
cd path/to/high_performence_computing
scons
```
or if there is a problem with existing external site configuration

```bash
scons --no-site-dir
```

## Cleaning the Build
```bash
scons -c --no-site-dir
```
or 
```bash
scons -c
```

## Running the Program
```bash
./build/program
```

## Alternative
Alternatively, setup vscode build task to run the build and run the program

## Project Structure
- `src/`: Source files
- `include/`: Header files
- `tests/`: Test files
- `build/`: Build output
- `SConstruct`: Build configuration