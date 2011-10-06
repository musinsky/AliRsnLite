#!/bin/bash

PRE_CMD=""
CMD=""
POST_CMD=""
ARGS=" "
MACRO=""
USE_OLD_DIR=""
ALIRSNLITE_OUTDIR="/tmp/alirsnlite"
ALIRSNLITE_IDFILE="/tmp/.alirsnliteid"
ALIRSNLITE_ID="1"
ALIRSNLITE_WKDIR=""
ALIRSNLITE_SRC_DIR=""
ALIRSNLITE_WORKSRC=""
ALIRSNLITE_TASKS_DIR=""
ALIRSNLITE_USE_ENV=""
ALIRSNLITE_CREATE_PARS=""
ALIRSNLITE_AN_PLUGIN_ARGS="grid:terminate"
#ALIRSNLITE_AN_INPUT="esd:mc"
ALIRSNLITE_AN_INPUT="aod"
ALIRSNLITE_AN_EVENTS="1e10:0"
ALIRSNLITE_MANAGERS=""

function ShowHelp() {
  echo "HELP TODO"
  exit 1
}

function SetAliRsnLiteSrcDir() {
  if [ "${0:0:1}" == "." ];then
    ALIRSNLITE_SRC_DIR="`cd ../;pwd`"
  elif [ "${0:0:1}" == "/" ];then
    ALIRSNLITE_SRC_DIR="`cd $(dirname $0)/../;pwd`"
  else
    ALIRSNLITE_SRC_DIR="`cd $(dirname ${PWD}/$0)/../;pwd`"
  fi

  ALIRSNLITE_WORKSRC="$ALIRSNLITE_SRC_DIR/macros/AliRsnLiteWork"
  ALIRSNLITE_TASKS_DIR="$ALIRSNLITE_WORKSRC/tasks"

  test -d $ALIRSNLITE_TASKS_DIR || mkdir -p $ALIRSNLITE_TASKS_DIR
}

function CreateParfiles() {
  local MY_PWD=`pwd`
  cd $ALIRSNLITE_SRC_DIR/build
  echo "Creating all parfiles ..."
  make par-all > /dev/null 2>&1
  test -d $ALIRSNLITE_SRC_DIR/pars || mkdir -p $ALIRSNLITE_SRC_DIR/pars
  mv -f *.par $ALIRSNLITE_SRC_DIR/pars/
  cd $MY_PWD
}

function InitWorkingDir() {

  # generates id
  test -f $ALIRSNLITE_IDFILE || echo "0" > $ALIRSNLITE_IDFILE
  ALIRSNLITE_ID="`cat $ALIRSNLITE_IDFILE`"
  test -z $USE_OLD_DIR && {
    ALIRSNLITE_ID=`expr $ALIRSNLITE_ID + 1`
    echo "$ALIRSNLITE_ID" > $ALIRSNLITE_IDFILE
  }

  # create working direcotry
  ALIRSNLITE_WKDIR="${ALIRSNLITE_OUTDIR}/`printf "%04.0f" $ALIRSNLITE_ID`"
  echo "Creating $ALIRSNLITE_WKDIR ..."
  test -d $ALIRSNLITE_WKDIR || mkdir -p $ALIRSNLITE_WKDIR

  # create last link
  ln -sfn $ALIRSNLITE_WKDIR $ALIRSNLITE_OUTDIR/last
  
  cd $ALIRSNLITE_WKDIR

  # copy global macros
  cp -f $ALIRSNLITE_WORKSRC/*.C .

  # copy datasets txt files
  test -d  $ALIRSNLITE_WORKSRC/datasets/ && cp -f $ALIRSNLITE_WORKSRC/datasets/* .

  for MY_DIR in `ls $ALIRSNLITE_WORKSRC/tasks 2> /dev/null`;do
    echo "$ALIRSNLITE_TASKS_DIR/$MY_DIR"
    if [ -d $ALIRSNLITE_TASKS_DIR/$MY_DIR -o -h $ALIRSNLITE_TASKS_DIR/$MY_DIR ];then
      cp -f $ALIRSNLITE_TASKS_DIR/$MY_DIR/* .
      if [ ! $? -eq 0 ];then
        echo ""
        echo "Error : Problem copying files from $ALIRSNLITE_TASKS_DIR/$MY_DIR to `pwd`"
        echo ""
        exit 2
      fi
    else
      exit 1
    fi
  done

  test -n "$ALIRSNLITE_CREATE_PARS" && cp -f $ALIRSNLITE_SRC_DIR/pars/*.par .

}

function Run() {
  test -n "$CMD" || ShowHelp
  # this is together because ARGS will have space in front and in back
  local AN_SRC=`echo "$ALIRSNLITE_AN_PLUGIN_ARGS" | awk -F ':' '{print $1}'`
  local AN_MODE=`echo "$ALIRSNLITE_AN_PLUGIN_ARGS" | awk -F ':' '{print $2}'`

  # handle input
  local AN_INPUT=`echo "$ALIRSNLITE_AN_INPUT" | awk -F ':' '{print $1}'`
  local AN_INPUT_MC=`echo "$ALIRSNLITE_AN_INPUT" | awk -F ':' '{print $2}'`

  # handle events
  local AN_EVENTS=`echo "$ALIRSNLITE_AN_EVENTS" | awk -F ':' '{print $1}'`
  local AN_EVENTS_SKIP=`echo "$ALIRSNLITE_AN_EVENTS" | awk -F ':' '{print $2}'`
  test -z "$AN_EVENTS_SKIP" && AN_EVENTS_SKIP="0"

  local AN_PATH="\$ALICE_ROOT"
  test "$ALIRSNLITE_USE_ENV" = "1" && AN_PATH="$ALIRSNLITE_SRC_DIR" 

  MACRO="RunALICE.C(\"$AN_SRC\",\"$AN_MODE\",\"$AN_INPUT\",\"$AN_INPUT_MC\",$AN_EVENTS,$AN_EVENTS_SKIP,\"$ALIRSNLITE_MANAGERS\",\"$AN_PATH\",\"$ALIRSNLITE_TASKS_DIR\")"
  echo "Running $PRE_CMD$CMD$ARGS$MACRO $POST_CMD ..."
  $PRE_CMD $CMD$ARGS$MACRO $POST_CMD
}

if [ "$#" == "0" ]; then
  ShowHelp
  exit 1
fi

# Sets AliRsnLiteSrcDir (we assume that this script is running from $ALIRSNLITE_SRC_DIR/scripts)
SetAliRsnLiteSrcDir

while [[ $1 = -* ]]; do
    arg=$1; shift
    case $arg in
    help)
      ShowHelp
    ;;
    --root)
      CMD="root"
    ;;
    --root.exe)
      CMD="root.exe"
    ;;
    --aliroot)
      CMD="aliroot"
    ;;
    --quit)
      ARGS="$ARGS-q "
    ;;
    -q)
      ARGS="$ARGS-q "
    ;;
    --batch)
      ARGS="$ARGS-b "
    ;;
    -b)
      ARGS="$ARGS-b "
    ;;
    -l)
      ARGS="$ARGS-l "
    ;;
    --old-dir)
      USE_OLD_DIR="1"
    ;;
    --plugin)
      ALIRSNLITE_AN_PLUGIN_ARGS="$1"
      shift
    ;;
    --input)
      ALIRSNLITE_AN_INPUT="$1"
      shift
    ;;
    --events)
      ALIRSNLITE_AN_EVENTS="$1"
      shift
    ;;
    --pars)
      ALIRSNLITE_CREATE_PARS="1"
    ;;
    --alirsnlite-env)
      ALIRSNLITE_USE_ENV="1"
      export LD_LIBRARY_PATH="$ALIRSNLITE_SRC_DIR/lib/tgt_`root-config --arch`:$LD_LIBRARY_PATH"
      export PATH="$ALIRSNLITE_SRC_DIR/bin/tgt_`root-config --arch`:$PATH"
    ;;
    --managers)
      ALIRSNLITE_MANAGERS="$1"
      shift
    ;;
    *)
    ShowHelp
    ;;
  esac
done

test -n "$ALIRSNLITE_CREATE_PARS" && CreateParfiles
InitWorkingDir
Run
