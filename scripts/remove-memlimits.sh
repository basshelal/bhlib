#!/usr/bin/env bash

# Add bash-utils, needs to use this complex syntax in order to work from any working directory
source "$(dirname "$(realpath "$0")")/bash-utils.sh"

function remove_limit() {
  if [ -f "$1" ]; then
    echo "sudo cap_ipc_lock+ep $1"
    sudo setcap cap_ipc_lock+ep "$1"
  fi
}

# Debug
remove_limit "$BUILD_DEBUG_DIR/test/bhlib-test"

# Release
remove_limit "$BUILD_RELEASE_DIR/test/bhlib-test"