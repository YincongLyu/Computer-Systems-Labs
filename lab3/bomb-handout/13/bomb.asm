
bomb:     file format elf64-x86-64


Disassembly of section .init:

0000000000001000 <_init>:
    1000:	f3 0f 1e fa          	endbr64 
    1004:	48 83 ec 08          	sub    $0x8,%rsp
    1008:	48 8b 05 d9 3f 00 00 	mov    0x3fd9(%rip),%rax        # 4fe8 <__gmon_start__>
    100f:	48 85 c0             	test   %rax,%rax
    1012:	74 02                	je     1016 <_init+0x16>
    1014:	ff d0                	callq  *%rax
    1016:	48 83 c4 08          	add    $0x8,%rsp
    101a:	c3                   	retq   

Disassembly of section .plt:

0000000000001020 <.plt>:
    1020:	ff 35 da 3e 00 00    	pushq  0x3eda(%rip)        # 4f00 <_GLOBAL_OFFSET_TABLE_+0x8>
    1026:	f2 ff 25 db 3e 00 00 	bnd jmpq *0x3edb(%rip)        # 4f08 <_GLOBAL_OFFSET_TABLE_+0x10>
    102d:	0f 1f 00             	nopl   (%rax)
    1030:	f3 0f 1e fa          	endbr64 
    1034:	68 00 00 00 00       	pushq  $0x0
    1039:	f2 e9 e1 ff ff ff    	bnd jmpq 1020 <.plt>
    103f:	90                   	nop
    1040:	f3 0f 1e fa          	endbr64 
    1044:	68 01 00 00 00       	pushq  $0x1
    1049:	f2 e9 d1 ff ff ff    	bnd jmpq 1020 <.plt>
    104f:	90                   	nop
    1050:	f3 0f 1e fa          	endbr64 
    1054:	68 02 00 00 00       	pushq  $0x2
    1059:	f2 e9 c1 ff ff ff    	bnd jmpq 1020 <.plt>
    105f:	90                   	nop
    1060:	f3 0f 1e fa          	endbr64 
    1064:	68 03 00 00 00       	pushq  $0x3
    1069:	f2 e9 b1 ff ff ff    	bnd jmpq 1020 <.plt>
    106f:	90                   	nop
    1070:	f3 0f 1e fa          	endbr64 
    1074:	68 04 00 00 00       	pushq  $0x4
    1079:	f2 e9 a1 ff ff ff    	bnd jmpq 1020 <.plt>
    107f:	90                   	nop
    1080:	f3 0f 1e fa          	endbr64 
    1084:	68 05 00 00 00       	pushq  $0x5
    1089:	f2 e9 91 ff ff ff    	bnd jmpq 1020 <.plt>
    108f:	90                   	nop
    1090:	f3 0f 1e fa          	endbr64 
    1094:	68 06 00 00 00       	pushq  $0x6
    1099:	f2 e9 81 ff ff ff    	bnd jmpq 1020 <.plt>
    109f:	90                   	nop
    10a0:	f3 0f 1e fa          	endbr64 
    10a4:	68 07 00 00 00       	pushq  $0x7
    10a9:	f2 e9 71 ff ff ff    	bnd jmpq 1020 <.plt>
    10af:	90                   	nop
    10b0:	f3 0f 1e fa          	endbr64 
    10b4:	68 08 00 00 00       	pushq  $0x8
    10b9:	f2 e9 61 ff ff ff    	bnd jmpq 1020 <.plt>
    10bf:	90                   	nop
    10c0:	f3 0f 1e fa          	endbr64 
    10c4:	68 09 00 00 00       	pushq  $0x9
    10c9:	f2 e9 51 ff ff ff    	bnd jmpq 1020 <.plt>
    10cf:	90                   	nop
    10d0:	f3 0f 1e fa          	endbr64 
    10d4:	68 0a 00 00 00       	pushq  $0xa
    10d9:	f2 e9 41 ff ff ff    	bnd jmpq 1020 <.plt>
    10df:	90                   	nop
    10e0:	f3 0f 1e fa          	endbr64 
    10e4:	68 0b 00 00 00       	pushq  $0xb
    10e9:	f2 e9 31 ff ff ff    	bnd jmpq 1020 <.plt>
    10ef:	90                   	nop
    10f0:	f3 0f 1e fa          	endbr64 
    10f4:	68 0c 00 00 00       	pushq  $0xc
    10f9:	f2 e9 21 ff ff ff    	bnd jmpq 1020 <.plt>
    10ff:	90                   	nop
    1100:	f3 0f 1e fa          	endbr64 
    1104:	68 0d 00 00 00       	pushq  $0xd
    1109:	f2 e9 11 ff ff ff    	bnd jmpq 1020 <.plt>
    110f:	90                   	nop
    1110:	f3 0f 1e fa          	endbr64 
    1114:	68 0e 00 00 00       	pushq  $0xe
    1119:	f2 e9 01 ff ff ff    	bnd jmpq 1020 <.plt>
    111f:	90                   	nop
    1120:	f3 0f 1e fa          	endbr64 
    1124:	68 0f 00 00 00       	pushq  $0xf
    1129:	f2 e9 f1 fe ff ff    	bnd jmpq 1020 <.plt>
    112f:	90                   	nop
    1130:	f3 0f 1e fa          	endbr64 
    1134:	68 10 00 00 00       	pushq  $0x10
    1139:	f2 e9 e1 fe ff ff    	bnd jmpq 1020 <.plt>
    113f:	90                   	nop
    1140:	f3 0f 1e fa          	endbr64 
    1144:	68 11 00 00 00       	pushq  $0x11
    1149:	f2 e9 d1 fe ff ff    	bnd jmpq 1020 <.plt>
    114f:	90                   	nop
    1150:	f3 0f 1e fa          	endbr64 
    1154:	68 12 00 00 00       	pushq  $0x12
    1159:	f2 e9 c1 fe ff ff    	bnd jmpq 1020 <.plt>
    115f:	90                   	nop
    1160:	f3 0f 1e fa          	endbr64 
    1164:	68 13 00 00 00       	pushq  $0x13
    1169:	f2 e9 b1 fe ff ff    	bnd jmpq 1020 <.plt>
    116f:	90                   	nop
    1170:	f3 0f 1e fa          	endbr64 
    1174:	68 14 00 00 00       	pushq  $0x14
    1179:	f2 e9 a1 fe ff ff    	bnd jmpq 1020 <.plt>
    117f:	90                   	nop
    1180:	f3 0f 1e fa          	endbr64 
    1184:	68 15 00 00 00       	pushq  $0x15
    1189:	f2 e9 91 fe ff ff    	bnd jmpq 1020 <.plt>
    118f:	90                   	nop
    1190:	f3 0f 1e fa          	endbr64 
    1194:	68 16 00 00 00       	pushq  $0x16
    1199:	f2 e9 81 fe ff ff    	bnd jmpq 1020 <.plt>
    119f:	90                   	nop
    11a0:	f3 0f 1e fa          	endbr64 
    11a4:	68 17 00 00 00       	pushq  $0x17
    11a9:	f2 e9 71 fe ff ff    	bnd jmpq 1020 <.plt>
    11af:	90                   	nop
    11b0:	f3 0f 1e fa          	endbr64 
    11b4:	68 18 00 00 00       	pushq  $0x18
    11b9:	f2 e9 61 fe ff ff    	bnd jmpq 1020 <.plt>
    11bf:	90                   	nop

Disassembly of section .plt.got:

00000000000011c0 <__cxa_finalize@plt>:
    11c0:	f3 0f 1e fa          	endbr64 
    11c4:	f2 ff 25 2d 3e 00 00 	bnd jmpq *0x3e2d(%rip)        # 4ff8 <__cxa_finalize@GLIBC_2.2.5>
    11cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .plt.sec:

00000000000011d0 <getenv@plt>:
    11d0:	f3 0f 1e fa          	endbr64 
    11d4:	f2 ff 25 35 3d 00 00 	bnd jmpq *0x3d35(%rip)        # 4f10 <getenv@GLIBC_2.2.5>
    11db:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000011e0 <__errno_location@plt>:
    11e0:	f3 0f 1e fa          	endbr64 
    11e4:	f2 ff 25 2d 3d 00 00 	bnd jmpq *0x3d2d(%rip)        # 4f18 <__errno_location@GLIBC_2.2.5>
    11eb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000011f0 <strcpy@plt>:
    11f0:	f3 0f 1e fa          	endbr64 
    11f4:	f2 ff 25 25 3d 00 00 	bnd jmpq *0x3d25(%rip)        # 4f20 <strcpy@GLIBC_2.2.5>
    11fb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001200 <puts@plt>:
    1200:	f3 0f 1e fa          	endbr64 
    1204:	f2 ff 25 1d 3d 00 00 	bnd jmpq *0x3d1d(%rip)        # 4f28 <puts@GLIBC_2.2.5>
    120b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001210 <write@plt>:
    1210:	f3 0f 1e fa          	endbr64 
    1214:	f2 ff 25 15 3d 00 00 	bnd jmpq *0x3d15(%rip)        # 4f30 <write@GLIBC_2.2.5>
    121b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001220 <__stack_chk_fail@plt>:
    1220:	f3 0f 1e fa          	endbr64 
    1224:	f2 ff 25 0d 3d 00 00 	bnd jmpq *0x3d0d(%rip)        # 4f38 <__stack_chk_fail@GLIBC_2.4>
    122b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001230 <alarm@plt>:
    1230:	f3 0f 1e fa          	endbr64 
    1234:	f2 ff 25 05 3d 00 00 	bnd jmpq *0x3d05(%rip)        # 4f40 <alarm@GLIBC_2.2.5>
    123b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001240 <close@plt>:
    1240:	f3 0f 1e fa          	endbr64 
    1244:	f2 ff 25 fd 3c 00 00 	bnd jmpq *0x3cfd(%rip)        # 4f48 <close@GLIBC_2.2.5>
    124b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001250 <read@plt>:
    1250:	f3 0f 1e fa          	endbr64 
    1254:	f2 ff 25 f5 3c 00 00 	bnd jmpq *0x3cf5(%rip)        # 4f50 <read@GLIBC_2.2.5>
    125b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001260 <fgets@plt>:
    1260:	f3 0f 1e fa          	endbr64 
    1264:	f2 ff 25 ed 3c 00 00 	bnd jmpq *0x3ced(%rip)        # 4f58 <fgets@GLIBC_2.2.5>
    126b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001270 <signal@plt>:
    1270:	f3 0f 1e fa          	endbr64 
    1274:	f2 ff 25 e5 3c 00 00 	bnd jmpq *0x3ce5(%rip)        # 4f60 <signal@GLIBC_2.2.5>
    127b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001280 <gethostbyname@plt>:
    1280:	f3 0f 1e fa          	endbr64 
    1284:	f2 ff 25 dd 3c 00 00 	bnd jmpq *0x3cdd(%rip)        # 4f68 <gethostbyname@GLIBC_2.2.5>
    128b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001290 <__memmove_chk@plt>:
    1290:	f3 0f 1e fa          	endbr64 
    1294:	f2 ff 25 d5 3c 00 00 	bnd jmpq *0x3cd5(%rip)        # 4f70 <__memmove_chk@GLIBC_2.3.4>
    129b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000012a0 <strtol@plt>:
    12a0:	f3 0f 1e fa          	endbr64 
    12a4:	f2 ff 25 cd 3c 00 00 	bnd jmpq *0x3ccd(%rip)        # 4f78 <strtol@GLIBC_2.2.5>
    12ab:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000012b0 <fflush@plt>:
    12b0:	f3 0f 1e fa          	endbr64 
    12b4:	f2 ff 25 c5 3c 00 00 	bnd jmpq *0x3cc5(%rip)        # 4f80 <fflush@GLIBC_2.2.5>
    12bb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000012c0 <__isoc99_sscanf@plt>:
    12c0:	f3 0f 1e fa          	endbr64 
    12c4:	f2 ff 25 bd 3c 00 00 	bnd jmpq *0x3cbd(%rip)        # 4f88 <__isoc99_sscanf@GLIBC_2.7>
    12cb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000012d0 <__printf_chk@plt>:
    12d0:	f3 0f 1e fa          	endbr64 
    12d4:	f2 ff 25 b5 3c 00 00 	bnd jmpq *0x3cb5(%rip)        # 4f90 <__printf_chk@GLIBC_2.3.4>
    12db:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000012e0 <fopen@plt>:
    12e0:	f3 0f 1e fa          	endbr64 
    12e4:	f2 ff 25 ad 3c 00 00 	bnd jmpq *0x3cad(%rip)        # 4f98 <fopen@GLIBC_2.2.5>
    12eb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

00000000000012f0 <exit@plt>:
    12f0:	f3 0f 1e fa          	endbr64 
    12f4:	f2 ff 25 a5 3c 00 00 	bnd jmpq *0x3ca5(%rip)        # 4fa0 <exit@GLIBC_2.2.5>
    12fb:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001300 <connect@plt>:
    1300:	f3 0f 1e fa          	endbr64 
    1304:	f2 ff 25 9d 3c 00 00 	bnd jmpq *0x3c9d(%rip)        # 4fa8 <connect@GLIBC_2.2.5>
    130b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001310 <__fprintf_chk@plt>:
    1310:	f3 0f 1e fa          	endbr64 
    1314:	f2 ff 25 95 3c 00 00 	bnd jmpq *0x3c95(%rip)        # 4fb0 <__fprintf_chk@GLIBC_2.3.4>
    131b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001320 <sleep@plt>:
    1320:	f3 0f 1e fa          	endbr64 
    1324:	f2 ff 25 8d 3c 00 00 	bnd jmpq *0x3c8d(%rip)        # 4fb8 <sleep@GLIBC_2.2.5>
    132b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001330 <__ctype_b_loc@plt>:
    1330:	f3 0f 1e fa          	endbr64 
    1334:	f2 ff 25 85 3c 00 00 	bnd jmpq *0x3c85(%rip)        # 4fc0 <__ctype_b_loc@GLIBC_2.3>
    133b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001340 <__sprintf_chk@plt>:
    1340:	f3 0f 1e fa          	endbr64 
    1344:	f2 ff 25 7d 3c 00 00 	bnd jmpq *0x3c7d(%rip)        # 4fc8 <__sprintf_chk@GLIBC_2.3.4>
    134b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

0000000000001350 <socket@plt>:
    1350:	f3 0f 1e fa          	endbr64 
    1354:	f2 ff 25 75 3c 00 00 	bnd jmpq *0x3c75(%rip)        # 4fd0 <socket@GLIBC_2.2.5>
    135b:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)

Disassembly of section .text:

0000000000001360 <_start>:
    1360:	f3 0f 1e fa          	endbr64 
    1364:	31 ed                	xor    %ebp,%ebp
    1366:	49 89 d1             	mov    %rdx,%r9
    1369:	5e                   	pop    %rsi
    136a:	48 89 e2             	mov    %rsp,%rdx
    136d:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
    1371:	50                   	push   %rax
    1372:	54                   	push   %rsp
    1373:	4c 8d 05 a6 17 00 00 	lea    0x17a6(%rip),%r8        # 2b20 <__libc_csu_fini>
    137a:	48 8d 0d 2f 17 00 00 	lea    0x172f(%rip),%rcx        # 2ab0 <__libc_csu_init>
    1381:	48 8d 3d c1 00 00 00 	lea    0xc1(%rip),%rdi        # 1449 <main>
    1388:	ff 15 52 3c 00 00    	callq  *0x3c52(%rip)        # 4fe0 <__libc_start_main@GLIBC_2.2.5>
    138e:	f4                   	hlt    
    138f:	90                   	nop

0000000000001390 <deregister_tm_clones>:
    1390:	48 8d 3d c9 42 00 00 	lea    0x42c9(%rip),%rdi        # 5660 <stdout@@GLIBC_2.2.5>
    1397:	48 8d 05 c2 42 00 00 	lea    0x42c2(%rip),%rax        # 5660 <stdout@@GLIBC_2.2.5>
    139e:	48 39 f8             	cmp    %rdi,%rax
    13a1:	74 15                	je     13b8 <deregister_tm_clones+0x28>
    13a3:	48 8b 05 2e 3c 00 00 	mov    0x3c2e(%rip),%rax        # 4fd8 <_ITM_deregisterTMCloneTable>
    13aa:	48 85 c0             	test   %rax,%rax
    13ad:	74 09                	je     13b8 <deregister_tm_clones+0x28>
    13af:	ff e0                	jmpq   *%rax
    13b1:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    13b8:	c3                   	retq   
    13b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000000013c0 <register_tm_clones>:
    13c0:	48 8d 3d 99 42 00 00 	lea    0x4299(%rip),%rdi        # 5660 <stdout@@GLIBC_2.2.5>
    13c7:	48 8d 35 92 42 00 00 	lea    0x4292(%rip),%rsi        # 5660 <stdout@@GLIBC_2.2.5>
    13ce:	48 29 fe             	sub    %rdi,%rsi
    13d1:	48 89 f0             	mov    %rsi,%rax
    13d4:	48 c1 ee 3f          	shr    $0x3f,%rsi
    13d8:	48 c1 f8 03          	sar    $0x3,%rax
    13dc:	48 01 c6             	add    %rax,%rsi
    13df:	48 d1 fe             	sar    %rsi
    13e2:	74 14                	je     13f8 <register_tm_clones+0x38>
    13e4:	48 8b 05 05 3c 00 00 	mov    0x3c05(%rip),%rax        # 4ff0 <_ITM_registerTMCloneTable>
    13eb:	48 85 c0             	test   %rax,%rax
    13ee:	74 08                	je     13f8 <register_tm_clones+0x38>
    13f0:	ff e0                	jmpq   *%rax
    13f2:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
    13f8:	c3                   	retq   
    13f9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001400 <__do_global_dtors_aux>:
    1400:	f3 0f 1e fa          	endbr64 
    1404:	80 3d 7d 42 00 00 00 	cmpb   $0x0,0x427d(%rip)        # 5688 <completed.8061>
    140b:	75 2b                	jne    1438 <__do_global_dtors_aux+0x38>
    140d:	55                   	push   %rbp
    140e:	48 83 3d e2 3b 00 00 	cmpq   $0x0,0x3be2(%rip)        # 4ff8 <__cxa_finalize@GLIBC_2.2.5>
    1415:	00 
    1416:	48 89 e5             	mov    %rsp,%rbp
    1419:	74 0c                	je     1427 <__do_global_dtors_aux+0x27>
    141b:	48 8b 3d e6 3b 00 00 	mov    0x3be6(%rip),%rdi        # 5008 <__dso_handle>
    1422:	e8 99 fd ff ff       	callq  11c0 <__cxa_finalize@plt>
    1427:	e8 64 ff ff ff       	callq  1390 <deregister_tm_clones>
    142c:	c6 05 55 42 00 00 01 	movb   $0x1,0x4255(%rip)        # 5688 <completed.8061>
    1433:	5d                   	pop    %rbp
    1434:	c3                   	retq   
    1435:	0f 1f 00             	nopl   (%rax)
    1438:	c3                   	retq   
    1439:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000001440 <frame_dummy>:
    1440:	f3 0f 1e fa          	endbr64 
    1444:	e9 77 ff ff ff       	jmpq   13c0 <register_tm_clones>

0000000000001449 <main>:
    1449:	f3 0f 1e fa          	endbr64 
    144d:	53                   	push   %rbx
    144e:	83 ff 01             	cmp    $0x1,%edi
    1451:	0f 84 f8 00 00 00    	je     154f <main+0x106>
    1457:	48 89 f3             	mov    %rsi,%rbx
    145a:	83 ff 02             	cmp    $0x2,%edi
    145d:	0f 85 21 01 00 00    	jne    1584 <main+0x13b>
    1463:	48 8b 7e 08          	mov    0x8(%rsi),%rdi
    1467:	48 8d 35 96 1b 00 00 	lea    0x1b96(%rip),%rsi        # 3004 <_IO_stdin_used+0x4>
    146e:	e8 6d fe ff ff       	callq  12e0 <fopen@plt>
    1473:	48 89 05 16 42 00 00 	mov    %rax,0x4216(%rip)        # 5690 <infile>
    147a:	48 85 c0             	test   %rax,%rax
    147d:	0f 84 df 00 00 00    	je     1562 <main+0x119>
    1483:	e8 48 07 00 00       	callq  1bd0 <initialize_bomb>
    1488:	48 8d 3d f9 1b 00 00 	lea    0x1bf9(%rip),%rdi        # 3088 <_IO_stdin_used+0x88>
    148f:	e8 6c fd ff ff       	callq  1200 <puts@plt>
    1494:	48 8d 3d 2d 1c 00 00 	lea    0x1c2d(%rip),%rdi        # 30c8 <_IO_stdin_used+0xc8>
    149b:	e8 60 fd ff ff       	callq  1200 <puts@plt>
    14a0:	e8 50 08 00 00       	callq  1cf5 <read_line>
    14a5:	48 89 c7             	mov    %rax,%rdi
    14a8:	e8 fa 00 00 00       	callq  15a7 <phase_1>
    14ad:	e8 8b 09 00 00       	callq  1e3d <phase_defused>
    14b2:	48 8d 3d 3f 1c 00 00 	lea    0x1c3f(%rip),%rdi        # 30f8 <_IO_stdin_used+0xf8>
    14b9:	e8 42 fd ff ff       	callq  1200 <puts@plt>
    14be:	e8 32 08 00 00       	callq  1cf5 <read_line>
    14c3:	48 89 c7             	mov    %rax,%rdi
    14c6:	e8 00 01 00 00       	callq  15cb <phase_2>
    14cb:	e8 6d 09 00 00       	callq  1e3d <phase_defused>
    14d0:	48 8d 3d 66 1b 00 00 	lea    0x1b66(%rip),%rdi        # 303d <_IO_stdin_used+0x3d>
    14d7:	e8 24 fd ff ff       	callq  1200 <puts@plt>
    14dc:	e8 14 08 00 00       	callq  1cf5 <read_line>
    14e1:	48 89 c7             	mov    %rax,%rdi
    14e4:	e8 50 01 00 00       	callq  1639 <phase_3>
    14e9:	e8 4f 09 00 00       	callq  1e3d <phase_defused>
    14ee:	48 8d 3d 66 1b 00 00 	lea    0x1b66(%rip),%rdi        # 305b <_IO_stdin_used+0x5b>
    14f5:	e8 06 fd ff ff       	callq  1200 <puts@plt>
    14fa:	e8 f6 07 00 00       	callq  1cf5 <read_line>
    14ff:	48 89 c7             	mov    %rax,%rdi
    1502:	e8 e7 02 00 00       	callq  17ee <phase_4>
    1507:	e8 31 09 00 00       	callq  1e3d <phase_defused>
    150c:	48 8d 3d 15 1c 00 00 	lea    0x1c15(%rip),%rdi        # 3128 <_IO_stdin_used+0x128>
    1513:	e8 e8 fc ff ff       	callq  1200 <puts@plt>
    1518:	e8 d8 07 00 00       	callq  1cf5 <read_line>
    151d:	48 89 c7             	mov    %rax,%rdi
    1520:	e8 3e 03 00 00       	callq  1863 <phase_5>
    1525:	e8 13 09 00 00       	callq  1e3d <phase_defused>
    152a:	48 8d 3d 39 1b 00 00 	lea    0x1b39(%rip),%rdi        # 306a <_IO_stdin_used+0x6a>
    1531:	e8 ca fc ff ff       	callq  1200 <puts@plt>
    1536:	e8 ba 07 00 00       	callq  1cf5 <read_line>
    153b:	48 89 c7             	mov    %rax,%rdi
    153e:	e8 b7 03 00 00       	callq  18fa <phase_6>
    1543:	e8 f5 08 00 00       	callq  1e3d <phase_defused>
    1548:	b8 00 00 00 00       	mov    $0x0,%eax
    154d:	5b                   	pop    %rbx
    154e:	c3                   	retq   
    154f:	48 8b 05 1a 41 00 00 	mov    0x411a(%rip),%rax        # 5670 <stdin@@GLIBC_2.2.5>
    1556:	48 89 05 33 41 00 00 	mov    %rax,0x4133(%rip)        # 5690 <infile>
    155d:	e9 21 ff ff ff       	jmpq   1483 <main+0x3a>
    1562:	48 8b 4b 08          	mov    0x8(%rbx),%rcx
    1566:	48 8b 13             	mov    (%rbx),%rdx
    1569:	48 8d 35 96 1a 00 00 	lea    0x1a96(%rip),%rsi        # 3006 <_IO_stdin_used+0x6>
    1570:	bf 01 00 00 00       	mov    $0x1,%edi
    1575:	e8 56 fd ff ff       	callq  12d0 <__printf_chk@plt>
    157a:	bf 08 00 00 00       	mov    $0x8,%edi
    157f:	e8 6c fd ff ff       	callq  12f0 <exit@plt>
    1584:	48 8b 16             	mov    (%rsi),%rdx
    1587:	48 8d 35 95 1a 00 00 	lea    0x1a95(%rip),%rsi        # 3023 <_IO_stdin_used+0x23>
    158e:	bf 01 00 00 00       	mov    $0x1,%edi
    1593:	b8 00 00 00 00       	mov    $0x0,%eax
    1598:	e8 33 fd ff ff       	callq  12d0 <__printf_chk@plt>
    159d:	bf 08 00 00 00       	mov    $0x8,%edi
    15a2:	e8 49 fd ff ff       	callq  12f0 <exit@plt>

00000000000015a7 <phase_1>:
    15a7:	f3 0f 1e fa          	endbr64 
    15ab:	48 83 ec 08          	sub    $0x8,%rsp
    15af:	48 8d 35 9a 1b 00 00 	lea    0x1b9a(%rip),%rsi        # 3150 <_IO_stdin_used+0x150>
    15b6:	e8 b5 05 00 00       	callq  1b70 <strings_not_equal>
    15bb:	85 c0                	test   %eax,%eax
    15bd:	75 05                	jne    15c4 <phase_1+0x1d>
    15bf:	48 83 c4 08          	add    $0x8,%rsp
    15c3:	c3                   	retq   
    15c4:	e8 bb 06 00 00       	callq  1c84 <explode_bomb>
    15c9:	eb f4                	jmp    15bf <phase_1+0x18>

00000000000015cb <phase_2>:
    15cb:	f3 0f 1e fa          	endbr64 
    15cf:	55                   	push   %rbp
    15d0:	53                   	push   %rbx
    15d1:	48 83 ec 28          	sub    $0x28,%rsp
    15d5:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    15dc:	00 00 
    15de:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
    15e3:	31 c0                	xor    %eax,%eax
    15e5:	48 89 e6             	mov    %rsp,%rsi
    15e8:	e8 c3 06 00 00       	callq  1cb0 <read_six_numbers>
    15ed:	83 3c 24 01          	cmpl   $0x1,(%rsp)
    15f1:	75 0a                	jne    15fd <phase_2+0x32>
    15f3:	48 89 e3             	mov    %rsp,%rbx
    15f6:	48 8d 6c 24 14       	lea    0x14(%rsp),%rbp
    15fb:	eb 15                	jmp    1612 <phase_2+0x47>
    15fd:	e8 82 06 00 00       	callq  1c84 <explode_bomb>
    1602:	eb ef                	jmp    15f3 <phase_2+0x28>
    1604:	e8 7b 06 00 00       	callq  1c84 <explode_bomb>
    1609:	48 83 c3 04          	add    $0x4,%rbx
    160d:	48 39 eb             	cmp    %rbp,%rbx
    1610:	74 0b                	je     161d <phase_2+0x52>
    1612:	8b 03                	mov    (%rbx),%eax
    1614:	01 c0                	add    %eax,%eax
    1616:	39 43 04             	cmp    %eax,0x4(%rbx)
    1619:	74 ee                	je     1609 <phase_2+0x3e>
    161b:	eb e7                	jmp    1604 <phase_2+0x39>
    161d:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
    1622:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    1629:	00 00 
    162b:	75 07                	jne    1634 <phase_2+0x69>
    162d:	48 83 c4 28          	add    $0x28,%rsp
    1631:	5b                   	pop    %rbx
    1632:	5d                   	pop    %rbp
    1633:	c3                   	retq   
    1634:	e8 e7 fb ff ff       	callq  1220 <__stack_chk_fail@plt>

0000000000001639 <phase_3>:
    1639:	f3 0f 1e fa          	endbr64 
    163d:	48 83 ec 28          	sub    $0x28,%rsp
    1641:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1648:	00 00 
    164a:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
    164f:	31 c0                	xor    %eax,%eax
    1651:	48 8d 4c 24 0f       	lea    0xf(%rsp),%rcx
    1656:	48 8d 54 24 10       	lea    0x10(%rsp),%rdx
    165b:	4c 8d 44 24 14       	lea    0x14(%rsp),%r8
    1660:	48 8d 35 47 1b 00 00 	lea    0x1b47(%rip),%rsi        # 31ae <_IO_stdin_used+0x1ae>
    1667:	e8 54 fc ff ff       	callq  12c0 <__isoc99_sscanf@plt>
    166c:	83 f8 02             	cmp    $0x2,%eax
    166f:	7e 20                	jle    1691 <phase_3+0x58>
    1671:	83 7c 24 10 07       	cmpl   $0x7,0x10(%rsp)
    1676:	0f 87 06 01 00 00    	ja     1782 <phase_3+0x149>
    167c:	8b 44 24 10          	mov    0x10(%rsp),%eax
    1680:	48 8d 15 39 1b 00 00 	lea    0x1b39(%rip),%rdx        # 31c0 <_IO_stdin_used+0x1c0>
    1687:	48 63 04 82          	movslq (%rdx,%rax,4),%rax
    168b:	48 01 d0             	add    %rdx,%rax
    168e:	3e ff e0             	notrack jmpq *%rax
    1691:	e8 ee 05 00 00       	callq  1c84 <explode_bomb>
    1696:	eb d9                	jmp    1671 <phase_3+0x38>
    1698:	b8 74 00 00 00       	mov    $0x74,%eax
    169d:	81 7c 24 14 3b 01 00 	cmpl   $0x13b,0x14(%rsp)
    16a4:	00 
    16a5:	0f 84 e1 00 00 00    	je     178c <phase_3+0x153>
    16ab:	e8 d4 05 00 00       	callq  1c84 <explode_bomb>
    16b0:	b8 74 00 00 00       	mov    $0x74,%eax
    16b5:	e9 d2 00 00 00       	jmpq   178c <phase_3+0x153>
    16ba:	b8 6f 00 00 00       	mov    $0x6f,%eax
    16bf:	81 7c 24 14 4d 03 00 	cmpl   $0x34d,0x14(%rsp)
    16c6:	00 
    16c7:	0f 84 bf 00 00 00    	je     178c <phase_3+0x153>
    16cd:	e8 b2 05 00 00       	callq  1c84 <explode_bomb>
    16d2:	b8 6f 00 00 00       	mov    $0x6f,%eax
    16d7:	e9 b0 00 00 00       	jmpq   178c <phase_3+0x153>
    16dc:	b8 6e 00 00 00       	mov    $0x6e,%eax
    16e1:	81 7c 24 14 0c 01 00 	cmpl   $0x10c,0x14(%rsp)
    16e8:	00 
    16e9:	0f 84 9d 00 00 00    	je     178c <phase_3+0x153>
    16ef:	e8 90 05 00 00       	callq  1c84 <explode_bomb>
    16f4:	b8 6e 00 00 00       	mov    $0x6e,%eax
    16f9:	e9 8e 00 00 00       	jmpq   178c <phase_3+0x153>
    16fe:	b8 64 00 00 00       	mov    $0x64,%eax
    1703:	81 7c 24 14 10 02 00 	cmpl   $0x210,0x14(%rsp)
    170a:	00 
    170b:	74 7f                	je     178c <phase_3+0x153>
    170d:	e8 72 05 00 00       	callq  1c84 <explode_bomb>
    1712:	b8 64 00 00 00       	mov    $0x64,%eax
    1717:	eb 73                	jmp    178c <phase_3+0x153>
    1719:	b8 78 00 00 00       	mov    $0x78,%eax
    171e:	81 7c 24 14 ba 00 00 	cmpl   $0xba,0x14(%rsp)
    1725:	00 
    1726:	74 64                	je     178c <phase_3+0x153>
    1728:	e8 57 05 00 00       	callq  1c84 <explode_bomb>
    172d:	b8 78 00 00 00       	mov    $0x78,%eax
    1732:	eb 58                	jmp    178c <phase_3+0x153>
    1734:	b8 6b 00 00 00       	mov    $0x6b,%eax
    1739:	81 7c 24 14 74 03 00 	cmpl   $0x374,0x14(%rsp)
    1740:	00 
    1741:	74 49                	je     178c <phase_3+0x153>
    1743:	e8 3c 05 00 00       	callq  1c84 <explode_bomb>
    1748:	b8 6b 00 00 00       	mov    $0x6b,%eax
    174d:	eb 3d                	jmp    178c <phase_3+0x153>
    174f:	b8 79 00 00 00       	mov    $0x79,%eax
    1754:	81 7c 24 14 9e 02 00 	cmpl   $0x29e,0x14(%rsp)
    175b:	00 
    175c:	74 2e                	je     178c <phase_3+0x153>
    175e:	e8 21 05 00 00       	callq  1c84 <explode_bomb>
    1763:	b8 79 00 00 00       	mov    $0x79,%eax
    1768:	eb 22                	jmp    178c <phase_3+0x153>
    176a:	b8 71 00 00 00       	mov    $0x71,%eax
    176f:	83 7c 24 14 7b       	cmpl   $0x7b,0x14(%rsp)
    1774:	74 16                	je     178c <phase_3+0x153>
    1776:	e8 09 05 00 00       	callq  1c84 <explode_bomb>
    177b:	b8 71 00 00 00       	mov    $0x71,%eax
    1780:	eb 0a                	jmp    178c <phase_3+0x153>
    1782:	e8 fd 04 00 00       	callq  1c84 <explode_bomb>
    1787:	b8 6e 00 00 00       	mov    $0x6e,%eax
    178c:	38 44 24 0f          	cmp    %al,0xf(%rsp)
    1790:	75 15                	jne    17a7 <phase_3+0x16e>
    1792:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
    1797:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    179e:	00 00 
    17a0:	75 0c                	jne    17ae <phase_3+0x175>
    17a2:	48 83 c4 28          	add    $0x28,%rsp
    17a6:	c3                   	retq   
    17a7:	e8 d8 04 00 00       	callq  1c84 <explode_bomb>
    17ac:	eb e4                	jmp    1792 <phase_3+0x159>
    17ae:	e8 6d fa ff ff       	callq  1220 <__stack_chk_fail@plt>

00000000000017b3 <func4>:
    17b3:	f3 0f 1e fa          	endbr64 
    17b7:	b8 00 00 00 00       	mov    $0x0,%eax
    17bc:	85 ff                	test   %edi,%edi
    17be:	7e 2d                	jle    17ed <func4+0x3a>
    17c0:	41 54                	push   %r12
    17c2:	55                   	push   %rbp
    17c3:	53                   	push   %rbx
    17c4:	89 fb                	mov    %edi,%ebx
    17c6:	89 f5                	mov    %esi,%ebp
    17c8:	89 f0                	mov    %esi,%eax
    17ca:	83 ff 01             	cmp    $0x1,%edi
    17cd:	74 19                	je     17e8 <func4+0x35>
    17cf:	8d 7f ff             	lea    -0x1(%rdi),%edi
    17d2:	e8 dc ff ff ff       	callq  17b3 <func4>
    17d7:	44 8d 24 28          	lea    (%rax,%rbp,1),%r12d
    17db:	8d 7b fe             	lea    -0x2(%rbx),%edi
    17de:	89 ee                	mov    %ebp,%esi
    17e0:	e8 ce ff ff ff       	callq  17b3 <func4>
    17e5:	44 01 e0             	add    %r12d,%eax
    17e8:	5b                   	pop    %rbx
    17e9:	5d                   	pop    %rbp
    17ea:	41 5c                	pop    %r12
    17ec:	c3                   	retq   
    17ed:	c3                   	retq   

00000000000017ee <phase_4>:
    17ee:	f3 0f 1e fa          	endbr64 
    17f2:	48 83 ec 18          	sub    $0x18,%rsp
    17f6:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    17fd:	00 00 
    17ff:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
    1804:	31 c0                	xor    %eax,%eax
    1806:	48 89 e1             	mov    %rsp,%rcx
    1809:	48 8d 54 24 04       	lea    0x4(%rsp),%rdx
    180e:	48 8d 35 1a 1b 00 00 	lea    0x1b1a(%rip),%rsi        # 332f <array.3470+0x14f>
    1815:	e8 a6 fa ff ff       	callq  12c0 <__isoc99_sscanf@plt>
    181a:	83 f8 02             	cmp    $0x2,%eax
    181d:	75 0b                	jne    182a <phase_4+0x3c>
    181f:	8b 04 24             	mov    (%rsp),%eax
    1822:	83 e8 02             	sub    $0x2,%eax
    1825:	83 f8 02             	cmp    $0x2,%eax
    1828:	76 05                	jbe    182f <phase_4+0x41>
    182a:	e8 55 04 00 00       	callq  1c84 <explode_bomb>
    182f:	8b 34 24             	mov    (%rsp),%esi
    1832:	bf 07 00 00 00       	mov    $0x7,%edi
    1837:	e8 77 ff ff ff       	callq  17b3 <func4>
    183c:	39 44 24 04          	cmp    %eax,0x4(%rsp)
    1840:	75 15                	jne    1857 <phase_4+0x69>
    1842:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
    1847:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    184e:	00 00 
    1850:	75 0c                	jne    185e <phase_4+0x70>
    1852:	48 83 c4 18          	add    $0x18,%rsp
    1856:	c3                   	retq   
    1857:	e8 28 04 00 00       	callq  1c84 <explode_bomb>
    185c:	eb e4                	jmp    1842 <phase_4+0x54>
    185e:	e8 bd f9 ff ff       	callq  1220 <__stack_chk_fail@plt>

0000000000001863 <phase_5>:
    1863:	f3 0f 1e fa          	endbr64 
    1867:	48 83 ec 18          	sub    $0x18,%rsp
    186b:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1872:	00 00 
    1874:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
    1879:	31 c0                	xor    %eax,%eax
    187b:	48 8d 4c 24 04       	lea    0x4(%rsp),%rcx
    1880:	48 89 e2             	mov    %rsp,%rdx
    1883:	48 8d 35 a5 1a 00 00 	lea    0x1aa5(%rip),%rsi        # 332f <array.3470+0x14f>
    188a:	e8 31 fa ff ff       	callq  12c0 <__isoc99_sscanf@plt>
    188f:	83 f8 01             	cmp    $0x1,%eax
    1892:	7e 5a                	jle    18ee <phase_5+0x8b>
    1894:	8b 04 24             	mov    (%rsp),%eax
    1897:	83 e0 0f             	and    $0xf,%eax
    189a:	89 04 24             	mov    %eax,(%rsp)
    189d:	83 f8 0f             	cmp    $0xf,%eax
    18a0:	74 32                	je     18d4 <phase_5+0x71>
    18a2:	b9 00 00 00 00       	mov    $0x0,%ecx
    18a7:	ba 00 00 00 00       	mov    $0x0,%edx
    18ac:	48 8d 35 2d 19 00 00 	lea    0x192d(%rip),%rsi        # 31e0 <array.3470>
    18b3:	83 c2 01             	add    $0x1,%edx
    18b6:	48 98                	cltq   
    18b8:	8b 04 86             	mov    (%rsi,%rax,4),%eax
    18bb:	01 c1                	add    %eax,%ecx
    18bd:	83 f8 0f             	cmp    $0xf,%eax
    18c0:	75 f1                	jne    18b3 <phase_5+0x50>
    18c2:	c7 04 24 0f 00 00 00 	movl   $0xf,(%rsp)
    18c9:	83 fa 0f             	cmp    $0xf,%edx
    18cc:	75 06                	jne    18d4 <phase_5+0x71>
    18ce:	39 4c 24 04          	cmp    %ecx,0x4(%rsp)
    18d2:	74 05                	je     18d9 <phase_5+0x76>
    18d4:	e8 ab 03 00 00       	callq  1c84 <explode_bomb>
    18d9:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
    18de:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    18e5:	00 00 
    18e7:	75 0c                	jne    18f5 <phase_5+0x92>
    18e9:	48 83 c4 18          	add    $0x18,%rsp
    18ed:	c3                   	retq   
    18ee:	e8 91 03 00 00       	callq  1c84 <explode_bomb>
    18f3:	eb 9f                	jmp    1894 <phase_5+0x31>
    18f5:	e8 26 f9 ff ff       	callq  1220 <__stack_chk_fail@plt>

00000000000018fa <phase_6>:
    18fa:	f3 0f 1e fa          	endbr64 
    18fe:	41 56                	push   %r14
    1900:	41 55                	push   %r13
    1902:	41 54                	push   %r12
    1904:	55                   	push   %rbp
    1905:	53                   	push   %rbx
    1906:	48 83 ec 60          	sub    $0x60,%rsp
    190a:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1911:	00 00 
    1913:	48 89 44 24 58       	mov    %rax,0x58(%rsp)
    1918:	31 c0                	xor    %eax,%eax
    191a:	49 89 e5             	mov    %rsp,%r13
    191d:	4c 89 ee             	mov    %r13,%rsi
    1920:	e8 8b 03 00 00       	callq  1cb0 <read_six_numbers>
    1925:	41 be 01 00 00 00    	mov    $0x1,%r14d
    192b:	49 89 e4             	mov    %rsp,%r12
    192e:	eb 28                	jmp    1958 <phase_6+0x5e>
    1930:	e8 4f 03 00 00       	callq  1c84 <explode_bomb>
    1935:	eb 30                	jmp    1967 <phase_6+0x6d>
    1937:	48 83 c3 01          	add    $0x1,%rbx
    193b:	83 fb 05             	cmp    $0x5,%ebx
    193e:	7f 10                	jg     1950 <phase_6+0x56>
    1940:	41 8b 04 9c          	mov    (%r12,%rbx,4),%eax
    1944:	39 45 00             	cmp    %eax,0x0(%rbp)
    1947:	75 ee                	jne    1937 <phase_6+0x3d>
    1949:	e8 36 03 00 00       	callq  1c84 <explode_bomb>
    194e:	eb e7                	jmp    1937 <phase_6+0x3d>
    1950:	49 83 c6 01          	add    $0x1,%r14
    1954:	49 83 c5 04          	add    $0x4,%r13
    1958:	4c 89 ed             	mov    %r13,%rbp
    195b:	41 8b 45 00          	mov    0x0(%r13),%eax
    195f:	83 e8 01             	sub    $0x1,%eax
    1962:	83 f8 05             	cmp    $0x5,%eax
    1965:	77 c9                	ja     1930 <phase_6+0x36>
    1967:	41 83 fe 05          	cmp    $0x5,%r14d
    196b:	7f 05                	jg     1972 <phase_6+0x78>
    196d:	4c 89 f3             	mov    %r14,%rbx
    1970:	eb ce                	jmp    1940 <phase_6+0x46>  
    1972:	be 00 00 00 00       	mov    $0x0,%esi
    1977:	8b 0c b4             	mov    (%rsp,%rsi,4),%ecx       #########0x555555555972
    197a:	b8 01 00 00 00       	mov    $0x1,%eax
    197f:	48 8d 15 8a 38 00 00 	lea    0x388a(%rip),%rdx        # 5210 <node1>
    1986:	83 f9 01             	cmp    $0x1,%ecx
    1989:	7e 0b                	jle    1996 <phase_6+0x9c>
    198b:	48 8b 52 08          	mov    0x8(%rdx),%rdx
    198f:	83 c0 01             	add    $0x1,%eax
    1992:	39 c8                	cmp    %ecx,%eax
    1994:	75 f5                	jne    198b <phase_6+0x91>
    1996:	48 89 54 f4 20       	mov    %rdx,0x20(%rsp,%rsi,8)
    199b:	48 83 c6 01          	add    $0x1,%rsi
    199f:	48 83 fe 06          	cmp    $0x6,%rsi
    19a3:	75 d2                	jne    1977 <phase_6+0x7d>
    19a5:	48 8b 5c 24 20       	mov    0x20(%rsp),%rbx
    19aa:	48 8b 44 24 28       	mov    0x28(%rsp),%rax
    19af:	48 89 43 08          	mov    %rax,0x8(%rbx)
    19b3:	48 8b 54 24 30       	mov    0x30(%rsp),%rdx
    19b8:	48 89 50 08          	mov    %rdx,0x8(%rax)
    19bc:	48 8b 44 24 38       	mov    0x38(%rsp),%rax
    19c1:	48 89 42 08          	mov    %rax,0x8(%rdx)
    19c5:	48 8b 54 24 40       	mov    0x40(%rsp),%rdx
    19ca:	48 89 50 08          	mov    %rdx,0x8(%rax)
    19ce:	48 8b 44 24 48       	mov    0x48(%rsp),%rax
    19d3:	48 89 42 08          	mov    %rax,0x8(%rdx)
    19d7:	48 c7 40 08 00 00 00 	movq   $0x0,0x8(%rax)
    19de:	00 
    19df:	bd 05 00 00 00       	mov    $0x5,%ebp
    19e4:	eb 09                	jmp    19ef <phase_6+0xf5>
    19e6:	48 8b 5b 08          	mov    0x8(%rbx),%rbx
    19ea:	83 ed 01             	sub    $0x1,%ebp
    19ed:	74 11                	je     1a00 <phase_6+0x106>
    19ef:	48 8b 43 08          	mov    0x8(%rbx),%rax
    19f3:	8b 00                	mov    (%rax),%eax
    19f5:	39 03                	cmp    %eax,(%rbx)
    19f7:	7e ed                	jle    19e6 <phase_6+0xec>
    19f9:	e8 86 02 00 00       	callq  1c84 <explode_bomb>
    19fe:	eb e6                	jmp    19e6 <phase_6+0xec>
    1a00:	48 8b 44 24 58       	mov    0x58(%rsp),%rax
    1a05:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    1a0c:	00 00 
    1a0e:	75 0d                	jne    1a1d <phase_6+0x123>
    1a10:	48 83 c4 60          	add    $0x60,%rsp
    1a14:	5b                   	pop    %rbx
    1a15:	5d                   	pop    %rbp
    1a16:	41 5c                	pop    %r12
    1a18:	41 5d                	pop    %r13
    1a1a:	41 5e                	pop    %r14
    1a1c:	c3                   	retq   
    1a1d:	e8 fe f7 ff ff       	callq  1220 <__stack_chk_fail@plt>

0000000000001a22 <fun7>:
    1a22:	f3 0f 1e fa          	endbr64 
    1a26:	48 85 ff             	test   %rdi,%rdi
    1a29:	74 32                	je     1a5d <fun7+0x3b>
    1a2b:	48 83 ec 08          	sub    $0x8,%rsp
    1a2f:	8b 17                	mov    (%rdi),%edx
    1a31:	39 f2                	cmp    %esi,%edx
    1a33:	7f 0c                	jg     1a41 <fun7+0x1f>
    1a35:	b8 00 00 00 00       	mov    $0x0,%eax
    1a3a:	75 12                	jne    1a4e <fun7+0x2c>
    1a3c:	48 83 c4 08          	add    $0x8,%rsp
    1a40:	c3                   	retq   
    1a41:	48 8b 7f 08          	mov    0x8(%rdi),%rdi
    1a45:	e8 d8 ff ff ff       	callq  1a22 <fun7>
    1a4a:	01 c0                	add    %eax,%eax
    1a4c:	eb ee                	jmp    1a3c <fun7+0x1a>
    1a4e:	48 8b 7f 10          	mov    0x10(%rdi),%rdi
    1a52:	e8 cb ff ff ff       	callq  1a22 <fun7>
    1a57:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax
    1a5b:	eb df                	jmp    1a3c <fun7+0x1a>
    1a5d:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    1a62:	c3                   	retq   

0000000000001a63 <secret_phase>:
    1a63:	f3 0f 1e fa          	endbr64 
    1a67:	53                   	push   %rbx
    1a68:	e8 88 02 00 00       	callq  1cf5 <read_line>
    1a6d:	48 89 c7             	mov    %rax,%rdi
    1a70:	ba 0a 00 00 00       	mov    $0xa,%edx
    1a75:	be 00 00 00 00       	mov    $0x0,%esi
    1a7a:	e8 21 f8 ff ff       	callq  12a0 <strtol@plt>
    1a7f:	48 89 c3             	mov    %rax,%rbx
    1a82:	8d 40 ff             	lea    -0x1(%rax),%eax
    1a85:	3d e8 03 00 00       	cmp    $0x3e8,%eax
    1a8a:	77 26                	ja     1ab2 <secret_phase+0x4f>
    1a8c:	89 de                	mov    %ebx,%esi
    1a8e:	48 8d 3d 9b 36 00 00 	lea    0x369b(%rip),%rdi        # 5130 <n1>
    1a95:	e8 88 ff ff ff       	callq  1a22 <fun7>
    1a9a:	83 f8 01             	cmp    $0x1,%eax
    1a9d:	75 1a                	jne    1ab9 <secret_phase+0x56>
    1a9f:	48 8d 3d e2 16 00 00 	lea    0x16e2(%rip),%rdi        # 3188 <_IO_stdin_used+0x188>
    1aa6:	e8 55 f7 ff ff       	callq  1200 <puts@plt>
    1aab:	e8 8d 03 00 00       	callq  1e3d <phase_defused>
    1ab0:	5b                   	pop    %rbx
    1ab1:	c3                   	retq   
    1ab2:	e8 cd 01 00 00       	callq  1c84 <explode_bomb>
    1ab7:	eb d3                	jmp    1a8c <secret_phase+0x29>
    1ab9:	e8 c6 01 00 00       	callq  1c84 <explode_bomb>
    1abe:	eb df                	jmp    1a9f <secret_phase+0x3c>

0000000000001ac0 <sig_handler>:
    1ac0:	f3 0f 1e fa          	endbr64 
    1ac4:	50                   	push   %rax
    1ac5:	58                   	pop    %rax
    1ac6:	48 83 ec 08          	sub    $0x8,%rsp
    1aca:	48 8d 3d 4f 17 00 00 	lea    0x174f(%rip),%rdi        # 3220 <array.3470+0x40>
    1ad1:	e8 2a f7 ff ff       	callq  1200 <puts@plt>
    1ad6:	bf 03 00 00 00       	mov    $0x3,%edi
    1adb:	e8 40 f8 ff ff       	callq  1320 <sleep@plt>
    1ae0:	48 8d 35 fb 17 00 00 	lea    0x17fb(%rip),%rsi        # 32e2 <array.3470+0x102>
    1ae7:	bf 01 00 00 00       	mov    $0x1,%edi
    1aec:	b8 00 00 00 00       	mov    $0x0,%eax
    1af1:	e8 da f7 ff ff       	callq  12d0 <__printf_chk@plt>
    1af6:	48 8b 3d 63 3b 00 00 	mov    0x3b63(%rip),%rdi        # 5660 <stdout@@GLIBC_2.2.5>
    1afd:	e8 ae f7 ff ff       	callq  12b0 <fflush@plt>
    1b02:	bf 01 00 00 00       	mov    $0x1,%edi
    1b07:	e8 14 f8 ff ff       	callq  1320 <sleep@plt>
    1b0c:	48 8d 3d d7 17 00 00 	lea    0x17d7(%rip),%rdi        # 32ea <array.3470+0x10a>
    1b13:	e8 e8 f6 ff ff       	callq  1200 <puts@plt>
    1b18:	bf 10 00 00 00       	mov    $0x10,%edi
    1b1d:	e8 ce f7 ff ff       	callq  12f0 <exit@plt>

0000000000001b22 <invalid_phase>:
    1b22:	f3 0f 1e fa          	endbr64 
    1b26:	50                   	push   %rax
    1b27:	58                   	pop    %rax
    1b28:	48 83 ec 08          	sub    $0x8,%rsp
    1b2c:	48 89 fa             	mov    %rdi,%rdx
    1b2f:	48 8d 35 bc 17 00 00 	lea    0x17bc(%rip),%rsi        # 32f2 <array.3470+0x112>
    1b36:	bf 01 00 00 00       	mov    $0x1,%edi
    1b3b:	b8 00 00 00 00       	mov    $0x0,%eax
    1b40:	e8 8b f7 ff ff       	callq  12d0 <__printf_chk@plt>
    1b45:	bf 08 00 00 00       	mov    $0x8,%edi
    1b4a:	e8 a1 f7 ff ff       	callq  12f0 <exit@plt>

0000000000001b4f <string_length>:
    1b4f:	f3 0f 1e fa          	endbr64 
    1b53:	80 3f 00             	cmpb   $0x0,(%rdi)
    1b56:	74 12                	je     1b6a <string_length+0x1b>
    1b58:	b8 00 00 00 00       	mov    $0x0,%eax
    1b5d:	48 83 c7 01          	add    $0x1,%rdi
    1b61:	83 c0 01             	add    $0x1,%eax
    1b64:	80 3f 00             	cmpb   $0x0,(%rdi)
    1b67:	75 f4                	jne    1b5d <string_length+0xe>
    1b69:	c3                   	retq   
    1b6a:	b8 00 00 00 00       	mov    $0x0,%eax
    1b6f:	c3                   	retq   

0000000000001b70 <strings_not_equal>:
    1b70:	f3 0f 1e fa          	endbr64 
    1b74:	41 54                	push   %r12
    1b76:	55                   	push   %rbp
    1b77:	53                   	push   %rbx
    1b78:	48 89 fb             	mov    %rdi,%rbx
    1b7b:	48 89 f5             	mov    %rsi,%rbp
    1b7e:	e8 cc ff ff ff       	callq  1b4f <string_length>
    1b83:	41 89 c4             	mov    %eax,%r12d
    1b86:	48 89 ef             	mov    %rbp,%rdi
    1b89:	e8 c1 ff ff ff       	callq  1b4f <string_length>
    1b8e:	89 c2                	mov    %eax,%edx
    1b90:	b8 01 00 00 00       	mov    $0x1,%eax
    1b95:	41 39 d4             	cmp    %edx,%r12d
    1b98:	75 31                	jne    1bcb <strings_not_equal+0x5b>
    1b9a:	0f b6 13             	movzbl (%rbx),%edx
    1b9d:	84 d2                	test   %dl,%dl
    1b9f:	74 1e                	je     1bbf <strings_not_equal+0x4f>
    1ba1:	b8 00 00 00 00       	mov    $0x0,%eax
    1ba6:	38 54 05 00          	cmp    %dl,0x0(%rbp,%rax,1)
    1baa:	75 1a                	jne    1bc6 <strings_not_equal+0x56>
    1bac:	48 83 c0 01          	add    $0x1,%rax
    1bb0:	0f b6 14 03          	movzbl (%rbx,%rax,1),%edx
    1bb4:	84 d2                	test   %dl,%dl
    1bb6:	75 ee                	jne    1ba6 <strings_not_equal+0x36>
    1bb8:	b8 00 00 00 00       	mov    $0x0,%eax
    1bbd:	eb 0c                	jmp    1bcb <strings_not_equal+0x5b>
    1bbf:	b8 00 00 00 00       	mov    $0x0,%eax
    1bc4:	eb 05                	jmp    1bcb <strings_not_equal+0x5b>
    1bc6:	b8 01 00 00 00       	mov    $0x1,%eax
    1bcb:	5b                   	pop    %rbx
    1bcc:	5d                   	pop    %rbp
    1bcd:	41 5c                	pop    %r12
    1bcf:	c3                   	retq   

0000000000001bd0 <initialize_bomb>:
    1bd0:	f3 0f 1e fa          	endbr64 
    1bd4:	48 83 ec 08          	sub    $0x8,%rsp
    1bd8:	48 8d 35 e1 fe ff ff 	lea    -0x11f(%rip),%rsi        # 1ac0 <sig_handler>
    1bdf:	bf 02 00 00 00       	mov    $0x2,%edi
    1be4:	e8 87 f6 ff ff       	callq  1270 <signal@plt>
    1be9:	48 83 c4 08          	add    $0x8,%rsp
    1bed:	c3                   	retq   

0000000000001bee <initialize_bomb_solve>:
    1bee:	f3 0f 1e fa          	endbr64 
    1bf2:	c3                   	retq   

0000000000001bf3 <blank_line>:
    1bf3:	f3 0f 1e fa          	endbr64 
    1bf7:	55                   	push   %rbp
    1bf8:	53                   	push   %rbx
    1bf9:	48 83 ec 08          	sub    $0x8,%rsp
    1bfd:	48 89 fd             	mov    %rdi,%rbp
    1c00:	0f b6 5d 00          	movzbl 0x0(%rbp),%ebx
    1c04:	84 db                	test   %bl,%bl
    1c06:	74 1e                	je     1c26 <blank_line+0x33>
    1c08:	e8 23 f7 ff ff       	callq  1330 <__ctype_b_loc@plt>
    1c0d:	48 83 c5 01          	add    $0x1,%rbp
    1c11:	48 0f be db          	movsbq %bl,%rbx
    1c15:	48 8b 00             	mov    (%rax),%rax
    1c18:	f6 44 58 01 20       	testb  $0x20,0x1(%rax,%rbx,2)
    1c1d:	75 e1                	jne    1c00 <blank_line+0xd>
    1c1f:	b8 00 00 00 00       	mov    $0x0,%eax
    1c24:	eb 05                	jmp    1c2b <blank_line+0x38>
    1c26:	b8 01 00 00 00       	mov    $0x1,%eax
    1c2b:	48 83 c4 08          	add    $0x8,%rsp
    1c2f:	5b                   	pop    %rbx
    1c30:	5d                   	pop    %rbp
    1c31:	c3                   	retq   

0000000000001c32 <skip>:
    1c32:	f3 0f 1e fa          	endbr64 
    1c36:	55                   	push   %rbp
    1c37:	53                   	push   %rbx
    1c38:	48 83 ec 08          	sub    $0x8,%rsp
    1c3c:	48 8d 2d 5d 3a 00 00 	lea    0x3a5d(%rip),%rbp        # 56a0 <input_strings>
    1c43:	48 63 05 42 3a 00 00 	movslq 0x3a42(%rip),%rax        # 568c <num_input_strings>
    1c4a:	48 8d 3c 80          	lea    (%rax,%rax,4),%rdi
    1c4e:	48 c1 e7 04          	shl    $0x4,%rdi
    1c52:	48 01 ef             	add    %rbp,%rdi
    1c55:	48 8b 15 34 3a 00 00 	mov    0x3a34(%rip),%rdx        # 5690 <infile>
    1c5c:	be 50 00 00 00       	mov    $0x50,%esi
    1c61:	e8 fa f5 ff ff       	callq  1260 <fgets@plt>
    1c66:	48 89 c3             	mov    %rax,%rbx
    1c69:	48 85 c0             	test   %rax,%rax
    1c6c:	74 0c                	je     1c7a <skip+0x48>
    1c6e:	48 89 c7             	mov    %rax,%rdi
    1c71:	e8 7d ff ff ff       	callq  1bf3 <blank_line>
    1c76:	85 c0                	test   %eax,%eax
    1c78:	75 c9                	jne    1c43 <skip+0x11>
    1c7a:	48 89 d8             	mov    %rbx,%rax
    1c7d:	48 83 c4 08          	add    $0x8,%rsp
    1c81:	5b                   	pop    %rbx
    1c82:	5d                   	pop    %rbp
    1c83:	c3                   	retq   

0000000000001c84 <explode_bomb>:
    1c84:	f3 0f 1e fa          	endbr64 
    1c88:	50                   	push   %rax
    1c89:	58                   	pop    %rax
    1c8a:	48 83 ec 08          	sub    $0x8,%rsp
    1c8e:	48 8d 3d 6e 16 00 00 	lea    0x166e(%rip),%rdi        # 3303 <array.3470+0x123>
    1c95:	e8 66 f5 ff ff       	callq  1200 <puts@plt>
    1c9a:	48 8d 3d 6b 16 00 00 	lea    0x166b(%rip),%rdi        # 330c <array.3470+0x12c>
    1ca1:	e8 5a f5 ff ff       	callq  1200 <puts@plt>
    1ca6:	bf 08 00 00 00       	mov    $0x8,%edi
    1cab:	e8 40 f6 ff ff       	callq  12f0 <exit@plt>

0000000000001cb0 <read_six_numbers>:
    1cb0:	f3 0f 1e fa          	endbr64 
    1cb4:	48 83 ec 08          	sub    $0x8,%rsp
    1cb8:	48 89 f2             	mov    %rsi,%rdx
    1cbb:	48 8d 4e 04          	lea    0x4(%rsi),%rcx
    1cbf:	48 8d 46 14          	lea    0x14(%rsi),%rax
    1cc3:	50                   	push   %rax
    1cc4:	48 8d 46 10          	lea    0x10(%rsi),%rax
    1cc8:	50                   	push   %rax
    1cc9:	4c 8d 4e 0c          	lea    0xc(%rsi),%r9
    1ccd:	4c 8d 46 08          	lea    0x8(%rsi),%r8
    1cd1:	48 8d 35 4b 16 00 00 	lea    0x164b(%rip),%rsi        # 3323 <array.3470+0x143>
    1cd8:	b8 00 00 00 00       	mov    $0x0,%eax
    1cdd:	e8 de f5 ff ff       	callq  12c0 <__isoc99_sscanf@plt>
    1ce2:	48 83 c4 10          	add    $0x10,%rsp
    1ce6:	83 f8 05             	cmp    $0x5,%eax
    1ce9:	7e 05                	jle    1cf0 <read_six_numbers+0x40>
    1ceb:	48 83 c4 08          	add    $0x8,%rsp
    1cef:	c3                   	retq   
    1cf0:	e8 8f ff ff ff       	callq  1c84 <explode_bomb>

0000000000001cf5 <read_line>:
    1cf5:	f3 0f 1e fa          	endbr64 
    1cf9:	48 83 ec 08          	sub    $0x8,%rsp
    1cfd:	b8 00 00 00 00       	mov    $0x0,%eax
    1d02:	e8 2b ff ff ff       	callq  1c32 <skip>
    1d07:	48 85 c0             	test   %rax,%rax
    1d0a:	74 6f                	je     1d7b <read_line+0x86>
    1d0c:	8b 35 7a 39 00 00    	mov    0x397a(%rip),%esi        # 568c <num_input_strings>
    1d12:	48 63 c6             	movslq %esi,%rax
    1d15:	48 8d 14 80          	lea    (%rax,%rax,4),%rdx
    1d19:	48 c1 e2 04          	shl    $0x4,%rdx
    1d1d:	48 8d 05 7c 39 00 00 	lea    0x397c(%rip),%rax        # 56a0 <input_strings>
    1d24:	48 01 c2             	add    %rax,%rdx
    1d27:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
    1d2e:	b8 00 00 00 00       	mov    $0x0,%eax
    1d33:	48 89 d7             	mov    %rdx,%rdi
    1d36:	f2 ae                	repnz scas %es:(%rdi),%al
    1d38:	48 f7 d1             	not    %rcx
    1d3b:	48 83 e9 01          	sub    $0x1,%rcx
    1d3f:	83 f9 4e             	cmp    $0x4e,%ecx
    1d42:	0f 8f ab 00 00 00    	jg     1df3 <read_line+0xfe>
    1d48:	83 e9 01             	sub    $0x1,%ecx
    1d4b:	48 63 c9             	movslq %ecx,%rcx
    1d4e:	48 63 c6             	movslq %esi,%rax
    1d51:	48 8d 04 80          	lea    (%rax,%rax,4),%rax
    1d55:	48 c1 e0 04          	shl    $0x4,%rax
    1d59:	48 89 c7             	mov    %rax,%rdi
    1d5c:	48 8d 05 3d 39 00 00 	lea    0x393d(%rip),%rax        # 56a0 <input_strings>
    1d63:	48 01 f8             	add    %rdi,%rax
    1d66:	c6 04 08 00          	movb   $0x0,(%rax,%rcx,1)
    1d6a:	83 c6 01             	add    $0x1,%esi
    1d6d:	89 35 19 39 00 00    	mov    %esi,0x3919(%rip)        # 568c <num_input_strings>
    1d73:	48 89 d0             	mov    %rdx,%rax
    1d76:	48 83 c4 08          	add    $0x8,%rsp
    1d7a:	c3                   	retq   
    1d7b:	48 8b 05 ee 38 00 00 	mov    0x38ee(%rip),%rax        # 5670 <stdin@@GLIBC_2.2.5>
    1d82:	48 39 05 07 39 00 00 	cmp    %rax,0x3907(%rip)        # 5690 <infile>
    1d89:	74 1b                	je     1da6 <read_line+0xb1>
    1d8b:	48 8d 3d c1 15 00 00 	lea    0x15c1(%rip),%rdi        # 3353 <array.3470+0x173>
    1d92:	e8 39 f4 ff ff       	callq  11d0 <getenv@plt>
    1d97:	48 85 c0             	test   %rax,%rax
    1d9a:	74 20                	je     1dbc <read_line+0xc7>
    1d9c:	bf 00 00 00 00       	mov    $0x0,%edi
    1da1:	e8 4a f5 ff ff       	callq  12f0 <exit@plt>
    1da6:	48 8d 3d 88 15 00 00 	lea    0x1588(%rip),%rdi        # 3335 <array.3470+0x155>
    1dad:	e8 4e f4 ff ff       	callq  1200 <puts@plt>
    1db2:	bf 08 00 00 00       	mov    $0x8,%edi
    1db7:	e8 34 f5 ff ff       	callq  12f0 <exit@plt>
    1dbc:	48 8b 05 ad 38 00 00 	mov    0x38ad(%rip),%rax        # 5670 <stdin@@GLIBC_2.2.5>
    1dc3:	48 89 05 c6 38 00 00 	mov    %rax,0x38c6(%rip)        # 5690 <infile>
    1dca:	b8 00 00 00 00       	mov    $0x0,%eax
    1dcf:	e8 5e fe ff ff       	callq  1c32 <skip>
    1dd4:	48 85 c0             	test   %rax,%rax
    1dd7:	0f 85 2f ff ff ff    	jne    1d0c <read_line+0x17>
    1ddd:	48 8d 3d 51 15 00 00 	lea    0x1551(%rip),%rdi        # 3335 <array.3470+0x155>
    1de4:	e8 17 f4 ff ff       	callq  1200 <puts@plt>
    1de9:	bf 00 00 00 00       	mov    $0x0,%edi
    1dee:	e8 fd f4 ff ff       	callq  12f0 <exit@plt>
    1df3:	48 8d 3d 64 15 00 00 	lea    0x1564(%rip),%rdi        # 335e <array.3470+0x17e>
    1dfa:	e8 01 f4 ff ff       	callq  1200 <puts@plt>
    1dff:	8b 05 87 38 00 00    	mov    0x3887(%rip),%eax        # 568c <num_input_strings>
    1e05:	8d 50 01             	lea    0x1(%rax),%edx
    1e08:	89 15 7e 38 00 00    	mov    %edx,0x387e(%rip)        # 568c <num_input_strings>
    1e0e:	48 98                	cltq   
    1e10:	48 6b c0 50          	imul   $0x50,%rax,%rax
    1e14:	48 8d 15 85 38 00 00 	lea    0x3885(%rip),%rdx        # 56a0 <input_strings>
    1e1b:	48 be 2a 2a 2a 74 72 	movabs $0x636e7572742a2a2a,%rsi
    1e22:	75 6e 63 
    1e25:	48 bf 61 74 65 64 2a 	movabs $0x2a2a2a64657461,%rdi
    1e2c:	2a 2a 00 
    1e2f:	48 89 34 02          	mov    %rsi,(%rdx,%rax,1)
    1e33:	48 89 7c 02 08       	mov    %rdi,0x8(%rdx,%rax,1)
    1e38:	e8 47 fe ff ff       	callq  1c84 <explode_bomb>

0000000000001e3d <phase_defused>:
    1e3d:	f3 0f 1e fa          	endbr64 
    1e41:	48 83 ec 78          	sub    $0x78,%rsp
    1e45:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1e4c:	00 00 
    1e4e:	48 89 44 24 68       	mov    %rax,0x68(%rsp)
    1e53:	31 c0                	xor    %eax,%eax
    1e55:	83 3d 30 38 00 00 06 	cmpl   $0x6,0x3830(%rip)        # 568c <num_input_strings>
    1e5c:	74 15                	je     1e73 <phase_defused+0x36>
    1e5e:	48 8b 44 24 68       	mov    0x68(%rsp),%rax
    1e63:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    1e6a:	00 00 
    1e6c:	75 73                	jne    1ee1 <phase_defused+0xa4>
    1e6e:	48 83 c4 78          	add    $0x78,%rsp
    1e72:	c3                   	retq   
    1e73:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
    1e78:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
    1e7d:	4c 8d 44 24 10       	lea    0x10(%rsp),%r8
    1e82:	48 8d 35 f0 14 00 00 	lea    0x14f0(%rip),%rsi        # 3379 <array.3470+0x199>
    1e89:	48 8d 3d 00 39 00 00 	lea    0x3900(%rip),%rdi        # 5790 <input_strings+0xf0>
    1e90:	e8 2b f4 ff ff       	callq  12c0 <__isoc99_sscanf@plt>
    1e95:	83 f8 03             	cmp    $0x3,%eax
    1e98:	74 0e                	je     1ea8 <phase_defused+0x6b>
    1e9a:	48 8d 3d 17 14 00 00 	lea    0x1417(%rip),%rdi        # 32b8 <array.3470+0xd8>
    1ea1:	e8 5a f3 ff ff       	callq  1200 <puts@plt>
    1ea6:	eb b6                	jmp    1e5e <phase_defused+0x21>
    1ea8:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
    1ead:	48 8d 35 ce 14 00 00 	lea    0x14ce(%rip),%rsi        # 3382 <array.3470+0x1a2>
    1eb4:	e8 b7 fc ff ff       	callq  1b70 <strings_not_equal>
    1eb9:	85 c0                	test   %eax,%eax
    1ebb:	75 dd                	jne    1e9a <phase_defused+0x5d>
    1ebd:	48 8d 3d 94 13 00 00 	lea    0x1394(%rip),%rdi        # 3258 <array.3470+0x78>
    1ec4:	e8 37 f3 ff ff       	callq  1200 <puts@plt>
    1ec9:	48 8d 3d b0 13 00 00 	lea    0x13b0(%rip),%rdi        # 3280 <array.3470+0xa0>
    1ed0:	e8 2b f3 ff ff       	callq  1200 <puts@plt>
    1ed5:	b8 00 00 00 00       	mov    $0x0,%eax
    1eda:	e8 84 fb ff ff       	callq  1a63 <secret_phase>
    1edf:	eb b9                	jmp    1e9a <phase_defused+0x5d>
    1ee1:	e8 3a f3 ff ff       	callq  1220 <__stack_chk_fail@plt>

0000000000001ee6 <sigalrm_handler>:
    1ee6:	f3 0f 1e fa          	endbr64 
    1eea:	50                   	push   %rax
    1eeb:	58                   	pop    %rax
    1eec:	48 83 ec 08          	sub    $0x8,%rsp
    1ef0:	b9 00 00 00 00       	mov    $0x0,%ecx
    1ef5:	48 8d 15 f4 14 00 00 	lea    0x14f4(%rip),%rdx        # 33f0 <array.3470+0x210>
    1efc:	be 01 00 00 00       	mov    $0x1,%esi
    1f01:	48 8b 3d 78 37 00 00 	mov    0x3778(%rip),%rdi        # 5680 <stderr@@GLIBC_2.2.5>
    1f08:	b8 00 00 00 00       	mov    $0x0,%eax
    1f0d:	e8 fe f3 ff ff       	callq  1310 <__fprintf_chk@plt>
    1f12:	bf 01 00 00 00       	mov    $0x1,%edi
    1f17:	e8 d4 f3 ff ff       	callq  12f0 <exit@plt>

0000000000001f1c <rio_readlineb>:
    1f1c:	41 56                	push   %r14
    1f1e:	41 55                	push   %r13
    1f20:	41 54                	push   %r12
    1f22:	55                   	push   %rbp
    1f23:	53                   	push   %rbx
    1f24:	48 89 f5             	mov    %rsi,%rbp
    1f27:	48 83 fa 01          	cmp    $0x1,%rdx
    1f2b:	0f 86 90 00 00 00    	jbe    1fc1 <rio_readlineb+0xa5>
    1f31:	48 89 fb             	mov    %rdi,%rbx
    1f34:	4c 8d 74 16 ff       	lea    -0x1(%rsi,%rdx,1),%r14
    1f39:	41 bd 01 00 00 00    	mov    $0x1,%r13d
    1f3f:	4c 8d 67 10          	lea    0x10(%rdi),%r12
    1f43:	eb 54                	jmp    1f99 <rio_readlineb+0x7d>
    1f45:	e8 96 f2 ff ff       	callq  11e0 <__errno_location@plt>
    1f4a:	83 38 04             	cmpl   $0x4,(%rax)
    1f4d:	75 53                	jne    1fa2 <rio_readlineb+0x86>
    1f4f:	ba 00 20 00 00       	mov    $0x2000,%edx
    1f54:	4c 89 e6             	mov    %r12,%rsi
    1f57:	8b 3b                	mov    (%rbx),%edi
    1f59:	e8 f2 f2 ff ff       	callq  1250 <read@plt>
    1f5e:	89 c2                	mov    %eax,%edx
    1f60:	89 43 04             	mov    %eax,0x4(%rbx)
    1f63:	85 c0                	test   %eax,%eax
    1f65:	78 de                	js     1f45 <rio_readlineb+0x29>
    1f67:	85 c0                	test   %eax,%eax
    1f69:	74 40                	je     1fab <rio_readlineb+0x8f>
    1f6b:	4c 89 63 08          	mov    %r12,0x8(%rbx)
    1f6f:	48 8b 43 08          	mov    0x8(%rbx),%rax
    1f73:	0f b6 08             	movzbl (%rax),%ecx
    1f76:	48 83 c0 01          	add    $0x1,%rax
    1f7a:	48 89 43 08          	mov    %rax,0x8(%rbx)
    1f7e:	83 ea 01             	sub    $0x1,%edx
    1f81:	89 53 04             	mov    %edx,0x4(%rbx)
    1f84:	48 83 c5 01          	add    $0x1,%rbp
    1f88:	88 4d ff             	mov    %cl,-0x1(%rbp)
    1f8b:	80 f9 0a             	cmp    $0xa,%cl
    1f8e:	74 3c                	je     1fcc <rio_readlineb+0xb0>
    1f90:	41 83 c5 01          	add    $0x1,%r13d
    1f94:	4c 39 f5             	cmp    %r14,%rbp
    1f97:	74 30                	je     1fc9 <rio_readlineb+0xad>
    1f99:	8b 53 04             	mov    0x4(%rbx),%edx
    1f9c:	85 d2                	test   %edx,%edx
    1f9e:	7e af                	jle    1f4f <rio_readlineb+0x33>
    1fa0:	eb cd                	jmp    1f6f <rio_readlineb+0x53>
    1fa2:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
    1fa9:	eb 05                	jmp    1fb0 <rio_readlineb+0x94>
    1fab:	b8 00 00 00 00       	mov    $0x0,%eax
    1fb0:	85 c0                	test   %eax,%eax
    1fb2:	75 28                	jne    1fdc <rio_readlineb+0xc0>
    1fb4:	b8 00 00 00 00       	mov    $0x0,%eax
    1fb9:	41 83 fd 01          	cmp    $0x1,%r13d
    1fbd:	75 0d                	jne    1fcc <rio_readlineb+0xb0>
    1fbf:	eb 12                	jmp    1fd3 <rio_readlineb+0xb7>
    1fc1:	41 bd 01 00 00 00    	mov    $0x1,%r13d
    1fc7:	eb 03                	jmp    1fcc <rio_readlineb+0xb0>
    1fc9:	4c 89 f5             	mov    %r14,%rbp
    1fcc:	c6 45 00 00          	movb   $0x0,0x0(%rbp)
    1fd0:	49 63 c5             	movslq %r13d,%rax
    1fd3:	5b                   	pop    %rbx
    1fd4:	5d                   	pop    %rbp
    1fd5:	41 5c                	pop    %r12
    1fd7:	41 5d                	pop    %r13
    1fd9:	41 5e                	pop    %r14
    1fdb:	c3                   	retq   
    1fdc:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
    1fe3:	eb ee                	jmp    1fd3 <rio_readlineb+0xb7>

0000000000001fe5 <submitr>:
    1fe5:	f3 0f 1e fa          	endbr64 
    1fe9:	41 57                	push   %r15
    1feb:	41 56                	push   %r14
    1fed:	41 55                	push   %r13
    1fef:	41 54                	push   %r12
    1ff1:	55                   	push   %rbp
    1ff2:	53                   	push   %rbx
    1ff3:	4c 8d 9c 24 00 60 ff 	lea    -0xa000(%rsp),%r11
    1ffa:	ff 
    1ffb:	48 81 ec 00 10 00 00 	sub    $0x1000,%rsp
    2002:	48 83 0c 24 00       	orq    $0x0,(%rsp)
    2007:	4c 39 dc             	cmp    %r11,%rsp
    200a:	75 ef                	jne    1ffb <submitr+0x16>
    200c:	48 83 ec 68          	sub    $0x68,%rsp
    2010:	49 89 fd             	mov    %rdi,%r13
    2013:	89 f5                	mov    %esi,%ebp
    2015:	48 89 14 24          	mov    %rdx,(%rsp)
    2019:	48 89 4c 24 08       	mov    %rcx,0x8(%rsp)
    201e:	4c 89 44 24 18       	mov    %r8,0x18(%rsp)
    2023:	4c 89 4c 24 10       	mov    %r9,0x10(%rsp)
    2028:	48 8b 9c 24 a0 a0 00 	mov    0xa0a0(%rsp),%rbx
    202f:	00 
    2030:	4c 8b bc 24 a8 a0 00 	mov    0xa0a8(%rsp),%r15
    2037:	00 
    2038:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    203f:	00 00 
    2041:	48 89 84 24 58 a0 00 	mov    %rax,0xa058(%rsp)
    2048:	00 
    2049:	31 c0                	xor    %eax,%eax
    204b:	c7 44 24 2c 00 00 00 	movl   $0x0,0x2c(%rsp)
    2052:	00 
    2053:	ba 00 00 00 00       	mov    $0x0,%edx
    2058:	be 01 00 00 00       	mov    $0x1,%esi
    205d:	bf 02 00 00 00       	mov    $0x2,%edi
    2062:	e8 e9 f2 ff ff       	callq  1350 <socket@plt>
    2067:	85 c0                	test   %eax,%eax
    2069:	0f 88 17 01 00 00    	js     2186 <submitr+0x1a1>
    206f:	41 89 c4             	mov    %eax,%r12d
    2072:	4c 89 ef             	mov    %r13,%rdi
    2075:	e8 06 f2 ff ff       	callq  1280 <gethostbyname@plt>
    207a:	48 85 c0             	test   %rax,%rax
    207d:	0f 84 53 01 00 00    	je     21d6 <submitr+0x1f1>
    2083:	4c 8d 6c 24 30       	lea    0x30(%rsp),%r13
    2088:	48 c7 44 24 30 00 00 	movq   $0x0,0x30(%rsp)
    208f:	00 00 
    2091:	48 c7 44 24 38 00 00 	movq   $0x0,0x38(%rsp)
    2098:	00 00 
    209a:	66 c7 44 24 30 02 00 	movw   $0x2,0x30(%rsp)
    20a1:	48 63 50 14          	movslq 0x14(%rax),%rdx
    20a5:	48 8b 40 18          	mov    0x18(%rax),%rax
    20a9:	48 8d 7c 24 34       	lea    0x34(%rsp),%rdi
    20ae:	b9 0c 00 00 00       	mov    $0xc,%ecx
    20b3:	48 8b 30             	mov    (%rax),%rsi
    20b6:	e8 d5 f1 ff ff       	callq  1290 <__memmove_chk@plt>
    20bb:	66 c1 c5 08          	rol    $0x8,%bp
    20bf:	66 89 6c 24 32       	mov    %bp,0x32(%rsp)
    20c4:	ba 10 00 00 00       	mov    $0x10,%edx
    20c9:	4c 89 ee             	mov    %r13,%rsi
    20cc:	44 89 e7             	mov    %r12d,%edi
    20cf:	e8 2c f2 ff ff       	callq  1300 <connect@plt>
    20d4:	85 c0                	test   %eax,%eax
    20d6:	0f 88 65 01 00 00    	js     2241 <submitr+0x25c>
    20dc:	49 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%r9
    20e3:	b8 00 00 00 00       	mov    $0x0,%eax
    20e8:	4c 89 c9             	mov    %r9,%rcx
    20eb:	48 89 df             	mov    %rbx,%rdi
    20ee:	f2 ae                	repnz scas %es:(%rdi),%al
    20f0:	48 f7 d1             	not    %rcx
    20f3:	48 89 ce             	mov    %rcx,%rsi
    20f6:	4c 89 c9             	mov    %r9,%rcx
    20f9:	48 8b 3c 24          	mov    (%rsp),%rdi
    20fd:	f2 ae                	repnz scas %es:(%rdi),%al
    20ff:	49 89 c8             	mov    %rcx,%r8
    2102:	4c 89 c9             	mov    %r9,%rcx
    2105:	48 8b 7c 24 08       	mov    0x8(%rsp),%rdi
    210a:	f2 ae                	repnz scas %es:(%rdi),%al
    210c:	48 89 ca             	mov    %rcx,%rdx
    210f:	48 f7 d2             	not    %rdx
    2112:	4c 89 c9             	mov    %r9,%rcx
    2115:	48 8b 7c 24 10       	mov    0x10(%rsp),%rdi
    211a:	f2 ae                	repnz scas %es:(%rdi),%al
    211c:	4c 29 c2             	sub    %r8,%rdx
    211f:	48 29 ca             	sub    %rcx,%rdx
    2122:	48 8d 44 76 fd       	lea    -0x3(%rsi,%rsi,2),%rax
    2127:	48 8d 44 02 7b       	lea    0x7b(%rdx,%rax,1),%rax
    212c:	48 3d 00 20 00 00    	cmp    $0x2000,%rax
    2132:	0f 87 66 01 00 00    	ja     229e <submitr+0x2b9>
    2138:	48 8d 94 24 50 40 00 	lea    0x4050(%rsp),%rdx
    213f:	00 
    2140:	b9 00 04 00 00       	mov    $0x400,%ecx
    2145:	b8 00 00 00 00       	mov    $0x0,%eax
    214a:	48 89 d7             	mov    %rdx,%rdi
    214d:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    2150:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
    2157:	48 89 df             	mov    %rbx,%rdi
    215a:	f2 ae                	repnz scas %es:(%rdi),%al
    215c:	48 f7 d1             	not    %rcx
    215f:	48 8d 41 ff          	lea    -0x1(%rcx),%rax
    2163:	83 f9 01             	cmp    $0x1,%ecx
    2166:	0f 84 08 05 00 00    	je     2674 <submitr+0x68f>
    216c:	8d 40 ff             	lea    -0x1(%rax),%eax
    216f:	4c 8d 74 03 01       	lea    0x1(%rbx,%rax,1),%r14
    2174:	48 89 d5             	mov    %rdx,%rbp
    2177:	49 bd d9 ff 00 00 00 	movabs $0x2000000000ffd9,%r13
    217e:	00 20 00 
    2181:	e9 a6 01 00 00       	jmpq   232c <submitr+0x347>
    2186:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    218d:	3a 20 43 
    2190:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    2197:	20 75 6e 
    219a:	49 89 07             	mov    %rax,(%r15)
    219d:	49 89 57 08          	mov    %rdx,0x8(%r15)
    21a1:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    21a8:	74 6f 20 
    21ab:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
    21b2:	65 20 73 
    21b5:	49 89 47 10          	mov    %rax,0x10(%r15)
    21b9:	49 89 57 18          	mov    %rdx,0x18(%r15)
    21bd:	41 c7 47 20 6f 63 6b 	movl   $0x656b636f,0x20(%r15)
    21c4:	65 
    21c5:	66 41 c7 47 24 74 00 	movw   $0x74,0x24(%r15)
    21cc:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    21d1:	e9 16 03 00 00       	jmpq   24ec <submitr+0x507>
    21d6:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
    21dd:	3a 20 44 
    21e0:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
    21e7:	20 75 6e 
    21ea:	49 89 07             	mov    %rax,(%r15)
    21ed:	49 89 57 08          	mov    %rdx,0x8(%r15)
    21f1:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    21f8:	74 6f 20 
    21fb:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
    2202:	76 65 20 
    2205:	49 89 47 10          	mov    %rax,0x10(%r15)
    2209:	49 89 57 18          	mov    %rdx,0x18(%r15)
    220d:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
    2214:	72 20 61 
    2217:	49 89 47 20          	mov    %rax,0x20(%r15)
    221b:	41 c7 47 28 64 64 72 	movl   $0x65726464,0x28(%r15)
    2222:	65 
    2223:	66 41 c7 47 2c 73 73 	movw   $0x7373,0x2c(%r15)
    222a:	41 c6 47 2e 00       	movb   $0x0,0x2e(%r15)
    222f:	44 89 e7             	mov    %r12d,%edi
    2232:	e8 09 f0 ff ff       	callq  1240 <close@plt>
    2237:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    223c:	e9 ab 02 00 00       	jmpq   24ec <submitr+0x507>
    2241:	48 b8 45 72 72 6f 72 	movabs $0x55203a726f727245,%rax
    2248:	3a 20 55 
    224b:	48 ba 6e 61 62 6c 65 	movabs $0x6f7420656c62616e,%rdx
    2252:	20 74 6f 
    2255:	49 89 07             	mov    %rax,(%r15)
    2258:	49 89 57 08          	mov    %rdx,0x8(%r15)
    225c:	48 b8 20 63 6f 6e 6e 	movabs $0x7463656e6e6f6320,%rax
    2263:	65 63 74 
    2266:	48 ba 20 74 6f 20 74 	movabs $0x20656874206f7420,%rdx
    226d:	68 65 20 
    2270:	49 89 47 10          	mov    %rax,0x10(%r15)
    2274:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2278:	41 c7 47 20 73 65 72 	movl   $0x76726573,0x20(%r15)
    227f:	76 
    2280:	66 41 c7 47 24 65 72 	movw   $0x7265,0x24(%r15)
    2287:	41 c6 47 26 00       	movb   $0x0,0x26(%r15)
    228c:	44 89 e7             	mov    %r12d,%edi
    228f:	e8 ac ef ff ff       	callq  1240 <close@plt>
    2294:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2299:	e9 4e 02 00 00       	jmpq   24ec <submitr+0x507>
    229e:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
    22a5:	3a 20 52 
    22a8:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
    22af:	20 73 74 
    22b2:	49 89 07             	mov    %rax,(%r15)
    22b5:	49 89 57 08          	mov    %rdx,0x8(%r15)
    22b9:	48 b8 72 69 6e 67 20 	movabs $0x6f6f7420676e6972,%rax
    22c0:	74 6f 6f 
    22c3:	48 ba 20 6c 61 72 67 	movabs $0x202e656772616c20,%rdx
    22ca:	65 2e 20 
    22cd:	49 89 47 10          	mov    %rax,0x10(%r15)
    22d1:	49 89 57 18          	mov    %rdx,0x18(%r15)
    22d5:	48 b8 49 6e 63 72 65 	movabs $0x6573616572636e49,%rax
    22dc:	61 73 65 
    22df:	48 ba 20 53 55 42 4d 	movabs $0x5254494d42555320,%rdx
    22e6:	49 54 52 
    22e9:	49 89 47 20          	mov    %rax,0x20(%r15)
    22ed:	49 89 57 28          	mov    %rdx,0x28(%r15)
    22f1:	48 b8 5f 4d 41 58 42 	movabs $0x46554258414d5f,%rax
    22f8:	55 46 00 
    22fb:	49 89 47 30          	mov    %rax,0x30(%r15)
    22ff:	44 89 e7             	mov    %r12d,%edi
    2302:	e8 39 ef ff ff       	callq  1240 <close@plt>
    2307:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    230c:	e9 db 01 00 00       	jmpq   24ec <submitr+0x507>
    2311:	49 0f a3 c5          	bt     %rax,%r13
    2315:	73 21                	jae    2338 <submitr+0x353>
    2317:	44 88 45 00          	mov    %r8b,0x0(%rbp)
    231b:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
    231f:	48 83 c3 01          	add    $0x1,%rbx
    2323:	4c 39 f3             	cmp    %r14,%rbx
    2326:	0f 84 48 03 00 00    	je     2674 <submitr+0x68f>
    232c:	44 0f b6 03          	movzbl (%rbx),%r8d
    2330:	41 8d 40 d6          	lea    -0x2a(%r8),%eax
    2334:	3c 35                	cmp    $0x35,%al
    2336:	76 d9                	jbe    2311 <submitr+0x32c>
    2338:	44 89 c0             	mov    %r8d,%eax
    233b:	83 e0 df             	and    $0xffffffdf,%eax
    233e:	83 e8 41             	sub    $0x41,%eax
    2341:	3c 19                	cmp    $0x19,%al
    2343:	76 d2                	jbe    2317 <submitr+0x332>
    2345:	41 80 f8 20          	cmp    $0x20,%r8b
    2349:	74 63                	je     23ae <submitr+0x3c9>
    234b:	41 8d 40 e0          	lea    -0x20(%r8),%eax
    234f:	3c 5f                	cmp    $0x5f,%al
    2351:	76 0a                	jbe    235d <submitr+0x378>
    2353:	41 80 f8 09          	cmp    $0x9,%r8b
    2357:	0f 85 8a 02 00 00    	jne    25e7 <submitr+0x602>
    235d:	48 8d bc 24 50 80 00 	lea    0x8050(%rsp),%rdi
    2364:	00 
    2365:	45 0f b6 c0          	movzbl %r8b,%r8d
    2369:	48 8d 0d 56 11 00 00 	lea    0x1156(%rip),%rcx        # 34c6 <array.3470+0x2e6>
    2370:	ba 08 00 00 00       	mov    $0x8,%edx
    2375:	be 01 00 00 00       	mov    $0x1,%esi
    237a:	b8 00 00 00 00       	mov    $0x0,%eax
    237f:	e8 bc ef ff ff       	callq  1340 <__sprintf_chk@plt>
    2384:	0f b6 84 24 50 80 00 	movzbl 0x8050(%rsp),%eax
    238b:	00 
    238c:	88 45 00             	mov    %al,0x0(%rbp)
    238f:	0f b6 84 24 51 80 00 	movzbl 0x8051(%rsp),%eax
    2396:	00 
    2397:	88 45 01             	mov    %al,0x1(%rbp)
    239a:	0f b6 84 24 52 80 00 	movzbl 0x8052(%rsp),%eax
    23a1:	00 
    23a2:	88 45 02             	mov    %al,0x2(%rbp)
    23a5:	48 8d 6d 03          	lea    0x3(%rbp),%rbp
    23a9:	e9 71 ff ff ff       	jmpq   231f <submitr+0x33a>
    23ae:	c6 45 00 2b          	movb   $0x2b,0x0(%rbp)
    23b2:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
    23b6:	e9 64 ff ff ff       	jmpq   231f <submitr+0x33a>
    23bb:	48 01 c5             	add    %rax,%rbp
    23be:	48 29 c3             	sub    %rax,%rbx
    23c1:	0f 84 25 03 00 00    	je     26ec <submitr+0x707>
    23c7:	48 89 da             	mov    %rbx,%rdx
    23ca:	48 89 ee             	mov    %rbp,%rsi
    23cd:	44 89 e7             	mov    %r12d,%edi
    23d0:	e8 3b ee ff ff       	callq  1210 <write@plt>
    23d5:	48 85 c0             	test   %rax,%rax
    23d8:	7f e1                	jg     23bb <submitr+0x3d6>
    23da:	e8 01 ee ff ff       	callq  11e0 <__errno_location@plt>
    23df:	83 38 04             	cmpl   $0x4,(%rax)
    23e2:	0f 85 a0 01 00 00    	jne    2588 <submitr+0x5a3>
    23e8:	4c 89 e8             	mov    %r13,%rax
    23eb:	eb ce                	jmp    23bb <submitr+0x3d6>
    23ed:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    23f4:	3a 20 43 
    23f7:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    23fe:	20 75 6e 
    2401:	49 89 07             	mov    %rax,(%r15)
    2404:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2408:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    240f:	74 6f 20 
    2412:	48 ba 72 65 61 64 20 	movabs $0x7269662064616572,%rdx
    2419:	66 69 72 
    241c:	49 89 47 10          	mov    %rax,0x10(%r15)
    2420:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2424:	48 b8 73 74 20 68 65 	movabs $0x6564616568207473,%rax
    242b:	61 64 65 
    242e:	48 ba 72 20 66 72 6f 	movabs $0x73206d6f72662072,%rdx
    2435:	6d 20 73 
    2438:	49 89 47 20          	mov    %rax,0x20(%r15)
    243c:	49 89 57 28          	mov    %rdx,0x28(%r15)
    2440:	41 c7 47 30 65 72 76 	movl   $0x65767265,0x30(%r15)
    2447:	65 
    2448:	66 41 c7 47 34 72 00 	movw   $0x72,0x34(%r15)
    244f:	44 89 e7             	mov    %r12d,%edi
    2452:	e8 e9 ed ff ff       	callq  1240 <close@plt>
    2457:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    245c:	e9 8b 00 00 00       	jmpq   24ec <submitr+0x507>
    2461:	4c 8d 8c 24 50 80 00 	lea    0x8050(%rsp),%r9
    2468:	00 
    2469:	48 8d 0d a8 0f 00 00 	lea    0xfa8(%rip),%rcx        # 3418 <array.3470+0x238>
    2470:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
    2477:	be 01 00 00 00       	mov    $0x1,%esi
    247c:	4c 89 ff             	mov    %r15,%rdi
    247f:	b8 00 00 00 00       	mov    $0x0,%eax
    2484:	e8 b7 ee ff ff       	callq  1340 <__sprintf_chk@plt>
    2489:	44 89 e7             	mov    %r12d,%edi
    248c:	e8 af ed ff ff       	callq  1240 <close@plt>
    2491:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2496:	eb 54                	jmp    24ec <submitr+0x507>
    2498:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
    249f:	00 
    24a0:	48 8d 7c 24 40       	lea    0x40(%rsp),%rdi
    24a5:	ba 00 20 00 00       	mov    $0x2000,%edx
    24aa:	e8 6d fa ff ff       	callq  1f1c <rio_readlineb>
    24af:	48 85 c0             	test   %rax,%rax
    24b2:	7e 61                	jle    2515 <submitr+0x530>
    24b4:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
    24bb:	00 
    24bc:	4c 89 ff             	mov    %r15,%rdi
    24bf:	e8 2c ed ff ff       	callq  11f0 <strcpy@plt>
    24c4:	44 89 e7             	mov    %r12d,%edi
    24c7:	e8 74 ed ff ff       	callq  1240 <close@plt>
    24cc:	b9 03 00 00 00       	mov    $0x3,%ecx
    24d1:	48 8d 3d 09 10 00 00 	lea    0x1009(%rip),%rdi        # 34e1 <array.3470+0x301>
    24d8:	4c 89 fe             	mov    %r15,%rsi
    24db:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
    24dd:	0f 97 c0             	seta   %al
    24e0:	1c 00                	sbb    $0x0,%al
    24e2:	84 c0                	test   %al,%al
    24e4:	0f 95 c0             	setne  %al
    24e7:	0f b6 c0             	movzbl %al,%eax
    24ea:	f7 d8                	neg    %eax
    24ec:	48 8b 94 24 58 a0 00 	mov    0xa058(%rsp),%rdx
    24f3:	00 
    24f4:	64 48 33 14 25 28 00 	xor    %fs:0x28,%rdx
    24fb:	00 00 
    24fd:	0f 85 0c 03 00 00    	jne    280f <submitr+0x82a>
    2503:	48 81 c4 68 a0 00 00 	add    $0xa068,%rsp
    250a:	5b                   	pop    %rbx
    250b:	5d                   	pop    %rbp
    250c:	41 5c                	pop    %r12
    250e:	41 5d                	pop    %r13
    2510:	41 5e                	pop    %r14
    2512:	41 5f                	pop    %r15
    2514:	c3                   	retq   
    2515:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    251c:	3a 20 43 
    251f:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    2526:	20 75 6e 
    2529:	49 89 07             	mov    %rax,(%r15)
    252c:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2530:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    2537:	74 6f 20 
    253a:	48 ba 72 65 61 64 20 	movabs $0x6174732064616572,%rdx
    2541:	73 74 61 
    2544:	49 89 47 10          	mov    %rax,0x10(%r15)
    2548:	49 89 57 18          	mov    %rdx,0x18(%r15)
    254c:	48 b8 74 75 73 20 6d 	movabs $0x7373656d20737574,%rax
    2553:	65 73 73 
    2556:	48 ba 61 67 65 20 66 	movabs $0x6d6f726620656761,%rdx
    255d:	72 6f 6d 
    2560:	49 89 47 20          	mov    %rax,0x20(%r15)
    2564:	49 89 57 28          	mov    %rdx,0x28(%r15)
    2568:	48 b8 20 73 65 72 76 	movabs $0x72657672657320,%rax
    256f:	65 72 00 
    2572:	49 89 47 30          	mov    %rax,0x30(%r15)
    2576:	44 89 e7             	mov    %r12d,%edi
    2579:	e8 c2 ec ff ff       	callq  1240 <close@plt>
    257e:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2583:	e9 64 ff ff ff       	jmpq   24ec <submitr+0x507>
    2588:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    258f:	3a 20 43 
    2592:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    2599:	20 75 6e 
    259c:	49 89 07             	mov    %rax,(%r15)
    259f:	49 89 57 08          	mov    %rdx,0x8(%r15)
    25a3:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    25aa:	74 6f 20 
    25ad:	48 ba 77 72 69 74 65 	movabs $0x6f74206574697277,%rdx
    25b4:	20 74 6f 
    25b7:	49 89 47 10          	mov    %rax,0x10(%r15)
    25bb:	49 89 57 18          	mov    %rdx,0x18(%r15)
    25bf:	48 b8 20 74 68 65 20 	movabs $0x7265732065687420,%rax
    25c6:	73 65 72 
    25c9:	49 89 47 20          	mov    %rax,0x20(%r15)
    25cd:	41 c7 47 28 76 65 72 	movl   $0x726576,0x28(%r15)
    25d4:	00 
    25d5:	44 89 e7             	mov    %r12d,%edi
    25d8:	e8 63 ec ff ff       	callq  1240 <close@plt>
    25dd:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    25e2:	e9 05 ff ff ff       	jmpq   24ec <submitr+0x507>
    25e7:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
    25ee:	3a 20 52 
    25f1:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
    25f8:	20 73 74 
    25fb:	49 89 07             	mov    %rax,(%r15)
    25fe:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2602:	48 b8 72 69 6e 67 20 	movabs $0x6e6f6320676e6972,%rax
    2609:	63 6f 6e 
    260c:	48 ba 74 61 69 6e 73 	movabs $0x6e6120736e696174,%rdx
    2613:	20 61 6e 
    2616:	49 89 47 10          	mov    %rax,0x10(%r15)
    261a:	49 89 57 18          	mov    %rdx,0x18(%r15)
    261e:	48 b8 20 69 6c 6c 65 	movabs $0x6c6167656c6c6920,%rax
    2625:	67 61 6c 
    2628:	48 ba 20 6f 72 20 75 	movabs $0x72706e7520726f20,%rdx
    262f:	6e 70 72 
    2632:	49 89 47 20          	mov    %rax,0x20(%r15)
    2636:	49 89 57 28          	mov    %rdx,0x28(%r15)
    263a:	48 b8 69 6e 74 61 62 	movabs $0x20656c6261746e69,%rax
    2641:	6c 65 20 
    2644:	48 ba 63 68 61 72 61 	movabs $0x6574636172616863,%rdx
    264b:	63 74 65 
    264e:	49 89 47 30          	mov    %rax,0x30(%r15)
    2652:	49 89 57 38          	mov    %rdx,0x38(%r15)
    2656:	66 41 c7 47 40 72 2e 	movw   $0x2e72,0x40(%r15)
    265d:	41 c6 47 42 00       	movb   $0x0,0x42(%r15)
    2662:	44 89 e7             	mov    %r12d,%edi
    2665:	e8 d6 eb ff ff       	callq  1240 <close@plt>
    266a:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    266f:	e9 78 fe ff ff       	jmpq   24ec <submitr+0x507>
    2674:	48 8d 9c 24 50 20 00 	lea    0x2050(%rsp),%rbx
    267b:	00 
    267c:	48 83 ec 08          	sub    $0x8,%rsp
    2680:	48 8d 84 24 58 40 00 	lea    0x4058(%rsp),%rax
    2687:	00 
    2688:	50                   	push   %rax
    2689:	ff 74 24 20          	pushq  0x20(%rsp)
    268d:	ff 74 24 30          	pushq  0x30(%rsp)
    2691:	4c 8b 4c 24 28       	mov    0x28(%rsp),%r9
    2696:	4c 8b 44 24 20       	mov    0x20(%rsp),%r8
    269b:	48 8d 0d a6 0d 00 00 	lea    0xda6(%rip),%rcx        # 3448 <array.3470+0x268>
    26a2:	ba 00 20 00 00       	mov    $0x2000,%edx
    26a7:	be 01 00 00 00       	mov    $0x1,%esi
    26ac:	48 89 df             	mov    %rbx,%rdi
    26af:	b8 00 00 00 00       	mov    $0x0,%eax
    26b4:	e8 87 ec ff ff       	callq  1340 <__sprintf_chk@plt>
    26b9:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
    26c0:	b8 00 00 00 00       	mov    $0x0,%eax
    26c5:	48 89 df             	mov    %rbx,%rdi
    26c8:	f2 ae                	repnz scas %es:(%rdi),%al
    26ca:	48 f7 d1             	not    %rcx
    26cd:	48 83 c4 20          	add    $0x20,%rsp
    26d1:	48 8d ac 24 50 20 00 	lea    0x2050(%rsp),%rbp
    26d8:	00 
    26d9:	41 bd 00 00 00 00    	mov    $0x0,%r13d
    26df:	48 89 cb             	mov    %rcx,%rbx
    26e2:	48 83 eb 01          	sub    $0x1,%rbx
    26e6:	0f 85 db fc ff ff    	jne    23c7 <submitr+0x3e2>
    26ec:	44 89 64 24 40       	mov    %r12d,0x40(%rsp)
    26f1:	c7 44 24 44 00 00 00 	movl   $0x0,0x44(%rsp)
    26f8:	00 
    26f9:	48 8d 7c 24 40       	lea    0x40(%rsp),%rdi
    26fe:	48 8d 44 24 50       	lea    0x50(%rsp),%rax
    2703:	48 89 44 24 48       	mov    %rax,0x48(%rsp)
    2708:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
    270f:	00 
    2710:	ba 00 20 00 00       	mov    $0x2000,%edx
    2715:	e8 02 f8 ff ff       	callq  1f1c <rio_readlineb>
    271a:	48 85 c0             	test   %rax,%rax
    271d:	0f 8e ca fc ff ff    	jle    23ed <submitr+0x408>
    2723:	48 8d 4c 24 2c       	lea    0x2c(%rsp),%rcx
    2728:	48 8d 94 24 50 60 00 	lea    0x6050(%rsp),%rdx
    272f:	00 
    2730:	48 8d bc 24 50 20 00 	lea    0x2050(%rsp),%rdi
    2737:	00 
    2738:	4c 8d 84 24 50 80 00 	lea    0x8050(%rsp),%r8
    273f:	00 
    2740:	48 8d 35 86 0d 00 00 	lea    0xd86(%rip),%rsi        # 34cd <array.3470+0x2ed>
    2747:	b8 00 00 00 00       	mov    $0x0,%eax
    274c:	e8 6f eb ff ff       	callq  12c0 <__isoc99_sscanf@plt>
    2751:	44 8b 44 24 2c       	mov    0x2c(%rsp),%r8d
    2756:	41 81 f8 c8 00 00 00 	cmp    $0xc8,%r8d
    275d:	0f 85 fe fc ff ff    	jne    2461 <submitr+0x47c>
    2763:	48 8d 1d 74 0d 00 00 	lea    0xd74(%rip),%rbx        # 34de <array.3470+0x2fe>
    276a:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
    2771:	00 
    2772:	b9 03 00 00 00       	mov    $0x3,%ecx
    2777:	48 89 df             	mov    %rbx,%rdi
    277a:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
    277c:	0f 97 c0             	seta   %al
    277f:	1c 00                	sbb    $0x0,%al
    2781:	84 c0                	test   %al,%al
    2783:	0f 84 0f fd ff ff    	je     2498 <submitr+0x4b3>
    2789:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
    2790:	00 
    2791:	48 8d 7c 24 40       	lea    0x40(%rsp),%rdi
    2796:	ba 00 20 00 00       	mov    $0x2000,%edx
    279b:	e8 7c f7 ff ff       	callq  1f1c <rio_readlineb>
    27a0:	48 85 c0             	test   %rax,%rax
    27a3:	7f c5                	jg     276a <submitr+0x785>
    27a5:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    27ac:	3a 20 43 
    27af:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    27b6:	20 75 6e 
    27b9:	49 89 07             	mov    %rax,(%r15)
    27bc:	49 89 57 08          	mov    %rdx,0x8(%r15)
    27c0:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    27c7:	74 6f 20 
    27ca:	48 ba 72 65 61 64 20 	movabs $0x6165682064616572,%rdx
    27d1:	68 65 61 
    27d4:	49 89 47 10          	mov    %rax,0x10(%r15)
    27d8:	49 89 57 18          	mov    %rdx,0x18(%r15)
    27dc:	48 b8 64 65 72 73 20 	movabs $0x6f72662073726564,%rax
    27e3:	66 72 6f 
    27e6:	48 ba 6d 20 73 65 72 	movabs $0x726576726573206d,%rdx
    27ed:	76 65 72 
    27f0:	49 89 47 20          	mov    %rax,0x20(%r15)
    27f4:	49 89 57 28          	mov    %rdx,0x28(%r15)
    27f8:	41 c6 47 30 00       	movb   $0x0,0x30(%r15)
    27fd:	44 89 e7             	mov    %r12d,%edi
    2800:	e8 3b ea ff ff       	callq  1240 <close@plt>
    2805:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    280a:	e9 dd fc ff ff       	jmpq   24ec <submitr+0x507>
    280f:	e8 0c ea ff ff       	callq  1220 <__stack_chk_fail@plt>

0000000000002814 <init_timeout>:
    2814:	f3 0f 1e fa          	endbr64 
    2818:	85 ff                	test   %edi,%edi
    281a:	75 01                	jne    281d <init_timeout+0x9>
    281c:	c3                   	retq   
    281d:	53                   	push   %rbx
    281e:	89 fb                	mov    %edi,%ebx
    2820:	48 8d 35 bf f6 ff ff 	lea    -0x941(%rip),%rsi        # 1ee6 <sigalrm_handler>
    2827:	bf 0e 00 00 00       	mov    $0xe,%edi
    282c:	e8 3f ea ff ff       	callq  1270 <signal@plt>
    2831:	85 db                	test   %ebx,%ebx
    2833:	bf 00 00 00 00       	mov    $0x0,%edi
    2838:	0f 49 fb             	cmovns %ebx,%edi
    283b:	e8 f0 e9 ff ff       	callq  1230 <alarm@plt>
    2840:	5b                   	pop    %rbx
    2841:	c3                   	retq   

0000000000002842 <init_driver>:
    2842:	f3 0f 1e fa          	endbr64 
    2846:	41 54                	push   %r12
    2848:	55                   	push   %rbp
    2849:	53                   	push   %rbx
    284a:	48 83 ec 20          	sub    $0x20,%rsp
    284e:	48 89 fd             	mov    %rdi,%rbp
    2851:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    2858:	00 00 
    285a:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
    285f:	31 c0                	xor    %eax,%eax
    2861:	be 01 00 00 00       	mov    $0x1,%esi
    2866:	bf 0d 00 00 00       	mov    $0xd,%edi
    286b:	e8 00 ea ff ff       	callq  1270 <signal@plt>
    2870:	be 01 00 00 00       	mov    $0x1,%esi
    2875:	bf 1d 00 00 00       	mov    $0x1d,%edi
    287a:	e8 f1 e9 ff ff       	callq  1270 <signal@plt>
    287f:	be 01 00 00 00       	mov    $0x1,%esi
    2884:	bf 1d 00 00 00       	mov    $0x1d,%edi
    2889:	e8 e2 e9 ff ff       	callq  1270 <signal@plt>
    288e:	ba 00 00 00 00       	mov    $0x0,%edx
    2893:	be 01 00 00 00       	mov    $0x1,%esi
    2898:	bf 02 00 00 00       	mov    $0x2,%edi
    289d:	e8 ae ea ff ff       	callq  1350 <socket@plt>
    28a2:	85 c0                	test   %eax,%eax
    28a4:	0f 88 9c 00 00 00    	js     2946 <init_driver+0x104>
    28aa:	89 c3                	mov    %eax,%ebx
    28ac:	48 8d 3d 31 0c 00 00 	lea    0xc31(%rip),%rdi        # 34e4 <array.3470+0x304>
    28b3:	e8 c8 e9 ff ff       	callq  1280 <gethostbyname@plt>
    28b8:	48 85 c0             	test   %rax,%rax
    28bb:	0f 84 d1 00 00 00    	je     2992 <init_driver+0x150>
    28c1:	49 89 e4             	mov    %rsp,%r12
    28c4:	48 c7 04 24 00 00 00 	movq   $0x0,(%rsp)
    28cb:	00 
    28cc:	48 c7 44 24 08 00 00 	movq   $0x0,0x8(%rsp)
    28d3:	00 00 
    28d5:	66 c7 04 24 02 00    	movw   $0x2,(%rsp)
    28db:	48 63 50 14          	movslq 0x14(%rax),%rdx
    28df:	48 8b 40 18          	mov    0x18(%rax),%rax
    28e3:	48 8d 7c 24 04       	lea    0x4(%rsp),%rdi
    28e8:	b9 0c 00 00 00       	mov    $0xc,%ecx
    28ed:	48 8b 30             	mov    (%rax),%rsi
    28f0:	e8 9b e9 ff ff       	callq  1290 <__memmove_chk@plt>
    28f5:	66 c7 44 24 02 3b 6e 	movw   $0x6e3b,0x2(%rsp)
    28fc:	ba 10 00 00 00       	mov    $0x10,%edx
    2901:	4c 89 e6             	mov    %r12,%rsi
    2904:	89 df                	mov    %ebx,%edi
    2906:	e8 f5 e9 ff ff       	callq  1300 <connect@plt>
    290b:	85 c0                	test   %eax,%eax
    290d:	0f 88 e7 00 00 00    	js     29fa <init_driver+0x1b8>
    2913:	89 df                	mov    %ebx,%edi
    2915:	e8 26 e9 ff ff       	callq  1240 <close@plt>
    291a:	66 c7 45 00 4f 4b    	movw   $0x4b4f,0x0(%rbp)
    2920:	c6 45 02 00          	movb   $0x0,0x2(%rbp)
    2924:	b8 00 00 00 00       	mov    $0x0,%eax
    2929:	48 8b 4c 24 18       	mov    0x18(%rsp),%rcx
    292e:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
    2935:	00 00 
    2937:	0f 85 f5 00 00 00    	jne    2a32 <init_driver+0x1f0>
    293d:	48 83 c4 20          	add    $0x20,%rsp
    2941:	5b                   	pop    %rbx
    2942:	5d                   	pop    %rbp
    2943:	41 5c                	pop    %r12
    2945:	c3                   	retq   
    2946:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    294d:	3a 20 43 
    2950:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    2957:	20 75 6e 
    295a:	48 89 45 00          	mov    %rax,0x0(%rbp)
    295e:	48 89 55 08          	mov    %rdx,0x8(%rbp)
    2962:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    2969:	74 6f 20 
    296c:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
    2973:	65 20 73 
    2976:	48 89 45 10          	mov    %rax,0x10(%rbp)
    297a:	48 89 55 18          	mov    %rdx,0x18(%rbp)
    297e:	c7 45 20 6f 63 6b 65 	movl   $0x656b636f,0x20(%rbp)
    2985:	66 c7 45 24 74 00    	movw   $0x74,0x24(%rbp)
    298b:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2990:	eb 97                	jmp    2929 <init_driver+0xe7>
    2992:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
    2999:	3a 20 44 
    299c:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
    29a3:	20 75 6e 
    29a6:	48 89 45 00          	mov    %rax,0x0(%rbp)
    29aa:	48 89 55 08          	mov    %rdx,0x8(%rbp)
    29ae:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    29b5:	74 6f 20 
    29b8:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
    29bf:	76 65 20 
    29c2:	48 89 45 10          	mov    %rax,0x10(%rbp)
    29c6:	48 89 55 18          	mov    %rdx,0x18(%rbp)
    29ca:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
    29d1:	72 20 61 
    29d4:	48 89 45 20          	mov    %rax,0x20(%rbp)
    29d8:	c7 45 28 64 64 72 65 	movl   $0x65726464,0x28(%rbp)
    29df:	66 c7 45 2c 73 73    	movw   $0x7373,0x2c(%rbp)
    29e5:	c6 45 2e 00          	movb   $0x0,0x2e(%rbp)
    29e9:	89 df                	mov    %ebx,%edi
    29eb:	e8 50 e8 ff ff       	callq  1240 <close@plt>
    29f0:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    29f5:	e9 2f ff ff ff       	jmpq   2929 <init_driver+0xe7>
    29fa:	4c 8d 05 e3 0a 00 00 	lea    0xae3(%rip),%r8        # 34e4 <array.3470+0x304>
    2a01:	48 8d 0d 98 0a 00 00 	lea    0xa98(%rip),%rcx        # 34a0 <array.3470+0x2c0>
    2a08:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
    2a0f:	be 01 00 00 00       	mov    $0x1,%esi
    2a14:	48 89 ef             	mov    %rbp,%rdi
    2a17:	b8 00 00 00 00       	mov    $0x0,%eax
    2a1c:	e8 1f e9 ff ff       	callq  1340 <__sprintf_chk@plt>
    2a21:	89 df                	mov    %ebx,%edi
    2a23:	e8 18 e8 ff ff       	callq  1240 <close@plt>
    2a28:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2a2d:	e9 f7 fe ff ff       	jmpq   2929 <init_driver+0xe7>
    2a32:	e8 e9 e7 ff ff       	callq  1220 <__stack_chk_fail@plt>

0000000000002a37 <driver_post>:
    2a37:	f3 0f 1e fa          	endbr64 
    2a3b:	53                   	push   %rbx
    2a3c:	4c 89 c3             	mov    %r8,%rbx
    2a3f:	85 c9                	test   %ecx,%ecx
    2a41:	75 17                	jne    2a5a <driver_post+0x23>
    2a43:	48 85 ff             	test   %rdi,%rdi
    2a46:	74 05                	je     2a4d <driver_post+0x16>
    2a48:	80 3f 00             	cmpb   $0x0,(%rdi)
    2a4b:	75 33                	jne    2a80 <driver_post+0x49>
    2a4d:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
    2a52:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
    2a56:	89 c8                	mov    %ecx,%eax
    2a58:	5b                   	pop    %rbx
    2a59:	c3                   	retq   
    2a5a:	48 8d 35 9b 0a 00 00 	lea    0xa9b(%rip),%rsi        # 34fc <array.3470+0x31c>
    2a61:	bf 01 00 00 00       	mov    $0x1,%edi
    2a66:	b8 00 00 00 00       	mov    $0x0,%eax
    2a6b:	e8 60 e8 ff ff       	callq  12d0 <__printf_chk@plt>
    2a70:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
    2a75:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
    2a79:	b8 00 00 00 00       	mov    $0x0,%eax
    2a7e:	eb d8                	jmp    2a58 <driver_post+0x21>
    2a80:	41 50                	push   %r8
    2a82:	52                   	push   %rdx
    2a83:	4c 8d 0d 89 0a 00 00 	lea    0xa89(%rip),%r9        # 3513 <array.3470+0x333>
    2a8a:	49 89 f0             	mov    %rsi,%r8
    2a8d:	48 89 f9             	mov    %rdi,%rcx
    2a90:	48 8d 15 84 0a 00 00 	lea    0xa84(%rip),%rdx        # 351b <array.3470+0x33b>
    2a97:	be 6e 3b 00 00       	mov    $0x3b6e,%esi
    2a9c:	48 8d 3d 41 0a 00 00 	lea    0xa41(%rip),%rdi        # 34e4 <array.3470+0x304>
    2aa3:	e8 3d f5 ff ff       	callq  1fe5 <submitr>
    2aa8:	48 83 c4 10          	add    $0x10,%rsp
    2aac:	eb aa                	jmp    2a58 <driver_post+0x21>
    2aae:	66 90                	xchg   %ax,%ax

0000000000002ab0 <__libc_csu_init>:
    2ab0:	f3 0f 1e fa          	endbr64 
    2ab4:	41 57                	push   %r15
    2ab6:	4c 8d 3d 3b 22 00 00 	lea    0x223b(%rip),%r15        # 4cf8 <__frame_dummy_init_array_entry>
    2abd:	41 56                	push   %r14
    2abf:	49 89 d6             	mov    %rdx,%r14
    2ac2:	41 55                	push   %r13
    2ac4:	49 89 f5             	mov    %rsi,%r13
    2ac7:	41 54                	push   %r12
    2ac9:	41 89 fc             	mov    %edi,%r12d
    2acc:	55                   	push   %rbp
    2acd:	48 8d 2d 2c 22 00 00 	lea    0x222c(%rip),%rbp        # 4d00 <__do_global_dtors_aux_fini_array_entry>
    2ad4:	53                   	push   %rbx
    2ad5:	4c 29 fd             	sub    %r15,%rbp
    2ad8:	48 83 ec 08          	sub    $0x8,%rsp
    2adc:	e8 1f e5 ff ff       	callq  1000 <_init>
    2ae1:	48 c1 fd 03          	sar    $0x3,%rbp
    2ae5:	74 1f                	je     2b06 <__libc_csu_init+0x56>
    2ae7:	31 db                	xor    %ebx,%ebx
    2ae9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    2af0:	4c 89 f2             	mov    %r14,%rdx
    2af3:	4c 89 ee             	mov    %r13,%rsi
    2af6:	44 89 e7             	mov    %r12d,%edi
    2af9:	41 ff 14 df          	callq  *(%r15,%rbx,8)
    2afd:	48 83 c3 01          	add    $0x1,%rbx
    2b01:	48 39 dd             	cmp    %rbx,%rbp
    2b04:	75 ea                	jne    2af0 <__libc_csu_init+0x40>
    2b06:	48 83 c4 08          	add    $0x8,%rsp
    2b0a:	5b                   	pop    %rbx
    2b0b:	5d                   	pop    %rbp
    2b0c:	41 5c                	pop    %r12
    2b0e:	41 5d                	pop    %r13
    2b10:	41 5e                	pop    %r14
    2b12:	41 5f                	pop    %r15
    2b14:	c3                   	retq   
    2b15:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
    2b1c:	00 00 00 00 

0000000000002b20 <__libc_csu_fini>:
    2b20:	f3 0f 1e fa          	endbr64 
    2b24:	c3                   	retq   

Disassembly of section .fini:

0000000000002b28 <_fini>:
    2b28:	f3 0f 1e fa          	endbr64 
    2b2c:	48 83 ec 08          	sub    $0x8,%rsp
    2b30:	48 83 c4 08          	add    $0x8,%rsp
    2b34:	c3                   	retq   
