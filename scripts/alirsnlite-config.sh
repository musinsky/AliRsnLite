#!/bin/bash
#!/bin/bash

ALIRSNLITE_SRC_DIR=""


function SetAliRsnLiteSrcDir() {
  if [ "${0:0:1}" == "." ];then
    ALIRSNLITE_SRC_DIR="`cd ../;pwd`"
  elif [ "${0:0:1}" == "/" ];then
    ALIRSNLITE_SRC_DIR="`cd $(dirname $0)/../;pwd`"
  else
    ALIRSNLITE_SRC_DIR="`cd $(dirname ${PWD}/$0)/../;pwd`"
  fi
}
function CreateDir() {
  
  if [ -z "$1" ];then
    return
  fi
  
  if [ ! -d $1 ];then
      mkdir -p $1  
  fi
}

function ShowHelp() {
  echo "./alirsnlite-config --list"
}

function ListMacros() {
  echo "=== TASK dir: $ALIRSNLITE_TASKS_DIR ==="
  ls -1 $ALIRSNLITE_TASKS_DIR
  echo ""
  echo "=== MACROS dir: $ALIRSNLITE_MACROS_DIR ==="
  ls -1 $ALIRSNLITE_MACROS_DIR | grep -v AliRsnLiteWork
  echo ""
}

function SetMacros() {
  
  rm -Rf $ALIRSNLITE_TASKS_DIR/*
  
  for MY_TASK in $*; do
    MY_TASK_NAME="`echo $MY_TASK | awk -F ':' '{print $1}'`"
    MY_TASK_OPT="`echo $MY_TASK | awk -F ':' '{print $2}'`"
    test -n "$MY_TASK_OPT" && MY_TASK_OPT="_$MY_TASK_OPT"
    echo "Setting task $MY_TASK_NAME opt='$MY_TASK_OPT' ..."
    ln -s $ALIRSNLITE_MACROS_DIR/$MY_TASK $ALIRSNLITE_TASKS_DIR/AddAM${MY_TASK_NAME}${MY_TASK_OPT}
  done

}

SetAliRsnLiteSrcDir
ALIRSNLITE_MACROS_DIR="$ALIRSNLITE_SRC_DIR/macros"
ALIRSNLITE_TASKS_DIR="$ALIRSNLITE_MACROS_DIR/AliRsnLiteWork/tasks"

CreateDir $ALIRSNLITE_TASKS_DIR

while [[ $1 = -* ]]; do
    arg=$1; shift
    case $arg in
    help)
      ShowHelp
    ;;
    --list)
      ListMacros
    ;;
    --set-tasks)
      SetMacros $*
    ;;
    *)
    ShowHelp
    ;;
  esac
done
