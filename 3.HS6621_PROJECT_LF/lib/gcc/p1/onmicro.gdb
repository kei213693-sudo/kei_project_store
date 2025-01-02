# connect the port
define connect
set prompt gdb>
target remote localhost:2331
end

define om_reset

mon reset 1
#Disable WDT
set *0x400e00e8 = 0x6666

# Power on all SRAM
set *0x400e0010 = 0x000FFFFF
set *0x40001050 = 0
set *0x4000100C = 1

# Reset pinmux
set *0x40000080 = 0x00000000
set *0x40000084 = 0x00000000
set *0x40000088 = 0x00000000
set *0x4000008C = 0x1a000000
set *0x40000090 = 0x1a1a1a1a
set *0x40000094 = 0x1a1a1a1a
set *0x40000098 = 0x1a1a1a1a
set *0x4000009C = 0x001a1a1a
set *0x400000A0 = 0x00000000
set *0x400000A4 = 0x00000000

# Disable All IRQ
set *0xE000E180 = 0xFFFFFFFF
set *0xE000E184 = 0xFFFFFFFF
set *0xE000E010 = 0

# Invalid Cache
set *0xE0042004 = 1
set *0xE0042008 = 0
set *0xE0042020 = 1
set *0xE0042004 = 0
set *0xE0042008 = 1

# switch to RC32M and power off xtal32m
set *0x400e0064 &= ~0x00002000
set *0x400e0024 &= ~0x01000000
set *0x400e0064 &= ~0x00040000
set *0x400e0064 |= 0x00001000
set *0x400e0018 &= ~0x00800000

#remap ram
set *0x400e003C |= (1<<5)
set *0x400e003C = ((*0x400e003C&0xFFFFFFF0) | 2 | (1<<5))

#load code
restore ../../../lib/misc/stack_data_p1.hex

mon exec InvalidateCache
load
set $sp = *0x50003000
set $pc = *0x50003004
c
end
