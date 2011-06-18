#!/bin/bash
MY_LIBS="STEERBase@STEER ESD@STEER AOD@STEER ANALYSIS@ANALYSIS OADB@OADB ANALYSISalice@ANALYSIS EventMixing@ANALYSIS CORRFW@CORRFW PWG2resonances@PWG2"
# MY_LIBS="STEERBase@STEER ESD@STEER AOD@STEER"

function Init_STEERBase() {
  return 0
}

function Init_ESD() {
  cp $2/patches/ESD/AliESDHeader.patch $2/STEER/
  local MYPWD=`pwd`
  cd $2/STEER/
  patch -p1 -i AliESDHeader.patch
  rm -f AliESDHeader.patch
  cd $MYPWD
  return 0
}

function Init_AOD() {
  return 0
}

function Init_ANALYSIS() {
  return 0
}

function Init_OADB() {
#   echo "OADB copy needed from $ALICE_ROOT/$1 to $2/OADB "
  cp -rf $ALICE_ROOT/$1/COMMON $2/OADB/
  cp -rf $ALICE_ROOT/$1/PWG1 $2/OADB/
  cp -rf $ALICE_ROOT/$1/PWG2 $2/OADB/
  cp -rf $ALICE_ROOT/$1/PWG3 $2/OADB/
  cp -rf $ALICE_ROOT/$1/PWG4 $2/OADB/
  return 0
}

function Init_ANALYSISalice() {
  return 0
}

function Init_EventMixing() {
  return 0
}

function Init_CORRFW() {
  return 0
}

function Init_PWG2resonances() {
  echo "set ( EXPORT \${HDRS})" >> $2/PWG2/CMakelibPWG2resonances.pkg
  return 0
}

function backupAliRsn() {
  if [ ! -d $1/tmp ];then
    mkdir -p $1/tmp
  fi

  if [ -d $1/$2 ];then
    rm -rf $1/tmp/$2
    mv $1/$2 $1/tmp/$2
  fi
}

function ImportAliRootLib() {
  test -z "$1" && return 1
  test -z "$2" && return 2

  local MY_TMP_LIB=`echo $1 | awk -F '@' '{print $1}'`
  local MY_TMP_DIR=`echo $1 | awk -F '@' '{print $2}'`
  test -z "$MY_TMP_LIB" && return 3
  test -z "$MY_TMP_DIR" && return 4

  echo "Processing $MY_TMP_LIB.par ..."

  backupAliRsn $2 $MY_TMP_LIB 

  # user has right to write to $ALICE_ROOT we create parfile
  make $MY_TMP_LIB.par > /dev/null 2>&1
  if [ "$?" != "0" ];then
    return $?
  fi

  cp $MY_TMP_LIB.par $2/pars/$MY_TMP_LIB.par

  # unpack parfile
  mkdir -p $2/$MY_TMP_DIR

  if [ "$MY_TMP_LIB" != "$MY_TMP_DIR" ];then
    mkdir -p $2/pars/tmp/$MY_TMP_DIR/
    tar xfz $2/pars/$MY_TMP_LIB.par -C $2/pars/tmp/$MY_TMP_DIR/
#     ls -al $2/$MY_TMP_DIR/
    cp -rf $2/pars/tmp/$MY_TMP_DIR//$MY_TMP_LIB/* $2/$MY_TMP_DIR/
    mv $2/$MY_TMP_DIR/PROOF-INF $2/$MY_TMP_DIR/PROOF-INF.$MY_TMP_LIB
  else
    tar xfz $2/pars/$MY_TMP_LIB.par -C $2/
    mv $2/$MY_TMP_DIR/PROOF-INF $2/$MY_TMP_DIR/PROOF-INF.$MY_TMP_LIB
  fi

  # copy CMakelib$MY_TMP_LIB.pkg from $ALICE_ROOT/$MY_TMP_DIR
  cp $ALICE_ROOT/$MY_TMP_DIR/CMakelib$MY_TMP_LIB.pkg $2/$MY_TMP_DIR/

  rm -f $2/$MY_TMP_DIR/Makefile
  rm -f $2/$MY_TMP_DIR/Makefile.arch
#   rm -Rf $2/$MY_TMP_DIR/$MY_TMP_LIB
  
  Init_$MY_TMP_LIB $MY_TMP_DIR $MY_ALIRSNLITE_DIR

}

function CleanAliRsnLiteDirs() {
  rm -rf $MY_ALIRSNLITE_DIR/cmake
  rm -Rf $MY_ALIRSNLITE_DIR/pars
  rm -Rf $MY_ALIRSNLITE_DIR/lib
  rm -Rf $MY_ALIRSNLITE_DIR/bin
  rm -Rf $MY_ALIRSNLITE_DIR/include
  rm -Rf $MY_ALIRSNLITE_DIR/build
}


if [ -z "$ALICE_ROOT" ];then
  echo "Define \$ALICE_ROOT !!!"
  exit 2
fi

if [ "${0:0:1}" == "." ];then
  MY_ALIRSNLITE_DIR="`cd ../;pwd`"
elif [ "${0:0:1}" == "/" ];then
  MY_ALIRSNLITE_DIR="`cd $(dirname $0)/../;pwd`"
else
  MY_ALIRSNLITE_DIR="`cd $(dirname ${PWD}/$0)/../;pwd`"
fi

CleanAliRsnLiteDirs


cp -rf $ALICE_ROOT/cmake $MY_ALIRSNLITE_DIR/
mkdir -p $MY_ALIRSNLITE_DIR/pars/tmp

for MY_DIR in $MY_LIBS;do
  MY_TMP_DIR=`echo $MY_DIR | awk -F '@' '{print $2}'`
  if [ -n $MY_TMP_DIR ];then
    if [ -d $MY_ALIRSNLITE_DIR/$MY_TMP_DIR ];then
      rm -Rf $MY_ALIRSNLITE_DIR/$MY_TMP_DIR
    fi
  fi
done

MYPWD=`pwd`
cd $MY_ALIRSNLITE_DIR/pars/tmp
cmake $ALICE_ROOT
for MY_LIB in $MY_LIBS;do
  ImportAliRootLib $MY_LIB $MY_ALIRSNLITE_DIR
  if [ "$?" != "0" ];then
    echo "Error in $MY_LIB !!!"
    exit 2
  fi
done
cd $MYPWD

# cleaning tmp dirs
rm -Rf $MY_ALIRSNLITE_DIR/pars/tmp
rm -Rf $MY_ALIRSNLITE_DIR/tmp

echo ""
echo "AliRsnLite is sync in to $MY_ALIRSNLITE_DIR ..."
echo ""
echo "mkdir -p $MY_ALIRSNLITE_DIR/build"
echo "cd $MY_ALIRSNLITE_DIR/build"
echo "cmake ../"
echo "make -j install"

echo ""