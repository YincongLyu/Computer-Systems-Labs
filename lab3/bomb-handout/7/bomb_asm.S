
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
    1373:	4c 8d 05 d6 16 00 00 	lea    0x16d6(%rip),%r8        # 2a50 <__libc_csu_fini>
    137a:	48 8d 0d 5f 16 00 00 	lea    0x165f(%rip),%rcx        # 29e0 <__libc_csu_init>
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
    1483:	e8 6c 06 00 00       	callq  1af4 <initialize_bomb>
    1488:	48 8d 3d f9 1b 00 00 	lea    0x1bf9(%rip),%rdi        # 3088 <_IO_stdin_used+0x88>
    148f:	e8 6c fd ff ff       	callq  1200 <puts@plt>
    1494:	48 8d 3d 2d 1c 00 00 	lea    0x1c2d(%rip),%rdi        # 30c8 <_IO_stdin_used+0xc8>
    149b:	e8 60 fd ff ff       	callq  1200 <puts@plt>
    14a0:	e8 74 07 00 00       	callq  1c19 <read_line>
    14a5:	48 89 c7             	mov    %rax,%rdi
    14a8:	e8 fa 00 00 00       	callq  15a7 <phase_1>
    14ad:	e8 af 08 00 00       	callq  1d61 <phase_defused>
    14b2:	48 8d 3d 3f 1c 00 00 	lea    0x1c3f(%rip),%rdi        # 30f8 <_IO_stdin_used+0xf8>
    14b9:	e8 42 fd ff ff       	callq  1200 <puts@plt>
    14be:	e8 56 07 00 00       	callq  1c19 <read_line>
    14c3:	48 89 c7             	mov    %rax,%rdi
    14c6:	e8 00 01 00 00       	callq  15cb <phase_2>
    14cb:	e8 91 08 00 00       	callq  1d61 <phase_defused>
    14d0:	48 8d 3d 66 1b 00 00 	lea    0x1b66(%rip),%rdi        # 303d <_IO_stdin_used+0x3d>
    14d7:	e8 24 fd ff ff       	callq  1200 <puts@plt>
    14dc:	e8 38 07 00 00       	callq  1c19 <read_line>
    14e1:	48 89 c7             	mov    %rax,%rdi
    14e4:	e8 54 01 00 00       	callq  163d <phase_3>
    14e9:	e8 73 08 00 00       	callq  1d61 <phase_defused>
    14ee:	48 8d 3d 66 1b 00 00 	lea    0x1b66(%rip),%rdi        # 305b <_IO_stdin_used+0x5b>
    14f5:	e8 06 fd ff ff       	callq  1200 <puts@plt>
    14fa:	e8 1a 07 00 00       	callq  1c19 <read_line>
    14ff:	48 89 c7             	mov    %rax,%rdi
    1502:	e8 52 02 00 00       	callq  1759 <phase_4>
    1507:	e8 55 08 00 00       	callq  1d61 <phase_defused>
    150c:	48 8d 3d 15 1c 00 00 	lea    0x1c15(%rip),%rdi        # 3128 <_IO_stdin_used+0x128>
    1513:	e8 e8 fc ff ff       	callq  1200 <puts@plt>
    1518:	e8 fc 06 00 00       	callq  1c19 <read_line>
    151d:	48 89 c7             	mov    %rax,%rdi
    1520:	e8 ad 02 00 00       	callq  17d2 <phase_5>
    1525:	e8 37 08 00 00       	callq  1d61 <phase_defused>
    152a:	48 8d 3d 39 1b 00 00 	lea    0x1b39(%rip),%rdi        # 306a <_IO_stdin_used+0x6a>
    1531:	e8 ca fc ff ff       	callq  1200 <puts@plt>
    1536:	e8 de 06 00 00       	callq  1c19 <read_line>
    153b:	48 89 c7             	mov    %rax,%rdi
    153e:	e8 db 02 00 00       	callq  181e <phase_6>
    1543:	e8 19 08 00 00       	callq  1d61 <phase_defused>
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
    15b6:	e8 d9 04 00 00       	callq  1a94 <strings_not_equal>
    15bb:	85 c0                	test   %eax,%eax
    15bd:	75 05                	jne    15c4 <phase_1+0x1d>
    15bf:	48 83 c4 08          	add    $0x8,%rsp
    15c3:	c3                   	retq   
    15c4:	e8 df 05 00 00       	callq  1ba8 <explode_bomb>
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
    15e8:	e8 e7 05 00 00       	callq  1bd4 <read_six_numbers>
    15ed:	83 3c 24 00          	cmpl   $0x0,(%rsp)
    15f1:	78 0a                	js     15fd <phase_2+0x32>
    15f3:	48 89 e5             	mov    %rsp,%rbp
    15f6:	bb 01 00 00 00       	mov    $0x1,%ebx
    15fb:	eb 18                	jmp    1615 <phase_2+0x4a>
    15fd:	e8 a6 05 00 00       	callq  1ba8 <explode_bomb>
    1602:	eb ef                	jmp    15f3 <phase_2+0x28>
    1604:	e8 9f 05 00 00       	callq  1ba8 <explode_bomb>
    1609:	83 c3 01             	add    $0x1,%ebx
    160c:	48 83 c5 04          	add    $0x4,%rbp
    1610:	83 fb 06             	cmp    $0x6,%ebx
    1613:	74 0c                	je     1621 <phase_2+0x56>
    1615:	89 d8                	mov    %ebx,%eax
    1617:	03 45 00             	add    0x0(%rbp),%eax
    161a:	39 45 04             	cmp    %eax,0x4(%rbp)
    161d:	74 ea                	je     1609 <phase_2+0x3e>
    161f:	eb e3                	jmp    1604 <phase_2+0x39>
    1621:	48 8b 44 24 18       	mov    0x18(%rsp),%rax
    1626:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    162d:	00 00 
    162f:	75 07                	jne    1638 <phase_2+0x6d>
    1631:	48 83 c4 28          	add    $0x28,%rsp
    1635:	5b                   	pop    %rbx
    1636:	5d                   	pop    %rbp
    1637:	c3                   	retq   
    1638:	e8 e3 fb ff ff       	callq  1220 <__stack_chk_fail@plt>

000000000000163d <phase_3>:
    163d:	f3 0f 1e fa          	endbr64 
    1641:	48 83 ec 18          	sub    $0x18,%rsp
    1645:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    164c:	00 00 
    164e:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
    1653:	31 c0                	xor    %eax,%eax
    1655:	48 8d 4c 24 04       	lea    0x4(%rsp),%rcx
    165a:	48 89 e2             	mov    %rsp,%rdx
    165d:	48 8d 35 cb 1c 00 00 	lea    0x1ccb(%rip),%rsi        # 332f <array.3471+0x14f>
    1664:	e8 57 fc ff ff       	callq  12c0 <__isoc99_sscanf@plt>
    1669:	83 f8 01             	cmp    $0x1,%eax
    166c:	7e 1e                	jle    168c <phase_3+0x4f>
    166e:	83 3c 24 07          	cmpl   $0x7,(%rsp)
    1672:	0f 87 9a 00 00 00    	ja     1712 <phase_3+0xd5>
    1678:	8b 04 24             	mov    (%rsp),%eax
    167b:	48 8d 15 3e 1b 00 00 	lea    0x1b3e(%rip),%rdx        # 31c0 <_IO_stdin_used+0x1c0>
    1682:	48 63 04 82          	movslq (%rdx,%rax,4),%rax
    1686:	48 01 d0             	add    %rdx,%rax
    1689:	3e ff e0             	notrack jmpq *%rax
    168c:	e8 17 05 00 00       	callq  1ba8 <explode_bomb>
    1691:	eb db                	jmp    166e <phase_3+0x31>
    1693:	b8 e9 01 00 00       	mov    $0x1e9,%eax
    1698:	2d 54 03 00 00       	sub    $0x354,%eax
    169d:	05 f8 02 00 00       	add    $0x2f8,%eax
    16a2:	2d df 03 00 00       	sub    $0x3df,%eax
    16a7:	05 df 03 00 00       	add    $0x3df,%eax
    16ac:	2d df 03 00 00       	sub    $0x3df,%eax
    16b1:	05 df 03 00 00       	add    $0x3df,%eax
    16b6:	2d df 03 00 00       	sub    $0x3df,%eax
    16bb:	83 3c 24 05          	cmpl   $0x5,(%rsp)
    16bf:	7f 06                	jg     16c7 <phase_3+0x8a>
    16c1:	39 44 24 04          	cmp    %eax,0x4(%rsp)
    16c5:	74 05                	je     16cc <phase_3+0x8f>
    16c7:	e8 dc 04 00 00       	callq  1ba8 <explode_bomb>
    16cc:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
    16d1:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    16d8:	00 00 
    16da:	75 42                	jne    171e <phase_3+0xe1>
    16dc:	48 83 c4 18          	add    $0x18,%rsp
    16e0:	c3                   	retq   
    16e1:	b8 00 00 00 00       	mov    $0x0,%eax
    16e6:	eb b0                	jmp    1698 <phase_3+0x5b>
    16e8:	b8 00 00 00 00       	mov    $0x0,%eax
    16ed:	eb ae                	jmp    169d <phase_3+0x60>
    16ef:	b8 00 00 00 00       	mov    $0x0,%eax
    16f4:	eb ac                	jmp    16a2 <phase_3+0x65>
    16f6:	b8 00 00 00 00       	mov    $0x0,%eax
    16fb:	eb aa                	jmp    16a7 <phase_3+0x6a>
    16fd:	b8 00 00 00 00       	mov    $0x0,%eax
    1702:	eb a8                	jmp    16ac <phase_3+0x6f>
    1704:	b8 00 00 00 00       	mov    $0x0,%eax
    1709:	eb a6                	jmp    16b1 <phase_3+0x74>
    170b:	b8 00 00 00 00       	mov    $0x0,%eax
    1710:	eb a4                	jmp    16b6 <phase_3+0x79>
    1712:	e8 91 04 00 00       	callq  1ba8 <explode_bomb>
    1717:	b8 00 00 00 00       	mov    $0x0,%eax
    171c:	eb 9d                	jmp    16bb <phase_3+0x7e>
    171e:	e8 fd fa ff ff       	callq  1220 <__stack_chk_fail@plt>

0000000000001723 <func4>:
    1723:	f3 0f 1e fa          	endbr64 
    1727:	53                   	push   %rbx
    1728:	89 d0                	mov    %edx,%eax
    172a:	29 f0                	sub    %esi,%eax
    172c:	89 c3                	mov    %eax,%ebx
    172e:	c1 eb 1f             	shr    $0x1f,%ebx
    1731:	01 c3                	add    %eax,%ebx
    1733:	d1 fb                	sar    %ebx
    1735:	01 f3                	add    %esi,%ebx
    1737:	39 fb                	cmp    %edi,%ebx
    1739:	7f 06                	jg     1741 <func4+0x1e>
    173b:	7c 10                	jl     174d <func4+0x2a>
    173d:	89 d8                	mov    %ebx,%eax
    173f:	5b                   	pop    %rbx
    1740:	c3                   	retq   
    1741:	8d 53 ff             	lea    -0x1(%rbx),%edx
    1744:	e8 da ff ff ff       	callq  1723 <func4>
    1749:	01 c3                	add    %eax,%ebx
    174b:	eb f0                	jmp    173d <func4+0x1a>
    174d:	8d 73 01             	lea    0x1(%rbx),%esi
    1750:	e8 ce ff ff ff       	callq  1723 <func4>
    1755:	01 c3                	add    %eax,%ebx
    1757:	eb e4                	jmp    173d <func4+0x1a>

0000000000001759 <phase_4>:
    1759:	f3 0f 1e fa          	endbr64 
    175d:	48 83 ec 18          	sub    $0x18,%rsp
    1761:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1768:	00 00 
    176a:	48 89 44 24 08       	mov    %rax,0x8(%rsp)
    176f:	31 c0                	xor    %eax,%eax
    1771:	48 8d 4c 24 04       	lea    0x4(%rsp),%rcx
    1776:	48 89 e2             	mov    %rsp,%rdx
    1779:	48 8d 35 af 1b 00 00 	lea    0x1baf(%rip),%rsi        # 332f <array.3471+0x14f>
    1780:	e8 3b fb ff ff       	callq  12c0 <__isoc99_sscanf@plt>
    1785:	83 f8 02             	cmp    $0x2,%eax
    1788:	75 06                	jne    1790 <phase_4+0x37>
    178a:	83 3c 24 0e          	cmpl   $0xe,(%rsp)
    178e:	76 05                	jbe    1795 <phase_4+0x3c>
    1790:	e8 13 04 00 00       	callq  1ba8 <explode_bomb>
    1795:	ba 0e 00 00 00       	mov    $0xe,%edx
    179a:	be 00 00 00 00       	mov    $0x0,%esi
    179f:	8b 3c 24             	mov    (%rsp),%edi
    17a2:	e8 7c ff ff ff       	callq  1723 <func4>
    17a7:	83 f8 12             	cmp    $0x12,%eax
    17aa:	75 07                	jne    17b3 <phase_4+0x5a>
    17ac:	83 7c 24 04 12       	cmpl   $0x12,0x4(%rsp)
    17b1:	74 05                	je     17b8 <phase_4+0x5f>
    17b3:	e8 f0 03 00 00       	callq  1ba8 <explode_bomb>
    17b8:	48 8b 44 24 08       	mov    0x8(%rsp),%rax
    17bd:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    17c4:	00 00 
    17c6:	75 05                	jne    17cd <phase_4+0x74>
    17c8:	48 83 c4 18          	add    $0x18,%rsp
    17cc:	c3                   	retq   
    17cd:	e8 4e fa ff ff       	callq  1220 <__stack_chk_fail@plt>

00000000000017d2 <phase_5>:
    17d2:	f3 0f 1e fa          	endbr64 
    17d6:	53                   	push   %rbx
    17d7:	48 89 fb             	mov    %rdi,%rbx
    17da:	e8 94 02 00 00       	callq  1a73 <string_length>
    17df:	83 f8 06             	cmp    $0x6,%eax
    17e2:	75 2c                	jne    1810 <phase_5+0x3e>
    17e4:	48 89 d8             	mov    %rbx,%rax
    17e7:	48 8d 7b 06          	lea    0x6(%rbx),%rdi
    17eb:	b9 00 00 00 00       	mov    $0x0,%ecx
    17f0:	48 8d 35 e9 19 00 00 	lea    0x19e9(%rip),%rsi        # 31e0 <array.3471>
    17f7:	0f b6 10             	movzbl (%rax),%edx
    17fa:	83 e2 0f             	and    $0xf,%edx
    17fd:	03 0c 96             	add    (%rsi,%rdx,4),%ecx
    1800:	48 83 c0 01          	add    $0x1,%rax
    1804:	48 39 f8             	cmp    %rdi,%rax
    1807:	75 ee                	jne    17f7 <phase_5+0x25>
    1809:	83 f9 25             	cmp    $0x25,%ecx
    180c:	75 09                	jne    1817 <phase_5+0x45>
    180e:	5b                   	pop    %rbx
    180f:	c3                   	retq   
    1810:	e8 93 03 00 00       	callq  1ba8 <explode_bomb>
    1815:	eb cd                	jmp    17e4 <phase_5+0x12>
    1817:	e8 8c 03 00 00       	callq  1ba8 <explode_bomb>
    181c:	eb f0                	jmp    180e <phase_5+0x3c>

000000000000181e <phase_6>:
    181e:	f3 0f 1e fa          	endbr64 
    1822:	41 56                	push   %r14
    1824:	41 55                	push   %r13
    1826:	41 54                	push   %r12
    1828:	55                   	push   %rbp
    1829:	53                   	push   %rbx
    182a:	48 83 ec 60          	sub    $0x60,%rsp
    182e:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1835:	00 00 
    1837:	48 89 44 24 58       	mov    %rax,0x58(%rsp)
    183c:	31 c0                	xor    %eax,%eax
    183e:	49 89 e5             	mov    %rsp,%r13
    1841:	4c 89 ee             	mov    %r13,%rsi
    1844:	e8 8b 03 00 00       	callq  1bd4 <read_six_numbers>
    1849:	41 be 01 00 00 00    	mov    $0x1,%r14d
    184f:	49 89 e4             	mov    %rsp,%r12
    1852:	eb 28                	jmp    187c <phase_6+0x5e>
    1854:	e8 4f 03 00 00       	callq  1ba8 <explode_bomb>
    1859:	eb 30                	jmp    188b <phase_6+0x6d>
    185b:	48 83 c3 01          	add    $0x1,%rbx
    185f:	83 fb 05             	cmp    $0x5,%ebx
    1862:	7f 10                	jg     1874 <phase_6+0x56>
    1864:	41 8b 04 9c          	mov    (%r12,%rbx,4),%eax
    1868:	39 45 00             	cmp    %eax,0x0(%rbp)
    186b:	75 ee                	jne    185b <phase_6+0x3d>
    186d:	e8 36 03 00 00       	callq  1ba8 <explode_bomb>
    1872:	eb e7                	jmp    185b <phase_6+0x3d>
    1874:	49 83 c6 01          	add    $0x1,%r14
    1878:	49 83 c5 04          	add    $0x4,%r13
    187c:	4c 89 ed             	mov    %r13,%rbp
    187f:	41 8b 45 00          	mov    0x0(%r13),%eax
    1883:	83 e8 01             	sub    $0x1,%eax
    1886:	83 f8 05             	cmp    $0x5,%eax
    1889:	77 c9                	ja     1854 <phase_6+0x36>
    188b:	41 83 fe 05          	cmp    $0x5,%r14d
    188f:	7f 05                	jg     1896 <phase_6+0x78>
    1891:	4c 89 f3             	mov    %r14,%rbx
    1894:	eb ce                	jmp    1864 <phase_6+0x46>
    1896:	be 00 00 00 00       	mov    $0x0,%esi
    189b:	8b 0c b4             	mov    (%rsp,%rsi,4),%ecx
    189e:	b8 01 00 00 00       	mov    $0x1,%eax
    18a3:	48 8d 15 66 39 00 00 	lea    0x3966(%rip),%rdx        # 5210 <node1>
    18aa:	83 f9 01             	cmp    $0x1,%ecx
    18ad:	7e 0b                	jle    18ba <phase_6+0x9c>
    18af:	48 8b 52 08          	mov    0x8(%rdx),%rdx
    18b3:	83 c0 01             	add    $0x1,%eax
    18b6:	39 c8                	cmp    %ecx,%eax
    18b8:	75 f5                	jne    18af <phase_6+0x91>
    18ba:	48 89 54 f4 20       	mov    %rdx,0x20(%rsp,%rsi,8)
    18bf:	48 83 c6 01          	add    $0x1,%rsi
    18c3:	48 83 fe 06          	cmp    $0x6,%rsi
    18c7:	75 d2                	jne    189b <phase_6+0x7d>
    18c9:	48 8b 5c 24 20       	mov    0x20(%rsp),%rbx
    18ce:	48 8b 44 24 28       	mov    0x28(%rsp),%rax
    18d3:	48 89 43 08          	mov    %rax,0x8(%rbx)
    18d7:	48 8b 54 24 30       	mov    0x30(%rsp),%rdx
    18dc:	48 89 50 08          	mov    %rdx,0x8(%rax)
    18e0:	48 8b 44 24 38       	mov    0x38(%rsp),%rax
    18e5:	48 89 42 08          	mov    %rax,0x8(%rdx)
    18e9:	48 8b 54 24 40       	mov    0x40(%rsp),%rdx
    18ee:	48 89 50 08          	mov    %rdx,0x8(%rax)
    18f2:	48 8b 44 24 48       	mov    0x48(%rsp),%rax
    18f7:	48 89 42 08          	mov    %rax,0x8(%rdx)
    18fb:	48 c7 40 08 00 00 00 	movq   $0x0,0x8(%rax)
    1902:	00 
    1903:	bd 05 00 00 00       	mov    $0x5,%ebp
    1908:	eb 09                	jmp    1913 <phase_6+0xf5>
    190a:	48 8b 5b 08          	mov    0x8(%rbx),%rbx
    190e:	83 ed 01             	sub    $0x1,%ebp
    1911:	74 11                	je     1924 <phase_6+0x106>
    1913:	48 8b 43 08          	mov    0x8(%rbx),%rax
    1917:	8b 00                	mov    (%rax),%eax
    1919:	39 03                	cmp    %eax,(%rbx)
    191b:	7d ed                	jge    190a <phase_6+0xec>
    191d:	e8 86 02 00 00       	callq  1ba8 <explode_bomb>
    1922:	eb e6                	jmp    190a <phase_6+0xec>
    1924:	48 8b 44 24 58       	mov    0x58(%rsp),%rax
    1929:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    1930:	00 00 
    1932:	75 0d                	jne    1941 <phase_6+0x123>
    1934:	48 83 c4 60          	add    $0x60,%rsp
    1938:	5b                   	pop    %rbx
    1939:	5d                   	pop    %rbp
    193a:	41 5c                	pop    %r12
    193c:	41 5d                	pop    %r13
    193e:	41 5e                	pop    %r14
    1940:	c3                   	retq   
    1941:	e8 da f8 ff ff       	callq  1220 <__stack_chk_fail@plt>

0000000000001946 <fun7>:
    1946:	f3 0f 1e fa          	endbr64 
    194a:	48 85 ff             	test   %rdi,%rdi
    194d:	74 32                	je     1981 <fun7+0x3b>
    194f:	48 83 ec 08          	sub    $0x8,%rsp
    1953:	8b 17                	mov    (%rdi),%edx
    1955:	39 f2                	cmp    %esi,%edx
    1957:	7f 0c                	jg     1965 <fun7+0x1f>
    1959:	b8 00 00 00 00       	mov    $0x0,%eax
    195e:	75 12                	jne    1972 <fun7+0x2c>
    1960:	48 83 c4 08          	add    $0x8,%rsp
    1964:	c3                   	retq   
    1965:	48 8b 7f 08          	mov    0x8(%rdi),%rdi
    1969:	e8 d8 ff ff ff       	callq  1946 <fun7>
    196e:	01 c0                	add    %eax,%eax
    1970:	eb ee                	jmp    1960 <fun7+0x1a>
    1972:	48 8b 7f 10          	mov    0x10(%rdi),%rdi
    1976:	e8 cb ff ff ff       	callq  1946 <fun7>
    197b:	8d 44 00 01          	lea    0x1(%rax,%rax,1),%eax
    197f:	eb df                	jmp    1960 <fun7+0x1a>
    1981:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    1986:	c3                   	retq   

0000000000001987 <secret_phase>:
    1987:	f3 0f 1e fa          	endbr64 
    198b:	53                   	push   %rbx
    198c:	e8 88 02 00 00       	callq  1c19 <read_line>
    1991:	48 89 c7             	mov    %rax,%rdi
    1994:	ba 0a 00 00 00       	mov    $0xa,%edx
    1999:	be 00 00 00 00       	mov    $0x0,%esi
    199e:	e8 fd f8 ff ff       	callq  12a0 <strtol@plt>
    19a3:	48 89 c3             	mov    %rax,%rbx
    19a6:	8d 40 ff             	lea    -0x1(%rax),%eax
    19a9:	3d e8 03 00 00       	cmp    $0x3e8,%eax
    19ae:	77 26                	ja     19d6 <secret_phase+0x4f>
    19b0:	89 de                	mov    %ebx,%esi
    19b2:	48 8d 3d 77 37 00 00 	lea    0x3777(%rip),%rdi        # 5130 <n1>
    19b9:	e8 88 ff ff ff       	callq  1946 <fun7>
    19be:	83 f8 02             	cmp    $0x2,%eax
    19c1:	75 1a                	jne    19dd <secret_phase+0x56>
    19c3:	48 8d 3d b6 17 00 00 	lea    0x17b6(%rip),%rdi        # 3180 <_IO_stdin_used+0x180>
    19ca:	e8 31 f8 ff ff       	callq  1200 <puts@plt>
    19cf:	e8 8d 03 00 00       	callq  1d61 <phase_defused>
    19d4:	5b                   	pop    %rbx
    19d5:	c3                   	retq   
    19d6:	e8 cd 01 00 00       	callq  1ba8 <explode_bomb>
    19db:	eb d3                	jmp    19b0 <secret_phase+0x29>
    19dd:	e8 c6 01 00 00       	callq  1ba8 <explode_bomb>
    19e2:	eb df                	jmp    19c3 <secret_phase+0x3c>

00000000000019e4 <sig_handler>:
    19e4:	f3 0f 1e fa          	endbr64 
    19e8:	50                   	push   %rax
    19e9:	58                   	pop    %rax
    19ea:	48 83 ec 08          	sub    $0x8,%rsp
    19ee:	48 8d 3d 2b 18 00 00 	lea    0x182b(%rip),%rdi        # 3220 <array.3471+0x40>
    19f5:	e8 06 f8 ff ff       	callq  1200 <puts@plt>
    19fa:	bf 03 00 00 00       	mov    $0x3,%edi
    19ff:	e8 1c f9 ff ff       	callq  1320 <sleep@plt>
    1a04:	48 8d 35 d7 18 00 00 	lea    0x18d7(%rip),%rsi        # 32e2 <array.3471+0x102>
    1a0b:	bf 01 00 00 00       	mov    $0x1,%edi
    1a10:	b8 00 00 00 00       	mov    $0x0,%eax
    1a15:	e8 b6 f8 ff ff       	callq  12d0 <__printf_chk@plt>
    1a1a:	48 8b 3d 3f 3c 00 00 	mov    0x3c3f(%rip),%rdi        # 5660 <stdout@@GLIBC_2.2.5>
    1a21:	e8 8a f8 ff ff       	callq  12b0 <fflush@plt>
    1a26:	bf 01 00 00 00       	mov    $0x1,%edi
    1a2b:	e8 f0 f8 ff ff       	callq  1320 <sleep@plt>
    1a30:	48 8d 3d b3 18 00 00 	lea    0x18b3(%rip),%rdi        # 32ea <array.3471+0x10a>
    1a37:	e8 c4 f7 ff ff       	callq  1200 <puts@plt>
    1a3c:	bf 10 00 00 00       	mov    $0x10,%edi
    1a41:	e8 aa f8 ff ff       	callq  12f0 <exit@plt>

0000000000001a46 <invalid_phase>:
    1a46:	f3 0f 1e fa          	endbr64 
    1a4a:	50                   	push   %rax
    1a4b:	58                   	pop    %rax
    1a4c:	48 83 ec 08          	sub    $0x8,%rsp
    1a50:	48 89 fa             	mov    %rdi,%rdx
    1a53:	48 8d 35 98 18 00 00 	lea    0x1898(%rip),%rsi        # 32f2 <array.3471+0x112>
    1a5a:	bf 01 00 00 00       	mov    $0x1,%edi
    1a5f:	b8 00 00 00 00       	mov    $0x0,%eax
    1a64:	e8 67 f8 ff ff       	callq  12d0 <__printf_chk@plt>
    1a69:	bf 08 00 00 00       	mov    $0x8,%edi
    1a6e:	e8 7d f8 ff ff       	callq  12f0 <exit@plt>

0000000000001a73 <string_length>:
    1a73:	f3 0f 1e fa          	endbr64 
    1a77:	80 3f 00             	cmpb   $0x0,(%rdi)
    1a7a:	74 12                	je     1a8e <string_length+0x1b>
    1a7c:	b8 00 00 00 00       	mov    $0x0,%eax
    1a81:	48 83 c7 01          	add    $0x1,%rdi
    1a85:	83 c0 01             	add    $0x1,%eax
    1a88:	80 3f 00             	cmpb   $0x0,(%rdi)
    1a8b:	75 f4                	jne    1a81 <string_length+0xe>
    1a8d:	c3                   	retq   
    1a8e:	b8 00 00 00 00       	mov    $0x0,%eax
    1a93:	c3                   	retq   

0000000000001a94 <strings_not_equal>:
    1a94:	f3 0f 1e fa          	endbr64 
    1a98:	41 54                	push   %r12
    1a9a:	55                   	push   %rbp
    1a9b:	53                   	push   %rbx
    1a9c:	48 89 fb             	mov    %rdi,%rbx
    1a9f:	48 89 f5             	mov    %rsi,%rbp
    1aa2:	e8 cc ff ff ff       	callq  1a73 <string_length>
    1aa7:	41 89 c4             	mov    %eax,%r12d
    1aaa:	48 89 ef             	mov    %rbp,%rdi
    1aad:	e8 c1 ff ff ff       	callq  1a73 <string_length>
    1ab2:	89 c2                	mov    %eax,%edx
    1ab4:	b8 01 00 00 00       	mov    $0x1,%eax
    1ab9:	41 39 d4             	cmp    %edx,%r12d
    1abc:	75 31                	jne    1aef <strings_not_equal+0x5b>
    1abe:	0f b6 13             	movzbl (%rbx),%edx
    1ac1:	84 d2                	test   %dl,%dl
    1ac3:	74 1e                	je     1ae3 <strings_not_equal+0x4f>
    1ac5:	b8 00 00 00 00       	mov    $0x0,%eax
    1aca:	38 54 05 00          	cmp    %dl,0x0(%rbp,%rax,1)
    1ace:	75 1a                	jne    1aea <strings_not_equal+0x56>
    1ad0:	48 83 c0 01          	add    $0x1,%rax
    1ad4:	0f b6 14 03          	movzbl (%rbx,%rax,1),%edx
    1ad8:	84 d2                	test   %dl,%dl
    1ada:	75 ee                	jne    1aca <strings_not_equal+0x36>
    1adc:	b8 00 00 00 00       	mov    $0x0,%eax
    1ae1:	eb 0c                	jmp    1aef <strings_not_equal+0x5b>
    1ae3:	b8 00 00 00 00       	mov    $0x0,%eax
    1ae8:	eb 05                	jmp    1aef <strings_not_equal+0x5b>
    1aea:	b8 01 00 00 00       	mov    $0x1,%eax
    1aef:	5b                   	pop    %rbx
    1af0:	5d                   	pop    %rbp
    1af1:	41 5c                	pop    %r12
    1af3:	c3                   	retq   

0000000000001af4 <initialize_bomb>:
    1af4:	f3 0f 1e fa          	endbr64 
    1af8:	48 83 ec 08          	sub    $0x8,%rsp
    1afc:	48 8d 35 e1 fe ff ff 	lea    -0x11f(%rip),%rsi        # 19e4 <sig_handler>
    1b03:	bf 02 00 00 00       	mov    $0x2,%edi
    1b08:	e8 63 f7 ff ff       	callq  1270 <signal@plt>
    1b0d:	48 83 c4 08          	add    $0x8,%rsp
    1b11:	c3                   	retq   

0000000000001b12 <initialize_bomb_solve>:
    1b12:	f3 0f 1e fa          	endbr64 
    1b16:	c3                   	retq   

0000000000001b17 <blank_line>:
    1b17:	f3 0f 1e fa          	endbr64 
    1b1b:	55                   	push   %rbp
    1b1c:	53                   	push   %rbx
    1b1d:	48 83 ec 08          	sub    $0x8,%rsp
    1b21:	48 89 fd             	mov    %rdi,%rbp
    1b24:	0f b6 5d 00          	movzbl 0x0(%rbp),%ebx
    1b28:	84 db                	test   %bl,%bl
    1b2a:	74 1e                	je     1b4a <blank_line+0x33>
    1b2c:	e8 ff f7 ff ff       	callq  1330 <__ctype_b_loc@plt>
    1b31:	48 83 c5 01          	add    $0x1,%rbp
    1b35:	48 0f be db          	movsbq %bl,%rbx
    1b39:	48 8b 00             	mov    (%rax),%rax
    1b3c:	f6 44 58 01 20       	testb  $0x20,0x1(%rax,%rbx,2)
    1b41:	75 e1                	jne    1b24 <blank_line+0xd>
    1b43:	b8 00 00 00 00       	mov    $0x0,%eax
    1b48:	eb 05                	jmp    1b4f <blank_line+0x38>
    1b4a:	b8 01 00 00 00       	mov    $0x1,%eax
    1b4f:	48 83 c4 08          	add    $0x8,%rsp
    1b53:	5b                   	pop    %rbx
    1b54:	5d                   	pop    %rbp
    1b55:	c3                   	retq   

0000000000001b56 <skip>:
    1b56:	f3 0f 1e fa          	endbr64 
    1b5a:	55                   	push   %rbp
    1b5b:	53                   	push   %rbx
    1b5c:	48 83 ec 08          	sub    $0x8,%rsp
    1b60:	48 8d 2d 39 3b 00 00 	lea    0x3b39(%rip),%rbp        # 56a0 <input_strings>
    1b67:	48 63 05 1e 3b 00 00 	movslq 0x3b1e(%rip),%rax        # 568c <num_input_strings>
    1b6e:	48 8d 3c 80          	lea    (%rax,%rax,4),%rdi
    1b72:	48 c1 e7 04          	shl    $0x4,%rdi
    1b76:	48 01 ef             	add    %rbp,%rdi
    1b79:	48 8b 15 10 3b 00 00 	mov    0x3b10(%rip),%rdx        # 5690 <infile>
    1b80:	be 50 00 00 00       	mov    $0x50,%esi
    1b85:	e8 d6 f6 ff ff       	callq  1260 <fgets@plt>
    1b8a:	48 89 c3             	mov    %rax,%rbx
    1b8d:	48 85 c0             	test   %rax,%rax
    1b90:	74 0c                	je     1b9e <skip+0x48>
    1b92:	48 89 c7             	mov    %rax,%rdi
    1b95:	e8 7d ff ff ff       	callq  1b17 <blank_line>
    1b9a:	85 c0                	test   %eax,%eax
    1b9c:	75 c9                	jne    1b67 <skip+0x11>
    1b9e:	48 89 d8             	mov    %rbx,%rax
    1ba1:	48 83 c4 08          	add    $0x8,%rsp
    1ba5:	5b                   	pop    %rbx
    1ba6:	5d                   	pop    %rbp
    1ba7:	c3                   	retq   

0000000000001ba8 <explode_bomb>:
    1ba8:	f3 0f 1e fa          	endbr64 
    1bac:	50                   	push   %rax
    1bad:	58                   	pop    %rax
    1bae:	48 83 ec 08          	sub    $0x8,%rsp
    1bb2:	48 8d 3d 4a 17 00 00 	lea    0x174a(%rip),%rdi        # 3303 <array.3471+0x123>
    1bb9:	e8 42 f6 ff ff       	callq  1200 <puts@plt>
    1bbe:	48 8d 3d 47 17 00 00 	lea    0x1747(%rip),%rdi        # 330c <array.3471+0x12c>
    1bc5:	e8 36 f6 ff ff       	callq  1200 <puts@plt>
    1bca:	bf 08 00 00 00       	mov    $0x8,%edi
    1bcf:	e8 1c f7 ff ff       	callq  12f0 <exit@plt>

0000000000001bd4 <read_six_numbers>:
    1bd4:	f3 0f 1e fa          	endbr64 
    1bd8:	48 83 ec 08          	sub    $0x8,%rsp
    1bdc:	48 89 f2             	mov    %rsi,%rdx
    1bdf:	48 8d 4e 04          	lea    0x4(%rsi),%rcx
    1be3:	48 8d 46 14          	lea    0x14(%rsi),%rax
    1be7:	50                   	push   %rax
    1be8:	48 8d 46 10          	lea    0x10(%rsi),%rax
    1bec:	50                   	push   %rax
    1bed:	4c 8d 4e 0c          	lea    0xc(%rsi),%r9
    1bf1:	4c 8d 46 08          	lea    0x8(%rsi),%r8
    1bf5:	48 8d 35 27 17 00 00 	lea    0x1727(%rip),%rsi        # 3323 <array.3471+0x143>
    1bfc:	b8 00 00 00 00       	mov    $0x0,%eax
    1c01:	e8 ba f6 ff ff       	callq  12c0 <__isoc99_sscanf@plt>
    1c06:	48 83 c4 10          	add    $0x10,%rsp
    1c0a:	83 f8 05             	cmp    $0x5,%eax
    1c0d:	7e 05                	jle    1c14 <read_six_numbers+0x40>
    1c0f:	48 83 c4 08          	add    $0x8,%rsp
    1c13:	c3                   	retq   
    1c14:	e8 8f ff ff ff       	callq  1ba8 <explode_bomb>

0000000000001c19 <read_line>:
    1c19:	f3 0f 1e fa          	endbr64 
    1c1d:	48 83 ec 08          	sub    $0x8,%rsp
    1c21:	b8 00 00 00 00       	mov    $0x0,%eax
    1c26:	e8 2b ff ff ff       	callq  1b56 <skip>
    1c2b:	48 85 c0             	test   %rax,%rax
    1c2e:	74 6f                	je     1c9f <read_line+0x86>
    1c30:	8b 35 56 3a 00 00    	mov    0x3a56(%rip),%esi        # 568c <num_input_strings>
    1c36:	48 63 c6             	movslq %esi,%rax
    1c39:	48 8d 14 80          	lea    (%rax,%rax,4),%rdx
    1c3d:	48 c1 e2 04          	shl    $0x4,%rdx
    1c41:	48 8d 05 58 3a 00 00 	lea    0x3a58(%rip),%rax        # 56a0 <input_strings>
    1c48:	48 01 c2             	add    %rax,%rdx
    1c4b:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
    1c52:	b8 00 00 00 00       	mov    $0x0,%eax
    1c57:	48 89 d7             	mov    %rdx,%rdi
    1c5a:	f2 ae                	repnz scas %es:(%rdi),%al
    1c5c:	48 f7 d1             	not    %rcx
    1c5f:	48 83 e9 01          	sub    $0x1,%rcx
    1c63:	83 f9 4e             	cmp    $0x4e,%ecx
    1c66:	0f 8f ab 00 00 00    	jg     1d17 <read_line+0xfe>
    1c6c:	83 e9 01             	sub    $0x1,%ecx
    1c6f:	48 63 c9             	movslq %ecx,%rcx
    1c72:	48 63 c6             	movslq %esi,%rax
    1c75:	48 8d 04 80          	lea    (%rax,%rax,4),%rax
    1c79:	48 c1 e0 04          	shl    $0x4,%rax
    1c7d:	48 89 c7             	mov    %rax,%rdi
    1c80:	48 8d 05 19 3a 00 00 	lea    0x3a19(%rip),%rax        # 56a0 <input_strings>
    1c87:	48 01 f8             	add    %rdi,%rax
    1c8a:	c6 04 08 00          	movb   $0x0,(%rax,%rcx,1)
    1c8e:	83 c6 01             	add    $0x1,%esi
    1c91:	89 35 f5 39 00 00    	mov    %esi,0x39f5(%rip)        # 568c <num_input_strings>
    1c97:	48 89 d0             	mov    %rdx,%rax
    1c9a:	48 83 c4 08          	add    $0x8,%rsp
    1c9e:	c3                   	retq   
    1c9f:	48 8b 05 ca 39 00 00 	mov    0x39ca(%rip),%rax        # 5670 <stdin@@GLIBC_2.2.5>
    1ca6:	48 39 05 e3 39 00 00 	cmp    %rax,0x39e3(%rip)        # 5690 <infile>
    1cad:	74 1b                	je     1cca <read_line+0xb1>
    1caf:	48 8d 3d 9d 16 00 00 	lea    0x169d(%rip),%rdi        # 3353 <array.3471+0x173>
    1cb6:	e8 15 f5 ff ff       	callq  11d0 <getenv@plt>
    1cbb:	48 85 c0             	test   %rax,%rax
    1cbe:	74 20                	je     1ce0 <read_line+0xc7>
    1cc0:	bf 00 00 00 00       	mov    $0x0,%edi
    1cc5:	e8 26 f6 ff ff       	callq  12f0 <exit@plt>
    1cca:	48 8d 3d 64 16 00 00 	lea    0x1664(%rip),%rdi        # 3335 <array.3471+0x155>
    1cd1:	e8 2a f5 ff ff       	callq  1200 <puts@plt>
    1cd6:	bf 08 00 00 00       	mov    $0x8,%edi
    1cdb:	e8 10 f6 ff ff       	callq  12f0 <exit@plt>
    1ce0:	48 8b 05 89 39 00 00 	mov    0x3989(%rip),%rax        # 5670 <stdin@@GLIBC_2.2.5>
    1ce7:	48 89 05 a2 39 00 00 	mov    %rax,0x39a2(%rip)        # 5690 <infile>
    1cee:	b8 00 00 00 00       	mov    $0x0,%eax
    1cf3:	e8 5e fe ff ff       	callq  1b56 <skip>
    1cf8:	48 85 c0             	test   %rax,%rax
    1cfb:	0f 85 2f ff ff ff    	jne    1c30 <read_line+0x17>
    1d01:	48 8d 3d 2d 16 00 00 	lea    0x162d(%rip),%rdi        # 3335 <array.3471+0x155>
    1d08:	e8 f3 f4 ff ff       	callq  1200 <puts@plt>
    1d0d:	bf 00 00 00 00       	mov    $0x0,%edi
    1d12:	e8 d9 f5 ff ff       	callq  12f0 <exit@plt>
    1d17:	48 8d 3d 40 16 00 00 	lea    0x1640(%rip),%rdi        # 335e <array.3471+0x17e>
    1d1e:	e8 dd f4 ff ff       	callq  1200 <puts@plt>
    1d23:	8b 05 63 39 00 00    	mov    0x3963(%rip),%eax        # 568c <num_input_strings>
    1d29:	8d 50 01             	lea    0x1(%rax),%edx
    1d2c:	89 15 5a 39 00 00    	mov    %edx,0x395a(%rip)        # 568c <num_input_strings>
    1d32:	48 98                	cltq   
    1d34:	48 6b c0 50          	imul   $0x50,%rax,%rax
    1d38:	48 8d 15 61 39 00 00 	lea    0x3961(%rip),%rdx        # 56a0 <input_strings>
    1d3f:	48 be 2a 2a 2a 74 72 	movabs $0x636e7572742a2a2a,%rsi
    1d46:	75 6e 63 
    1d49:	48 bf 61 74 65 64 2a 	movabs $0x2a2a2a64657461,%rdi
    1d50:	2a 2a 00 
    1d53:	48 89 34 02          	mov    %rsi,(%rdx,%rax,1)
    1d57:	48 89 7c 02 08       	mov    %rdi,0x8(%rdx,%rax,1)
    1d5c:	e8 47 fe ff ff       	callq  1ba8 <explode_bomb>

0000000000001d61 <phase_defused>:
    1d61:	f3 0f 1e fa          	endbr64 
    1d65:	48 83 ec 78          	sub    $0x78,%rsp
    1d69:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1d70:	00 00 
    1d72:	48 89 44 24 68       	mov    %rax,0x68(%rsp)
    1d77:	31 c0                	xor    %eax,%eax
    1d79:	83 3d 0c 39 00 00 06 	cmpl   $0x6,0x390c(%rip)        # 568c <num_input_strings>
    1d80:	74 15                	je     1d97 <phase_defused+0x36>
    1d82:	48 8b 44 24 68       	mov    0x68(%rsp),%rax
    1d87:	64 48 33 04 25 28 00 	xor    %fs:0x28,%rax
    1d8e:	00 00 
    1d90:	75 73                	jne    1e05 <phase_defused+0xa4>
    1d92:	48 83 c4 78          	add    $0x78,%rsp
    1d96:	c3                   	retq   
    1d97:	48 8d 4c 24 0c       	lea    0xc(%rsp),%rcx
    1d9c:	48 8d 54 24 08       	lea    0x8(%rsp),%rdx
    1da1:	4c 8d 44 24 10       	lea    0x10(%rsp),%r8
    1da6:	48 8d 35 cc 15 00 00 	lea    0x15cc(%rip),%rsi        # 3379 <array.3471+0x199>
    1dad:	48 8d 3d dc 39 00 00 	lea    0x39dc(%rip),%rdi        # 5790 <input_strings+0xf0>
    1db4:	e8 07 f5 ff ff       	callq  12c0 <__isoc99_sscanf@plt>
    1db9:	83 f8 03             	cmp    $0x3,%eax
    1dbc:	74 0e                	je     1dcc <phase_defused+0x6b>
    1dbe:	48 8d 3d f3 14 00 00 	lea    0x14f3(%rip),%rdi        # 32b8 <array.3471+0xd8>
    1dc5:	e8 36 f4 ff ff       	callq  1200 <puts@plt>
    1dca:	eb b6                	jmp    1d82 <phase_defused+0x21>
    1dcc:	48 8d 7c 24 10       	lea    0x10(%rsp),%rdi
    1dd1:	48 8d 35 aa 15 00 00 	lea    0x15aa(%rip),%rsi        # 3382 <array.3471+0x1a2>
    1dd8:	e8 b7 fc ff ff       	callq  1a94 <strings_not_equal>
    1ddd:	85 c0                	test   %eax,%eax
    1ddf:	75 dd                	jne    1dbe <phase_defused+0x5d>
    1de1:	48 8d 3d 70 14 00 00 	lea    0x1470(%rip),%rdi        # 3258 <array.3471+0x78>
    1de8:	e8 13 f4 ff ff       	callq  1200 <puts@plt>
    1ded:	48 8d 3d 8c 14 00 00 	lea    0x148c(%rip),%rdi        # 3280 <array.3471+0xa0>
    1df4:	e8 07 f4 ff ff       	callq  1200 <puts@plt>
    1df9:	b8 00 00 00 00       	mov    $0x0,%eax
    1dfe:	e8 84 fb ff ff       	callq  1987 <secret_phase>
    1e03:	eb b9                	jmp    1dbe <phase_defused+0x5d>
    1e05:	e8 16 f4 ff ff       	callq  1220 <__stack_chk_fail@plt>

0000000000001e0a <sigalrm_handler>:
    1e0a:	f3 0f 1e fa          	endbr64 
    1e0e:	50                   	push   %rax
    1e0f:	58                   	pop    %rax
    1e10:	48 83 ec 08          	sub    $0x8,%rsp
    1e14:	b9 00 00 00 00       	mov    $0x0,%ecx
    1e19:	48 8d 15 d0 15 00 00 	lea    0x15d0(%rip),%rdx        # 33f0 <array.3471+0x210>
    1e20:	be 01 00 00 00       	mov    $0x1,%esi
    1e25:	48 8b 3d 54 38 00 00 	mov    0x3854(%rip),%rdi        # 5680 <stderr@@GLIBC_2.2.5>
    1e2c:	b8 00 00 00 00       	mov    $0x0,%eax
    1e31:	e8 da f4 ff ff       	callq  1310 <__fprintf_chk@plt>
    1e36:	bf 01 00 00 00       	mov    $0x1,%edi
    1e3b:	e8 b0 f4 ff ff       	callq  12f0 <exit@plt>

0000000000001e40 <rio_readlineb>:
    1e40:	41 56                	push   %r14
    1e42:	41 55                	push   %r13
    1e44:	41 54                	push   %r12
    1e46:	55                   	push   %rbp
    1e47:	53                   	push   %rbx
    1e48:	48 89 f5             	mov    %rsi,%rbp
    1e4b:	48 83 fa 01          	cmp    $0x1,%rdx
    1e4f:	0f 86 90 00 00 00    	jbe    1ee5 <rio_readlineb+0xa5>
    1e55:	48 89 fb             	mov    %rdi,%rbx
    1e58:	4c 8d 74 16 ff       	lea    -0x1(%rsi,%rdx,1),%r14
    1e5d:	41 bd 01 00 00 00    	mov    $0x1,%r13d
    1e63:	4c 8d 67 10          	lea    0x10(%rdi),%r12
    1e67:	eb 54                	jmp    1ebd <rio_readlineb+0x7d>
    1e69:	e8 72 f3 ff ff       	callq  11e0 <__errno_location@plt>
    1e6e:	83 38 04             	cmpl   $0x4,(%rax)
    1e71:	75 53                	jne    1ec6 <rio_readlineb+0x86>
    1e73:	ba 00 20 00 00       	mov    $0x2000,%edx
    1e78:	4c 89 e6             	mov    %r12,%rsi
    1e7b:	8b 3b                	mov    (%rbx),%edi
    1e7d:	e8 ce f3 ff ff       	callq  1250 <read@plt>
    1e82:	89 c2                	mov    %eax,%edx
    1e84:	89 43 04             	mov    %eax,0x4(%rbx)
    1e87:	85 c0                	test   %eax,%eax
    1e89:	78 de                	js     1e69 <rio_readlineb+0x29>
    1e8b:	85 c0                	test   %eax,%eax
    1e8d:	74 40                	je     1ecf <rio_readlineb+0x8f>
    1e8f:	4c 89 63 08          	mov    %r12,0x8(%rbx)
    1e93:	48 8b 43 08          	mov    0x8(%rbx),%rax
    1e97:	0f b6 08             	movzbl (%rax),%ecx
    1e9a:	48 83 c0 01          	add    $0x1,%rax
    1e9e:	48 89 43 08          	mov    %rax,0x8(%rbx)
    1ea2:	83 ea 01             	sub    $0x1,%edx
    1ea5:	89 53 04             	mov    %edx,0x4(%rbx)
    1ea8:	48 83 c5 01          	add    $0x1,%rbp
    1eac:	88 4d ff             	mov    %cl,-0x1(%rbp)
    1eaf:	80 f9 0a             	cmp    $0xa,%cl
    1eb2:	74 3c                	je     1ef0 <rio_readlineb+0xb0>
    1eb4:	41 83 c5 01          	add    $0x1,%r13d
    1eb8:	4c 39 f5             	cmp    %r14,%rbp
    1ebb:	74 30                	je     1eed <rio_readlineb+0xad>
    1ebd:	8b 53 04             	mov    0x4(%rbx),%edx
    1ec0:	85 d2                	test   %edx,%edx
    1ec2:	7e af                	jle    1e73 <rio_readlineb+0x33>
    1ec4:	eb cd                	jmp    1e93 <rio_readlineb+0x53>
    1ec6:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
    1ecd:	eb 05                	jmp    1ed4 <rio_readlineb+0x94>
    1ecf:	b8 00 00 00 00       	mov    $0x0,%eax
    1ed4:	85 c0                	test   %eax,%eax
    1ed6:	75 28                	jne    1f00 <rio_readlineb+0xc0>
    1ed8:	b8 00 00 00 00       	mov    $0x0,%eax
    1edd:	41 83 fd 01          	cmp    $0x1,%r13d
    1ee1:	75 0d                	jne    1ef0 <rio_readlineb+0xb0>
    1ee3:	eb 12                	jmp    1ef7 <rio_readlineb+0xb7>
    1ee5:	41 bd 01 00 00 00    	mov    $0x1,%r13d
    1eeb:	eb 03                	jmp    1ef0 <rio_readlineb+0xb0>
    1eed:	4c 89 f5             	mov    %r14,%rbp
    1ef0:	c6 45 00 00          	movb   $0x0,0x0(%rbp)
    1ef4:	49 63 c5             	movslq %r13d,%rax
    1ef7:	5b                   	pop    %rbx
    1ef8:	5d                   	pop    %rbp
    1ef9:	41 5c                	pop    %r12
    1efb:	41 5d                	pop    %r13
    1efd:	41 5e                	pop    %r14
    1eff:	c3                   	retq   
    1f00:	48 c7 c0 ff ff ff ff 	mov    $0xffffffffffffffff,%rax
    1f07:	eb ee                	jmp    1ef7 <rio_readlineb+0xb7>

0000000000001f09 <submitr>:
    1f09:	f3 0f 1e fa          	endbr64 
    1f0d:	41 57                	push   %r15
    1f0f:	41 56                	push   %r14
    1f11:	41 55                	push   %r13
    1f13:	41 54                	push   %r12
    1f15:	55                   	push   %rbp
    1f16:	53                   	push   %rbx
    1f17:	4c 8d 9c 24 00 60 ff 	lea    -0xa000(%rsp),%r11
    1f1e:	ff 
    1f1f:	48 81 ec 00 10 00 00 	sub    $0x1000,%rsp
    1f26:	48 83 0c 24 00       	orq    $0x0,(%rsp)
    1f2b:	4c 39 dc             	cmp    %r11,%rsp
    1f2e:	75 ef                	jne    1f1f <submitr+0x16>
    1f30:	48 83 ec 68          	sub    $0x68,%rsp
    1f34:	49 89 fd             	mov    %rdi,%r13
    1f37:	89 f5                	mov    %esi,%ebp
    1f39:	48 89 14 24          	mov    %rdx,(%rsp)
    1f3d:	48 89 4c 24 08       	mov    %rcx,0x8(%rsp)
    1f42:	4c 89 44 24 18       	mov    %r8,0x18(%rsp)
    1f47:	4c 89 4c 24 10       	mov    %r9,0x10(%rsp)
    1f4c:	48 8b 9c 24 a0 a0 00 	mov    0xa0a0(%rsp),%rbx
    1f53:	00 
    1f54:	4c 8b bc 24 a8 a0 00 	mov    0xa0a8(%rsp),%r15
    1f5b:	00 
    1f5c:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    1f63:	00 00 
    1f65:	48 89 84 24 58 a0 00 	mov    %rax,0xa058(%rsp)
    1f6c:	00 
    1f6d:	31 c0                	xor    %eax,%eax
    1f6f:	c7 44 24 2c 00 00 00 	movl   $0x0,0x2c(%rsp)
    1f76:	00 
    1f77:	ba 00 00 00 00       	mov    $0x0,%edx
    1f7c:	be 01 00 00 00       	mov    $0x1,%esi
    1f81:	bf 02 00 00 00       	mov    $0x2,%edi
    1f86:	e8 c5 f3 ff ff       	callq  1350 <socket@plt>
    1f8b:	85 c0                	test   %eax,%eax
    1f8d:	0f 88 17 01 00 00    	js     20aa <submitr+0x1a1>
    1f93:	41 89 c4             	mov    %eax,%r12d
    1f96:	4c 89 ef             	mov    %r13,%rdi
    1f99:	e8 e2 f2 ff ff       	callq  1280 <gethostbyname@plt>
    1f9e:	48 85 c0             	test   %rax,%rax
    1fa1:	0f 84 53 01 00 00    	je     20fa <submitr+0x1f1>
    1fa7:	4c 8d 6c 24 30       	lea    0x30(%rsp),%r13
    1fac:	48 c7 44 24 30 00 00 	movq   $0x0,0x30(%rsp)
    1fb3:	00 00 
    1fb5:	48 c7 44 24 38 00 00 	movq   $0x0,0x38(%rsp)
    1fbc:	00 00 
    1fbe:	66 c7 44 24 30 02 00 	movw   $0x2,0x30(%rsp)
    1fc5:	48 63 50 14          	movslq 0x14(%rax),%rdx
    1fc9:	48 8b 40 18          	mov    0x18(%rax),%rax
    1fcd:	48 8d 7c 24 34       	lea    0x34(%rsp),%rdi
    1fd2:	b9 0c 00 00 00       	mov    $0xc,%ecx
    1fd7:	48 8b 30             	mov    (%rax),%rsi
    1fda:	e8 b1 f2 ff ff       	callq  1290 <__memmove_chk@plt>
    1fdf:	66 c1 c5 08          	rol    $0x8,%bp
    1fe3:	66 89 6c 24 32       	mov    %bp,0x32(%rsp)
    1fe8:	ba 10 00 00 00       	mov    $0x10,%edx
    1fed:	4c 89 ee             	mov    %r13,%rsi
    1ff0:	44 89 e7             	mov    %r12d,%edi
    1ff3:	e8 08 f3 ff ff       	callq  1300 <connect@plt>
    1ff8:	85 c0                	test   %eax,%eax
    1ffa:	0f 88 65 01 00 00    	js     2165 <submitr+0x25c>
    2000:	49 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%r9
    2007:	b8 00 00 00 00       	mov    $0x0,%eax
    200c:	4c 89 c9             	mov    %r9,%rcx
    200f:	48 89 df             	mov    %rbx,%rdi
    2012:	f2 ae                	repnz scas %es:(%rdi),%al
    2014:	48 f7 d1             	not    %rcx
    2017:	48 89 ce             	mov    %rcx,%rsi
    201a:	4c 89 c9             	mov    %r9,%rcx
    201d:	48 8b 3c 24          	mov    (%rsp),%rdi
    2021:	f2 ae                	repnz scas %es:(%rdi),%al
    2023:	49 89 c8             	mov    %rcx,%r8
    2026:	4c 89 c9             	mov    %r9,%rcx
    2029:	48 8b 7c 24 08       	mov    0x8(%rsp),%rdi
    202e:	f2 ae                	repnz scas %es:(%rdi),%al
    2030:	48 89 ca             	mov    %rcx,%rdx
    2033:	48 f7 d2             	not    %rdx
    2036:	4c 89 c9             	mov    %r9,%rcx
    2039:	48 8b 7c 24 10       	mov    0x10(%rsp),%rdi
    203e:	f2 ae                	repnz scas %es:(%rdi),%al
    2040:	4c 29 c2             	sub    %r8,%rdx
    2043:	48 29 ca             	sub    %rcx,%rdx
    2046:	48 8d 44 76 fd       	lea    -0x3(%rsi,%rsi,2),%rax
    204b:	48 8d 44 02 7b       	lea    0x7b(%rdx,%rax,1),%rax
    2050:	48 3d 00 20 00 00    	cmp    $0x2000,%rax
    2056:	0f 87 66 01 00 00    	ja     21c2 <submitr+0x2b9>
    205c:	48 8d 94 24 50 40 00 	lea    0x4050(%rsp),%rdx
    2063:	00 
    2064:	b9 00 04 00 00       	mov    $0x400,%ecx
    2069:	b8 00 00 00 00       	mov    $0x0,%eax
    206e:	48 89 d7             	mov    %rdx,%rdi
    2071:	f3 48 ab             	rep stos %rax,%es:(%rdi)
    2074:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
    207b:	48 89 df             	mov    %rbx,%rdi
    207e:	f2 ae                	repnz scas %es:(%rdi),%al
    2080:	48 f7 d1             	not    %rcx
    2083:	48 8d 41 ff          	lea    -0x1(%rcx),%rax
    2087:	83 f9 01             	cmp    $0x1,%ecx
    208a:	0f 84 08 05 00 00    	je     2598 <submitr+0x68f>
    2090:	8d 40 ff             	lea    -0x1(%rax),%eax
    2093:	4c 8d 74 03 01       	lea    0x1(%rbx,%rax,1),%r14
    2098:	48 89 d5             	mov    %rdx,%rbp
    209b:	49 bd d9 ff 00 00 00 	movabs $0x2000000000ffd9,%r13
    20a2:	00 20 00 
    20a5:	e9 a6 01 00 00       	jmpq   2250 <submitr+0x347>
    20aa:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    20b1:	3a 20 43 
    20b4:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    20bb:	20 75 6e 
    20be:	49 89 07             	mov    %rax,(%r15)
    20c1:	49 89 57 08          	mov    %rdx,0x8(%r15)
    20c5:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    20cc:	74 6f 20 
    20cf:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
    20d6:	65 20 73 
    20d9:	49 89 47 10          	mov    %rax,0x10(%r15)
    20dd:	49 89 57 18          	mov    %rdx,0x18(%r15)
    20e1:	41 c7 47 20 6f 63 6b 	movl   $0x656b636f,0x20(%r15)
    20e8:	65 
    20e9:	66 41 c7 47 24 74 00 	movw   $0x74,0x24(%r15)
    20f0:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    20f5:	e9 16 03 00 00       	jmpq   2410 <submitr+0x507>
    20fa:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
    2101:	3a 20 44 
    2104:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
    210b:	20 75 6e 
    210e:	49 89 07             	mov    %rax,(%r15)
    2111:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2115:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    211c:	74 6f 20 
    211f:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
    2126:	76 65 20 
    2129:	49 89 47 10          	mov    %rax,0x10(%r15)
    212d:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2131:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
    2138:	72 20 61 
    213b:	49 89 47 20          	mov    %rax,0x20(%r15)
    213f:	41 c7 47 28 64 64 72 	movl   $0x65726464,0x28(%r15)
    2146:	65 
    2147:	66 41 c7 47 2c 73 73 	movw   $0x7373,0x2c(%r15)
    214e:	41 c6 47 2e 00       	movb   $0x0,0x2e(%r15)
    2153:	44 89 e7             	mov    %r12d,%edi
    2156:	e8 e5 f0 ff ff       	callq  1240 <close@plt>
    215b:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2160:	e9 ab 02 00 00       	jmpq   2410 <submitr+0x507>
    2165:	48 b8 45 72 72 6f 72 	movabs $0x55203a726f727245,%rax
    216c:	3a 20 55 
    216f:	48 ba 6e 61 62 6c 65 	movabs $0x6f7420656c62616e,%rdx
    2176:	20 74 6f 
    2179:	49 89 07             	mov    %rax,(%r15)
    217c:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2180:	48 b8 20 63 6f 6e 6e 	movabs $0x7463656e6e6f6320,%rax
    2187:	65 63 74 
    218a:	48 ba 20 74 6f 20 74 	movabs $0x20656874206f7420,%rdx
    2191:	68 65 20 
    2194:	49 89 47 10          	mov    %rax,0x10(%r15)
    2198:	49 89 57 18          	mov    %rdx,0x18(%r15)
    219c:	41 c7 47 20 73 65 72 	movl   $0x76726573,0x20(%r15)
    21a3:	76 
    21a4:	66 41 c7 47 24 65 72 	movw   $0x7265,0x24(%r15)
    21ab:	41 c6 47 26 00       	movb   $0x0,0x26(%r15)
    21b0:	44 89 e7             	mov    %r12d,%edi
    21b3:	e8 88 f0 ff ff       	callq  1240 <close@plt>
    21b8:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    21bd:	e9 4e 02 00 00       	jmpq   2410 <submitr+0x507>
    21c2:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
    21c9:	3a 20 52 
    21cc:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
    21d3:	20 73 74 
    21d6:	49 89 07             	mov    %rax,(%r15)
    21d9:	49 89 57 08          	mov    %rdx,0x8(%r15)
    21dd:	48 b8 72 69 6e 67 20 	movabs $0x6f6f7420676e6972,%rax
    21e4:	74 6f 6f 
    21e7:	48 ba 20 6c 61 72 67 	movabs $0x202e656772616c20,%rdx
    21ee:	65 2e 20 
    21f1:	49 89 47 10          	mov    %rax,0x10(%r15)
    21f5:	49 89 57 18          	mov    %rdx,0x18(%r15)
    21f9:	48 b8 49 6e 63 72 65 	movabs $0x6573616572636e49,%rax
    2200:	61 73 65 
    2203:	48 ba 20 53 55 42 4d 	movabs $0x5254494d42555320,%rdx
    220a:	49 54 52 
    220d:	49 89 47 20          	mov    %rax,0x20(%r15)
    2211:	49 89 57 28          	mov    %rdx,0x28(%r15)
    2215:	48 b8 5f 4d 41 58 42 	movabs $0x46554258414d5f,%rax
    221c:	55 46 00 
    221f:	49 89 47 30          	mov    %rax,0x30(%r15)
    2223:	44 89 e7             	mov    %r12d,%edi
    2226:	e8 15 f0 ff ff       	callq  1240 <close@plt>
    222b:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2230:	e9 db 01 00 00       	jmpq   2410 <submitr+0x507>
    2235:	49 0f a3 c5          	bt     %rax,%r13
    2239:	73 21                	jae    225c <submitr+0x353>
    223b:	44 88 45 00          	mov    %r8b,0x0(%rbp)
    223f:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
    2243:	48 83 c3 01          	add    $0x1,%rbx
    2247:	4c 39 f3             	cmp    %r14,%rbx
    224a:	0f 84 48 03 00 00    	je     2598 <submitr+0x68f>
    2250:	44 0f b6 03          	movzbl (%rbx),%r8d
    2254:	41 8d 40 d6          	lea    -0x2a(%r8),%eax
    2258:	3c 35                	cmp    $0x35,%al
    225a:	76 d9                	jbe    2235 <submitr+0x32c>
    225c:	44 89 c0             	mov    %r8d,%eax
    225f:	83 e0 df             	and    $0xffffffdf,%eax
    2262:	83 e8 41             	sub    $0x41,%eax
    2265:	3c 19                	cmp    $0x19,%al
    2267:	76 d2                	jbe    223b <submitr+0x332>
    2269:	41 80 f8 20          	cmp    $0x20,%r8b
    226d:	74 63                	je     22d2 <submitr+0x3c9>
    226f:	41 8d 40 e0          	lea    -0x20(%r8),%eax
    2273:	3c 5f                	cmp    $0x5f,%al
    2275:	76 0a                	jbe    2281 <submitr+0x378>
    2277:	41 80 f8 09          	cmp    $0x9,%r8b
    227b:	0f 85 8a 02 00 00    	jne    250b <submitr+0x602>
    2281:	48 8d bc 24 50 80 00 	lea    0x8050(%rsp),%rdi
    2288:	00 
    2289:	45 0f b6 c0          	movzbl %r8b,%r8d
    228d:	48 8d 0d 32 12 00 00 	lea    0x1232(%rip),%rcx        # 34c6 <array.3471+0x2e6>
    2294:	ba 08 00 00 00       	mov    $0x8,%edx
    2299:	be 01 00 00 00       	mov    $0x1,%esi
    229e:	b8 00 00 00 00       	mov    $0x0,%eax
    22a3:	e8 98 f0 ff ff       	callq  1340 <__sprintf_chk@plt>
    22a8:	0f b6 84 24 50 80 00 	movzbl 0x8050(%rsp),%eax
    22af:	00 
    22b0:	88 45 00             	mov    %al,0x0(%rbp)
    22b3:	0f b6 84 24 51 80 00 	movzbl 0x8051(%rsp),%eax
    22ba:	00 
    22bb:	88 45 01             	mov    %al,0x1(%rbp)
    22be:	0f b6 84 24 52 80 00 	movzbl 0x8052(%rsp),%eax
    22c5:	00 
    22c6:	88 45 02             	mov    %al,0x2(%rbp)
    22c9:	48 8d 6d 03          	lea    0x3(%rbp),%rbp
    22cd:	e9 71 ff ff ff       	jmpq   2243 <submitr+0x33a>
    22d2:	c6 45 00 2b          	movb   $0x2b,0x0(%rbp)
    22d6:	48 8d 6d 01          	lea    0x1(%rbp),%rbp
    22da:	e9 64 ff ff ff       	jmpq   2243 <submitr+0x33a>
    22df:	48 01 c5             	add    %rax,%rbp
    22e2:	48 29 c3             	sub    %rax,%rbx
    22e5:	0f 84 25 03 00 00    	je     2610 <submitr+0x707>
    22eb:	48 89 da             	mov    %rbx,%rdx
    22ee:	48 89 ee             	mov    %rbp,%rsi
    22f1:	44 89 e7             	mov    %r12d,%edi
    22f4:	e8 17 ef ff ff       	callq  1210 <write@plt>
    22f9:	48 85 c0             	test   %rax,%rax
    22fc:	7f e1                	jg     22df <submitr+0x3d6>
    22fe:	e8 dd ee ff ff       	callq  11e0 <__errno_location@plt>
    2303:	83 38 04             	cmpl   $0x4,(%rax)
    2306:	0f 85 a0 01 00 00    	jne    24ac <submitr+0x5a3>
    230c:	4c 89 e8             	mov    %r13,%rax
    230f:	eb ce                	jmp    22df <submitr+0x3d6>
    2311:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    2318:	3a 20 43 
    231b:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    2322:	20 75 6e 
    2325:	49 89 07             	mov    %rax,(%r15)
    2328:	49 89 57 08          	mov    %rdx,0x8(%r15)
    232c:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    2333:	74 6f 20 
    2336:	48 ba 72 65 61 64 20 	movabs $0x7269662064616572,%rdx
    233d:	66 69 72 
    2340:	49 89 47 10          	mov    %rax,0x10(%r15)
    2344:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2348:	48 b8 73 74 20 68 65 	movabs $0x6564616568207473,%rax
    234f:	61 64 65 
    2352:	48 ba 72 20 66 72 6f 	movabs $0x73206d6f72662072,%rdx
    2359:	6d 20 73 
    235c:	49 89 47 20          	mov    %rax,0x20(%r15)
    2360:	49 89 57 28          	mov    %rdx,0x28(%r15)
    2364:	41 c7 47 30 65 72 76 	movl   $0x65767265,0x30(%r15)
    236b:	65 
    236c:	66 41 c7 47 34 72 00 	movw   $0x72,0x34(%r15)
    2373:	44 89 e7             	mov    %r12d,%edi
    2376:	e8 c5 ee ff ff       	callq  1240 <close@plt>
    237b:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2380:	e9 8b 00 00 00       	jmpq   2410 <submitr+0x507>
    2385:	4c 8d 8c 24 50 80 00 	lea    0x8050(%rsp),%r9
    238c:	00 
    238d:	48 8d 0d 84 10 00 00 	lea    0x1084(%rip),%rcx        # 3418 <array.3471+0x238>
    2394:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
    239b:	be 01 00 00 00       	mov    $0x1,%esi
    23a0:	4c 89 ff             	mov    %r15,%rdi
    23a3:	b8 00 00 00 00       	mov    $0x0,%eax
    23a8:	e8 93 ef ff ff       	callq  1340 <__sprintf_chk@plt>
    23ad:	44 89 e7             	mov    %r12d,%edi
    23b0:	e8 8b ee ff ff       	callq  1240 <close@plt>
    23b5:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    23ba:	eb 54                	jmp    2410 <submitr+0x507>
    23bc:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
    23c3:	00 
    23c4:	48 8d 7c 24 40       	lea    0x40(%rsp),%rdi
    23c9:	ba 00 20 00 00       	mov    $0x2000,%edx
    23ce:	e8 6d fa ff ff       	callq  1e40 <rio_readlineb>
    23d3:	48 85 c0             	test   %rax,%rax
    23d6:	7e 61                	jle    2439 <submitr+0x530>
    23d8:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
    23df:	00 
    23e0:	4c 89 ff             	mov    %r15,%rdi
    23e3:	e8 08 ee ff ff       	callq  11f0 <strcpy@plt>
    23e8:	44 89 e7             	mov    %r12d,%edi
    23eb:	e8 50 ee ff ff       	callq  1240 <close@plt>
    23f0:	b9 03 00 00 00       	mov    $0x3,%ecx
    23f5:	48 8d 3d e5 10 00 00 	lea    0x10e5(%rip),%rdi        # 34e1 <array.3471+0x301>
    23fc:	4c 89 fe             	mov    %r15,%rsi
    23ff:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
    2401:	0f 97 c0             	seta   %al
    2404:	1c 00                	sbb    $0x0,%al
    2406:	84 c0                	test   %al,%al
    2408:	0f 95 c0             	setne  %al
    240b:	0f b6 c0             	movzbl %al,%eax
    240e:	f7 d8                	neg    %eax
    2410:	48 8b 94 24 58 a0 00 	mov    0xa058(%rsp),%rdx
    2417:	00 
    2418:	64 48 33 14 25 28 00 	xor    %fs:0x28,%rdx
    241f:	00 00 
    2421:	0f 85 0c 03 00 00    	jne    2733 <submitr+0x82a>
    2427:	48 81 c4 68 a0 00 00 	add    $0xa068,%rsp
    242e:	5b                   	pop    %rbx
    242f:	5d                   	pop    %rbp
    2430:	41 5c                	pop    %r12
    2432:	41 5d                	pop    %r13
    2434:	41 5e                	pop    %r14
    2436:	41 5f                	pop    %r15
    2438:	c3                   	retq   
    2439:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    2440:	3a 20 43 
    2443:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    244a:	20 75 6e 
    244d:	49 89 07             	mov    %rax,(%r15)
    2450:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2454:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    245b:	74 6f 20 
    245e:	48 ba 72 65 61 64 20 	movabs $0x6174732064616572,%rdx
    2465:	73 74 61 
    2468:	49 89 47 10          	mov    %rax,0x10(%r15)
    246c:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2470:	48 b8 74 75 73 20 6d 	movabs $0x7373656d20737574,%rax
    2477:	65 73 73 
    247a:	48 ba 61 67 65 20 66 	movabs $0x6d6f726620656761,%rdx
    2481:	72 6f 6d 
    2484:	49 89 47 20          	mov    %rax,0x20(%r15)
    2488:	49 89 57 28          	mov    %rdx,0x28(%r15)
    248c:	48 b8 20 73 65 72 76 	movabs $0x72657672657320,%rax
    2493:	65 72 00 
    2496:	49 89 47 30          	mov    %rax,0x30(%r15)
    249a:	44 89 e7             	mov    %r12d,%edi
    249d:	e8 9e ed ff ff       	callq  1240 <close@plt>
    24a2:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    24a7:	e9 64 ff ff ff       	jmpq   2410 <submitr+0x507>
    24ac:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    24b3:	3a 20 43 
    24b6:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    24bd:	20 75 6e 
    24c0:	49 89 07             	mov    %rax,(%r15)
    24c3:	49 89 57 08          	mov    %rdx,0x8(%r15)
    24c7:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    24ce:	74 6f 20 
    24d1:	48 ba 77 72 69 74 65 	movabs $0x6f74206574697277,%rdx
    24d8:	20 74 6f 
    24db:	49 89 47 10          	mov    %rax,0x10(%r15)
    24df:	49 89 57 18          	mov    %rdx,0x18(%r15)
    24e3:	48 b8 20 74 68 65 20 	movabs $0x7265732065687420,%rax
    24ea:	73 65 72 
    24ed:	49 89 47 20          	mov    %rax,0x20(%r15)
    24f1:	41 c7 47 28 76 65 72 	movl   $0x726576,0x28(%r15)
    24f8:	00 
    24f9:	44 89 e7             	mov    %r12d,%edi
    24fc:	e8 3f ed ff ff       	callq  1240 <close@plt>
    2501:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2506:	e9 05 ff ff ff       	jmpq   2410 <submitr+0x507>
    250b:	48 b8 45 72 72 6f 72 	movabs $0x52203a726f727245,%rax
    2512:	3a 20 52 
    2515:	48 ba 65 73 75 6c 74 	movabs $0x747320746c757365,%rdx
    251c:	20 73 74 
    251f:	49 89 07             	mov    %rax,(%r15)
    2522:	49 89 57 08          	mov    %rdx,0x8(%r15)
    2526:	48 b8 72 69 6e 67 20 	movabs $0x6e6f6320676e6972,%rax
    252d:	63 6f 6e 
    2530:	48 ba 74 61 69 6e 73 	movabs $0x6e6120736e696174,%rdx
    2537:	20 61 6e 
    253a:	49 89 47 10          	mov    %rax,0x10(%r15)
    253e:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2542:	48 b8 20 69 6c 6c 65 	movabs $0x6c6167656c6c6920,%rax
    2549:	67 61 6c 
    254c:	48 ba 20 6f 72 20 75 	movabs $0x72706e7520726f20,%rdx
    2553:	6e 70 72 
    2556:	49 89 47 20          	mov    %rax,0x20(%r15)
    255a:	49 89 57 28          	mov    %rdx,0x28(%r15)
    255e:	48 b8 69 6e 74 61 62 	movabs $0x20656c6261746e69,%rax
    2565:	6c 65 20 
    2568:	48 ba 63 68 61 72 61 	movabs $0x6574636172616863,%rdx
    256f:	63 74 65 
    2572:	49 89 47 30          	mov    %rax,0x30(%r15)
    2576:	49 89 57 38          	mov    %rdx,0x38(%r15)
    257a:	66 41 c7 47 40 72 2e 	movw   $0x2e72,0x40(%r15)
    2581:	41 c6 47 42 00       	movb   $0x0,0x42(%r15)
    2586:	44 89 e7             	mov    %r12d,%edi
    2589:	e8 b2 ec ff ff       	callq  1240 <close@plt>
    258e:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2593:	e9 78 fe ff ff       	jmpq   2410 <submitr+0x507>
    2598:	48 8d 9c 24 50 20 00 	lea    0x2050(%rsp),%rbx
    259f:	00 
    25a0:	48 83 ec 08          	sub    $0x8,%rsp
    25a4:	48 8d 84 24 58 40 00 	lea    0x4058(%rsp),%rax
    25ab:	00 
    25ac:	50                   	push   %rax
    25ad:	ff 74 24 20          	pushq  0x20(%rsp)
    25b1:	ff 74 24 30          	pushq  0x30(%rsp)
    25b5:	4c 8b 4c 24 28       	mov    0x28(%rsp),%r9
    25ba:	4c 8b 44 24 20       	mov    0x20(%rsp),%r8
    25bf:	48 8d 0d 82 0e 00 00 	lea    0xe82(%rip),%rcx        # 3448 <array.3471+0x268>
    25c6:	ba 00 20 00 00       	mov    $0x2000,%edx
    25cb:	be 01 00 00 00       	mov    $0x1,%esi
    25d0:	48 89 df             	mov    %rbx,%rdi
    25d3:	b8 00 00 00 00       	mov    $0x0,%eax
    25d8:	e8 63 ed ff ff       	callq  1340 <__sprintf_chk@plt>
    25dd:	48 c7 c1 ff ff ff ff 	mov    $0xffffffffffffffff,%rcx
    25e4:	b8 00 00 00 00       	mov    $0x0,%eax
    25e9:	48 89 df             	mov    %rbx,%rdi
    25ec:	f2 ae                	repnz scas %es:(%rdi),%al
    25ee:	48 f7 d1             	not    %rcx
    25f1:	48 83 c4 20          	add    $0x20,%rsp
    25f5:	48 8d ac 24 50 20 00 	lea    0x2050(%rsp),%rbp
    25fc:	00 
    25fd:	41 bd 00 00 00 00    	mov    $0x0,%r13d
    2603:	48 89 cb             	mov    %rcx,%rbx
    2606:	48 83 eb 01          	sub    $0x1,%rbx
    260a:	0f 85 db fc ff ff    	jne    22eb <submitr+0x3e2>
    2610:	44 89 64 24 40       	mov    %r12d,0x40(%rsp)
    2615:	c7 44 24 44 00 00 00 	movl   $0x0,0x44(%rsp)
    261c:	00 
    261d:	48 8d 7c 24 40       	lea    0x40(%rsp),%rdi
    2622:	48 8d 44 24 50       	lea    0x50(%rsp),%rax
    2627:	48 89 44 24 48       	mov    %rax,0x48(%rsp)
    262c:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
    2633:	00 
    2634:	ba 00 20 00 00       	mov    $0x2000,%edx
    2639:	e8 02 f8 ff ff       	callq  1e40 <rio_readlineb>
    263e:	48 85 c0             	test   %rax,%rax
    2641:	0f 8e ca fc ff ff    	jle    2311 <submitr+0x408>
    2647:	48 8d 4c 24 2c       	lea    0x2c(%rsp),%rcx
    264c:	48 8d 94 24 50 60 00 	lea    0x6050(%rsp),%rdx
    2653:	00 
    2654:	48 8d bc 24 50 20 00 	lea    0x2050(%rsp),%rdi
    265b:	00 
    265c:	4c 8d 84 24 50 80 00 	lea    0x8050(%rsp),%r8
    2663:	00 
    2664:	48 8d 35 62 0e 00 00 	lea    0xe62(%rip),%rsi        # 34cd <array.3471+0x2ed>
    266b:	b8 00 00 00 00       	mov    $0x0,%eax
    2670:	e8 4b ec ff ff       	callq  12c0 <__isoc99_sscanf@plt>
    2675:	44 8b 44 24 2c       	mov    0x2c(%rsp),%r8d
    267a:	41 81 f8 c8 00 00 00 	cmp    $0xc8,%r8d
    2681:	0f 85 fe fc ff ff    	jne    2385 <submitr+0x47c>
    2687:	48 8d 1d 50 0e 00 00 	lea    0xe50(%rip),%rbx        # 34de <array.3471+0x2fe>
    268e:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
    2695:	00 
    2696:	b9 03 00 00 00       	mov    $0x3,%ecx
    269b:	48 89 df             	mov    %rbx,%rdi
    269e:	f3 a6                	repz cmpsb %es:(%rdi),%ds:(%rsi)
    26a0:	0f 97 c0             	seta   %al
    26a3:	1c 00                	sbb    $0x0,%al
    26a5:	84 c0                	test   %al,%al
    26a7:	0f 84 0f fd ff ff    	je     23bc <submitr+0x4b3>
    26ad:	48 8d b4 24 50 20 00 	lea    0x2050(%rsp),%rsi
    26b4:	00 
    26b5:	48 8d 7c 24 40       	lea    0x40(%rsp),%rdi
    26ba:	ba 00 20 00 00       	mov    $0x2000,%edx
    26bf:	e8 7c f7 ff ff       	callq  1e40 <rio_readlineb>
    26c4:	48 85 c0             	test   %rax,%rax
    26c7:	7f c5                	jg     268e <submitr+0x785>
    26c9:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    26d0:	3a 20 43 
    26d3:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    26da:	20 75 6e 
    26dd:	49 89 07             	mov    %rax,(%r15)
    26e0:	49 89 57 08          	mov    %rdx,0x8(%r15)
    26e4:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    26eb:	74 6f 20 
    26ee:	48 ba 72 65 61 64 20 	movabs $0x6165682064616572,%rdx
    26f5:	68 65 61 
    26f8:	49 89 47 10          	mov    %rax,0x10(%r15)
    26fc:	49 89 57 18          	mov    %rdx,0x18(%r15)
    2700:	48 b8 64 65 72 73 20 	movabs $0x6f72662073726564,%rax
    2707:	66 72 6f 
    270a:	48 ba 6d 20 73 65 72 	movabs $0x726576726573206d,%rdx
    2711:	76 65 72 
    2714:	49 89 47 20          	mov    %rax,0x20(%r15)
    2718:	49 89 57 28          	mov    %rdx,0x28(%r15)
    271c:	41 c6 47 30 00       	movb   $0x0,0x30(%r15)
    2721:	44 89 e7             	mov    %r12d,%edi
    2724:	e8 17 eb ff ff       	callq  1240 <close@plt>
    2729:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    272e:	e9 dd fc ff ff       	jmpq   2410 <submitr+0x507>
    2733:	e8 e8 ea ff ff       	callq  1220 <__stack_chk_fail@plt>

0000000000002738 <init_timeout>:
    2738:	f3 0f 1e fa          	endbr64 
    273c:	85 ff                	test   %edi,%edi
    273e:	75 01                	jne    2741 <init_timeout+0x9>
    2740:	c3                   	retq   
    2741:	53                   	push   %rbx
    2742:	89 fb                	mov    %edi,%ebx
    2744:	48 8d 35 bf f6 ff ff 	lea    -0x941(%rip),%rsi        # 1e0a <sigalrm_handler>
    274b:	bf 0e 00 00 00       	mov    $0xe,%edi
    2750:	e8 1b eb ff ff       	callq  1270 <signal@plt>
    2755:	85 db                	test   %ebx,%ebx
    2757:	bf 00 00 00 00       	mov    $0x0,%edi
    275c:	0f 49 fb             	cmovns %ebx,%edi
    275f:	e8 cc ea ff ff       	callq  1230 <alarm@plt>
    2764:	5b                   	pop    %rbx
    2765:	c3                   	retq   

0000000000002766 <init_driver>:
    2766:	f3 0f 1e fa          	endbr64 
    276a:	41 54                	push   %r12
    276c:	55                   	push   %rbp
    276d:	53                   	push   %rbx
    276e:	48 83 ec 20          	sub    $0x20,%rsp
    2772:	48 89 fd             	mov    %rdi,%rbp
    2775:	64 48 8b 04 25 28 00 	mov    %fs:0x28,%rax
    277c:	00 00 
    277e:	48 89 44 24 18       	mov    %rax,0x18(%rsp)
    2783:	31 c0                	xor    %eax,%eax
    2785:	be 01 00 00 00       	mov    $0x1,%esi
    278a:	bf 0d 00 00 00       	mov    $0xd,%edi
    278f:	e8 dc ea ff ff       	callq  1270 <signal@plt>
    2794:	be 01 00 00 00       	mov    $0x1,%esi
    2799:	bf 1d 00 00 00       	mov    $0x1d,%edi
    279e:	e8 cd ea ff ff       	callq  1270 <signal@plt>
    27a3:	be 01 00 00 00       	mov    $0x1,%esi
    27a8:	bf 1d 00 00 00       	mov    $0x1d,%edi
    27ad:	e8 be ea ff ff       	callq  1270 <signal@plt>
    27b2:	ba 00 00 00 00       	mov    $0x0,%edx
    27b7:	be 01 00 00 00       	mov    $0x1,%esi
    27bc:	bf 02 00 00 00       	mov    $0x2,%edi
    27c1:	e8 8a eb ff ff       	callq  1350 <socket@plt>
    27c6:	85 c0                	test   %eax,%eax
    27c8:	0f 88 9c 00 00 00    	js     286a <init_driver+0x104>
    27ce:	89 c3                	mov    %eax,%ebx
    27d0:	48 8d 3d 0d 0d 00 00 	lea    0xd0d(%rip),%rdi        # 34e4 <array.3471+0x304>
    27d7:	e8 a4 ea ff ff       	callq  1280 <gethostbyname@plt>
    27dc:	48 85 c0             	test   %rax,%rax
    27df:	0f 84 d1 00 00 00    	je     28b6 <init_driver+0x150>
    27e5:	49 89 e4             	mov    %rsp,%r12
    27e8:	48 c7 04 24 00 00 00 	movq   $0x0,(%rsp)
    27ef:	00 
    27f0:	48 c7 44 24 08 00 00 	movq   $0x0,0x8(%rsp)
    27f7:	00 00 
    27f9:	66 c7 04 24 02 00    	movw   $0x2,(%rsp)
    27ff:	48 63 50 14          	movslq 0x14(%rax),%rdx
    2803:	48 8b 40 18          	mov    0x18(%rax),%rax
    2807:	48 8d 7c 24 04       	lea    0x4(%rsp),%rdi
    280c:	b9 0c 00 00 00       	mov    $0xc,%ecx
    2811:	48 8b 30             	mov    (%rax),%rsi
    2814:	e8 77 ea ff ff       	callq  1290 <__memmove_chk@plt>
    2819:	66 c7 44 24 02 3b 6e 	movw   $0x6e3b,0x2(%rsp)
    2820:	ba 10 00 00 00       	mov    $0x10,%edx
    2825:	4c 89 e6             	mov    %r12,%rsi
    2828:	89 df                	mov    %ebx,%edi
    282a:	e8 d1 ea ff ff       	callq  1300 <connect@plt>
    282f:	85 c0                	test   %eax,%eax
    2831:	0f 88 e7 00 00 00    	js     291e <init_driver+0x1b8>
    2837:	89 df                	mov    %ebx,%edi
    2839:	e8 02 ea ff ff       	callq  1240 <close@plt>
    283e:	66 c7 45 00 4f 4b    	movw   $0x4b4f,0x0(%rbp)
    2844:	c6 45 02 00          	movb   $0x0,0x2(%rbp)
    2848:	b8 00 00 00 00       	mov    $0x0,%eax
    284d:	48 8b 4c 24 18       	mov    0x18(%rsp),%rcx
    2852:	64 48 33 0c 25 28 00 	xor    %fs:0x28,%rcx
    2859:	00 00 
    285b:	0f 85 f5 00 00 00    	jne    2956 <init_driver+0x1f0>
    2861:	48 83 c4 20          	add    $0x20,%rsp
    2865:	5b                   	pop    %rbx
    2866:	5d                   	pop    %rbp
    2867:	41 5c                	pop    %r12
    2869:	c3                   	retq   
    286a:	48 b8 45 72 72 6f 72 	movabs $0x43203a726f727245,%rax
    2871:	3a 20 43 
    2874:	48 ba 6c 69 65 6e 74 	movabs $0x6e7520746e65696c,%rdx
    287b:	20 75 6e 
    287e:	48 89 45 00          	mov    %rax,0x0(%rbp)
    2882:	48 89 55 08          	mov    %rdx,0x8(%rbp)
    2886:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    288d:	74 6f 20 
    2890:	48 ba 63 72 65 61 74 	movabs $0x7320657461657263,%rdx
    2897:	65 20 73 
    289a:	48 89 45 10          	mov    %rax,0x10(%rbp)
    289e:	48 89 55 18          	mov    %rdx,0x18(%rbp)
    28a2:	c7 45 20 6f 63 6b 65 	movl   $0x656b636f,0x20(%rbp)
    28a9:	66 c7 45 24 74 00    	movw   $0x74,0x24(%rbp)
    28af:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    28b4:	eb 97                	jmp    284d <init_driver+0xe7>
    28b6:	48 b8 45 72 72 6f 72 	movabs $0x44203a726f727245,%rax
    28bd:	3a 20 44 
    28c0:	48 ba 4e 53 20 69 73 	movabs $0x6e7520736920534e,%rdx
    28c7:	20 75 6e 
    28ca:	48 89 45 00          	mov    %rax,0x0(%rbp)
    28ce:	48 89 55 08          	mov    %rdx,0x8(%rbp)
    28d2:	48 b8 61 62 6c 65 20 	movabs $0x206f7420656c6261,%rax
    28d9:	74 6f 20 
    28dc:	48 ba 72 65 73 6f 6c 	movabs $0x2065766c6f736572,%rdx
    28e3:	76 65 20 
    28e6:	48 89 45 10          	mov    %rax,0x10(%rbp)
    28ea:	48 89 55 18          	mov    %rdx,0x18(%rbp)
    28ee:	48 b8 73 65 72 76 65 	movabs $0x6120726576726573,%rax
    28f5:	72 20 61 
    28f8:	48 89 45 20          	mov    %rax,0x20(%rbp)
    28fc:	c7 45 28 64 64 72 65 	movl   $0x65726464,0x28(%rbp)
    2903:	66 c7 45 2c 73 73    	movw   $0x7373,0x2c(%rbp)
    2909:	c6 45 2e 00          	movb   $0x0,0x2e(%rbp)
    290d:	89 df                	mov    %ebx,%edi
    290f:	e8 2c e9 ff ff       	callq  1240 <close@plt>
    2914:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2919:	e9 2f ff ff ff       	jmpq   284d <init_driver+0xe7>
    291e:	4c 8d 05 bf 0b 00 00 	lea    0xbbf(%rip),%r8        # 34e4 <array.3471+0x304>
    2925:	48 8d 0d 74 0b 00 00 	lea    0xb74(%rip),%rcx        # 34a0 <array.3471+0x2c0>
    292c:	48 c7 c2 ff ff ff ff 	mov    $0xffffffffffffffff,%rdx
    2933:	be 01 00 00 00       	mov    $0x1,%esi
    2938:	48 89 ef             	mov    %rbp,%rdi
    293b:	b8 00 00 00 00       	mov    $0x0,%eax
    2940:	e8 fb e9 ff ff       	callq  1340 <__sprintf_chk@plt>
    2945:	89 df                	mov    %ebx,%edi
    2947:	e8 f4 e8 ff ff       	callq  1240 <close@plt>
    294c:	b8 ff ff ff ff       	mov    $0xffffffff,%eax
    2951:	e9 f7 fe ff ff       	jmpq   284d <init_driver+0xe7>
    2956:	e8 c5 e8 ff ff       	callq  1220 <__stack_chk_fail@plt>

000000000000295b <driver_post>:
    295b:	f3 0f 1e fa          	endbr64 
    295f:	53                   	push   %rbx
    2960:	4c 89 c3             	mov    %r8,%rbx
    2963:	85 c9                	test   %ecx,%ecx
    2965:	75 17                	jne    297e <driver_post+0x23>
    2967:	48 85 ff             	test   %rdi,%rdi
    296a:	74 05                	je     2971 <driver_post+0x16>
    296c:	80 3f 00             	cmpb   $0x0,(%rdi)
    296f:	75 33                	jne    29a4 <driver_post+0x49>
    2971:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
    2976:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
    297a:	89 c8                	mov    %ecx,%eax
    297c:	5b                   	pop    %rbx
    297d:	c3                   	retq   
    297e:	48 8d 35 77 0b 00 00 	lea    0xb77(%rip),%rsi        # 34fc <array.3471+0x31c>
    2985:	bf 01 00 00 00       	mov    $0x1,%edi
    298a:	b8 00 00 00 00       	mov    $0x0,%eax
    298f:	e8 3c e9 ff ff       	callq  12d0 <__printf_chk@plt>
    2994:	66 c7 03 4f 4b       	movw   $0x4b4f,(%rbx)
    2999:	c6 43 02 00          	movb   $0x0,0x2(%rbx)
    299d:	b8 00 00 00 00       	mov    $0x0,%eax
    29a2:	eb d8                	jmp    297c <driver_post+0x21>
    29a4:	41 50                	push   %r8
    29a6:	52                   	push   %rdx
    29a7:	4c 8d 0d 65 0b 00 00 	lea    0xb65(%rip),%r9        # 3513 <array.3471+0x333>
    29ae:	49 89 f0             	mov    %rsi,%r8
    29b1:	48 89 f9             	mov    %rdi,%rcx
    29b4:	48 8d 15 60 0b 00 00 	lea    0xb60(%rip),%rdx        # 351b <array.3471+0x33b>
    29bb:	be 6e 3b 00 00       	mov    $0x3b6e,%esi
    29c0:	48 8d 3d 1d 0b 00 00 	lea    0xb1d(%rip),%rdi        # 34e4 <array.3471+0x304>
    29c7:	e8 3d f5 ff ff       	callq  1f09 <submitr>
    29cc:	48 83 c4 10          	add    $0x10,%rsp
    29d0:	eb aa                	jmp    297c <driver_post+0x21>
    29d2:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
    29d9:	00 00 00 
    29dc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000000029e0 <__libc_csu_init>:
    29e0:	f3 0f 1e fa          	endbr64 
    29e4:	41 57                	push   %r15
    29e6:	4c 8d 3d 0b 23 00 00 	lea    0x230b(%rip),%r15        # 4cf8 <__frame_dummy_init_array_entry>
    29ed:	41 56                	push   %r14
    29ef:	49 89 d6             	mov    %rdx,%r14
    29f2:	41 55                	push   %r13
    29f4:	49 89 f5             	mov    %rsi,%r13
    29f7:	41 54                	push   %r12
    29f9:	41 89 fc             	mov    %edi,%r12d
    29fc:	55                   	push   %rbp
    29fd:	48 8d 2d fc 22 00 00 	lea    0x22fc(%rip),%rbp        # 4d00 <__do_global_dtors_aux_fini_array_entry>
    2a04:	53                   	push   %rbx
    2a05:	4c 29 fd             	sub    %r15,%rbp
    2a08:	48 83 ec 08          	sub    $0x8,%rsp
    2a0c:	e8 ef e5 ff ff       	callq  1000 <_init>
    2a11:	48 c1 fd 03          	sar    $0x3,%rbp
    2a15:	74 1f                	je     2a36 <__libc_csu_init+0x56>
    2a17:	31 db                	xor    %ebx,%ebx
    2a19:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
    2a20:	4c 89 f2             	mov    %r14,%rdx
    2a23:	4c 89 ee             	mov    %r13,%rsi
    2a26:	44 89 e7             	mov    %r12d,%edi
    2a29:	41 ff 14 df          	callq  *(%r15,%rbx,8)
    2a2d:	48 83 c3 01          	add    $0x1,%rbx
    2a31:	48 39 dd             	cmp    %rbx,%rbp
    2a34:	75 ea                	jne    2a20 <__libc_csu_init+0x40>
    2a36:	48 83 c4 08          	add    $0x8,%rsp
    2a3a:	5b                   	pop    %rbx
    2a3b:	5d                   	pop    %rbp
    2a3c:	41 5c                	pop    %r12
    2a3e:	41 5d                	pop    %r13
    2a40:	41 5e                	pop    %r14
    2a42:	41 5f                	pop    %r15
    2a44:	c3                   	retq   
    2a45:	66 66 2e 0f 1f 84 00 	data16 nopw %cs:0x0(%rax,%rax,1)
    2a4c:	00 00 00 00 

0000000000002a50 <__libc_csu_fini>:
    2a50:	f3 0f 1e fa          	endbr64 
    2a54:	c3                   	retq   

Disassembly of section .fini:

0000000000002a58 <_fini>:
    2a58:	f3 0f 1e fa          	endbr64 
    2a5c:	48 83 ec 08          	sub    $0x8,%rsp
    2a60:	48 83 c4 08          	add    $0x8,%rsp
    2a64:	c3                   	retq   
