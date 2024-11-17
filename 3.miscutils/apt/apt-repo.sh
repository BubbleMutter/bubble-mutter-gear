#!/bin/bash -x
key_id=
case "$1" in
    add)
        [ -z "$2" -o -z "$3" ] && echo "Usage: $0 $1 <suite> <codename>" && exit 1
        suite=$2
        codename=$3
        pool=pool/$codename
        echo "add  suite=[$suite] codename=[$codename]"
        mkdir -p $pool
        mkdir -p dists/$suite/$codename/binary-amd64
        echo "Now, upload the *.deb into $pool/binary-<arch>/"
        ;;
    scan)
        [ -z "$2" -o -z "$3" ] && echo "Usage: $0 $1 <suite> <codename>" && exit 1
        suite=$2
        codename=$3
        pool=pool/$codename
        [ ! -d "$pool" ] && echo "$pool doesn't exist" && exit 1
        echo "scan suite=[$suite] codename=[$codename]"
        dpkg-scanpackages -m pool/$codename > dists/$suite/$codename/binary-amd64/Packages
        ;;
    release)
        [ -z "$2" ] && echo "Usage: $0 $1 <suite>" && exit 1
        suite=$2
        [ ! -d dists/"$suite" ] && echo "suite=[$suite] doesn't exist" && exit 1
        apt-ftparchive -oAPT::FTPArchive::Release::Suite="$suite" release dists/$suite > dists/$suite/Release
        gpg --default-key $key_id --clearsign -o dists/$suite/InRelease dists/$suite/Release
        ;;
    *)
        echo "Usage: "
        echo "       $0 add     <suite> <codename>"
        echo "       $0 scan    <suite> <codename>"
        echo "       $0 release <suite>"
        ;;
esac

#    *
#    |- dists/
#    |  |- $suite/
#    |     |-- InRelease
#    |     |-- Release
#    |     |-- $codename0/binary-amd64/Packages
#    |     |-- $codename1/binary-amd64/Packages
#    |- pool/
#    |  |- $codename0/*.deb
#    |  |- $codename1/*.deb
