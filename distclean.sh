#!/bin/sh
# run this after jam -q clean
rm -r -f muletool.bootstrapstuff/ src/core/muleconfig.* src/ideintegration/muletool/*.o `find . -type f -name "*.o"`
sh -c "cd src/3rdparty/ && ./download-3rdparty-components.sh clean"
