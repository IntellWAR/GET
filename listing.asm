global main
section.text
main:
	push   rbp                                        
	mov    rbp,rsp                                    
	mov    eax,0x0                                    
	call   sum                             
	mov    eax,0x0                                    
	pop    rbp                                        
	ret        

sum:	
	push   rbp                                        
	mov    rbp,rsp                                    
	mov    DWORD PTR [rbp-0xc],0x7                      
	mov    DWORD PTR [rbp-0x8],0x8                   
	mov    edx,DWORD PTR [rbp-0xc]                    
	mov    eax,DWORD PTR [rbp-0x8]                    
	add    eax,edx                                    
	mov    DWORD PTR [rbp-0x4],eax                    
	mov    eax,DWORD PTR [rbp-0x4]                    
	pop    rbp                                        
	ret  