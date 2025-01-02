#!/bin/bash

base="$(dirname "$0")"
ver=cc1

function error_exit ()
{
    echo -e "\e[31m!!! Switch Fail !!!\e[0m"
    exit -1
}

echo Copy armcc.via...
cp "$base/keil/$ver/armcc.via" "$base/keil" || error_exit

echo Copy armclang.via...
cp "$base/keil/$ver/armclang.via" "$base/keil" || error_exit

echo Copy armcc_allroles.via...
cp "$base/keil/$ver/armcc_allroles.via" "$base/keil" || error_exit

echo Copy armclang_allroles.via...
cp "$base/keil/$ver/armclang_allroles.via" "$base/keil" || error_exit

echo Copy armlink.via...
cp "$base/keil/$ver/armlink.via" "$base/keil" || error_exit

echo Copy armlink6.via...
cp "$base/keil/$ver/armlink6.via" "$base/keil" || error_exit

echo Copy armlink_allroles.via...
cp "$base/keil/$ver/armlink_allroles.via" "$base/keil" || error_exit

echo Copy armlink6_allroles.via...
cp "$base/keil/$ver/armlink6_allroles.via" "$base/keil" || error_exit

echo Copy debug.ini...
cp "$base/keil/$ver/debug.ini" "$base/keil" || error_exit

echo Copy scatter_hs6621.sct...
cp "$base/keil/$ver/scatter_hs6621.sct" "$base/keil" || error_exit

echo Copy scatter_hs6621_armclang.sct...
cp "$base/keil/$ver/scatter_hs6621_armclang.sct" "$base/keil" || error_exit

echo Copy startup_hs6621.s...
cp "$base/keil/$ver/startup_hs6621.s" "$base/keil" || error_exit

echo Copy debug_flash.ini...
cp "$base/keil/$ver/debug_flash.ini" "$base/keil" || error_exit

echo Copy scatter_hs6621_flash.sct...
cp "$base/keil/$ver/scatter_hs6621_flash.sct" "$base/keil" || error_exit

echo Copy scatter_hs6621_flash_armclang.sct...
cp "$base/keil/$ver/scatter_hs6621_flash_armclang.sct" "$base/keil" || error_exit

echo Copy startup_hs6621_flash.s...
cp "$base/keil/$ver/startup_hs6621_flash.s" "$base/keil" || error_exit

echo Copy GCC fiels
echo Copy autoconf_version.h
cp "$base/gcc/$ver/autoconf_version.h" "$base/gcc" || error_exit

echo Copy onmicro.gdb
cp "$base/gcc/$ver/onmicro.gdb" "$base/gcc" || error_exit

echo Copy ldscripts.mk
cp "$base/gcc/$ver/ldscripts.mk" "$base/gcc" || error_exit

echo Copy libs.mk
cp "$base/gcc/$ver/libs.mk" "$base/gcc" || error_exit

echo Copy libs_allroles.mk
cp "$base/gcc/$ver/libs_allroles.mk" "$base/gcc" || error_exit

