#!/usr/bin/env bash

if [[ -n "$IN_NIX_SHELL" ]]; then
    echo "Please exit your nix-shell before processing this script" >&2
    exit 1
fi

rm -rf build/

if [[ ! -L "result" ]]; then
    echo "No 'result' symlink found. Nothing to delete." >&2
    exit 0
fi

store_path=$(readlink -f result)
if [[ ! "$store_path" =~ ^/nix/store/ ]]; then
    echo "Result symlink does not point to a Nix store path: $store_path" >&2
    exit 1
fi

nix store delete "$store_path"
exit_code=$?
if [ $exit_code -ne 0 ]; then
    echo "nix failed to delete actual build store (exit code $exit_code)." >&2
    read -p "Do you want to force deletion using --ignore-liveness? [Y/n] " response
    case "$response" in
    [yY])
        sudo nix store delete --ignore-liveness "$store_path"
        ;;
    *)
        exit 1
        ;;
    esac
fi

rm -rf result
