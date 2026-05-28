default: ci


ci:
    echo "not there yet"

build-all:
    make
    make embed

valgrind:
    ./scripts/val.sh

clean-build:
    rm -rf buid/
    rm -f vgcore* core*
    ./scripts/nixclean.sh
