; FAT32 data structures
; Because they have to be defined before use

struc part_entry
	.attrib: resb 1
	.chs_start: resb 3
	.type: resb 1
	.chs_end: resb 3
	.lba_start: resd 1
	.lba_end: resd 1
endstruc

struc dir_entry
	.name: resb 11
	.attr: resb 1
	.ntres: resb 1
	.crttimetenth: resb 1
	.crttime: resw 1
	.crtdate: resw 1
	.lstaccdate: resw 1
	.firstclushi: resw 1
	.wrttime: resw 1
	.wrtdate: resw 1
	.firstcluslo: resw 1
	.filesize: resd 1
endstruc

; BPB definition, we use offsets to bp to save space
	%define BS_jmpBoot [bp]
	%define BS_OemName [bp+0x3]
	%define BPB_BytsPerSec [bp+0xb]
	%define BPB_SecPerClus [bp+0xd]
	%define BPB_RsvdSecCnt [bp+0xe]
	%define BPB_NumFats [bp+0x10]
	%define BPB_RootEntCnt [bp+0x11]
	%define BPB_TotSec16 [bp+0x13]
	%define BPB_Media [bp+0x15]
	%define BPB_FatSz16 [bp+0x16] ; count of sectors
	%define BPB_SecPerTrk [bp+0x18]
	%define BPB_NumHeads [bp+0x1a]
	%define BPB_HiddSec [bp+0x1c]
	%define BPB_TotSec32 [bp+0x20]
	%define BPB_FatSz32 [bp+0x24]
	%define BPB_ExtFlags [bp+0x28]
	%define BPB_FSVer [bp+0x2a]
	%define BPB_RootClus [bp+0x2c]
	%define BPB_FsInfo [bp+0x30]
	%define BPB_BkBootSec [bp+0x32] ; number of sector
	%define BPB_Reserved [bp+0x34]
	%define BS_DrvNum [bp+0x40]
	%define BS_Reserved1 [bp+0x41]
	%define BS_BootSig [bp+0x42]
	%define BS_VolId [bp+0x43]
	%define BS_VolLab [bp+0x47]
	%define BS_FilSysType [bp+0x52]

