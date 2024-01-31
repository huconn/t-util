####################################################
# This project use CMake.
# but, Mafefile has been created just user's convenience.
# Do not modify Makefile and Makeoption.
# in order to modify or add make option,
# you should modify CMakeLists.txt or *.cmake
#
# Do not modify Makefile in order to modify make option.
#####################################################

#color define for echo
RED_COLOR := \e[1;31m
BLUE_COLOR := \e[1;34m
ORIGIN_COLOR := $<\e[0m

# CMake option
CMAKE_BUILD_DIR = build/
ifeq ($(INSTALL_DIR),)
CMAKE_INSTALL_DIR = install/
else
CMAKE_INSTALL_DIR = $(INSTALL_DIR)
endif
CMAKE_OPTION = -DCMAKE_INSTALL_PREFIX=../$(CMAKE_BUILD_DIR)/$(CMAKE_INSTALL_DIR)

USE_T_TEST = $(shell echo "-DUSE_T_TEST=$$USE_T_TEST")

all: configure title install
install : title

title : configure

configure:
	@echo "================================================"
	@echo -e "$(BLUE_COLOR)Cmake configure info"
	@echo -e "Chipset Option    : $(CHIPSET)"
	@echo -e "Enable Tester     : $(USE_T_TEST)"
	@echo "================================================"
	mkdir -p $(CMAKE_BUILD_DIR)
	cd $(CMAKE_BUILD_DIR) && \
	cmake $(CMAKE_OPTION) $(USE_T_TEST) ../

title:
	@echo "================================================"
	@echo -e "$(BLUE_COLOR)Cmake building start $(ORIGIN_COLOR)"
	@echo "================================================"
	$(MAKE) -C $(CMAKE_BUILD_DIR)

install:
	@echo "================================================"
	@echo -e "$(BLUE_COLOR)Cmake installing start $(ORIGIN_COLOR)"
	@echo -e "Install Dir : $(BLUE_COLOR) $(PWD)/$(CMAKE_INSTALL_DIR) $(ORIGIN_COLOR)"
	@echo "================================================"
	cd $(CMAKE_BUILD_DIR) && \
	$(MAKE) install

clean:
	@echo "================================================"
	@echo -e "$(BLUE_COLOR)Cmake clean $(ORIGIN_COLOR)"
	@echo "================================================"
	cd $(CMAKE_BUILD_DIR) && \
	$(MAKE) clean

distclean:
	rm -rf $(CMAKE_BUILD_DIR)
