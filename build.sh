#!/usr/bin/env bash

set -euo pipefail

# ==========================================================
# Configuration
# ==========================================================

JOBS="${JOBS:-$(nproc)}"

MODE="${1:-}"
PLATFORM="${2:-}"
ARCH="${3:-}"

# ==========================================================
# Helpers
# ==========================================================

usage() {
cat << EOF

Usage:

  ./build.sh templates all [arch]
  ./build.sh templates windows [arch]
  ./build.sh templates linux [arch]
  ./build.sh templates macos [arch]

  ./build.sh editor all [arch]
  ./build.sh editor windows [arch]
  ./build.sh editor linux [arch]
  ./build.sh editor macos [arch]

  ./build.sh everything all [arch]
  ./build.sh everything windows [arch]
  ./build.sh everything linux [arch]
  ./build.sh everything macos [arch]

Examples:

  ./build.sh templates all
  ./build.sh editor linux
  ./build.sh everything all
  ./build.sh everything all arm64
  ./build.sh templates windows x86_64

EOF

exit 1
}

log() {
    echo
    echo "[$(date '+%Y-%m-%d %H:%M:%S')] $1"
    echo
}

run_build() {
    local platform="$1"
    local target="$2"

    local args=(
        "platform=$platform"
        "target=$target"
    )

    if [[ -n "$ARCH" ]]; then
        args+=("arch=$ARCH")
    fi

    log "Building platform=$platform target=$target"

    scons -j"$JOBS" "${args[@]}"
}

# ==========================================================
# Build Types
# ==========================================================

build_templates() {
    local platform="$1"

    run_build "$platform" template_release
    run_build "$platform" template_debug
}

build_editor() {
    local platform="$1"

    run_build "$platform" editor
}

build_everything() {
    local platform="$1"

    build_editor "$platform"
    build_templates "$platform"
}

# ==========================================================
# Platform Dispatcher
# ==========================================================

dispatch() {
    local action="$1"

    case "$PLATFORM" in

        linux)
            "$action" linuxbsd
            ;;

        windows)
            "$action" windows
            ;;

        macos)
            "$action" macos
            ;;

        all)

            log "Starting Linux build"
            "$action" linuxbsd

            log "Starting Windows build"
            "$action" windows

            log "Starting macOS build"
            "$action" macos
            ;;

        *)
            usage
            ;;
    esac
}

# ==========================================================
# Validation
# ==========================================================

[[ -z "$MODE" ]] && usage
[[ -z "$PLATFORM" ]] && usage

# ==========================================================
# Entry Point
# ==========================================================

START_TIME=$(date +%s)

log "Build started"
log "Mode      : $MODE"
log "Platform  : $PLATFORM"
log "Arch      : ${ARCH:-default}"
log "Jobs      : $JOBS"

case "$MODE" in

    templates)
        dispatch build_templates
        ;;

    editor)
        dispatch build_editor
        ;;

    everything)
        dispatch build_everything
        ;;

    *)
        usage
        ;;
esac

END_TIME=$(date +%s)
DURATION=$((END_TIME - START_TIME))

log "Build completed successfully"
log "Total build time: ${DURATION}s"
