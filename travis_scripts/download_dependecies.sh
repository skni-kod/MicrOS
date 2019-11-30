# wget http://newos.org/toolchains/i386-elf-4.9.1-Linux-x86_64.tar.xz
# tar xf i386-elf-4.9.1-Linux-x86_64.tar.xz
# mv i386-elf-4.9.1-Linux-x86_64 gcc_cross
# ls gcc_cross
curl -LO https://raw.githubusercontent.com/jaenek/MicrOS-DevTools/master/configure.sh
chmod +x configure.sh
# rm i386-elf-4.9.1-Linux-x86_64.tar.xz
pwd
echo $TRAVIS_BUILD_DIR
# export gcc_compiler=$(readlink -f ./gcc_cross)
# echo $gcc_compiler