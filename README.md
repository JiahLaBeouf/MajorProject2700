# MajorProject2700
Repo for the MTRX2700 Major project for Semester 1 2022

Members: 
Jiah Pang
Joe Ovelgonene 
Shivam Varyani
Jadon Routley 
Lachlan Mizon
Oscar Mower


# Overview of project
- Our group is looking to automate stock tracing for supermarkets
- This would be a robust solution that is capable of performing tracing for numerous different items, but will focus on the supermarket drink aisles (particularly milk cartons). Our solution can handle different shapes and sizes with relative ease.
- Since many supermarkets already implement tilted aisles that allow the cartons to fall down as one is removed, we can use this premise to identify any substantial changes in distance detected by a LiDAR sensor, which would indicate a carton has been removed.
- A magnetometer allows the device to be set up in a semi-automated fashion, by redirecting towards the desired direction of the cartons. 
- LCD display indicates how many bottles are left, and a soft audio cue indicates a change, or if inventory is low.

# Components

## LiDAR
The lidar is the primary sensor in the design for the detection of change in environment in the application of our product. We use it primarily for detecting the distance to the closest bottle/carton
Essentially uses laser return time back to source to determine the distance of the closest milk carton
Utilises timer and timer interrupts to measure time from light incident
Checked continually and data is processed off board in python
Data is ported using serial communication link and putty to python port off board

## Smoothing and Error Checking 
In both the python code and the c code, we employ Gaussian double pass averaging to smooth the data being outputted by the lidar and the magnetometer. 
This involves the  

## Motors
The lidar is mounted on a multi axis dual servo configuration which allows for rotation, panning and tilting. 
The motors in the PTU unit are primarily used for tracking across the range of vision for the device
Unit starts by panning range of vision, recording closest object and then moving back to location
Stretch target uses motors to detect multiple aisle stock levels
Using PWM signal to continually move the servo at increments, then storing ideal positions to refer back to
Could be expanded to use in tandem with the gyro


## Magnetometer
The PCU unit contains a HMC5883L 3-axis magnetometer.
Measures magnetic intensity across all 3 axes from device
Magnets placed at the end of each tracked aisle
Magnetometer will pick up placed magnet and can use intensity and position to determine stock levels and bottle type (2L, 1L, etc)
Data serialized and processed off board in python


## External computer / off board processing
One of the key components of the design is the data processing from points collected by the magnetometer and LiDAR sensor. Due to the limited capability of the HCS12, porting to an off-board processing point became the most viable solution to the task
Serial out data to Putty via I2C
Python connected to putty comms
Python data from putty to csv
Csv data processed by python, remove noise and refactor data
Present clean data from the board at end of python pipeline
Can also be expanded for case-by-case use to develop stock trends and reorder forms etc. by introducing additional analysis points

## Displays and output
LCD display unit which will output a message indicating the number of milk cartons left on the shelf, and give a warning if this number is low.
A small tune will be played each time a carton is removed, and an alternate siren-like sound plays when stock is low.

# Code environment


# Commiting code to github (push)
- ensure you have the repository cloned to your workspace
- navigate to the project folder of the code you're trying to change/view in your IDE
- when going to edit code, open the file straight from the location you cloned the repository to (ie repo/project/cources/x.c or whatever)
- SAVE YOUR CHANGES
- once you've done so, using either github desktop or command line, commit your changes and add a title/message to your commit so we can roll back code if necessary
- make sure to pull cofe prior to making changes to ensure you're working on the most recent code

## Github Desktop
If you're unfamiliar with git, it might be easiest to use the desktop client rather than using terminals/zip files from the web client.  

### installation
https://desktop.github.com/  
click the link and login with your github account

## uploading projects to repo
Ensure you upload the entire project when uploading code for the first time
- Navigate to the repository on your machine
- create the project in the repo
- once configured, make a commit with the project file in it

## editing code
Testing
