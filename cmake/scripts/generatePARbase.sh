#!/bin/bash

PROOF_INF_DIR="$1"
shift
LIBNAME="$1"
shift
INCLUDES="$1"
shift
DEP_LIBS="$*"

function GenerateBUILD_sh() {

if [ -d $PROOF_INF_DIR ];then
cat > $PROOF_INF_DIR/BUILD.sh <<EOF
#!/bin/bash

MAKEOPTS="-j\`cat /proc/cpuinfo | grep processor | wc -l\`"
make \$MAKEOPTS
EOF
  chmod +x $PROOF_INF_DIR/BUILD.sh
fi
}

function GenerateSETUP_C() { 

if [ -d $PROOF_INF_DIR ];then
cat > $PROOF_INF_DIR/SETUP.C <<EOF
Int_t SETUP() {

   gSystem->SetDynamicPath(Form("%s:%s", gSystem->pwd(), gSystem->GetDynamicPath()));
   
EOF

for DEP in $DEP_LIBS;do
  echo "   if (gSystem->Load(\"lib$DEP.so\")<0) return 1;" >> $PROOF_INF_DIR/SETUP.C
done

if [ -n "$DEP_LIBS" ];then
  echo "" >> $PROOF_INF_DIR/SETUP.C
fi

cat >> $PROOF_INF_DIR/SETUP.C <<EOF
   // load current lib$LIBNAME.so
   if (gSystem->Load("lib$LIBNAME.so")<0) return 1;
EOF



echo "" >> $PROOF_INF_DIR/SETUP.C
echo "   // Set the include paths" >> $PROOF_INF_DIR/SETUP.C
echo "   gROOT->ProcessLine(\".include $LIBNAME\");" >> $PROOF_INF_DIR/SETUP.C

MAIN_SRC_INCLUDE="$LIBNAME"
if [ -n "$INCLUDES" ];then
  INCLUDES=${INCLUDES//://}
  for INC in $INCLUDES;do
    echo "   gROOT->ProcessLine(\".include $INC\");" >> $PROOF_INF_DIR/SETUP.C
    MAIN_SRC_INCLUDE="$INC"
  done
fi

cat >> $PROOF_INF_DIR/SETUP.C <<EOF

   // Set our location, so that other packages can find us
   gSystem->Setenv("${LIBNAME}_INCLUDE", "$MAIN_SRC_INCLUDE");
   
   return 0;
}
EOF

fi
}

# function GenerateMakefile() {
# MAKEFILE_DIR=`cd $PROOF_INF_DIR;cd ../;pwd`
# 
# cat > $MAKEFILE_DIR/Makefile <<EOF
# include Makefile.arch
# 
# PACKAGE = $LIBNAME
# 
# default-target: lib\$(PACKAGE).so
# 
# ALICEINC      = -I.
# 
# # add include paths from other par files
# ifneq ($(ESD_INCLUDE),)
#    ALICEINC += -I../\$(ESD_INCLUDE)
# endif
# 
# ifneq (\$(AOD_INCLUDE),)
#    ALICEINC += -I../\$(AOD_INCLUDE)
# endif
# 
# ifneq (\$(STEERBase_INCLUDE),)
#    ALICEINC += -I../\$(STEERBase_INCLUDE)
# endif
# 
# ifneq (\$(ANALYSIS_INCLUDE),)
#    ALICEINC += -I../\$(ANALYSIS_INCLUDE)
# endif
# 
# ifneq (\$(OADB_INCLUDE),)
#    ALICEINC += -I../\$(OADB_INCLUDE)
# endif
# 
# ifneq (\$(ANALYSISalice_INCLUDE),)
#    ALICEINC += -I../\$(ANALYSISalice_INCLUDE)
# endif
# 
# # only if no par file was loaded before
# ifeq (\$(ALICEINC),-I.)
#   ifneq (\$(ALICE_ROOT),)
#     ALICEINC += -I\$(ALICE_ROOT)/include -I\$(ALICE_ROOT)/ANALYSIS/Tender -I\$(ALICE_ROOT)/TOF -I\$(ALICE_ROOT)/TRD/Cal -I\$(ALICE_ROOT)/VZERO -I\$(ALICE_ROOT)/TPC -I\$(ALICE_ROOT)/T0 -I\$(ALICE_ROOT)/EMCAL
#   endif
# endif
# 
# CHECKXML      := \$(shell root-config --has-xml)
# 
# ifeq (\$(CHECKXML),yes)
#   CXXFLAGS += -DWITHXML
#   CINTFLAGS += -DWITHXML
# endif
# 
# CXXFLAGS += \$(ALICEINC) -g
# 
# EOF
# cat >> $MAKEFILE_DIR/Makefile <<EOF
# 
# SRCS = AliAnalysisDataContainer.cxx AliAnalysisDataSlot.cxx AliAnalysisManager.cxx AliAnalysisTask.cxx AliAnalysisSelector.cxx AliAnalysisGrid.cxx AliAnalysisStatistics.cxx AliAnalysisTaskCfg.cxx
# HDRS = AliAnalysisDataContainer.h AliAnalysisDataSlot.h AliAnalysisManager.h AliAnalysisTask.h AliAnalysisSelector.h AliAnalysisGrid.h AliAnalysisStatistics.h AliAnalysisTaskCfg.h
# DHDR = ANALYSISLinkDef.h
# 
# DHDR_ANALYSIS := \$(DHDR)
# HDRS_ANALYSIS := \$(HDRS)
# SRCS_ANALYSIS := \$(SRCS) G__\$(PACKAGE).cxx
# OBJS_ANALYSIS := \$(SRCS_ANALYSIS:.cxx=.o)
# 
# PARFILE       = \$(PACKAGE).par
# 
# 
# lib\$(PACKAGE).so: \$(OBJS_ANALYSIS)
# 	@echo "Linking" \$@ ...
# 	@/bin/rm -f \$@
# ifeq (\$(PLATFORM),macosx)
# # We need to make both the .dylib and the .so
# 		\$(LD) \$(SOFLAGS)\$@ \$(LDFLAGS) $^ \$(OutPutOpt) \$@
# ifneq (\$(subst \$(MACOSX_MINOR),,1234),1234)
# ifeq (\$(MACOSX_MINOR),4)
# 		ln -sf \$@ \$(subst .\$(DllSuf),.so,\$@)
# else
# 		\$(LD) -bundle -undefined \$(UNDEFOPT) \$(LDFLAGS) $^ \
# 		   \$(OutPutOpt) \$(subst .\$(DllSuf),.so,\$@)
# endif
# endif
# else
# ifeq (\$(PLATFORM),win32)
# 		bindexplib \$* $^ > \$*.def
# 		lib -nologo -MACHINE:IX86 $^ -def:\$*.def \
# 		   \$(OutPutOpt)\$(EVENTLIB)
# 		\$(LD) \$(SOFLAGS) \$(LDFLAGS) $^ \$*.exp \$(LIBS) \
# 		   \$(OutPutOpt)\$@
# else
# 		\$(LD) \$(SOFLAGS) \$(LDFLAGS) $^ \$(OutPutOpt) \$@ \$(EXPLLINKLIBS)
# endif
# endif
# 	@chmod a+x \$@
# 	@echo "done"
# 
# %.o:    %.cxx %.h
# 	\$(CXX) \$(CXXFLAGS) \$(PACKCXXFLAGS) -c $< -o \$@
# 
# clean:
# 	@rm -f \$(OBJS_ANALYSIS) *.so G__\$(PACKAGE).* \$(PARFILE)
# 
# G__\$(PACKAGE).cxx G__\$(PACKAGE).h: \$(HDRS) \$(DHDR)
# 	@echo "Generating dictionary ..."
# 	rootcint -f \$@ -c \$(CINTFLAGS) \$(ALICEINC) $^
# 
# ### CREATE PAR FILE
# 
# \$(PARFILE): \$(patsubst %,\$(PACKAGE)/%,\$(filter-out G__%, \$(HDRS_ANALYSIS) \$(SRCS_ANALYSIS) \$(DHDR_ANALYSIS) Makefile Makefile.arch PROOF-INF))
# 	@echo "Creating archive" \$@ ...
# 	@tar cfzh \$@ \$(PACKAGE)
# 	@rm -rf \$(PACKAGE)
# 	@echo "done"
# 
# \$(PACKAGE)/Makefile: Makefile #.\$(PACKAGE)
# 	@echo Copying $< to \$@ with transformations
# 	@[ -d \$(dir \$@) ] || mkdir -p \$(dir \$@)
# 	@sed 's/include \$\$(ROOTSYS)\/test\/Makefile.arch/include Makefile.arch/' < $^ > \$@
# 
# \$(PACKAGE)/Makefile.arch: \$(ROOTSYS)/test/Makefile.arch
# 	@echo Copying $< to \$@
# 	@[ -d \$(dir \$@) ] || mkdir -p \$(dir \$@)
# 	@cp -a $^ \$@
# 
# \$(PACKAGE)/PROOF-INF: PROOF-INF.\$(PACKAGE)
# 	@echo Copying $< to \$@
# 	@[ -d \$(dir \$@) ] || mkdir -p \$(dir \$@)
# 	@cp -a -r $^ \$@
# 
# \$(PACKAGE)/%: %
# 	@echo Copying $< to \$@
# 	@[ -d \$(dir \$@) ] || mkdir -p \$(dir \$@)
# 	@cp -a $< \$@
# 
# test-%.par: %.par
# 	@echo "INFO: The file $< is now tested, in case of an error check in par-tmp."
# 	@mkdir -p par-tmp
# 	@cd par-tmp; tar xfz ../$<;	cd \$(subst .par,,$<); PROOF-INF/BUILD.sh
# 	@rm -rf par-tmp
# 	@echo "INFO: Testing succeeded (already cleaned up)"
# EOF
# 
# }

GenerateBUILD_sh
GenerateSETUP_C
# GenerateMakefile
