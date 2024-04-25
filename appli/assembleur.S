/**
  Mon premier programme en assembleur
  Sujet de TD - ESEO 2017
  @Author: Samuel Poiraud
  @Student: METTEZ VOTRE NOM ICI
  @MAJ: S. Poiraud, S. Aubin 02/09/14, S. Poiraud 04/2015, S. Poiraud 08/2015 & 09/2017
  */
  
// Nous n'entrerons pas dans le d�tail de ces quelques lignes qui permettent de configurer correctement les outils de la chaine de compilation.
  .syntax unified
  .cpu cortex-m3
  .fpu softvfp
  .thumb


.data				//D�but de la section data : d�claration des labels des variables / tableaux...
	variable_int: .space 4		//Une variable en int occupe 4 octets en m�moire

	variable_byte: .space 1		//Une variable d'un octet

	.set TAB_SIZE,	6	//Equivalent d'un "#define TAB_SIZE 6". Cette indication n'attribue rien en m�moire.
	variable_tableau: .space TAB_SIZE
	variable_int2: .space 4
	chaine1: .asciz "ch�ne"
	chaine2: .asciz "�rable"
	chaine3: .asciz "h�tre"

.section  .text.User_section	//D�but de la section text : Les instructions du programme en assembleur commencent ici.

.global asm_main	//D�clare que le label main_asm est �galement accessible de l'ext�rieur du fichier assembleur. (en C, on parle de fonction "publique").
asm_main:

//Mission 0
	BL M0_useless_function

//Mission 1
	MOV R0, #21
	//MOV R0, #-3
	//MOV R0, #18
	BL M1_ecrete_note

//Mission 2
	BL M2_incremente_variable_int

//Mission 3
	MOV R2, #'A'		//Le caract�re � mettre dans le tableau
	MOV R1, #TAB_SIZE	//La taille du tableau (en nombre de char), ici, TAB_SIZE est d�fini � 6
	LDR R0,=variable_tableau
	BL M3_init_tab

infinite_loop:
	B infinite_loop



M0_useless_function:
	//Choix arbitraire, A : R0, B : R1, C : R2
	

    MOV R0, #3
    MOV R1, R0
    ADD R2, R1, R0
    MOV R4, #4
    MUL R1, R1, R4

	MOV pc, lr		//retourne � l'appelant.


// la note � �creter est plac�e dans R0, la note ecret�e est remise aussi dans R0.
M1_ecrete_note:	//En utilisant des branchements conditionnels


	MOV PC, LR		//return


M1_bis_ecrete_note:	//En utilisant des instructions conditionelles
	MOV R0, #25
	CMP R0, #20
	BLT la
	MOV R0, #20
	la:

	CMP R0, #0
	BGT apres
	MOV R0, #0
	apres:
	
	
	MOV PC, LR		//return


M2_incremente_variable_int:
	LDR R0,= variable_int
	LDR R1, [R0]
	ADD R1, #1
	STR R1, [R0]
	MOV PC, LR

	
//Dans R0 : l'adresse du tableau
//Dans R1 : la taille du tableau
//Dans R2 : l'octet � ranger dans chaque case du tableau
//Vous pouvez mettre i dans R3
M3_init_tab:


	MOV PC, LR		//return

//Retourne la taille d'une chaine de caract�res
my_strlen:
	MOV R1, R0			//Copie de l'adresse de la chaine
my_strlen_loop:
	LDRB R2, [R1], #1	//Lecture dans R2 de l'octet point� par R1, puis incr�mentation de R1
	CMP R2, #0			//Est-ce que le caract�re lu est 0 ?
	BNE my_strlen_loop 	//On reboucle si on est pas � la fin
	SUB R0, R1, R0		//Nombre de caract�res = adresse finale - adresse initiale
	SUB R0, #1
	MOV PC, LR

		//Remarque : ce sous-programme est bogu� volontairement...
.global TD_mission_scratch
TD_mission_scratch:
	MOV R4, #0			//J'initialise la somme

	LDR R0,=chaine1
	BL my_strlen
	ADD R4, R0			//J'ajoute la taille de chaine1 � la somme

	LDR R0,=chaine2
	BL my_strlen
	ADD R4, R0			//J'ajoute la taille de chaine2 � la somme

	LDR R0,=chaine3
	BL my_strlen
	ADD R0, R4			//J'ajoute la taille de chaine3 � la somme

	MOV PC, LR




.global additionner_7
additionner_7:
	
	

	mov pc, lr



.global additionner_n
additionner_n:
	
	
	MOV PC, LR




.global factorielle_n
factorielle_n:
	
	
	MOV PC, LR


	
