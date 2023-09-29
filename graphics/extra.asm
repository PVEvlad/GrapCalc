format MS64 COFF
public testingfunction as 'testfunc'
public asmsinus as 'asmsin'
public asmcosinus as 'asmcos'
extrn width
extrn heigh 
extrn colors
extrn Xmax
extrn Xmin
extrn Ymax
extrn Ymin
;r8d-byte R, r9d-byte G, xmm1 - double y, xmm0-double x _test(11.0,-12.0,13,14,15,16); [rsp+48]-byte B [rsp+56]-byte mode
;subsd movsd movapd divsd
testingfunction:
push rbp
mov rbp,rsp
movq xmm2,[Xmax]
movq xmm3,[Xmin]

subsd xmm2,xmm0

mov rax,-1.0
movq xmm4,rax
mulsd xmm3,xmm4
	addsd xmm3,xmm0


divsd xmm2,xmm3

mov rax,1.0
movq xmm4,rax
	addsd xmm2,xmm4 
;в xmm2 результат
    movd xmm3,[width]
	cvtdq2pd xmm3,xmm3
	divsd xmm3,xmm2
cvtpd2dq  xmm0,xmm3 
;xmm0 xpix ///

;Далее считаем ypix
movq xmm2,[Ymax]
movq xmm3,[Ymin]

subsd xmm2,xmm1

mov rax,-1.0
movq xmm4,rax
mulsd xmm3,xmm4
	addsd xmm3,xmm1

divsd xmm2,xmm3

mov rax,1.0
movq xmm4,rax
	addsd xmm2,xmm4
;в xmm2 результат
	movd xmm3,[heigh]
    cvtdq2pd xmm3,xmm3
	divsd xmm3,xmm2
cvtpd2dq  xmm1,xmm3 
;в xmm1 -ypix в xmm0-xpix; tekpix=(ypix*width+xpix)*3;
movq rax,xmm1
movq rbx,xmm0

cmp rax,[heigh]
jae ending
cmp rbx,[width]
jae ending

mov rcx,[colors]
mul dword[width]
add rax,rbx
mov rbx,3
mul rbx

add rcx,rax

mov ebx,r8d
mov byte[rcx],bl
mov ebx,r9d
mov byte[rcx+1],bl
mov ebx,[rsp+48]
mov byte[rcx+2],bl

cmp byte[rsp+56],1
jne ending

mov ebx,r8d
mov byte[rcx+3],bl
mov ebx,r9d
mov byte[rcx+4],bl
mov ebx,[rsp+48]
mov byte[rcx+5],bl

mov ebx,r8d
mov byte[rcx-3],bl
mov ebx,r9d
mov byte[rcx-2],bl
mov ebx,[rsp+48]
mov byte[rcx-1],bl

mov rax,0
mov eax,[width]
mov ebx,3
mul ebx

add rcx,rax
mov ebx,r8d
mov byte[rcx+3],bl
mov ebx,r9d
mov byte[rcx+4],bl
mov ebx,[rsp+48]
mov byte[rcx+5],bl

sub rcx,rax
sub rcx,rax
cmp rcx,[colors]
jb ending
mov ebx,r8d
mov byte[rcx],bl
mov ebx,r9d
mov byte[rcx+1],bl
mov ebx,[rsp+48]
mov byte[rcx+2],bl

ending:pop rbp
ret

asmsinus:
push rbp
mov rbp,rsp
movq [rbp-8],xmm0
fld qword[rbp-8]
fsin
fstp qword[rbp-8]
movq xmm0,qword[rbp-8]

pop rbp
ret

asmcosinus:
push rbp
mov rbp,rsp
movq [rbp-8],xmm0
fld qword[rbp-8]
fcos
fstp qword[rbp-8]
movq xmm0,qword[rbp-8]

pop rbp
ret