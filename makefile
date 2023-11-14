#
#   PGMcpp : PRIMED Grid Modelling (in C++)
#
#   Anthony Truelove MASc, P.Eng.
#   email:  gears1763@tutanota.com
#   github: gears1763-2
#
#   See license terms
#
#   Makefile for PGMcpp: PRIMED Grid Modelling (in C++)
#


## ======== USER INPUT ============================================================== ##

PROJECT_NAME = example

## ======== END USER INPUTS ========================================================= ##



## ======== COMPILER FLAGS ========================================================== ##

CXX = g++

CXXFLAGS = -std=c++17 -Wall -g -fPIC
#CXXFLAGS = -O3 -std=c++17 -fPIC

LIBS = -lpthread

## ======== END COMPILER FLAGS ====================================================== ##



## ======== BUILD =================================================================== ##

#### ==== Production Hierarchy <-- Combustion ==== ####

SRC_PRODUCTION = source/Production/Production.cpp
OBJ_PRODUCTION = object/Production/Production.o

.PHONY: Production
Production: $(SRC_PRODUCTION)
	$(CXX) $(CXXFLAGS) -c $(SRC_PRODUCTION) -o $(OBJ_PRODUCTION)


SRC_COMBUSTION = source/Production/Combustion/Combustion.cpp
OBJ_COMBUSTION = object/Production/Combustion/Combustion.o

.PHONY: Combustion
Combustion: $(SRC_COMBUSTION)
	$(CXX) $(CXXFLAGS) -c $(SRC_COMBUSTION) -o $(OBJ_COMBUSTION)


SRC_DIESEL = source/Production/Combustion/Diesel.cpp
OBJ_DIESEL = object/Production/Combustion/Diesel.o

.PHONY: Diesel
Diesel: $(SRC_DIESEL)
	$(CXX) $(CXXFLAGS) -c $(SRC_DIESEL) -o $(OBJ_DIESEL)


COMBUSTION_HIERARCHY = Production \
                       Combustion \
                       Diesel

OBJ_COMBUSTION_HIERARCHY = $(OBJ_PRODUCTION) \
                           $(OBJ_COMBUSTION) \
                           $(OBJ_DIESEL)


#### ==== Production Hierarchy <-- Renewable ==== ####

SRC_RENEWABLE = source/Production/Renewable/Renewable.cpp
OBJ_RENEWABLE = object/Production/Renewable/Renewable.o

.PHONY: Renewable
Renewable: $(SRC_RENEWABLE)
	$(CXX) $(CXXFLAGS) -c $(SRC_RENEWABLE) -o $(OBJ_RENEWABLE)


SRC_SOLAR = source/Production/Renewable/Solar.cpp
OBJ_SOLAR = object/Production/Renewable/Solar.o

.PHONY: Solar
Solar: $(SRC_SOLAR)
	$(CXX) $(CXXFLAGS) -c $(SRC_SOLAR) -o $(OBJ_SOLAR)


SRC_TIDAL = source/Production/Renewable/Tidal.cpp
OBJ_TIDAL = object/Production/Renewable/Tidal.o

.PHONY: Tidal
Tidal: $(SRC_TIDAL)
	$(CXX) $(CXXFLAGS) -c $(SRC_TIDAL) -o $(OBJ_TIDAL)


SRC_WAVE = source/Production/Renewable/Wave.cpp
OBJ_WAVE = object/Production/Renewable/Wave.o

.PHONY: Wave
Wave: $(SRC_WAVE)
	$(CXX) $(CXXFLAGS) -c $(SRC_WAVE) -o $(OBJ_WAVE)


SRC_WIND = source/Production/Renewable/Wind.cpp
OBJ_WIND = object/Production/Renewable/Wind.o

.PHONY: Wind
Wind: $(SRC_WIND)
	$(CXX) $(CXXFLAGS) -c $(SRC_WIND) -o $(OBJ_WIND)


RENEWABLE_HIERARCHY = Renewable \
                      Solar \
                      Tidal \
                      Wave \
                      Wind

OBJ_RENEWABLE_HIERARCHY = $(OBJ_RENEWABLE) \
                          $(OBJ_SOLAR) \
                          $(OBJ_TIDAL) \
                          $(OBJ_WAVE) \
                          $(OBJ_WIND)


#### ==== Storage Hierarchy ==== ####

SRC_STORAGE = source/Storage/Storage.cpp
OBJ_STORAGE = object/Storage/Storage.o

.PHONY: Storage
Storage: $(SRC_STORAGE)
	$(CXX) $(CXXFLAGS) -c $(SRC_STORAGE) -o $(OBJ_STORAGE)


SRC_LIION = source/Storage/LiIon.cpp
OBJ_LIION = object/Storage/LiIon.o

.PHONY: LiIon
LiIon: $(SRC_LIION)
	$(CXX) $(CXXFLAGS) -c $(SRC_LIION) -o $(OBJ_LIION)


STORAGE_HIERARCHY = Storage \
                    LiIon

OBJ_STORAGE_HIERARCHY = $(OBJ_STORAGE) \
                        $(OBJ_LIION)


#### ==== Model and Components ==== ####

SRC_CONTROLLER = source/Controller.cpp
OBJ_CONTROLLER = object/Controller.o

.PHONY: Controller
Controller: $(SRC_CONTROLLER)
	$(CXX) $(CXXFLAGS) -c $(SRC_CONTROLLER) -o $(OBJ_CONTROLLER)


SRC_ELECTRICALLOAD = source/ElectricalLoad.cpp
OBJ_ELECTRICALLOAD = object/ElectricalLoad.o

.PHONY: ElectricalLoad
ElectricalLoad: $(SRC_ELECTRICALLOAD)
	$(CXX) $(CXXFLAGS) -c $(SRC_ELECTRICALLOAD) -o $(OBJ_ELECTRICALLOAD)


SRC_RESOURCES = source/Resources.cpp
OBJ_RESOURCES = object/Resources.o

.PHONY: Resources
Resources: $(SRC_RESOURCES)
	$(CXX) $(CXXFLAGS) -c $(SRC_RESOURCES) -o $(OBJ_RESOURCES)


SRC_MODEL = source/Model.cpp
OBJ_MODEL = object/Model.o

.PHONY: Model
Model: $(SRC_MODEL)
	$(CXX) $(CXXFLAGS) -c $(SRC_MODEL) -o $(OBJ_MODEL)


MODEL = Controller \
        ElectricalLoad \
        Resources \
        Model

OBJ_MODEL_COMPONENTS = $(OBJ_CONTROLLER) \
                       $(OBJ_ELECTRICALLOAD) \
                       $(OBJ_RESOURCES) \
                       $(OBJ_MODEL)


#### ==== Tests ==== ####

OBJ_ALL = $(OBJ_COMBUSTION_HIERARCHY) \
          $(OBJ_RENEWABLE_HIERARCHY) \
          $(OBJ_STORAGE_HIERARCHY) \
          $(OBJ_MODEL_COMPONENTS)

#...


#### ==== Project ==== ####

SRC_PROJECT = projects/$(PROJECT_NAME).cpp
OBJ_PROJECT = object/$(PROJECT_NAME).o
OUT_PROJECT = bin/$(PROJECT_NAME).out

.PHONY: PGMcpp_project
PGMcpp_project: $(OBJ_PROJECT)
	$(CXX) $(CXXFLAGS) $(OBJ_PROJECT) $(OBJ_ALL) -o $(OUT_PROJECT) $(LIBS)

$(OBJ_PROJECT): $(SRC_PROJECT)
	$(CXX) $(CXXFLAGS) -c $(SRC_PROJECT) -o $(OBJ_PROJECT)

## ======== END BUILD =============================================================== ##



## ======== TARGETS ================================================================= ##


.PHONY: clean
clean:
	rm -frv bin
	rm -frv core
	rm -frv object
	rm -frv test/bin
	rm -frv test/object


.PHONY: dirs
dirs:
	mkdir -pv bin/Production/Combustion
	mkdir -pv bin/Production/Renewable
	mkdir -pv bin/Storage
	mkdir -pv core
	mkdir -pv object/Production/Combustion
	mkdir -pv object/Production/Renewable
	mkdir -pv object/Storage
	mkdir -pv test/bin/Production/Combustion
	mkdir -pv test/bin/Production/Renewable
	mkdir -pv test/bin/Storage
	mkdir -pv test/object/Production/Combustion
	mkdir -pv test/object/Production/Renewable
	mkdir -pv test/object/Storage


.PHONY: docs
docs:
	rm -fv docs/PGMcpp_manual_html.7z
	rm -fv docs/PGMcpp_manual_LaTeX.pdf
	doxygen docs/config.doxygen
	(cd docs/latex && \
     make pdf && \
     mv -fv refman.pdf ../PGMcpp_manual_LaTeX.pdf)
	7z a -t7z -m0=lzma -mx=9 -mfb=64 -md=32m -ms=on docs/PGMcpp_manual_html.7z docs/html
	rm -frv docs/html
	rm -frv docs/latex


.PHONY: PGMcpp
PGMcpp:
	make dirs
	make $(COMBUSTION_HIERARCHY)
	make $(RENEWABLE_HIERARCHY)
	make $(STORAGE_HIERARCHY)
	make $(MODEL)


.PHONY: project
project:
	make PGMcpp_project
	$(OUT_PROJECT)

## ======== END TARGETS ============================================================= ##
