SET C51INC=d:\Keil\C51\INC\STC\;d:\Keil\C51\INC\
SET C51LIB=d:\Keil\C51\LIB
SET CPU_TYPE=STC89C51RC
SET CPU_VENDOR=STC-STC89
SET UV2_TARGET=Target 1
SET CPU_XTAL=0x02AEA540
"d:\Keil\C51\BIN\C51.EXE" @.\eeprom.__i
"d:\Keil\C51\BIN\C51.EXE" @.\key.__i
"d:\Keil\C51\BIN\C51.EXE" @.\led.__i
"d:\Keil\C51\BIN\C51.EXE" @.\main.__i
"d:\Keil\C51\BIN\C51.EXE" @.\uart.__i
"d:\Keil\C51\BIN\C51.EXE" @.\process.__i
"d:\Keil\C51\BIN\BL51.EXE" @910U-P6.lnp
"d:\Keil\C51\BIN\OH51.EXE" "910U-P6" 
