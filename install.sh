#!/bin/bash

erase=false
while getops ":e:-:" opt;
do
        case $opt in
                e)
                        erase=true
                        ;;
                -)
                        case "$OPTARG" in
                                erase)
                                        erase=true
                                        ;;
                                *)
                                        echo "Invalid option: --$OPTARG" >&2
                                        ;;
                        esac
                        ;;
                \?)
                        echo "Invalid option: -$OPTARG" >&2
                        ;;
        esac
done

if [ -d "build" ]
then
	rm -rf build
fi
mkdir build
cd build
cmake ..
cmake --build .
cd ..
if [ $erase == true ]
then
	rm -rf build
fi