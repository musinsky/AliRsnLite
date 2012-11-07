#!/bin/bash

ALIRSNLITE_SRC_DIR="$(dirname $(dirname $(readlink -m $0)))"

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
  echo "./alirsnlite-config --clear"
  echo "./alirsnlite-config --set <TASK>"
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
    ln -s $ALIRSNLITE_MACROS_DIR/$MY_TASK_NAME $ALIRSNLITE_TASKS_DIR/AddAM${MY_TASK_NAME}${MY_TASK_OPT}
  done

}

ALIRSNLITE_MACROS_DIR="$ALIRSNLITE_SRC_DIR/macros"
ALIRSNLITE_TASKS_DIR="$ALIRSNLITE_MACROS_DIR/AliRsnLiteWork/tasks"

CreateDir $ALIRSNLITE_TASKS_DIR

if [ "$#" = "0" ]; then
  echo ""
  ShowHelp
  echo ""
  ListMacros
  exit 1
fi


while [[ $1 = -* ]]; do
    arg=$1; shift
    case $arg in
    help)
      ShowHelp
    ;;
    --list)
      ListMacros
    ;;
    --set)
      SetMacros $*
    ;;
    --clear)
      rm -Rf $ALIRSNLITE_TASKS_DIR/*
    ;;
    *)
      ListMacros
    ;;
  esac
done
